#ifndef _NT99143_H
#define  _NT99143_H

#define N143_MCLK                       24
#define N143_PCLK_DIV                   0  //EXT_Clk_Div_1
#define N143_PCLK_MUL                   0  //PLL_CKO1_Div

/*The pixel clock runs at a frequency rate determined by the sensor¡¯s master
input clock (MCLK) and internal PLL configuration, where the rising edge of the PCLK signal occurs at one-half
of each pixel clock period after the transitions in HREF, VSYNC, and DOUT. (see the below: pixel data timing
example / row timing and HREF, VSYNC signals)*/
#define N143_PCLK                       (N143_MCLK*3*(1<<N143_PCLK_MUL)/(2*(1<<N143_PCLK_DIV)))  // PLL Bypass==0
#define N143_CLK_PER_ROW                0x632
#define N143_ROW_TIME_US                (N143_CLK_PER_ROW / N143_PCLK)   //us
#define N143_GET_SHUTTER(exp_time)      exp_time/(N143_ROW_TIME_US)

#define N143_OUTPUT_W  1280
#define N143_OUTPUT_H  720

//是否启动spec隐藏的0x32CF作为A Gain调节的方法；
// A Gain =  (2^msb4b) * (1+(lsb4b/16))
#define NT99143_NEW_A_GAIN              1

s32 NT99143_set_output_size(u16 *width, u16 *height, u8 *freq);
s32 NT99143_power_ctl(u8 isp_dev, u8 is_work);
s32 NT99143_init(u8 isp_dev, u16 *width, u16 *height, u8 *format, u8 *frame_freq);
s32 NT99143_check(u8 isp_dev);
s32 NT99143_update_cfg(u32 cur);
s32 NT99143_flush_cfg();
void NT99143_sleep();
void NT99143_wakeup();
void NT99143_W_Reg(u16 addr, u16 val);
u16 NT99143_R_Reg(u16 addr);
//#define NT99143_MAX_EV_LEVEL    141
#define NT99143_MAX_EV_LEVEL    122

#define NT99143_TARGET_LUMA     68

typedef struct
{
    u32 exp_time;
    u8 a_gain;
    u8 d_gain;
    u8 ev_level;
} NT99143_AE_lut_t;


#endif

