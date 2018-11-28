#include "isp_customize.h"
#include "sdfile.h"
#include "my_ui.h"
#include "app_cfg.h"
extern SYS_INF sys_info;
extern volatile u8 imc_isr_stop_flag;

#define ISP_CFG_FLAG	0xFFFE66AA

#define TRIAL_VERSION   100    //无version 试用版
#define VERSION_1_01    101    //第一版
#define VERSION_1_02    102    //第二版(添加了手动白平衡和黑电平校正配置）
/*
 ...
 */
#pragma pack(1)
/******2015/11/20********/
typedef struct isp_cfg_header
{
    u32 flag;
    u32 version;
}ISP_CFG_HEADER;

typedef struct _isp_manual_wb_blc
{
    u8 reserved1; //保留位1用于后面2个字节对齐
    u8 manual_wb_en;
    u16 manual_wb_tbl[6 * 3];
    u16 shpn_threshold;
    u16 shpn_strength;
    u8 reserved2;//保留位2用于后面2个字节对齐
    u8 blc_en;
    s16 blc_r_off;
    s16 blc_gr_off;
    s16 blc_gb_off;
    s16 blc_b_off;
}ISP_MANUAL_WB_BLC;

typedef struct _isp_cfg_param
{
    u8 ev_mode;
	u8 white_balance;
	u8 md_level;
	u8 nr_level;
	u8 sharpness_level;
	u8 gamma[256];
	s8 ev;
	u16 saturation;
	s16 ccm3x4[12];
	s16 lsc_x;
	s16 lsc_y;
	//首地址按2字节对齐
	u16 lsc_r[1024];
	u16 lsc_g[1024];
	u16 lsc_b[1024];

    ISP_MANUAL_WB_BLC manual_wb_blc;
}ISP_CFG_PARAM;
//目前结构最大为数据位宽为16，如果添加32位结构要进行32位对齐排列
#pragma pack()


//static ISP_PARAM_ST isp_cfg_day;
//static ISP_PARAM_ST isp_cfg_night;

static ISP_CFG_PARAM isp_cfg_day ALIGNE(4);
static ISP_CFG_PARAM isp_cfg_night ALIGNE(4);
static u32 cfg_version = 100;

static void updata_isp_cfg(ISP_CFG_PARAM *isp_cfg)
{
    ispt_customize_evMode(isp_cfg->ev_mode);

	ispt_customize_gamma(isp_cfg->gamma);

	ispt_customize_ccm3x4(isp_cfg->ccm3x4);

	ispt_customize_nrLevel(isp_cfg->nr_level);

	ispt_customize_shpnLevel(isp_cfg->sharpness_level);

//	ispt_customize_lsc(isp_cfg->lsc_x, isp_cfg->lsc_y, isp_cfg->lsc_r, isp_cfg->lsc_g, isp_cfg->lsc_b);

	ispt_saturation(isp_cfg->saturation);
#if 0//根据需要配置
	if (cfg_version == VERSION_1_02)
	{
        ispt_set_blc(isp_cfg->manual_wb_blc.blc_en, isp_cfg->manual_wb_blc.blc_r_off, isp_cfg->manual_wb_blc.blc_gr_off,
                     isp_cfg->manual_wb_blc.blc_gb_off, isp_cfg->manual_wb_blc.blc_b_off);
        ispt_customize_shpnThreshold(isp_cfg->manual_wb_blc.shpn_threshold);
        ispt_customize_shpnAmount(isp_cfg->manual_wb_blc.shpn_strength);
//        ispt_mannual_wb()
	}
#endif

}
static u8 init_mart;
void reset_isp_cfg()
{
    u8 lv_value;
    lv_value = ispt_get_lv();
    if(lv_value <6) //夜间
    {
//        ispt_customize_drcEnable(sys_info.wdr);
		  #if (DEBUG_ISP == 0)
            ispt_customize_drcEnable(sys_info.wdr);
          #else
            ispt_customize_drcEnable(0);
          #endif
        updata_isp_cfg(&isp_cfg_night);
        //这个时候，ISP后台不更新，相当于进入ISP的临界区,必须成对的出现
#if LSC_EN
        ispt_customize_lsc(isp_cfg_day.lsc_x, isp_cfg_day.lsc_y, isp_cfg_day.lsc_r, isp_cfg_day.lsc_g, isp_cfg_day.lsc_b);
#endif
    }
    else
    {
        ispt_customize_drcEnable(0);
        updata_isp_cfg(&isp_cfg_day);
        //这个时候，ISP后台不更新，相当于进入ISP的临界区,必须成对的出现
#if LSC_EN
        ispt_customize_lsc(isp_cfg_day.lsc_x, isp_cfg_day.lsc_y, isp_cfg_day.lsc_r, isp_cfg_day.lsc_g, isp_cfg_day.lsc_b);
#endif
    }

    updata_menu_cfg();
}


void user_isp_cfg(void)
{
    if(imc_isr_stop_flag){
        return;
    }

#if USER_ISP_CFG_EN

	ispt_customize_evMode(USER_EV_MODE);

	ispt_customize_gamma(user_gamma);

	ispt_customize_ccm3x4(user_ccm);

	ispt_customize_nrLevel(USER_NR_LEVAL);

	ispt_customize_shpnLevel(USER_SHARPNESS_LEVAL);

	ispt_customize_lsc(USER_LSC_X, USER_LSC_Y, user_lsc_r, user_lsc_g, user_lsc_b);

	ispt_saturation(USER_SATURATION);

//	ispt_customize_ev(USER_EV);
//	ispt_customize_mdLevel(USER_MD_LEVEL);
//	ispt_preset_wb(USER_WHITE_BALANCE);
#else
    #if 0
	sdfile_t sdfile;
	ISP_PARAM_ST isp_cfg;
	if (!sd_fopen("isp_cfg.bin", &sdfile))
	{
		printf("%s() %d, no have isp_cfg.bin.\n", __FUNCTION__, __LINE__);
		return;
	}

	sd_fread(&sdfile, (void *)&isp_cfg, sizeof(ISP_PARAM_ST));

	if (isp_cfg.flag != ISP_CFG_FLAG)
	{
		puts("isp cfg file error.\n");
		return;
	}
	ispt_customize_evMode(isp_cfg.ev_mode);

	ispt_customize_gamma(isp_cfg.gamma);

	ispt_customize_ccm3x4(isp_cfg.ccm3x4);

	ispt_customize_nrLevel(isp_cfg.nr_level);

	ispt_customize_shpnLevel(isp_cfg.sharpness_level);

	ispt_customize_lsc(isp_cfg.lsc_x, isp_cfg.lsc_y, isp_cfg.lsc_r, isp_cfg.lsc_g, isp_cfg.lsc_b);

	ispt_saturation(isp_cfg.saturation);
    #else
    //static u8 init_mart = 0;
    static u8 lv_value_pre = 16;
    static u8 time_cnt = 0;
    u8 lv_value;
    sdfile_t sdfile;
    ISP_CFG_HEADER isp_cfg_header;


    if(init_mart == 0)
    {
        if (!sd_fopen("isp_cfg.bin", &sdfile))
        {
            printf("%s() %d, no have isp_cfg.bin.\n", __FUNCTION__, __LINE__);
            return;
        }
        else
        {
            sd_fread(&sdfile, &isp_cfg_header, sizeof(ISP_CFG_HEADER));

            if (isp_cfg_header.flag != ISP_CFG_FLAG)
            {
                //试用版不含flag和version
                sd_fseek(&sdfile, SD_SEEK_SET, 0);
            }
            else
            {
                cfg_version = isp_cfg_header.version;
            }

            memset(&isp_cfg_day, 0x0, sizeof(ISP_CFG_PARAM));
            sd_fread(&sdfile, &isp_cfg_day, sizeof(ISP_CFG_PARAM));//读取cfg,兼容新旧版本

            init_mart |=BIT(0);
        }

        if (!sd_fopen("isp_cfg_B.bin", &sdfile))
        {
            printf("%s() %d, no have isp_cfg.bin.\n", __FUNCTION__, __LINE__);
            return;
        }
        else
        {
            sd_fread(&sdfile, &isp_cfg_header, sizeof(ISP_CFG_HEADER));

            if (isp_cfg_header.flag != ISP_CFG_FLAG)
            {
                sd_fseek(&sdfile, SD_SEEK_SET, 0);
            }
            else
            {
                cfg_version = isp_cfg_header.version;
            }

            memset(&isp_cfg_night, 0x0, sizeof(ISP_CFG_PARAM));
            sd_fread(&sdfile, &isp_cfg_night, sizeof(ISP_CFG_PARAM));

            init_mart |=BIT(1);
        }

        lv_value = ispt_get_lv();
        if(lv_value <6) //夜间
        {
            if(init_mart & BIT(1))
            {
//                ispt_customize_drcEnable(sys_info.wdr);
                #if (DEBUG_ISP == 0)
                    ispt_customize_drcEnable(sys_info.wdr);
                #else
                    ispt_customize_drcEnable(0);
                #endif
                updata_isp_cfg(&isp_cfg_night);
#if LSC_EN
                ispt_customize_lsc(isp_cfg_night.lsc_x, isp_cfg_night.lsc_y, isp_cfg_night.lsc_r, isp_cfg_night.lsc_g, isp_cfg_night.lsc_b);
#endif
//            #ifdef FLASH_LIGHT  //JOHNSON 20160531
//            flash_light_ctrl(TRUE);
//            #endif
            }
        }
        else
        {
            if(init_mart & BIT(0)) //默认白天模式
            {
                ispt_customize_drcEnable(0);
                updata_isp_cfg(&isp_cfg_day);
#if LSC_EN
                ispt_customize_lsc(isp_cfg_day.lsc_x, isp_cfg_day.lsc_y, isp_cfg_day.lsc_r, isp_cfg_day.lsc_g, isp_cfg_day.lsc_b);
#endif
//            #ifdef FLASH_LIGHT  //JOHNSON 20160531
//            flash_light_ctrl(FALSE);
//            #endif

            }
        }

        if(init_mart == 0)
        {
            init_mart |= BIT(2);
        }
    }
    else
    {
        if(init_mart == 3)
        {
            time_cnt++;
            if((time_cnt % 1) == 0)
            {
                lv_value = ispt_get_lv();
                if(lv_value <=6 && lv_value_pre>=8) //白天->夜晚
                {
//                    ispt_customize_drcEnable(sys_info.wdr);
                  #if (DEBUG_ISP == 0)
                    ispt_customize_drcEnable(sys_info.wdr);
                  #else
                    ispt_customize_drcEnable(0);
                  #endif
                    updata_isp_cfg(&isp_cfg_night);
                    lv_value_pre = lv_value;
                    puts("\n isp_cfg_night \n");

//                #ifdef FLASH_LIGHT  //JOHNSON 20160531
//                flash_light_ctrl(TRUE);
//                #endif
                }
                else if(lv_value >=8 && lv_value_pre<=6) //夜晚->白天
                {
                    ispt_customize_drcEnable(0);
                    updata_isp_cfg(&isp_cfg_day);
                    lv_value_pre = lv_value;
                    puts("\n isp_cfg_day \n");

//                    #ifdef FLASH_LIGHT  //JOHNSON 20160531
//                    flash_light_ctrl(FALSE);
//                    #endif
                }
            }
        }
    }



    #endif
#endif
}



