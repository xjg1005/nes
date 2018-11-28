#ifndef __UI_DYNAMIC_EFFECT_H__
#define __UI_DYNAMIC_EFFECT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "typedef.h"
#include "ui_con.h"

#define DIRECT_RIGHT_TO_LEFT    0x00
#define DIRECT_LEFT_TO_RIGHT    0x01
#define DIRECT_UP_TO_DOWN       0x02
#define DIRECT_DOWN_TO_UP       0x03

void ui_dynamic_in(u8 direction,Rect *rect);
void ui_dynamic_out(u8 direction,Rect *rect);


#ifdef __cplusplus
}
#endif

#endif