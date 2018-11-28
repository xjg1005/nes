#ifndef __KEY_DRV_AD_H__
#define __KEY_DRV_AD_H__

#include "key_drv_manager.h"

#if USE_AD_KEY

#if USE_AD_KEY0

extern void ad_key0_init(void);

extern  u8  ad_key0_get_value(void);

extern  u8  ad_key0_pressed_to_msg(u8 key_style, u8 key_num);

#endif


#if USE_AD_KEY1

extern void ad_key1_init(void);

extern  u8  ad_key1_get_value(void);

extern  u8  ad_key1_pressed_to_msg(u8 key_style, u8 key_num);

#endif

#endif

#endif
