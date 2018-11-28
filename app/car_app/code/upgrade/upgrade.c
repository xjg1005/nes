#include "config.h"
#include "upgrade.h"
#include "fileSelecter.h"
#include "sdfile.h"
#include "HWI.h"
#include "lcd.h"
#include "dsc.h"

#define FILE_EXT      "BFU"
#define UPDATA_INFO_ADDR    (0x1F14000-0x200)
#define CMD_ZONE_SIZE      4*1024
#define FLASH_FILE_HEADER_SIZE 32
#define CLOSE_WDT()     CRC_REG = 0x6EA5;WDT_CON &= ~BIT(4);CRC_REG = 0
#define TYPE_A          0x30086000
#define FLAG_ADDR       0x1f50000

enum
{
    UPGRADE_CODE = 0,
    UPGRADE_RES,
    UPGRADE_AUD_LOGO, //工程下开机动画文件
    UPGRADE_AUD_LOGO1,//工厂开机动画文件
    UPGRADE_CODE_RES,
    UPGRADE_FLASH,
};

#pragma pack(1)
typedef struct __SFUHEADER
{
    u8  vid[8];
    u32 headerLen;
    u32 headercrc;
    u32 dataAddr;
    u32 dataLen;
    u32 dataCrc;
    u32 ldAddr;
    u32 runAddr;
    u8  fileName[12];
    u32 version;
    u32 update_zone_offset;
    u32 update_zone_len;
    u16 update_zone_crc;
}SFUHEADER;
#pragma pack()


static u8 upgrade_mode[][12] =
{
  "CODE",
  "RESOURCE",
  "AUDLOGO",
  "AUDLOGO1",
  "FLASH",
};

static u8 upgrade_filename[16];
extern u32 flash_size;

u8 *get_upgrade_filename(void)
{
    if (*upgrade_filename != '\0')
    {
        return &upgrade_filename[0];
    }
    return NULL;
}

void set_upgrade_filename(u8 *filename)
{
    if (filename)
    {
        strcpy((char *)upgrade_filename, (char *)filename);
    }
}


s32 femu_ext_callback(void *fileparam, void *pram2callback)
{
    u8 *ext = NULL;
    u8 *p_ext_name = NULL;
    u32 i;
    FILE_PARAM * curFile = (FILE_PARAM*)fileparam;

    ext = (u8 *)pram2callback;

    if (ext)
    {
        if ((curFile->fattrib & 0x20) == 0x20)
        {

            for(i = 0; i < strlen((char*)(curFile->fname)); i++)
            {
                char c = curFile->fname[i];
                if (c == '.')
                    break;
            }

            p_ext_name = &curFile->fname[++i];//检查文件名后缀

//            printf("%s() %d, fname : %s, ext : %s, cm ext : %s\n", __FUNCTION__, __LINE__, curFile->fname, p_ext_name, ext);
            for (ext; *ext!='\0'; ext+=3)
            {
                if (0 == strncmp(p_ext_name, ext, 3))
                {
                    set_upgrade_filename(curFile->fname);
                    return TRUE;
                }
            }
        }
    }

    return FALSE;
}

u32 scan_upgrade_file_ext(u8 *path, u8 *file_ext)
{
    return f_emu((u8*)path, (void*)file_ext, femu_ext_callback);
}

u8 get_upgrade_mode(u8 *filename)
{
    u8 mode_index = 0;
    for (mode_index = 0; mode_index < sizeof(upgrade_mode) / sizeof(upgrade_mode[0]); mode_index++)
    {
        if (0 == strcmp(filename, upgrade_mode[mode_index]))
        {
            break;
        }
    }
    return mode_index;
}

static void trans_audlogo_head(u8 *buf1, u8 *buf2)
{
    u32 cnt;
    u8 header[FLASH_FILE_HEADER_SIZE];

    for(cnt = 0; cnt < FLASH_FILE_HEADER_SIZE; cnt++)
    {
        header[cnt] = buf1[cnt] ^ buf2[cnt];
        buf1[cnt] = header[cnt];
    }
}

//检测工厂开机动画文件是否可以升级
#define AUDLOGO_DATA_ADDR 512
static u8 check_audlogo_zone(u8 *p_file_cmd)
{
    DEV_HANDLE dev;
    s32 (*sd_read)(void *buf, u32 addr , u16 len) = NULL;
    u8 cnt;
    u8 total_cnt = 0;
    u8 file_buf[32];
    u8 cmp_buf[32];
    sdfile_t audlogo_file;
    u16 screen_w;
    u16 screen_h;

    memset(cmp_buf, 0xFF, sizeof(cmp_buf));
    total_cnt = 512 / 32;

    if (!sd_fopen("audlogo.res", &audlogo_file))
    {
        puts("open audio_logo res err.\n");
        return FALSE;
    }
    dev = dev_get_flash_hdl();
    sd_read = dev->dev_io->private_data;
    sd_read(file_buf, audlogo_file.addr, sizeof(file_buf));
    trans_audlogo_head(&p_file_cmd[0], &p_file_cmd[AUDLOGO_DATA_ADDR]);
    if(0 != memcmp(&p_file_cmd[0], file_buf, sizeof(file_buf)))
    {
        screen_w = p_file_cmd[AUDLOGO_DATA_ADDR + 0x10] | p_file_cmd[AUDLOGO_DATA_ADDR + 0x11] << 8;
        screen_h = p_file_cmd[AUDLOGO_DATA_ADDR + 0x12] | p_file_cmd[AUDLOGO_DATA_ADDR + 0x13] << 8;
//        printf("%s() %d, w : %d, h : %d\n", __FUNCTION__, __LINE__, screen_w, screen_h);
        if (screen_w == LCD_W && screen_h == LCD_H)
        {
            return TRUE;
        }
    }


    return FALSE;

}

u8 check_cmd_zone(u8 *p_flash_cmd, u8 *p_file_cmd, u8 mode)
{
    u16 cmd_header = 512;
    u8  cmd_head_offset = 0;
//    u16 crc = 0;
    u8 cmp_buf[512];

    if (!p_file_cmd || !p_flash_cmd)
    {
        puts("check cmd zone err.\n");
        return FALSE;
    }
//    return TRUE;
//
    memset(cmp_buf, 0xFF, sizeof(cmp_buf));
//    printf("%s() %d, upgrade mode : %d\n", __FUNCTION__, __LINE__, mode);
    if (0 == memcmp((void *)&p_flash_cmd[cmd_header], (void *)cmp_buf, sizeof(cmp_buf)))
    {
        return FALSE;
    }

    switch(mode)
    {
        case UPGRADE_CODE:
            cmd_head_offset = 4;
            if (0 == memcmp(&p_flash_cmd[cmd_header + cmd_head_offset * FLASH_FILE_HEADER_SIZE],
                            &p_file_cmd[cmd_header + cmd_head_offset * FLASH_FILE_HEADER_SIZE], FLASH_FILE_HEADER_SIZE))
            {
                return FALSE;
            }
            break;
        case UPGRADE_RES:
            cmd_head_offset = 5;
            if (0 == memcmp(&p_flash_cmd[cmd_header + cmd_head_offset * FLASH_FILE_HEADER_SIZE],
                            &p_file_cmd[cmd_header + cmd_head_offset * FLASH_FILE_HEADER_SIZE], FLASH_FILE_HEADER_SIZE))
            {
                return FALSE;
            }
            break;
        case UPGRADE_AUD_LOGO:
            cmd_head_offset = 6;
            if (0 == memcmp(&p_flash_cmd[cmd_header + cmd_head_offset * FLASH_FILE_HEADER_SIZE],
                            &p_file_cmd[cmd_header + cmd_head_offset * FLASH_FILE_HEADER_SIZE], FLASH_FILE_HEADER_SIZE))
            {
                return FALSE;
            }
            break;
        case UPGRADE_CODE_RES:
            cmd_head_offset = 4;
            if (0 == memcmp(&p_flash_cmd[cmd_header + cmd_head_offset * FLASH_FILE_HEADER_SIZE],
                            &p_file_cmd[cmd_header + cmd_head_offset * FLASH_FILE_HEADER_SIZE], 3 * FLASH_FILE_HEADER_SIZE))
            {
                return FALSE;
            }
            break;
        default:

            break;
    }

    return TRUE;
}

u32 get_flash_upgrade_addr(u8 mode, u32 upgrade_data_len)
{
    sdfile_t code_file;
    sdfile_t res_file;
    u32 flash_file_len = 0;
    u32 upgrade_flash_addr = 0;

    switch(mode)
    {
        case UPGRADE_CODE://code
        case UPGRADE_CODE_RES://code+res+logo
            if (!sd_fopen("sdram.apu", &code_file))
            {
                return 0;
            }
            upgrade_flash_addr = code_file.addr;
            flash_file_len = code_file.len;
            break;
        case UPGRADE_RES://res
            if (!sd_fopen("res.res", &res_file))
            {
                return 0;
            }
            upgrade_flash_addr = res_file.addr;
            flash_file_len = res_file.len;
            break;
        case UPGRADE_AUD_LOGO://logo加密
        case UPGRADE_AUD_LOGO1://logo工具生成
            if (!sd_fopen("audlogo.res", &res_file))
            {
                return 0;
            }
            upgrade_flash_addr = res_file.addr;
            flash_file_len = res_file.len;
            break;
        default:
            break;
    }

    if (mode != UPGRADE_CODE_RES && mode != UPGRADE_AUD_LOGO && mode != UPGRADE_AUD_LOGO1 && upgrade_data_len > flash_file_len)
    {
        printf("upgrade len : 0x%x, flash file len : 0x%x.\n", upgrade_data_len, flash_file_len);
        return 0;
    }
    else if (mode == UPGRADE_AUD_LOGO && upgrade_data_len > (flash_size - upgrade_flash_addr - 68 * 1024))
    {
        printf("logo length bigger than flash, 0x%x 0x%x", upgrade_data_len, flash_size - upgrade_flash_addr);
        return 0;
    }
    else if (mode == UPGRADE_AUD_LOGO1 && (upgrade_data_len + 2 * CMD_ZONE_SIZE) > (flash_size - upgrade_flash_addr - 68 * 1024))
    {
        printf("logo length bigger than flash, 0x%x 0x%x", upgrade_data_len, flash_size - upgrade_flash_addr);
        return 0;
    }

    return upgrade_flash_addr;
}


u16 CRC16_noinit(void *ptr, u32  len)
{
    u8 *p = (u8 *)ptr;

    while(len--)
    {
        CRC_FIFO = *p++;
    }

    return CRC_REG;
}

u32 check_upgrade_file_info(u8 *file_path, void *param)
{
    u32 ret = 0;
    file_t *fp = NULL;
    sdfile_t sd_file;
    SFUHEADER file_header;
    UPDATE_INFO *p_upgrade_info = NULL;

    u32 cmd_zone_addr = 0;
    u8 *file_cmd_zone = NULL;
    u8 *flash_cmd_zone = NULL;
    u8 upgrade_mode;

    u32 read_len = 0;
    u32 data_len = 0;
    u16 crc = 0;
    u8 *data_buf = NULL;
    p_upgrade_info = (UPDATE_INFO *)param;

    fp = fopen(file_path, openReadOnly);
    if (!fp)
    {
        return 1;
    }

    if (0 == fread(fp, (u8 *)&file_header, sizeof(file_header)))
    {
        ret = 1;
        goto _EXIT;
    }
    printf("sfu header, vid : %s,\n"
           "header len : 0x%x\n"
           "header crc : 0x%x\n"
           "data addr : 0x%x\n"
           "data len : 0x%x\n"
           "data crc : 0x%x\n"
           "file name : %s\n",
           file_header.vid,
           file_header.headerLen, file_header.headercrc, file_header.dataAddr,file_header.dataLen,
           file_header.dataCrc, file_header.fileName);

    file_cmd_zone = (u8 *)malloc(CMD_ZONE_SIZE);
    flash_cmd_zone = (u8 *)malloc(CMD_ZONE_SIZE);
    if (!file_cmd_zone || !flash_cmd_zone)
    {
        puts("flash 4k  err.\n");
        ret = 1;
        goto _EXIT;
    }


    crc = CRC16(&file_header.dataAddr, file_header.headerLen);
// printf("%s() %d, head crc : 0x%x.\n", __FUNCTION__, __LINE__, crc);

    fseek(fp, SEEK_SET, file_header.dataAddr);
    CRC_REG = 0 ;
    while(1)
    {

        read_len = fread(fp, file_cmd_zone, CMD_ZONE_SIZE);
        crc = CRC16_noinit(file_cmd_zone, read_len);
        data_len += read_len;
        if(read_len != CMD_ZONE_SIZE || data_len >= file_header.dataLen)
            break;

    }
//printf("%s() %d, data crc : 0x%x.\n", __FUNCTION__, __LINE__, crc);


    fseek(fp, SEEK_SET, file_header.dataAddr);
    fread(fp, file_cmd_zone, CMD_ZONE_SIZE);

    if (!sd_fopen("cmd_zone", &sd_file))
    {
        puts("sd open cmd zone err.\n");
        ret = 1;
        goto _EXIT;
    }
    cmd_zone_addr = sd_file.addr;
    sd_fread(&sd_file, flash_cmd_zone, CMD_ZONE_SIZE);

    upgrade_mode = get_upgrade_mode(file_header.fileName);
    printf("%s() %d, upgrade mode : %d\n", __FUNCTION__, __LINE__, upgrade_mode);
    if (upgrade_mode == UPGRADE_AUD_LOGO1)
    {
        if (!check_audlogo_zone(file_cmd_zone))
        {
            ret = 2;
            puts("no need upgrade.\n");
            goto _EXIT;
        }
    }
    else if (!check_cmd_zone(flash_cmd_zone, file_cmd_zone, upgrade_mode))
    {
        sd_fread(&sd_file, flash_cmd_zone, CMD_ZONE_SIZE);
        if (!check_cmd_zone(flash_cmd_zone, file_cmd_zone, upgrade_mode))
        {
            puts("no need upgrade.\n");
            ret = 2;
            goto _EXIT;
        }
    }

    p_upgrade_info->dest_addr = get_flash_upgrade_addr(upgrade_mode, file_header.dataLen - 2 * CMD_ZONE_SIZE - 512);

    if (!p_upgrade_info->dest_addr)
    {
        puts("no need upgrade or dest is 0.\n");
        ret = 2;
        goto _EXIT;
    }

    p_upgrade_info->change_head_addr = cmd_zone_addr;
    p_upgrade_info->src_addr = file_header.dataAddr;
    printf("\n%s() %d, check success, need upgrade.\n"
           "dest addr : 0x%x,\n"
           "src  addr : 0x%x,\n"
           "change_head_addr : 0x%x\n", __FUNCTION__, __LINE__, p_upgrade_info->dest_addr,
           p_upgrade_info->src_addr, p_upgrade_info->change_head_addr);

_EXIT:
    if (file_cmd_zone)
    {
        free(file_cmd_zone);
    }
    if (flash_cmd_zone)
    {
        free(flash_cmd_zone);
    }

    fclose(fp);
    return ret;
}


u8 upgrade_set_info(u8 *file_path)
{
    UPDATE_INFO upgrade_info;
    UPDATE_INFO *p_set_update;

    u16 crc = 0;

    p_set_update = (UPDATE_INFO *)UPDATA_INFO_ADDR;

    //检查升级文件
    if (check_upgrade_file_info(file_path, &upgrade_info))
    {
        return FALSE;
    }
    //配置设备、设备出口、NWIRE、分频
    upgrade_info.dev_type = UPGRADE_DEV_SD | SD_PF4_9 | SD_4WIRE_MODE | SD_CLK_DIV4;

    strcpy(upgrade_info.updata_file_name, get_upgrade_filename());
    crc = CRC16((void *)&upgrade_info, sizeof(upgrade_info) - sizeof(upgrade_info.crc));
    upgrade_info.crc = crc;

    memcpy(p_set_update, &upgrade_info, sizeof(upgrade_info));

    return TRUE;
}

extern dsc_hdl_t * g_dsc0_hdl;
extern dsc_hdl_t * g_dsc1_hdl;
/**************************************************************
* description : 升级检测，检测到不同版本升级文件后直接升级

* return :  SD卡等待超时、无升级文件或者版本错文件返回，
*           正常则复位升级
*
***************************************************************/
u8 upgrade_check(void)
{
    u8 wait_sd_cnt = 0;
    u8 root_path[] = "B:/";
    u8 file_path[16];
    u32 *ptr = FLAG_ADDR;
    //return UPGRADE_SD_TIMEOUT;
    while (!is_phy_sd_online())
    {
        //wait_sd_ok();
        os_time_dly(10);
        if (++wait_sd_cnt == 10)
        {
            puts("\n--------wait sd timeout.---------\n");
            return UPGRADE_SD_TIMEOUT;
        }
    }
//    printf("%s() %d, sd is online.\n", __FUNCTION__, __LINE__);

    scan_upgrade_file_ext(root_path, FILE_EXT);
    if (!get_upgrade_filename())
    {
        puts("no valid upgrade file.\n");
        return UPGRADE_NO_VALID_FILE;
    }

    strcpy(file_path, root_path);
    strcat(file_path, get_upgrade_filename());

    if (FALSE == upgrade_set_info(file_path))//升级信息配置
    {
        return 0;
    }
//    return 0;
//    printf("%s() %d, debug upgrade.\n", __FUNCTION__, __LINE__);
    dac_api_close();

DISABLE_INT();
    ALCD_BL_OFF();
    imb_close();
    CLOSE_WDT();

    hwi_init();
    T2_CON = BIT(14);
    T1_CON = BIT(14);
    T0_CON = BIT(14);
    CLK_CON0 = ((CLK_CON0 & ~BIT(8)) & ~BIT(6)) | BIT(7);

    if(*ptr == TYPE_A)
    {
//       puts("TYPE A\n");
        ((void(*)(void))(0x01f500c8))();
    }
    else
    {
//        puts("TYPE B\n");
        ((void(*)(void))(0x1f500fc))();
    }


    return 0;
}
