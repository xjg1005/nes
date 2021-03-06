#ifndef __UI_CON_H__
#define __UI_CON_H__

#include "typedef.h"
#include "font_out.h"
#include "imb_module.h"
#include "ui_com.h"
#include "ui_sdfs.h"
#include "sdfile.h"

//#define flush_dcache(a,b)
//#define flushinv_dcache(a,b)

#ifdef WIN32
#pragma pack(push)
#pragma pack(1)
#endif

//字体颜色
typedef struct
{
	u8 forecolor;
	u8 backcolor;
}OSDCOLOR;

typedef struct
{
	u16 x;
	u16 y;
	u16 width;
	u16 height;
}GUI_RECT;

typedef struct
{
	u16 x;
	u16 y;
}GUI_POINT;

//OSD图层颜色定义
#define COLOR_LUCENCY		0   //透明
#define COLOR_WHITE			1   //白色
#define COLOR_RED			2   //红色
#define COLOR_GREEN			3   //绿色
#define COLOR_BLUE			4   //蓝色
#define COLOR_YELLOW		5   //黄色
#define COLOR_PURPLE		6   //紫色
#define COLOR_CYAN			7   //青色
#define COLOR_BLACK			8   //黑色
//IMAGE图层颜色定义
#define IMAGE_COLOR_WHITE   0xeb8080
#define IMAGE_COLOR_RED     0x515aef
#define IMAGE_COLOR_GREEN   0x903522
#define IMAGE_COLOR_BLUE    0x28ef6d
#define IMAGE_COLOR_YELLOW  0xd21092
#define IMAGE_COLOR_PURPLE  0x6acadd
#define IMAGE_COLOR_CYAN    0xa9a510
#define IMAGE_COLOR_BLACK   0x108080

//PIC/TIME/NUMBER控件组合标志
#define USE_ORIG_COLOR      0x00//使用原始颜色
#define USE_NEW_COLOR       0x01//使用新颜色
#define ERASE_DRAW			0x02//先擦再画
#define ERASE_ONLY			0x04//只擦
#define SHOW_ALL			0x08//全显(默认只显示非透明部分)

//文本编码
#define ANSI			    0x01
#define UNICODE_BE		    0x02
#define UNICODE_LE		    0x04
#define	SCROLL_RESET	    0x08

typedef struct
{
	u8 *buf;
	u16 len;
	u8 flags;		//编码类型 UNICODE_BE、UNICODE_LE、ANSI
}DTEXT;

typedef struct
{
	u16 year;		//年
	u8 month;		//月
	u8 day;			//日
	u8 hour;		//时
	u8 minute;		//分
	u8 second;		//秒
}TIME;


#define TIME_HH_MM					0x00
#define TIME_HH_MM_SS				0x01
#define TIME_YYYY_MM_DD_HH_MM_SS	0x02
#define TIME_YYYY_MM_DD				0x03
#define TIME_YY_MM_DD				0x04
#define TIME_MM_DD_YY				0x05
#define TIME_DD_MM_YY				0x06
#define TIME_MM_SS				    0x07  //for ui_time1
enum
{
	MENU_INIT,
	MENU_ACTIVE,
	MENU_SELECT,
};

enum
{
	MENU_CMD_INIT,
	MENU_CMD_PREV,
	MENU_CMD_NEXT,
	MENU_CMD_CONFIRM,
	MENU_CMD_SWITCH,
	MENU_CMD_BACK,
	MENU_CMD_500MS,
	MENU_CMD_EXIT,
};

#define OTHER_SCREEN		0xfe//其他界面
#define NO_MENU				0xff


//菜单样式
typedef enum
{
    STYLE_NORMAL,       //正常
	STYLE_CONFIRM,		//确认
    STYLE_ONE_CHOICE,   //单选
    STYLE_MULTI_CHOICE, //多选
    STYLE_USER_DEFINED, //自定义
} MENU_STYLE;

typedef struct
{
    u16 id;					    //菜单项索引号
	u16 icon;                   //菜单项图标
    u8 child_node;	            //下一级菜单 NO_MENU:无下级菜单 其他:下级菜单索引
} S_MENU;

typedef struct
{
    u8 father_node;             //上一级菜单
    u16 title;				    //菜单标题,0无标题,非0有标题
    MENU_STYLE style;		    //菜单样式 0:正常 1:单选 2:多选 3.自定义
    const S_MENU  *menu;	    //菜单项 指向S_MENU的指针
    u8 (*fun)(u16 id, u8 mode); //菜单回调函数
    u8 menuCnt;				    //该级菜单个数
}S_MENULIST;

typedef struct
{
	u16 icon;
	S_MENULIST *list;
}MENU_ARRAY;

typedef struct
{
	S_MENULIST *mlist;	//菜单列表
	u8 node;			//菜单节点
	u8 draw_flags;		//刷新标志
	s8 active_item;		//活动菜单项
	u8 select_item;		//选中菜单项
	s8 active_menu;		//活动菜单
	u8 select_menu;		//选中菜单
	s8 curr_item_pos;	//活动菜单项位置
	u32 multi_sel;		//多选项,对应的bit为1,表示该项被选中
}MVAR;

typedef struct
{
	MENU_ARRAY *array;
	OSDCOLOR act_color;		    //活动菜单项字体颜色
	OSDCOLOR unact_color;	    //非活动菜单项背景颜色
	u16 listcnt;
	u16 timeout;		        //超时退出时间设置(单位：秒),为0则禁止超时退出
	u8  effect_en;				//效果使能
	u8  item_vertical_center;   //菜单项是否垂直居中
	u8  show_title;             //是否显示标题图片
    u16 item_point_x[10];
	void (*callback)(u8 dialog,u8 mode,void *parm, MVAR menu_var);
}MENU;

#define	REDRAW_NONE			0x00
#define REDRAW_ERASE        0x01
#define REDRAW_FRAME		0x02
#define REDRAW_MENU			0x04
#define REDRAW_BROADSIDE	0x08
#define REDRAW_ALL		    (REDRAW_ERASE|REDRAW_FRAME|REDRAW_MENU|REDRAW_BROADSIDE)



#define MAX_MENU_DEEPTH		16	//菜单级数
#define NO_VALID_PARA		0xff//无有效参数

typedef struct
{
	s8 g_menu_counter;
	u8 g_menu_read;
	u8 g_menu_buf[MAX_MENU_DEEPTH * 3];
}MSG_FIFO;


//////////////菜单状态////////////////
enum
{
	MENU_STATE_INIT,
	MENU_STATE_DISP,
	MENU_STATE_END,
};
//////////////////////////////////////

/////////////////////////////////////DV10结构//////////////////////////////////////
//菜单消息
enum
{
	UI_MSG_PREV,
	UI_MSG_NEXT,
	UI_MSG_CONFIRM,
//	UI_MSG_100MS,
	UI_MSG_200MS,
	UI_MSG_500MS,
	UI_MSG_RETURN,
	UI_MSG_EXIT,
	UI_MSG_INVALID,
};

typedef struct
{
	u8 layer;
	u8 color;
}BORDER;//图层蒙板


typedef void (*MenuHook)(void);
typedef u16 (*MsgHook)(void);
typedef struct
{
	MenuHook title;
	MenuHook bottom;
	MsgHook  msg_hook;
	S_MENULIST *list;
	u16 timeout;			//超时退出时间设置(单位：秒),为0则禁止超时退出
	OSDCOLOR act_color;		//活动菜单项字体颜色
	OSDCOLOR unact_color;	//非活动菜单项背景颜色
	BORDER border;			//图层蒙板边框
	u16 selpic2;			//高亮选中图片2(子菜单)
	s16 icon_offset;        //菜单左侧图标偏移
	s16 child_area_x;       //子菜单区域x偏移
	s16 child_area_y;       //子菜单区域y偏移
	s16 child_area_width;   //子菜单区域width偏移
	s16 child_area_height;  //子菜单区域height偏移
}MPARAM;


typedef struct
{
	u32 min_val;
	u32 curr_val;
	u32 max_val;
	u16 total_len;
	u16 curr_pos;
}DSLIDER;


#define UI_SLIDER_CURR   0
#define UI_SLIDER_PREV   1
#define UI_SLIDER_NEXT   2
typedef u16 (*SLIDER_HOOK)(u8 mode,DSLIDER *s_param);
//mode：表示获取当前值，前一位置值，后一位置值


typedef struct
{
	u16 id;				//id号
	u8 type;			//控件类型
	void *p;			//参数
	u8 state;			//状态 显示/隐藏/重绘
	GUI_RECT rect;		//区域
	void *pNextObj;		//指向下一个控件
}WIN;


#ifdef WIN32
#pragma pack(pop)
#endif


enum
{
	SHOW_CURR_VALUE,
	SHOW_NEXT_VALUE,
	SHOW_PREV_VALUE,
};

typedef struct
{
	u8 layer;
	u16 x;
	u16 y;
	u16 width;
	u16 height;
}Rect;


//--------------------------------------------------------------
//菜单回调函数

#define DIALOG_CONFIRM      0x00//取消确认框
#define DIALOG_BROADSIDE    0x01//侧边栏
#define DIALOG_MENU			0x02//菜单对话框
#define DIALOG_MOVE_IN		0x03
#define DIALOG_MOVE_OUT		0x04
#define DIALOG_SECOND_MENU		0x05

#define DRAW_BACKPIC        0x00
#define DRAW_ITEM           0x01
#define GET_TITLE_RECT      0x02
#define DRAW_M_BACKPIC        0x03
typedef struct
{
	u8 is_active;    //是否活动
	u8 is_select;    //是否选中
	u8 item_idx;     //item序号
	u8 have_child;   //是否有子菜单
	Rect rect;       //获取参数
} DVAR;
//--------------------------------------------------------------

typedef struct
{
	u8  type;				//图层序号
	u8  alpha;				//图层透明度，64 level(0~63)
	u16 x;					//x坐标
	u16 y;					//y坐标
	u16 width;				//图层宽度
	u16 height;				//图层高度
} LAYER_INFO;

////变量声明区////
extern sdfile_t resfile;
extern sdfile_t styfile;

////函数声明区////
extern void my_putchar(u8 *buf,u16 width,u16 height,u16 x,u16 y);
extern bool ui_init(u8 *styfilename,u16 width,u16 height);
extern bool ui_screen_to_layer(u8 layer,u16 x,u16 y,u16 w,u16 h,GUI_POINT *point);
extern void ui_clear_screen(u8 layer,u16 x,u16 y,u16 width,u16 height,u32 color);
extern bool ui_pic(u16 id,u16 x,u16 y,u8 flags);
extern bool ui_pic_spec_area(u16 id,u16 x,u16 y,u16 width,u16 height,u8 flags);
extern bool ui_multi_pic(u16 id,u16 num,u16 delay10ms,u16 x,u16 y,u8 flags);
extern bool ui_multi_rgb565(u16 id,u16 num,u16 delay10ms,u16 x,u16 y,u8 flags);
extern bool ui_menu(u16 id,MENU *pMenu,u8 cmd);
extern bool ui_menua(u16 id,MPARAM *pMenu);
extern bool ui_menub(u16 id,MENU *pMenu,u8 cmd);
extern bool ui_number(u16 id,u32 num,u16 x,u16 y,u8 flags);
extern bool ui_time(u16 id,TIME *time,u16 x,u16 y,u8 flags);
extern bool ui_time1(u16 id,TIME *time,u8 mode,u16 x,u16 y,u8 flags);
extern bool ui_text(u16 id,DTEXT *text);
extern bool ui_texta(u16 id,DTEXT *text,u16 x,u16 y,u8 flags);
extern bool ui_progress(u16 id,u8 percent);
extern bool ui_slider(u16 id,SLIDER_HOOK hook,u8 cmd);
extern bool ui_layer(u16 id,LAYER_INFO *layer);
extern bool ui_radio(u16 id,u8 status);
extern bool ui_check(u16 id,u8 status);
extern bool ui_button(u16 id);
extern bool ui_region(u16 id,u32 color);
extern bool ui_get_info(u8 type,u16 id,Rect *rect);
extern void ui_clear_controls(u8 type,u16 id);
extern u8   ui_get_status();
extern u8   get_select_item();
extern void set_select_item(u8 item);
extern u8   get_active_item(void);
extern void set_active_item(u8 item);
extern u8   get_counter();
extern void ui_set_speed(u8 speed);
#endif
