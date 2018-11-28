
#ifndef _SAA7113_H
#define  _SAA7113_H



s32 SAA7113_set_output_size(u16 *width, u16 *height, u8 *freq);
s32 SAA7113_power_ctl(u8 isp_dev, u8 is_work);

s32 SAA7113_check(u8 isp_dev);
s32 SAA7113_init(u8 isp_dev, u16 *width, u16 *height, u8 *format, u8 *frame_freq);


#endif



