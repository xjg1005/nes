

#ifndef _OV9712_H
#define  _OV9712_H



s32 OV9712_set_output_size(u16 *width, u16 *height, u8 *freq);
s32 OV9712_power_ctl(u8 isp_dev, u8 is_work);

s32 OV9712_check(u8 isp_dev);
s32 OV9712_init(u8 isp_dev, u16 *width, u16 *height, u8 *format, u8 *frame_freq);


#endif


