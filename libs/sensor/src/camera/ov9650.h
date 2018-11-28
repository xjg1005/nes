

#ifndef _OV9650_H
#define  _OV9650_H



s32 OV9650_set_output_size(u16 *width, u16 *height, u8 *freq);
s32 OV9650_power_ctl(u8 isp_dev, u8 is_work);

s32 OV9650_check(u8 isp_dev);
s32 OV9650_init(u8 isp_dev, u16 *width, u16 *height, u8 *format, u8 *frame_freq);


#endif


