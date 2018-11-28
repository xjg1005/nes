
#include "config.h"
#include "dev_ctrl.h"
#include "drv_manage.h"
#include "mango_dev_sd.h"
#include "msg.h"
#include "vfs.h"
#include "fileSelecter.h"


static DEV_HANDLE dev_hdl_flash = 0;
static u32 spi_parm = 0;

static DEV_HANDLE dev_hdl_sd;

DEV_HANDLE dev_get_sd1_hdl()
{
    return dev_hdl_sd;
}


bool app_sd_io_dect(void)
{

    APP_SD_IO_HIGH();
    //delay(200);
    delay(100);

    if(APP_SD_IO_READ()){
//        putchar('a');
        return 1;
    }else{
//        putchar('b');
        return 0;
    }

//    return 0;//����
}

void app_sd_reg()
{
    u32 parm;
    u32 cpu_sr;

//    struct DEV_IO *dev_hdl_sdio = dev_reg_sd1(NULL);
//    dev_hdl_sdio->ioctrl(&app_sd_io_dect, SD_SET_DECT_FUNC);

OS_ENTER_CRITICAL();
    APP_SD_IO_INIT();
OS_EXIT_CRITICAL();

//    parm = SD_4WIRE_MODE|SD_CLK_DIV2|SD_PF4_9|SD_IO_DECT;	// ���豸�Ĳ���
    //parm = SD_4WIRE_MODE|SD_CLK_DIV3|SD_PF4_9|SD_CMD_DECT; // ���豸�Ĳ���
    parm = SD_4WIRE_MODE|SD_CLK_DIV3|SD_PF4_9|SD_CLK_DECT;	//5203 JOHNSON 20160509

    DEVICE_REG(sd1,&parm);	//ע��sd1��ϵͳ

    dev_hdl_sd = dev_open( "sd1", &parm);
    if (dev_hdl_sd == NULL){
        printf("sd fail\n");
        while(1);
    }
    //dev_ioctl(dev_hdl_sd, &app_sd_io_dect, SD_SET_DECT_FUNC);
    dev_ioctl(dev_hdl_sd, NULL, SD_SET_DECT_FUNC);//5203 JOHNSON 20160509

}


void app_flash_reg()
{
    u32 parm;
    void * flash_priv;

    DEVICE_REG(flash,NULL);
    flash_priv = &spi_parm;
    dev_hdl_flash = dev_open("flash", flash_priv);
    if (dev_hdl_flash == NULL){
        printf("flash fail\n");
        while(1);
    }else{
        //�Ȳ�����������
//////////        //parm = PROTECT_2M_0_TO_15;//����flash 0��1M�ռ�
//////////        //dev_ioctl(dev_hdl_flash, (void *)SPI0FLASH_PROTECT, (u32)&parm);
    }
}


void dev_reg_ctrl()
{
    u32 parm;
    void * flash_priv;

    DEVICE_REG(flash,NULL);
    flash_priv = &spi_parm;
    dev_hdl_flash = dev_open("flash", flash_priv);
    if (dev_hdl_flash == NULL){
        printf("flash fail\n");
        while(1);
    }else{
        //�Ȳ�����������
//////////        //parm = PROTECT_2M_0_TO_15;//����flash 0��1M�ռ�
//////////        //dev_ioctl(dev_hdl_flash, (void *)SPI0FLASH_PROTECT, (u32)&parm);
    }

}

DEV_HANDLE dev_get_flash_hdl(void)
{
    return dev_hdl_flash;
}


/*************************************************
Function:	u32 dev_ctrl_sys_init(void *parm)
Description:	�豸����ϵͳ��ʼ��
Input:	*parm	�߳̾��
Output:	��
Return:	0		�ɹ�
		��0		��������Ϣ�б�
Others:	���豸����ϵͳ����ǰ��Ҫ��ʼ���ļ�ϵͳ
*************************************************/
u32 dev_ctrl_sys_init(void *parm)
{
    u32 err;

    //ע���豸(�����豸���ڴ˴�ע��)
    dev_reg_ctrl();

    return err;
}

/*----------------------------------------------------------------------------*/
/** @brief: SPI������Ϣ��ʼ��
    @param: val1��val2ϵͳ������Ϣ
    @author:
    @note:  ֻ��MAIN�����е���һ��
*/
/*----------------------------------------------------------------------------*/
void init_spi_info(u32 val1, u32 val2)
{
    u32 spi_wire_mode = 0;
    u32 spi_div = 0;
    u32 spi_continue_read_mode = 0;
    u32 spi_output_io_mode = 0;

    spi_parm = 0;

    spi_wire_mode = val1&0x3;

    if(spi_wire_mode == 0)
    {
        spi_parm |=  SPI_2WIRE_MODE;
    }
    else if(spi_wire_mode == 1)
    {
        spi_parm |=  SPI_ODD_MODE;
    }
    else if(spi_wire_mode == 2)
    {
        spi_parm |=  SPI_DUAL_MODE;
    }
    else
    {
        spi_parm |=  SPI_QUAD_MODE;
    }

    spi_div = (val2>>(8+8+9))&0x7;
//    puts_without_mutex("\n spi_div : ");
//    put_u32hex(spi_div);

    spi_parm |= spi_div;

    spi_continue_read_mode = (val1>>2)&0x1;
//    puts_without_mutex("\nspi_continue_read_mode : ");
//    put_u32hex(spi_continue_read_mode);

    spi_output_io_mode = (val1>>3)&0x1;
//    puts_without_mutex("\nspi_output_io_mode : ");
//    put_u32hex(spi_output_io_mode);

    if(spi_output_io_mode == 1)
    {
        spi_parm |= FAST_READ_OUTPUT_MODE;
    }
    else
    {
        if(spi_continue_read_mode == 1)
        {
            spi_parm |= FAST_READ_IO_CONTINUOUS_READ_MODE;
        }
        else
        {
            spi_parm |= FAST_READ_IO_MODE;
        }
    }
//    puts_without_mutex("\nspi_parm : ");
//    put_u32hex(spi_parm);

}





//void  Task_storedevice_ctrl(void *p_arg)
//{
//    u32 msg;
//    msg_t *msg_d;
//    msg_devicechange_t *devsta;
//    dev_ctrl_sys_init(NULL);
//    puts("\n---enter devctrl------\n");
//    Load_vfs(5);
//
//    while(1)
//    {
//
//        msg = get_msg();
//
//        if(msg  > (u32)0x10000)
//        {
//            msg_d = msg ;
//            msg = msg_d->type ;
//            printf("---------msg = %d ----------\n",msg);
//        }
//        switch(msg)
//        {
//        case MSG_DEVICECHANGE:
//
//            puts("msg dev change\n");
//            devsta = msg_d->ptr;
//            file_sel_msg_ctrl(msg,devsta);
//            printf("\n %s--%d \n",devsta->hdev->dev_io->name, devsta->sta);
//
//            free(msg_d);
//            break;
//        default :
//            break ;
//        }
//    }
//}

