#ifndef _SC1045_H
#define  _SC1045_H

#define SC1045_MCLK                       24
//#define SC1045_PCLK_DIV                   0  //EXT_Clk_Div_1
//#define SC1045_PCLK_MUL                   0  //PLL_CKO1_Div

/*The pixel clock runs at a frequency rate determined by the sensor?¡¥s master
input clock (MCLK) and internal PLL configuration, where the rising edge of the PCLK signal occurs at one-half
of each pixel clock period after the transitions in HREF, VSYNC, and DOUT. (see the below: pixel data timing
example / row timing and HREF, VSYNC signals)*/
//#define SC1045_PCLK                       36 //(36*6/5)  //(SC1045_MCLK*3*(1<<SC1045_PCLK_MUL)/(2*(1<<SC1045_PCLK_DIV)))  // PLL Bypass==0
u32 sc1045_pclk;
#define SC1045_CLK_PER_ROW                1920
#define SC1045_ROW_TIME_US                (SC1045_CLK_PER_ROW / sc1045_pclk)   //us
#define SC1045_GET_SHUTTER(exp_time)      exp_time/(SC1045_ROW_TIME_US)

#define SC1045_OUTPUT_W  1280
#define SC1045_OUTPUT_H  720

s32 SC1045_set_output_size(u16 *width, u16 *height, u8 *freq);
s32 SC1045_power_ctl(u8 isp_dev, u8 is_work);
s32 SC1045_init(u8 isp_dev, u16 *width, u16 *height, u8 *format, u8 *frame_freq);
s32 SC1045_check(u8 isp_dev);
s32 SC1045_update_cfg(u32 cur);
s32 SC1045_flush_cfg();
void SC1045_sleep();
void SC1045_wakeup();
void SC1045_W_Reg(u16 addr, u16 val);
u16 SC1045_R_Reg(u16 addr);

u32 SC1045_MAX_EV_LEVEL;
//#define SC1045_MAX_EV_LEVEL    141
//#define SC1045_MAX_EV_LEVEL    156  // 192 for 25 fps

#define SC1045_TARGET_LUMA     80   //68

typedef struct
{
    u32 exp_time;
    u16 a_gain;
    u8 d_gain;
    u8 ev_level;
} SC1045_AE_lut_t;   // ppk add on 20151015 refering NT99143 tbl.c

/*--------------------from H42------------------------*/
extern SC1045_AE_lut_t SC1045_AE_lut_50Hz_min_30fps[];
extern SC1045_AE_lut_t SC1045_AE_lut_min_30fps[];
extern SC1045_AE_lut_t SC1045_AE_lut_50Hz_min_25fps[];
extern SC1045_AE_lut_t SC1045_AE_lut_50Hz_min_20fps[];
extern SC1045_AE_lut_t SC1045_AE_lut_50Hz_min_16fps[];
extern SC1045_AE_lut_t SC1045_AE_lut_50Hz_min_33fps[];

#endif

