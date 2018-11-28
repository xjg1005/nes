
#ifndef _NT99140_H
#define  _NT99140_H


s32 NT99140_set_output_size(u16 *width, u16 *height, u8 *freq);
s32 NT99140_power_ctl(u8 isp_dev, u8 is_work);

s32 NT99140_init(u8 isp_dev, u16 *width, u16 *height, u8 *format, u8 *frame_freq);
s32 NT99140_check(u8 isp_dev);
void updateNT9914XAE(u8 update);
void NT99140_W_Reg(u16 addr, u16 val);
u16 NT99140_R_Reg(u16 addr);
#endif

