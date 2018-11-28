#ifndef __GM7150_H__
#define __GM7150_H__

#include "dv12.h"
#include "typedef.h"
#include "isp_dev.h"
#include "iic.h"

//PCLK:PC0
//HSYNC:PC3 VSYNC:PC4 PB7~PB0

#define GM7150_NORMAL()   //PORTA_DIR &=~BIT(9);PORTA_OUT |= BIT(9)
#define GM7150_LOWPOWER() //PORTA_DIR &=~BIT(9);PORTA_OUT &=~BIT(9)

#define GM7150_RESET_H()  SET_RESET_OUT_B();SET_RESET_H_B()//PORTA_DIR &= ~BIT(4);PORTA_OUT |= BIT(4)
#define GM7150_RESET_L()  SET_RESET_OUT_B();SET_RESET_L_B()//PORTA_DIR &= ~BIT(4);PORTA_OUT &= ~BIT(4)


#define GM7150_656      //输出ITU656格式

extern s32 gm7150_check(u8 isp_dev);
extern s32 gm7150_initialize(u8 isp_dev, u16 *width, u16 *height, u8 *format, u8 *frame_freq);
extern s32 gm7150_power_ctl(u8 isp_dev, u8 is_work);
extern s32 gm7150_set_output_size(u16 *width, u16 *height, u8 *freq);
#endif
