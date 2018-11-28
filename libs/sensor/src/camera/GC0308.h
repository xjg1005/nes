
#ifndef _GC0308_H
#define  _GC0308_H


s32 GC0308_set_output_size(u16 *width, u16 *height, u8 *freq);
s32 GC0308_power_ctl(u8 isp_dev, u8 is_work);

s32 GC0308_init(u8 isp_dev, u16 *width, u16 *height, u8 *format, u8 *frame_freq);
s32 GC0308_check(u8 isp_dev);

#endif


