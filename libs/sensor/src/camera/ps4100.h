#ifndef _PS4100_H
#define _PS4100_H

#include "typedef.h"

#define PS4100_DEBUG printf

#define PS4100_MAX_EV_LEVEL         201


#define PS4100_OUTPUT_W             1280
#define PS4100_OUTPUT_H             720
#define PS4100_TARGET_LUMA          76

typedef struct
{
    u32 exp_time;
    u8 a_gain;
    u8 d_gain;
    u8 ev_level;
} PS4100_AE_lut_t;


s32 PS4100_set_output_size(u16 *width, u16 *height, u8 *freq);
s32 PS4100_power_ctl(u8 isp_dev, u8 is_work);

s32 PS4100_check(u8 isp_dev);
s32 PS4100_init(u8 isp_dev, u16 *width, u16 *height, u8 *format, u8 *frame_freq);
s32 PS4100_update_cfg(u32 m_mean);
s32 PS4100_flush_cfg();


s32 PS4100_flush_cfg();
void PS4100_sleep();
void PS4100_wakeup();
void PS4100_W_Reg(u16 addr, u16 val);
u16 PS4100_R_Reg(u16 addr);
void PS4100_set_ae_bf_type(Sensor_AE_BF_TYPE t);

extern PS4100_AE_lut_t PS4100_AE_lut_50Hz_min_25fps[];

#endif


