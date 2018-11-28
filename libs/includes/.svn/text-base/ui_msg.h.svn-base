#ifndef __UI_MSG_H__
#define __UI_MSG_H__

#include "typedef.h"
//#include "gui_msg.h"
#include "ui_con.h"

typedef void (*MsgCB)(void);

typedef struct
{
	//WIN *win;
	u16 id;
	u16 x;
	u16 y;
	u8 flags;
	MsgCB cb;
}VAR_PIC;

typedef struct
{
	//WIN *win;
	u16 id;
	u8 status;
	MsgCB cb;
}VAR_RADIO;

typedef struct
{
	//WIN *win;
	u16 id;
	u8 status;
	MsgCB cb;
}VAR_CHECK;

typedef struct
{
	//WIN *win;
	u16 id;
	MsgCB cb;
}VAR_BUTTON;


typedef struct
{
	//WIN *win;
	u16 id;
	MPARAM *pMenu;
	u8 mode;
	MsgCB cb;
}VAR_MENU;

typedef struct
{
	//WIN *win;
	u16 id;
	u32 num;
	u16 x;
	u16 y;
	u8 flags;
	MsgCB cb;
}VAR_NUMBER;

typedef struct
{
	//WIN *win;
	u16 id;
	TIME *time;
	u16 x;
	u16 y;
	u8 flags;
	MsgCB cb;
}VAR_TIME;

typedef struct
{
	//WIN *win;
	u16 id;
	DTEXT *text;
	MsgCB cb;
}VAR_TEXT;

typedef struct
{
	//WIN *win;
	u16 id;
	u8 percent;
	MsgCB cb;
}VAR_PROGRESS;

typedef struct
{
	//WIN *win;
	u16 id;
	SLIDER_HOOK hook;
	u8 cmd;
	MsgCB cb;
}VAR_SLIDER;

typedef struct
{
	u8 layer;
	u16 x;
	u16 y;
	u16 width;
	u16 height;
	u32 color;
}VAR_CLEAR;

typedef struct
{
	u8 layer_index;
	u16 id;
	u16 x;
	u16 y;
	u16 limit_width;
	u16 align_mode;
	u8 step;
}VAR_DRAWSTR;

typedef struct
{
	u8 layer_index;
	u16 id;
	u16 x;
	u16 y;
	u8 osd_color;
}VAR_DRAWPIC;

void gui_pic(u16 id,u16 x,u16 y,u8 flags);
void gui_text(u16 id,DTEXT *text);
void gui_time(u16 id,TIME *time,u16 x,u16 y,u8 flags);
void gui_time1(u16 id,TIME *time);
void gui_number(u16 id,u32 num,u16 x,u16 y,u8 flags);
void gui_progress(u16 id,u8 percent);
void gui_slider(u16 id,SLIDER_HOOK hook,u8 cmd);
void gui_menu(u16 id,MPARAM *pMenu,u8 mode);
void gui_radio(u16 id,u8 status);
void gui_check(u16 id,u8 status);
void gui_button(u16 id);
void gui_clear_screen(u8 layer,u16 x,u16 y,u16 width,u16 height,u32 color);
void gui_drawstring(u8 layer_index,u16 id,u16 x,u16 y,u16 limit_width,u8 align_mode,u8 step);
void gui_drawpic(u8 layer_index,u16 id,u16 x,u16 y,u8 osd_color);

#endif
