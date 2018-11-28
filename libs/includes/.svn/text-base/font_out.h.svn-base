#ifndef __FONT_OUT_H__
#define __FONT_OUT_H__

#include "typedef.h"

typedef void (*FONT_HOOK)(u8* pixbuf, u16 width, u16 height, u16 x, u16 y);
extern FONT_HOOK font_putchar;

typedef enum
{
	SMALL_FONT,
	MIDDLE_FONT,
	BIG_FONT,
}FONT_SIZE;


extern bool font_init(FONT_HOOK hook, u8 language);
extern u16  font_textout(u8 *buf, u16 len, u16 x, u16 y, u8 fontsize, u8 flags);
extern u16  font_textoutw(u8 *buf, u16 len, u16 x, u16 y, bool isBigEndian, u8 fontsize, u8 flags);
extern u16  font_utf16toansi(u16 utf);
extern void font_set_type(u8 font_size);
extern u8   font_get_type();
extern u8   font_get_pixel_width();
extern u8   font_get_language();
extern u16  TextOut_ANSI(u8 *str,  u16 length,  u16 x,  u16 y, u8 flags);

#endif
