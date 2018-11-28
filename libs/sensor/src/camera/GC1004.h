#ifndef _GC1004_H
#define _GC1004_H

#include "typedef.h"

#define GC1004_DEBUG printf

#define PCLK_48M_30FPS      0
#define PCLK_72M_50FPS      0 //not valid
#define PCLK_84M_50FPS      0
#define PCLK_96M_60FPS      1


#define HOST_MCLK           24
#define REG_SH_DELAY        0x18

#define GC_PLL_DIV2            0
#define GC_DCLK_DIV            0

#if PCLK_48M_30FPS
#undef REG_VBLANK
#define REG_VBLANK           0x10
#undef REG_HBLANK
#define REG_HBLANK          0x177
#undef GC_PLL_DIV5B
#define GC_PLL_DIV5B        0x3
#endif


#if PCLK_72M_50FPS
#undef REG_VBLANK
#define REG_VBLANK           0x4
#undef REG_HBLANK
#define REG_HBLANK          0x122  //50fps
#undef GC_PLL_DIV5B
#define GC_PLL_DIV5B        0x5  //0x5:72M pclk, 0x3:48M
#endif

#if PCLK_84M_50FPS
#undef REG_VBLANK
#define REG_VBLANK           0x4
#undef REG_HBLANK
#define REG_HBLANK          0x1c2  //50fps
#undef GC_PLL_DIV5B
#define GC_PLL_DIV5B        0x6
#endif

#if PCLK_96M_60FPS
#undef REG_VBLANK
#define REG_VBLANK           0x4
#undef REG_HBLANK
#define REG_HBLANK          0x18d  //60fps
#undef GC_PLL_DIV5B
#define GC_PLL_DIV5B        0x7
#endif

#define REG_CLK             ((HOST_MCLK*(GC_PLL_DIV5B+1)/(1+GC_PLL_DIV2))/(2*(GC_DCLK_DIV+1)))


#define GC1004_OUTPUT_W    1280
#define GC1004_OUTPUT_H     720
#define REG_WIN_WIDTH       1288
#define REG_WIN_HEIGHT      728



#define ROW_TIME_US         ((REG_HBLANK+REG_SH_DELAY+(REG_WIN_WIDTH/2)+4)/(REG_CLK/2))  //us


// fps= min(  1/[(REG_WIN_HEIGHT+VB+16)*ROW_TIME_US)], 1/(exp_time*4))

//#define GET_SHUTTER(exp_time)        (((REG_CLK/2) * 4ul * (exp_time)) / (REG_HBLANK+REG_SH_DELAY+(REG_WIN_WIDTH/2)+4))
//#define DGAIN_PRECISE     128

#define IS_GC1014         1   //1014 or 1004


#define GC1004_TARGET_LUMA  72


#define MAX_EV_LEVEL       170


#define GAIN_MAX 0x8
#define GAIN_MIN 0x0
#define ANALOG_GAIN_1 64 // 1.00x
#define ANALOG_GAIN_2 90 // 1.4x
#define ANALOG_GAIN_3 118 // 1.8x
#define ANALOG_GAIN_4 163 // 2.56x
#define ANALOG_GAIN_5 218 // 3.40x
#define ANALOG_GAIN_6 304 // 4.7x
#define ANALOG_GAIN_7 438 // 6.84x
#define ANALOG_GAIN_8 602 // 9.4x
#define ANALOG_GAIN_9 851 // 13.2x


typedef struct
{
    u32 exp_time;
    u8 a_gain;
    u8 d_gain;

    union {
        u8 g_gain;
        u8 diff_th;
    };
    u8 ev_level;

} Sensor_AE_lut_t;


s32 GC1004_set_output_size(u16 *width, u16 *height, u8 *freq);
s32 GC1004_power_ctl(u8 isp_dev, u8 is_work);

s32 GC1004_check(u8 isp_dev);
s32 GC1004_init(u8 isp_dev, u16 *width, u16 *height, u8 *format, u8 *frame_freq);
s32 GC1004_update_cfg(u32 cur);
s32 GC1004_flush_cfg();



void GC1004_sleep();
void GC1004_wakeup();
void GC1004_W_Reg(u16 addr, u16 val);
u16 GC1004_R_Reg(u16 addr);
void GC1004_set_ae_bf_type(Sensor_AE_BF_TYPE t);

extern Sensor_AE_lut_t GC1004_AE_lut_60Hz_min_30fps[];
extern Sensor_AE_lut_t GC1004_AE_lut_50Hz_min_33fps[];
extern Sensor_AE_lut_t GC1004_AE_lut_50Hz_min_25fps[];
extern Sensor_AE_lut_t GC1004_AE_lut_min_30fps[];
#endif


