

#ifndef _ISP_DEV_H
#define _ISP_DEV_H

#include "typedef.h"
#include "ioctl.h"
#include "device_drive.h"
#include "errno-base.h"


/*isp_ioctrl command*/
#define ISP_DEV_MAGIC  			'i'
#define ISP_CMD_SET_INPUT_SIZE	_IOW(ISP_DEV_MAGIC, 5, pic_size_t)
#define ISP_CMD_SET_OUTPUT_SIZE	_IOW(ISP_DEV_MAGIC, 6, pic_size_t)
#define ISP_CMD_GET_SEN_SIZE		_IOR(ISP_DEV_MAGIC, 7, pic_size_t)
#define ISP_CMD_GET_ISP_SIZE		_IOR(ISP_DEV_MAGIC, 8, pic_size_t)
#define ISP_CMD_GET_FREQ			_IOR(ISP_DEV_MAGIC, 9, u8)
#define ISP_CMD_GET_SEN_STATUS	_IOR(ISP_DEV_MAGIC, 10, u8)

/*************************************************************/
/************************ ISP_CTRL ************************/
/*************************************************************/

/*isp sensor control function*/
typedef struct{
	s32 (* set_size)(u16 *width, u16 *height, u8 *freq);		/*sensor size control*/
	s32 (* power_ctrl)(u8 isp_dev, u8 is_work);			/*sensor power control*/
} S_SENSOR_CTRL_FUN;


/*************************************************************/
/************************* S_ISP_SEN **************************/
/*************************************************************/

typedef struct _isp0_register
{
    volatile u16 reg_change;

    u16 risp0_sen_con;
    u16 risp0_sen_vblk;
    u16 risp0_sen_vact;
    u16 risp0_sen_hblk;
    u16 risp0_sen_hact;
    u16 risp0_sen_hinv;

    u16 risp0_sta_win_width;
    u16 risp0_sta_win_high;
    u16 risp0_sta_base_h;
    u16 risp0_sta_base_l;

    u16 risp0_blc_off_r;
    u16 risp0_blc_off_g;
    u16 risp0_blc_off_b;

    u16 risp0_dpc_max_th;
    u16 risp0_dpc_min_th;

    u16 risp0_lsc_cen_x;
    u16 risp0_lsc_cen_y;
    u16 risp0_lsc_lut_cnt;

    u16 risp0_awb_gain_r;
    u16 risp0_awb_gain_g;
    u16 risp0_awb_gain_b;

    u16 risp0_drc_con;
    u16 risp0_drc_lut_cnt;

    u16 risp0_sta_ae_con;
    u16 risp0_sta_ae_lvl1;
    u16 risp0_sta_ae_lvl2;
    u16 risp0_sta_ae_lvl3;
    u16 risp0_sta_ae_lvl4;
    u16 risp0_sta_ae_lvl5;
    u16 risp0_sta_ae_lvl6;
    u16 risp0_sta_ae_lvl7;

    u16 risp0_sta_wb_con;
    u16 risp0_sta_wb_w_th;
    u16 risp0_sta_wb_r_th;
    u16 risp0_sta_wb_g_th;
    u16 risp0_sta_wb_b_th;

    u16 risp0_ccm_r_coe0;
    u16 risp0_ccm_r_coe1;
    u16 risp0_ccm_r_coe2;
    u16 risp0_ccm_g_coe0;
    u16 risp0_ccm_g_coe1;
    u16 risp0_ccm_g_coe2;
    u16 risp0_ccm_b_coe0;
    u16 risp0_ccm_b_coe1;
    u16 risp0_ccm_b_coe2;
    u16 risp0_ccm_r_off;
    u16 risp0_ccm_g_off;
    u16 risp0_ccm_b_off;

    u16 risp0_gma0_con;
    u8 risp0_gma_r_lut_buf[256];
    u8 risp0_gma_g_lut_buf[256];
    u8 risp0_gma_b_lut_buf[256];
//    u16 risp0_gma_r_lut;
//    u16 risp0_gma_g_lut;
//    u16 risp0_gma_b_lut;
    u16 risp0_gma1_con;
//    u16 risp0_gma_y_lut;
    u8 risp0_gma_y_lut_buf[256];

    u16 risp0_shp_amount;
    u16 risp0_shp_lone_th;
    u16 risp0_shp_en_th;
    u16 risp0_shp_max_th;
    u16 risp0_shp_min_th;
    u16 risp0_shp_coe0;
    u16 risp0_shp_coe1;
    u16 risp0_shp_coe2;
    u16 risp0_shp_coe3;
    u16 risp0_shp_coe4;
    u16 risp0_shp_coe5;
    u16 risp0_shp_shft;

    u16 risp0_smt_en_th;
    u16 risp0_smt_coe0;
    u16 risp0_smt_coe1;
    u16 risp0_smt_coe2;
    u16 risp0_smt_coe3;
    u16 risp0_smt_coe4;
    u16 risp0_smt_coe5;
    u16 risp0_smt_shft;

    u16 risp0_dnr_mod;
    u16 risp0_dnr_th;

    u16 risp0_cbs_y_gain;
    u16 risp0_cbs_u_gain;
    u16 risp0_cbs_v_gain;
    u16 risp0_cbs_y_offs;
    u16 risp0_cbs_u_offs;
    u16 risp0_cbs_v_offs;

    u16 risp0_out_hst;
    u16 risp0_out_hed;
    u16 risp0_out_vst;
    u16 risp0_out_ved;
    u16 risp0_out_base_h;
    u16 risp0_out_base_l;

}isp0_register;

typedef enum {
    AE_BF_NOTCARE=0,
    AE_BF_50HZ,
    AE_BF_60HZ,
}Sensor_AE_BF_TYPE;


/*isp sensor config parameters*/
typedef struct S_ISP_SEN {
	u8 logo[8];		/*sensor logo*/ /*e.g "GC0308"*/
	u8 isp_dev;		/*ISP device*/
	u8 status;		/*sensor status*/
	u8 vsync_mode;	/*Vertical Sync mode*/
	u8 in_format;		/*(raw input)0:R; 1:Gr; 2:Gb; 3:B;
					   (yuv input)4:YUYV; 5:UYVY*/
	u8 out_format;	/*0:yuv; 1:arw*/
	u8 frame_freq;		/*frame frequency*/ /* e.g 30 frame/s*/
	u16 isp0_sen_config;	/*isp0 sensor config*/
	u16 isp1_sen_config;	/*isp1 sensor config*/
	u16 ie_pend_con;	/*sensor interrupt or pending config*/
	pic_size_t sen_size;	/*sensor size*/
	pic_size_t isp_size;	/*isp output size*/
	s32 (* sensor_check)(u8 isp_dev);	/*sensor check online*/
	/*camera init callback function*/
	/*e.g xclk io\frequency init; sensor reset; sensor width\height\format\frame_freq set...*/
	s32 (* camera_init)(u8 isp_dev, u16 *width, u16 *height, u8 *format, u8 *frame_freq);
	/*isp sensor control*/
	S_SENSOR_CTRL_FUN ctrl;

	u16 sen_hinv;

	s32 (*update_cfg)(u32 cur_mean);
	s32 (*flush_cfg)();
    void (*camera_sleep)();
    void (*camera_wakeup)();
    void (*WrSensorReg)(u16 addr , u16 val);
    u16  (*RdSensorReg)(u16 addr);

    void (*set_AE_bf_type)(Sensor_AE_BF_TYPE ae_bf); //set banding filter type, i.e. anti 50hz/60hz
}isp_sen_t;

/*S_ISP_SEN : vsync_mode*/
/*sensor Vertical Sync mode*/
enum{
	SEN_VSYNC_NORMAL = 0,
	SEN_VSYNC_ODD_EVEN,
};


/*S_ISP_SEN : in_format*/
/*sensor in format*/
enum{
	SEN_IN_FORMAT_RGRG = 0,
	SEN_IN_FORMAT_GRGR,
	SEN_IN_FORMAT_GBGB,
	SEN_IN_FORMAT_BGBG,
	SEN_IN_FORMAT_YUYV,
	SEN_IN_FORMAT_UYVY,
};

/*S_ISP_SEN : out_format*/
/*isp out format*/
enum{
	ISP_OUT_FORMAT_YUV = 0,
	ISP_OUT_FORMAT_RAW,
};


/*S_ISP_SEN : isp0_sen_con*/
/*isp0 sensor config*/
#define ISP0_SEN_SE_EN			BIT(0)	/*0:sensor enable;	1:sensor disable*/
#define ISP0_SEN_CK_MOD			BIT(1)	/*0:use pclk;	1:invert pclk*/
#define ISP0_SEN_HS_MOD			BIT(2)	/*0:use hsync;	1:invert hsync*/
#define ISP0_SEN_VS_MOD			BIT(3)	/*0:use vsync;	1:invert vsync*/
#define ISP0_SEN_D8_MOD			BIT(4)	/*0:10 bit mode;	1:8 bit mode*/
#define ISP0_SEN_REV_IDATA		BIT(5)	/*0:normal; 1:reverse(8bit only)*/
#define ISP0_SEN_I656_MOD		BIT(6)	/*0:DVP mode;	1:itu656 mode 2:itu601 mode*/
#define ISP0_SEN_I601_MOD		BIT(7)	/*itu601 mode*/

/*S_ISP_SEN : isp1_sen_con*/
/*isp1 sensor config*/
#define ISP1_SEN_SE_EN			BIT(0)	/*0:sensor enable;	1:sensor disable*/
#define ISP1_SEN_CK_MOD			BIT(1)	/*0:use pclk;	1:invert pclk*/
#define ISP1_SEN_HS_MOD			BIT(2)	/*0:use hsync;	1:invert hsync*/
#define ISP1_SEN_VS_MOD			BIT(3)	/*0:use vsync;	1:invert vsync*/
#define ISP1_SEN_D16_MOD		BIT(4)	/*0:8 bit mode;	1:16 bit mode*/
#define ISP1_SEN_I601_MOD		BIT(5)	/*0:HREF mode;	1:HSYNC mode*/
#define ISP1_SEN_I656_MOD		BIT(6)	/*0:DVP mode;	1:itu656 mode*/
#define ISP1_SEN_INTERLACE		BIT(7)	/*0:progress;	1:interlace*/
#define ISP1_SEN_FIELD_MOD		BIT(8)	/*0:normal;	1:reverse field state*/
#define ISP1_SEN_YUV_PHASE		BIT(9)	/*0:YUYV;	1:UYVY*/
#define ISP1_SEN_OUT_SEL		BIT(10)	/*0:normal;	1:de-interlace output*/
#define ISP1_SEN_IO_REVERSE		BIT(11)	/*0:normal;	1:reverse*/

/*S_ISP_SEN : ie_pend_con*/
/*isp0 sensor ie*/
#define SEN_F_IE        BIT(0)//sensor frame interrupt enable
#define SEN_L_IE        BIT(1)//sensor line interrupt enable
#define OUT_F_IE        BIT(2)//output frame interrupt enable
#define OUT_L_IE        BIT(3)//output line interrupt enable
#define CFG_D_IE        BIT(4)//config done interrupt enable
#define RAW_D_IE        BIT(5)//raw_data done interrupt enable



/*S_ISP_SEN : ie_pend_con*/
/*isp1 sensor ie*/
#define SEN_IN_F_EN		BIT(0)	/*sensor input frame pending enable*/
#define SEN_IN_F_IE		BIT(1)	/*sensor input frame interrupt enable*/
#define SEN_IN_L_EN		BIT(2)	/*sensor input line pending enable*/
#define SEN_IN_L_IE		BIT(3)	/*sensor input line interrupt enable*/



/*isp0 work mode*/
enum{
	ISP_WORK_RAW_TO_YUV = 1,
	ISP_WORK_YUV_BYPASS,
	ISP_WORK_RAW_BYPASS,
};


///////////////////////////////////////////////////////////////

#define ISP_XCLK_MAPOUT()     IOMC1 |= (0xf<<12)


//278行右斜线是转行符号，下一行内容继续 5203 JOHNSON 20160509
//#define ISP0_XCLK_IO_ABLE()		do{PORTH_DIR &= ~BIT(15) ;PORTH_PD |= BIT(15);PORTH_PU |= BIT(15);PORTH_DIE |= BIT(15);}while(0)
#define ISP0_XCLK_IO_ABLE()		do{PORTH_DIR &= ~BIT(14) ;PORTH_PD |= BIT(14);PORTH_PU |= BIT(14);PORTH_DIE |= BIT(14);\
                                   PORTH_DIR &= ~BIT(12) ;PORTH_PD |= BIT(12);PORTH_PU |= BIT(12);PORTH_DIE |= BIT(12);}while(0)
#define ISP1_XCLK_IO_ABLE()



#define ISP0_XCLK_IO_DISABLE()
#define ISP1_XCLK_IO_DISABLE()

s32 isp1_mount(isp_sen_t *parm) ;
s32 isp1_unmount();
s32 isp1_ioctrl(void *parm , u32 cmd);
s32 isp1_power(u32 mod);



s32 isp0_mount(isp_sen_t *parm) ;
s32 isp0_unmount();
s32 isp0_ioctrl(void *parm , u32 cmd);
s32 isp0_power(u32 mod);



dev_io_t * get_isp0_ops(void *parm);
dev_io_t * get_isp1_ops(void *parm);


extern isp0_register isp0_reg;
u32 isp0_set_updata();
isp0_register * isp0_get_regptr();

#endif


