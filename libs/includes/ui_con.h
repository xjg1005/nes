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

//������ɫ
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

//OSDͼ����ɫ����
#define COLOR_LUCENCY		0   //͸��
#define COLOR_WHITE			1   //��ɫ
#define COLOR_RED			2   //��ɫ
#define COLOR_GREEN			3   //��ɫ
#define COLOR_BLUE			4   //��ɫ
#define COLOR_YELLOW		5   //��ɫ
#define COLOR_PURPLE		6   //��ɫ
#define COLOR_CYAN			7   //��ɫ
#define COLOR_BLACK			8   //��ɫ
//IMAGEͼ����ɫ����
#define IMAGE_COLOR_WHITE   0xeb8080
#define IMAGE_COLOR_RED     0x515aef
#define IMAGE_COLOR_GREEN   0x903522
#define IMAGE_COLOR_BLUE    0x28ef6d
#define IMAGE_COLOR_YELLOW  0xd21092
#define IMAGE_COLOR_PURPLE  0x6acadd
#define IMAGE_COLOR_CYAN    0xa9a510
#define IMAGE_COLOR_BLACK   0x108080

//PIC/TIME/NUMBER�ؼ���ϱ�־
#define USE_ORIG_COLOR      0x00//ʹ��ԭʼ��ɫ
#define USE_NEW_COLOR       0x01//ʹ������ɫ
#define ERASE_DRAW			0x02//�Ȳ��ٻ�
#define ERASE_ONLY			0x04//ֻ��
#define SHOW_ALL			0x08//ȫ��(Ĭ��ֻ��ʾ��͸������)

//�ı�����
#define ANSI			    0x01
#define UNICODE_BE		    0x02
#define UNICODE_LE		    0x04
#define	SCROLL_RESET	    0x08

typedef struct
{
	u8 *buf;
	u16 len;
	u8 flags;		//�������� UNICODE_BE��UNICODE_LE��ANSI
}DTEXT;

typedef struct
{
	u16 year;		//��
	u8 month;		//��
	u8 day;			//��
	u8 hour;		//ʱ
	u8 minute;		//��
	u8 second;		//��
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

#define OTHER_SCREEN		0xfe//��������
#define NO_MENU				0xff


//�˵���ʽ
typedef enum
{
    STYLE_NORMAL,       //����
	STYLE_CONFIRM,		//ȷ��
    STYLE_ONE_CHOICE,   //��ѡ
    STYLE_MULTI_CHOICE, //��ѡ
    STYLE_USER_DEFINED, //�Զ���
} MENU_STYLE;

typedef struct
{
    u16 id;					    //�˵���������
	u16 icon;                   //�˵���ͼ��
    u8 child_node;	            //��һ���˵� NO_MENU:���¼��˵� ����:�¼��˵�����
} S_MENU;

typedef struct
{
    u8 father_node;             //��һ���˵�
    u16 title;				    //�˵�����,0�ޱ���,��0�б���
    MENU_STYLE style;		    //�˵���ʽ 0:���� 1:��ѡ 2:��ѡ 3.�Զ���
    const S_MENU  *menu;	    //�˵��� ָ��S_MENU��ָ��
    u8 (*fun)(u16 id, u8 mode); //�˵��ص�����
    u8 menuCnt;				    //�ü��˵�����
}S_MENULIST;

typedef struct
{
	u16 icon;
	S_MENULIST *list;
}MENU_ARRAY;

typedef struct
{
	S_MENULIST *mlist;	//�˵��б�
	u8 node;			//�˵��ڵ�
	u8 draw_flags;		//ˢ�±�־
	s8 active_item;		//��˵���
	u8 select_item;		//ѡ�в˵���
	s8 active_menu;		//��˵�
	u8 select_menu;		//ѡ�в˵�
	s8 curr_item_pos;	//��˵���λ��
	u32 multi_sel;		//��ѡ��,��Ӧ��bitΪ1,��ʾ���ѡ��
}MVAR;

typedef struct
{
	MENU_ARRAY *array;
	OSDCOLOR act_color;		    //��˵���������ɫ
	OSDCOLOR unact_color;	    //�ǻ�˵������ɫ
	u16 listcnt;
	u16 timeout;		        //��ʱ�˳�ʱ������(��λ����),Ϊ0���ֹ��ʱ�˳�
	u8  effect_en;				//Ч��ʹ��
	u8  item_vertical_center;   //�˵����Ƿ�ֱ����
	u8  show_title;             //�Ƿ���ʾ����ͼƬ
    u16 item_point_x[10];
	void (*callback)(u8 dialog,u8 mode,void *parm, MVAR menu_var);
}MENU;

#define	REDRAW_NONE			0x00
#define REDRAW_ERASE        0x01
#define REDRAW_FRAME		0x02
#define REDRAW_MENU			0x04
#define REDRAW_BROADSIDE	0x08
#define REDRAW_ALL		    (REDRAW_ERASE|REDRAW_FRAME|REDRAW_MENU|REDRAW_BROADSIDE)



#define MAX_MENU_DEEPTH		16	//�˵�����
#define NO_VALID_PARA		0xff//����Ч����

typedef struct
{
	s8 g_menu_counter;
	u8 g_menu_read;
	u8 g_menu_buf[MAX_MENU_DEEPTH * 3];
}MSG_FIFO;


//////////////�˵�״̬////////////////
enum
{
	MENU_STATE_INIT,
	MENU_STATE_DISP,
	MENU_STATE_END,
};
//////////////////////////////////////

/////////////////////////////////////DV10�ṹ//////////////////////////////////////
//�˵���Ϣ
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
}BORDER;//ͼ���ɰ�


typedef void (*MenuHook)(void);
typedef u16 (*MsgHook)(void);
typedef struct
{
	MenuHook title;
	MenuHook bottom;
	MsgHook  msg_hook;
	S_MENULIST *list;
	u16 timeout;			//��ʱ�˳�ʱ������(��λ����),Ϊ0���ֹ��ʱ�˳�
	OSDCOLOR act_color;		//��˵���������ɫ
	OSDCOLOR unact_color;	//�ǻ�˵������ɫ
	BORDER border;			//ͼ���ɰ�߿�
	u16 selpic2;			//����ѡ��ͼƬ2(�Ӳ˵�)
	s16 icon_offset;        //�˵����ͼ��ƫ��
	s16 child_area_x;       //�Ӳ˵�����xƫ��
	s16 child_area_y;       //�Ӳ˵�����yƫ��
	s16 child_area_width;   //�Ӳ˵�����widthƫ��
	s16 child_area_height;  //�Ӳ˵�����heightƫ��
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
//mode����ʾ��ȡ��ǰֵ��ǰһλ��ֵ����һλ��ֵ


typedef struct
{
	u16 id;				//id��
	u8 type;			//�ؼ�����
	void *p;			//����
	u8 state;			//״̬ ��ʾ/����/�ػ�
	GUI_RECT rect;		//����
	void *pNextObj;		//ָ����һ���ؼ�
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
//�˵��ص�����

#define DIALOG_CONFIRM      0x00//ȡ��ȷ�Ͽ�
#define DIALOG_BROADSIDE    0x01//�����
#define DIALOG_MENU			0x02//�˵��Ի���
#define DIALOG_MOVE_IN		0x03
#define DIALOG_MOVE_OUT		0x04
#define DIALOG_SECOND_MENU		0x05

#define DRAW_BACKPIC        0x00
#define DRAW_ITEM           0x01
#define GET_TITLE_RECT      0x02
#define DRAW_M_BACKPIC        0x03
typedef struct
{
	u8 is_active;    //�Ƿ�
	u8 is_select;    //�Ƿ�ѡ��
	u8 item_idx;     //item���
	u8 have_child;   //�Ƿ����Ӳ˵�
	Rect rect;       //��ȡ����
} DVAR;
//--------------------------------------------------------------

typedef struct
{
	u8  type;				//ͼ�����
	u8  alpha;				//ͼ��͸���ȣ�64 level(0~63)
	u16 x;					//x����
	u16 y;					//y����
	u16 width;				//ͼ�����
	u16 height;				//ͼ��߶�
} LAYER_INFO;

////����������////
extern sdfile_t resfile;
extern sdfile_t styfile;

////����������////
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