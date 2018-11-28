
#ifndef _OV9710_H
#define  _OV9710_H


s32 OV9710_set_output_size(u16 *width, u16 *height, u8 *freq);
s32 OV9710_power_ctl(u8 isp_dev, u8 is_work);

s32 OV9710_init(u8 isp_dev, u16 *width, u16 *height, u8 *format, u8 *frame_freq);
s32 OV9710_check(u8 isp_dev);
void OV9710_sleep();
void OV9710_wakeup();
void OV9710_W_Reg(u16 addr, u16 val);
u16 OV9710_R_Reg(u16 addr);
#endif

