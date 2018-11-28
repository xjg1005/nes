#ifndef _H42_H
#define _H42_H

#include "typedef.h"


#define H42_OUTPUT_W  1280
#define H42_OUTPUT_H   720

#define H42_TARGET_LUMA  74


s32 H42_power_ctl(u8 isp_dev, u8 is_work);
s32 H42_set_output_size(u16 *width, u16 *height, u8 *frame_freq);
void H42_reset(u8 isp_dev);
s32 H42_check(u8 isp_dev);
s32 H42_init(u8 isp_dev, u16 *width, u16 *height, u8 *format, u8 *frame_freq);
s32 H42_flush_cfg();
s32 H42_update_cfg(u32 cur_mean);
void H42_sleep();
void H42_wakeup();
void H42_W_Reg(u16 addr, u16 val);
u16 H42_R_Reg(u16 addr);
void H42_set_ae_bf_type(Sensor_AE_BF_TYPE t);

typedef struct
{
    u32 exp_time;
    u8 a_gain;
    u8 d_gain;
    u8 ev_level;
} H42_AE_lut_t;

#endif


