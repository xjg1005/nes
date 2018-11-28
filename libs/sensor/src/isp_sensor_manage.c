
#include "isp_sensor_manage.h"
#include "isp_com.h"

#include "gc0308.h"
#include "ov9710.h"
//#include "ov9650.h"
//#include "saa7113.h"
#include "gc1004.h"
#include "ov9712.h"
#include "NT99140.h"
#include "NT99143.h"
#include "gm7150.h"
#include "H42.h"
#include "SC1045.h"
#include "PS4100.h"
//#include "BG070.h"
//u8 g_isp0_sen_dev_cur;		/*当前挂在ISP0上的摄像头*/
//u8 g_isp1_sen_dev_cur;		/*当前挂在ISP1上的摄像头*/


static isp_sen_t  gsc_isp_sen_dev[] =
{
//	{
//		"GC0308",		/*u8 logo[8]*/
//		ISP_DEV_NONE,	/*u8 isp_dev*/
//		0,				/*u8 status*/
//		SEN_VSYNC_NORMAL,		/*u8 vsync_mode;*/
//		SEN_IN_FORMAT_RGRG,	/*u8 in_format;	*/
//		ISP_OUT_FORMAT_YUV,	/*u8 out_format;*/
//		0,						/*u8 frame_freq;*/
//		ISP0_SEN_SE_EN | ISP0_SEN_D8_MOD,	/*u16 isp0_sen_config;*/
//		ISP1_SEN_SE_EN,			/*u16 isp1_sen_config;*/
//		CFG_D_IE,		/*u16 ie_pend_con;*/
//		{648, 488,},	/*sensor size*/
//		{0, 0,},	/*isp output size*/
//		GC0308_check,	/*s32 (* sensor_check)(u8 isp_dev)*/
//		GC0308_init,		/*s32 (* camera_init)(u8 isp_dev, u16 *width, u16 *height, u8 *format, u8 *frame_freq);*/
//		{/*S_SENSOR_CTRL_FUN ctrl;*/
//			GC0308_set_output_size,	/*s32 (* set_size)(u16 *width, u16 *height, u8 *freq);*/
//			GC0308_power_ctl,		/*s32 (* power_ctrl)(u32 mod);*/
//		},
//		100,
//	},

    {
		"GC0308",		/*u8 logo[8]*/
		ISP_DEV_NONE,	/*u8 isp_dev*/
		0,				/*u8 status*/
		SEN_VSYNC_NORMAL,		/*u8 vsync_mode;*/
		SEN_IN_FORMAT_UYVY,	/*u8 in_format;	*/
		ISP_OUT_FORMAT_YUV,	/*u8 out_format;*/
		0,						/*u8 frame_freq;*/
		ISP0_SEN_SE_EN | ISP0_SEN_D8_MOD,	/*u16 isp0_sen_config;*/
		ISP1_SEN_SE_EN,			/*u16 isp1_sen_config;*/
		CFG_D_IE,		/*u16 ie_pend_con;*/
		{640, 480,},	/*sensor size*/
		{0, 0,},	/*isp output size*/
		GC0308_check,	/*s32 (* sensor_check)(u8 isp_dev)*/
		GC0308_init,		/*s32 (* camera_init)(u8 isp_dev, u16 *width, u16 *height, u8 *format, u8 *frame_freq);*/
		{/*S_SENSOR_CTRL_FUN ctrl;*/
			GC0308_set_output_size,	/*s32 (* set_size)(u16 *width, u16 *height, u8 *freq);*/
			GC0308_power_ctl,		/*s32 (* power_ctrl)(u32 mod);*/
		},
		100,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,//regW
		NULL,//regR
		NULL,//banding filter
	},

	/*----------------sc1045---------------------*/
#if 1
{
		"SC1045",		/*u8 logo[8]*/
		ISP_DEV_NONE,	/*u8 isp_dev*/
		0,				/*u8 status*/
		SEN_VSYNC_NORMAL,		/*u8 vsync_mode;*/
		SEN_IN_FORMAT_BGBG,	/*u8 in_format;	*/
		ISP_OUT_FORMAT_YUV,	/*u8 out_format;*/
		30,						/*u8 frame_freq;*/
		ISP0_SEN_SE_EN | ISP0_SEN_D8_MOD ,	/*u16 isp0_sen_config;*/
		ISP1_SEN_SE_EN ,			/*u16 isp1_sen_config;*/
		CFG_D_IE,		/*u16 ie_pend_con;*/
		//{1288, 728,},	/*sensor size*/
		{SC1045_OUTPUT_W, SC1045_OUTPUT_H},	/*sensor size*/
		{SC1045_OUTPUT_W, SC1045_OUTPUT_H},	/*isp output size*/
	//	{1280, 720,},	/*isp output size*/
		SC1045_check,	/*s32 (* sensor_check)(u8 isp_dev)*/
		SC1045_init,		/*s32 (* camera_init)(u8 isp_dev, u16 *width, u16 *height, u8 *format, u8 *frame_freq);*/
		{/*S_SENSOR_CTRL_FUN ctrl;*/
			SC1045_set_output_size,	/*s32 (* set_size)(u16 *width, u16 *height, u8 *freq);*/
			SC1045_power_ctl,		/*s32 (* power_ctrl)(u32 mod);*/
		},
		480,
		SC1045_update_cfg,
		SC1045_flush_cfg,
		SC1045_sleep,
		SC1045_wakeup,
		SC1045_W_Reg,//regW
		SC1045_R_Reg,//regR
		NULL,
	},

#endif

    {
		"PS4100",		/*u8 logo[8]*/
		ISP_DEV_NONE,	/*u8 isp_dev*/
		0,				/*u8 status*/
		SEN_VSYNC_NORMAL,		/*u8 vsync_mode;*/
		SEN_IN_FORMAT_BGBG,	/*u8 in_format;	*/
		ISP_OUT_FORMAT_YUV,	/*u8 out_format;*/
		30,						/*u8 frame_freq;*/
		ISP0_SEN_SE_EN | ISP0_SEN_D8_MOD ,	/*u16 isp0_sen_config;*/
		ISP1_SEN_SE_EN ,			/*u16 isp1_sen_config;*/
		CFG_D_IE,		/*u16 ie_pend_con;*/
		//{1288, 728,},	/*sensor size*/
		{PS4100_OUTPUT_W, PS4100_OUTPUT_H},	/*sensor size*/
		{PS4100_OUTPUT_W, PS4100_OUTPUT_H},	/*isp output size*/
	//	{1280, 720,},	/*isp output size*/
		PS4100_check,	/*s32 (* sensor_check)(u8 isp_dev)*/
		PS4100_init,		/*s32 (* camera_init)(u8 isp_dev, u16 *width, u16 *height, u8 *format, u8 *frame_freq);*/
		{/*S_SENSOR_CTRL_FUN ctrl;*/
			PS4100_set_output_size,	/*s32 (* set_size)(u16 *width, u16 *height, u8 *freq);*/
			PS4100_power_ctl,		/*s32 (* power_ctrl)(u32 mod);*/
		},
		480,
		PS4100_update_cfg,
		PS4100_flush_cfg,
		PS4100_sleep,
		PS4100_wakeup,
		PS4100_W_Reg,//regW
		PS4100_R_Reg,//regR
		NULL,
	},
#if 0 //5203 JOHNSON 20160509
{
		"BG070X",		/*u8 logo[8]*/
		ISP_DEV_NONE,	/*u8 isp_dev*/
		0,				/*u8 status*/
		SEN_VSYNC_NORMAL,		/*u8 vsync_mode;*/
		SEN_IN_FORMAT_BGBG,	/*u8 in_format;	*/
		ISP_OUT_FORMAT_YUV,	/*u8 out_format;*/
		30,						/*u8 frame_freq;*/
		ISP0_SEN_SE_EN | ISP0_SEN_D8_MOD ,	/*u16 isp0_sen_config;*/
		ISP1_SEN_SE_EN ,			/*u16 isp1_sen_config;*/
		CFG_D_IE,		/*u16 ie_pend_con;*/
		//{1288, 728,},	/*sensor size*/
		{BG070X_OUTPUT_W, BG070X_OUTPUT_H},	/*sensor size*/
		{BG070X_OUTPUT_W, BG070X_OUTPUT_H},	/*isp output size*/
	//	{1280, 720,},	/*isp output size*/
		BG070X_check,	/*s32 (* sensor_check)(u8 isp_dev)*/
		BG070X_init,		/*s32 (* camera_init)(u8 isp_dev, u16 *width, u16 *height, u8 *format, u8 *frame_freq);*/
		{/*S_SENSOR_CTRL_FUN ctrl;*/
			BG070X_set_output_size,	/*s32 (* set_size)(u16 *width, u16 *height, u8 *freq);*/
			BG070X_power_ctl,		/*s32 (* power_ctrl)(u32 mod);*/
		},
		480,
		BG070X_update_cfg,
		BG070X_flush_cfg,
		BG070X_sleep,
		BG070X_wakeup,
		BG070X_W_Reg,//regW
		BG070X_R_Reg,//regR
		NULL,
	},
#endif
	{
		"OV9710",		/*u8 logo[8]*/
		ISP_DEV_NONE,	/*u8 isp_dev*/
		0,				/*u8 status*/
		SEN_VSYNC_NORMAL,		/*u8 vsync_mode;*/
		SEN_IN_FORMAT_BGBG,	/*u8 in_format;	*/
		ISP_OUT_FORMAT_YUV,	/*u8 out_format;*/
		0,						/*u8 frame_freq;*/
		ISP0_SEN_SE_EN | ISP0_SEN_D8_MOD,	/*u16 isp0_sen_config;*/
		ISP1_SEN_SE_EN | ISP1_SEN_CK_MOD,			/*u16 isp1_sen_config;*/
		CFG_D_IE,		/*u16 ie_pend_con;*/
		{1280, 720},	/*sensor size*/
		{1280, 720},	/*isp output size*/
		OV9710_check,
		OV9710_init,
		{/*S_SENSOR_CTRL_FUN ctrl;*/
			OV9710_set_output_size,	/*s32 (* set_size)(u16 *width, u16 *height, u8 *freq);*/
			OV9710_power_ctl,		/*s32 (* power_ctrl)(u32 mod);*/
		},
		480,
		NULL,
		NULL,
		OV9710_sleep,
		OV9710_wakeup,
		OV9710_W_Reg,//regW
		OV9710_R_Reg,//regR
		NULL,//banding filter
	},

	{
		"NT9914?",		/*u8 logo[8]*/
		ISP_DEV_NONE,	/*u8 isp_dev*/
		0,				/*u8 status*/
		SEN_VSYNC_NORMAL,		/*u8 vsync_mode;*/
		SEN_IN_FORMAT_YUYV,	/*u8 in_format;	*/
		ISP_OUT_FORMAT_YUV,	/*u8 out_format;*/
		0,						/*u8 frame_freq;*/
		ISP0_SEN_SE_EN | ISP0_SEN_D8_MOD,	/*u16 isp0_sen_config;*/
		ISP1_SEN_SE_EN ,			/*u16 isp1_sen_config;*/
		CFG_D_IE,		/*u16 ie_pend_con;*/
		{1280, 720,},	/*sensor size*/
		{0, 0,},//{1280, 720,},	/*isp output size*/
		NT99140_check,
		NT99140_init,
		{/*S_SENSOR_CTRL_FUN ctrl;*/
			NT99140_set_output_size,	/*s32 (* set_size)(u16 *width, u16 *height, u8 *freq);*/
			NT99140_power_ctl,		/*s32 (* power_ctrl)(u32 mod);*/
		},
        50,
		NULL,
		NULL,
		NULL,
		NULL,
		NT99140_W_Reg,//regW
		NT99140_R_Reg,//regR
		NULL,//banding filter
	},
	{
		"NT99143",		/*u8 logo[8]*/
		ISP_DEV_NONE,	/*u8 isp_dev*/
		0,				/*u8 status*/
		SEN_VSYNC_NORMAL,		/*u8 vsync_mode;*/
		SEN_IN_FORMAT_RGRG,	/*u8 in_format;	*/
		ISP_OUT_FORMAT_YUV,	/*u8 out_format;*/
		0,						/*u8 frame_freq;*/
		ISP0_SEN_SE_EN | ISP0_SEN_D8_MOD,	/*u16 isp0_sen_config;*/
		ISP1_SEN_SE_EN ,			/*u16 isp1_sen_config;*/
		CFG_D_IE,		/*u16 ie_pend_con;*/
		{N143_OUTPUT_W, N143_OUTPUT_H},	/*sensor size*/
		{N143_OUTPUT_W, N143_OUTPUT_H},//{1280, 720,},	/*isp output size*/
		NT99143_check,	/*s32 (* sensor_check)(u8 isp_dev)*/
		NT99143_init,		/*s32 (* camera_init)(u8 isp_dev, u16 *width, u16 *height, u8 *format, u8 *frame_freq);*/
		{/*S_SENSOR_CTRL_FUN ctrl;*/
			NT99143_set_output_size,	/*s32 (* set_size)(u16 *width, u16 *height, u8 *freq);*/
			NT99143_power_ctl,		/*s32 (* power_ctrl)(u32 mod);*/
		},
        50,
        NT99143_update_cfg,
        NT99143_flush_cfg,
		NULL,
		NULL,
		NT99143_W_Reg,//regW
		NT99143_R_Reg,//regR
		NULL,//banding filter
	},

	{
		"GC1004",		/*u8 logo[8]*/
		ISP_DEV_NONE,	/*u8 isp_dev*/
		0,				/*u8 status*/
		SEN_VSYNC_NORMAL,		/*u8 vsync_mode;*/
		SEN_IN_FORMAT_RGRG,	/*u8 in_format;	*/
		ISP_OUT_FORMAT_YUV,	/*u8 out_format;*/
		30,						/*u8 frame_freq;*/
		ISP0_SEN_SE_EN | ISP0_SEN_D8_MOD ,	/*u16 isp0_sen_config;*/
		ISP1_SEN_SE_EN ,			/*u16 isp1_sen_config;*/
		CFG_D_IE,		/*u16 ie_pend_con;*/
		//{1288, 728,},	/*sensor size*/
		{GC1004_OUTPUT_W, GC1004_OUTPUT_H},	/*sensor size*/
		{GC1004_OUTPUT_W, GC1004_OUTPUT_H},	/*isp output size*/
	//	{1280, 720,},	/*isp output size*/
		GC1004_check,	/*s32 (* sensor_check)(u8 isp_dev)*/
		GC1004_init,		/*s32 (* camera_init)(u8 isp_dev, u16 *width, u16 *height, u8 *format, u8 *frame_freq);*/
		{/*S_SENSOR_CTRL_FUN ctrl;*/
			GC1004_set_output_size,	/*s32 (* set_size)(u16 *width, u16 *height, u8 *freq);*/
			GC1004_power_ctl,		/*s32 (* power_ctrl)(u32 mod);*/
		},
		480,
		GC1004_update_cfg,
		GC1004_flush_cfg,
		GC1004_sleep,
		GC1004_wakeup,
		GC1004_W_Reg,//regW
		GC1004_R_Reg,//regR
		GC1004_set_ae_bf_type,//banding filter
	},
	{
		"H42",		/*u8 logo[8]*/
		ISP_DEV_NONE,	/*u8 isp_dev*/
		0,				/*u8 status*/
		SEN_VSYNC_NORMAL,		/*u8 vsync_mode;*/
		SEN_IN_FORMAT_BGBG,	/*u8 in_format;	*/
		ISP_OUT_FORMAT_YUV,	/*u8 out_format;*/
		25,						/*u8 frame_freq; 帧数设置 25 FPS 或者 30 FPS*/
		ISP0_SEN_SE_EN | ISP0_SEN_D8_MOD ,	/*u16 isp0_sen_config;*/
		ISP1_SEN_SE_EN ,			/*u16 isp1_sen_config;*/
		CFG_D_IE,		/*u16 ie_pend_con;*/
		//{1288, 728,},	/*sensor size*/
		{H42_OUTPUT_W, H42_OUTPUT_H},	/*sensor size*/
		{H42_OUTPUT_W, H42_OUTPUT_H},	/*isp output size*/
	//	{1280, 720,},	/*isp output size*/
		H42_check,	/*s32 (* sensor_check)(u8 isp_dev)*/
		H42_init,		/*s32 (* camera_init)(u8 isp_dev, u16 *width, u16 *height, u8 *format, u8 *frame_freq);*/
		{/*S_SENSOR_CTRL_FUN ctrl;*/
			H42_set_output_size,	/*s32 (* set_size)(u16 *width, u16 *height, u8 *freq);*/
			H42_power_ctl,		/*s32 (* power_ctrl)(u32 mod);*/
		},
		480,
		H42_update_cfg,
		H42_flush_cfg,
		H42_sleep,
		H42_wakeup,
		H42_W_Reg,//regW
		H42_R_Reg,//regR
		H42_set_ae_bf_type,//banding filter
	},
#if 0 //JOHNSON 20160607
    {
		"sc1045",		/*u8 logo[8]*/
		ISP_DEV_NONE,	/*u8 isp_dev*/
		0,				/*u8 status*/
		SEN_VSYNC_NORMAL,		/*u8 vsync_mode;*/
		SEN_IN_FORMAT_BGBG,	/*u8 in_format;	*/
		ISP_OUT_FORMAT_YUV,	/*u8 out_format;*/
		30,						/*u8 frame_freq;*/
		ISP0_SEN_SE_EN | ISP0_SEN_D8_MOD ,	/*u16 isp0_sen_config;*/
		ISP1_SEN_SE_EN ,			/*u16 isp1_sen_config;*/
		CFG_D_IE,		/*u16 ie_pend_con;*/
		//{1288, 728,},	/*sensor size*/
		{SC1045_OUTPUT_W, SC1045_OUTPUT_H},	/*sensor size*/
		{SC1045_OUTPUT_W, SC1045_OUTPUT_H},	/*isp output size*/
	//	{1280, 720,},	/*isp output size*/
		SC1045_check,	/*s32 (* sensor_check)(u8 isp_dev)*/
		SC1045_init,		/*s32 (* camera_init)(u8 isp_dev, u16 *width, u16 *height, u8 *format, u8 *frame_freq);*/
		{/*S_SENSOR_CTRL_FUN ctrl;*/
			SC1045_set_output_size,	/*s32 (* set_size)(u16 *width, u16 *height, u8 *freq);*/
			SC1045_power_ctl,		/*s32 (* power_ctrl)(u32 mod);*/
		},
		480,
		SC1045_update_cfg,
		SC1045_flush_cfg,
		SC1045_sleep,
		SC1045_wakeup,
		SC1045_W_Reg,//regW
		SC1045_R_Reg,//regR
		NULL,//banding filter
	},
#endif
#if 0
    {
		"OV9712",		/*u8 logo[8]*/
		ISP_DEV_NONE,	/*u8 isp_dev*/
		0,				/*u8 status*/
		SEN_VSYNC_NORMAL,		/*u8 vsync_mode;*/
		SEN_IN_FORMAT_BGBG,	/*u8 in_format;	*/
		ISP_OUT_FORMAT_YUV,	/*u8 out_format;*/
		30,						/*u8 frame_freq;*/
		ISP0_SEN_SE_EN | ISP0_SEN_D8_MOD,	/*u16 isp0_sen_config;*/
		ISP1_SEN_SE_EN,			/*u16 isp1_sen_config;*/
		CFG_D_IE,		/*u16 ie_pend_con;*/
		{1280, 720,},	/*sensor size*/
		{1280, 720,},	/*isp output size*/
		OV9712_check,
		OV9712_init,
		{/*S_SENSOR_CTRL_FUN ctrl;*/
			OV9712_set_output_size,	/*s32 (* set_size)(u16 *width, u16 *height, u8 *freq);*/
			OV9712_power_ctl,		/*s32 (* power_ctrl)(u32 mod);*/
		},
		480,
		NULL,
		NULL,
		OV9712_sleep,
		OV9712_wakeup
	},
#endif
//	{
//		"OV9650",		/*u8 logo[8]*/
//		ISP_DEV_NONE,	/*u8 isp_dev*/
//		0,				/*u8 status*/
//		SEN_VSYNC_NORMAL,		/*u8 vsync_mode;*/
//		SEN_IN_FORMAT_UYVY,	/*u8 in_format;	*/
//		ISP_OUT_FORMAT_YUV,	/*u8 out_format;*/
//		0,						/*u8 frame_freq;*/
//		ISP0_SEN_SE_EN | ISP0_SEN_D8_MOD,	/*u16 isp0_sen_config;*/
//		ISP1_SEN_SE_EN ,			/*u16 isp1_sen_config;*/
//		CFG_D_IE,		/*u16 ie_pend_con;*/
//		{640, 480,},	/*sensor size*/
//		{0, 0,},	/*isp output size*/
//		OV9650_check,
//		OV9650_init,
//		{/*S_SENSOR_CTRL_FUN ctrl;*/
//			OV9650_set_output_size,	/*s32 (* set_size)(u16 *width, u16 *height, u8 *freq);*/
//			OV9650_power_ctl,		/*s32 (* power_ctrl)(u32 mod);*/
//		},
//		50,
//		NULL,
//		NULL,
//
//	},

//	{
//		"SAA7113",		/*u8 logo[8]*/
//		ISP_DEV_NONE,	/*u8 isp_dev*/
//		0,				/*u8 status*/
//		SEN_VSYNC_ODD_EVEN,	/*u8 vsync_mode;*/
//		SEN_IN_FORMAT_UYVY,	/*u8 in_format;	*/
//		ISP_OUT_FORMAT_YUV,	/*u8 out_format;*/
//		0,						/*u8 frame_freq;*/
//		ISP0_SEN_SE_EN | ISP0_SEN_HS_MOD | ISP0_SEN_VS_MOD | ISP0_SEN_I656_MOD | ISP0_SEN_D8_MOD,	/*u16 isp0_sen_config;*/
//        ISP1_SEN_OUT_SEL | ISP1_SEN_SE_EN | ISP1_SEN_HS_MOD | ISP1_SEN_VS_MOD | ISP1_SEN_I656_MOD | ISP1_SEN_INTERLACE | ISP1_SEN_FIELD_MOD,/*u16 isp1_sen_config;*/
//        //SEN_IN_F_EN | SEN_IN_L_EN ,		/*u16 ie_pend_con;*/
//		CFG_D_IE,		/*u16 ie_pend_con;*/
//		{720, 480,},	/*sensor size*/
//		{0, 0,},	/*isp output size*/
//		SAA7113_check,
//		SAA7113_init,
//		{/*S_SENSOR_CTRL_FUN ctrl;*/
//			SAA7113_set_output_size,	/*s32 (* set_size)(u16 *width, u16 *height, u8 *freq);*/
//			SAA7113_power_ctl,		/*s32 (* power_ctrl)(u32 mod);*/
//		},
//		0,
//		NULL,
//		NULL,
//	},

    {
		"GM7150",		/*u8 logo[8]*/
		ISP_DEV_NONE,	/*u8 isp_dev*/
		0,				/*u8 status*/
		SEN_VSYNC_ODD_EVEN,	/*u8 vsync_mode;*/
		SEN_IN_FORMAT_UYVY,	/*u8 in_format;	*/
		ISP_OUT_FORMAT_YUV,	/*u8 out_format;*/
		0,						/*u8 frame_freq;*/
		ISP0_SEN_SE_EN | ISP0_SEN_HS_MOD | ISP0_SEN_VS_MOD | ISP0_SEN_I656_MOD | ISP0_SEN_D8_MOD,	/*u16 isp0_sen_config;*/
		ISP1_SEN_OUT_SEL | ISP1_SEN_SE_EN | ISP1_SEN_HS_MOD | ISP1_SEN_VS_MOD | ISP1_SEN_I656_MOD | ISP1_SEN_INTERLACE | ISP1_SEN_FIELD_MOD,/*u16 isp1_sen_config;*/
		CFG_D_IE,		/*u16 ie_pend_con;*/
		{720, 480,},	/*sensor size*/
		{0, 0,},	    /*isp output size*/
		gm7150_check,
		gm7150_initialize,
		{/*S_SENSOR_CTRL_FUN ctrl;*/
			gm7150_set_output_size,	/*s32 (* set_size)(u16 *width, u16 *height, u8 *freq);*/
			gm7150_power_ctl,		/*s32 (* power_ctrl)(u32 mod);*/
		},
        0,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,//regW
		NULL,//regR
		NULL,//banding filter
	},

};


static u32 cur_isp_sensor;
u8 *get_cur_sensor_logo()
{
    return (gsc_isp_sen_dev[cur_isp_sensor].logo);
}

isp_sen_t* isp_sensor_check(u8 *name)
{
	s32 i;
	u8 type ;
	if(strcmp(name,"isp0") ==0)
	{
	    type = ISP_DEV_0 ;
	}
	else
	{
	    type = ISP_DEV_1 ;
	}
	u32 sensor_num = sizeof(gsc_isp_sen_dev) /sizeof(isp_sen_t) ;
	for (i=sensor_num-1; i>=0; i--)
	{
		if (gsc_isp_sen_dev[i].isp_dev == type)
		{
			return &gsc_isp_sen_dev[i];
		}
	}

	for (i=sensor_num-1; i>=0; i--)
	{
		if ( (ISP_DEV_NONE == gsc_isp_sen_dev[i].isp_dev)
            && (0 == gsc_isp_sen_dev[i].sensor_check(type)) )
		{
		    if (type == ISP_DEV_0)
		    {
		        cur_isp_sensor = i;
		    }
			printf(gsc_isp_sen_dev[i].logo);
			return &gsc_isp_sen_dev[i];
		}
	}


	return 0;
}


u8 av_in_fps_read()
{
    u8 status ;

    rdGM7150Reg(0x88, &status);
    if(status & BIT(5)){
        return 1;
    }
    return 0;

//    rdARK7116Reg(0XFCE7, &status);
//    if(status & BIT(0)){ //0:PAL(25FPS) 1:NTSC(30FPS)
//        return 0;
//    }
//    return 1;
}


u8 av_in_read()
{
    u8 status ;

    rdGM7150Reg(0x88, &status);
    if(status & 0x06){
        return 1;
    }

//    rdARK7116Reg(0XFA26, &status);
//    if(status & 0x06){
//        return 1;
//    }

    return 0;
}



extern void reset_H42();
extern void reset_gc10X4();
extern void reset_99143();
extern void reset_sc1045();

void sensor_reset_val()
{
    reset_H42();
    reset_gc10X4();
    reset_99143();
    reset_sc1045();

}



