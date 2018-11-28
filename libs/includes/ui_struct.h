#ifndef __UI_CON1_H__
#define __UI_CON1_H__

#include "typedef.h"
#include "sdfile.h"
#include "ui_com.h"

#ifdef WIN32
#pragma pack(push)
#pragma pack(1)
#endif

//控件类型
//enum
//{
//	//以下顺序不可修改
//	TYPE_PIC,
//	TYPE_TEXT,
//	TYPE_TIME,
//	TYPE_MENU,
//	TYPE_NUMBER,
//	TYPE_SLIDER,
//	TYPE_PROGRESS,
//	TYPE_RADIO,
//	TYPE_CHECKBOX,
//	TYPE_BUTTON,
//	TYPE_LAYER,
//};

typedef struct 
{
	u32 flag;
	u16 fileversion;
	u16 filecount;
}STYFILEHEADER;//8 bytes

typedef struct
{
	u32 num;				//该类控件总数
	u32 offset;				//该类控件偏移地址
} NO_ALIGN CONTROL_HEADER;		//8 bytes

typedef struct
{
	u16 x;
	u16 y;
	u16 id;
}PIC;

//------------------- 1.图片控件  -------------------//
typedef struct
{
	u8 layer;
	u16 x;		//x坐标
	u16 y;		//y坐标
	u16 width;	//图片宽度
	u16 height;	//图片高度
	u16 id;		//图片id号
} NO_ALIGN CPIC; //11 Bytes

//------------------  2.文本控件  -------------------//
typedef enum
{	
	UI_STATIC_TEXT,			//静态显示
	UI_SCROLL_TEXT,			//滚动显示
	UI_KLOK_TEXT,			//卡拉OK滚动显示
}DISP_MODE;

typedef struct
{
	u8 layer;				//图层
	u16 x;					//起始x坐标
	u16 y;					//起始y坐标
	u16 width;				//显示区域的宽
	u16 height;				//显示区域的高
	u16 backpicture;		//背景图片 0:无背景图片,背景颜色有效 其它:背景图片ID号,背景颜色无效
	u8 backcolor;			//背景颜色
	u8 forecolor;			//前景颜色
	u8 /*FONT_SIZE*/ font_size;	//字号,支持小(16x16),中(20x20),大(24x24)三种字号
	u8 /*DISP_MODE*/ dispmode;		//显示方式
} NO_ALIGN CTEXT; //16 Bytes
//说明:背景颜色和前景颜色为8位色

//时间控件类型
//0:	hh:mm					时钟：分钟
//1:	hh:mm:ss				时钟：分钟：秒钟
//2:	yyyy-mm-dd hh:mm:ss		年-月-日 时钟：分钟：秒钟
//3:    yyyy-mm-dd				年-月-日
//4:    yy-mm-dd				年-月-日(年取两位)
//5:    mm-dd-yy                月-日-年
//6:    dd-mm-yy                日-月-年

#define TIME_HH_MM					0x00
#define TIME_HH_MM_SS				0x01
#define TIME_YYYY_MM_DD_HH_MM_SS	0x02
#define TIME_YYYY_MM_DD				0x03
#define TIME_YY_MM_DD				0x04
#define TIME_MM_DD_YY				0x05
#define TIME_DD_MM_YY				0x06

//------------------  3.时间控件 -------------------//
typedef struct
{
	u8 layer;
	u16 x;					//起始x坐标
	u16 y;					//起始y坐标
	u16 width;				//控件宽度
	u16 height;				//控件高度
	u8  type;				//时间显示类型
	u16 num_id[12];			//0~9为数字的ID号，10~11为分隔符ID号,前两种类型记录到num_id[10],最后一种记录到num_id[11]
}NO_ALIGN CTIME;// 26 Bytes

//------------------  4.菜单控件   ----------------//
typedef struct
{
	u8 layer;
	u16 x;					//起始x坐标
	u16 y;					//起始y坐标
	u16 width;				//菜单宽度
	u16 height;				//菜单高度
	u16 mItemWidth;			//菜单项左边图标宽度
	u16 mItemHeight;		//菜单项高度
	u16 backpic;			//背景图片(图片控件id号)
	u16 titlepic;			//标题栏图片
	u16 bottompic;			//状态栏图片
	u16 activepic;			//活动项图标
	u16 unactivepic;		//非活动项图标
	u16 OneChoiceSel;		//单选已选中图标
	u16 OneChoiceNoSel;		//单选未选中图标
	u16 MultiChoiceSel;		//多选已选中图标
	u16 MultiChoiceNoSel;	//多选未选中图标
	u16 selpic;				//高亮选中图片1
	u16 scrollbar;			//滚动条
	u16 scrollbar_p;		//滚动块图片ID号
	u8 menuNumber;          //菜单项数
}NO_ALIGN CMENU; //40 Bytes

//-----------------   5.滑动条控件  ----------------//
typedef struct
{
	u8 layer;
	u16 x;					//起始x坐标
	u16 y;					//起始y坐标
	u16 width;				//控件宽度
	u16 height;				//控件高度
	PIC bar;				//滑动块图片id号	
	PIC slider;				//滑动条图片id号
	u16 cnum[3];			//数字控件的id号,分别表示最小值/当前值/最大值(数字控件id)
	u16 backpic;			//背景图片(图片控件id)
}NO_ALIGN CSLIDER;

//----------------    6.进度条控件  ------------------//
typedef struct
{
	u8 layer;
	u16 x;					//起始x坐标
	u16 y;					//起始y坐标
	u16 width;				//控件宽度
	u16 height;				//控件高度
	PIC bar;				//进度小方块										
	PIC progress;			//进度条	
	PIC percent;			//百分号
	u16 number;				//数字控件id号
	u16 backpicture;		//背景图片(图片控件id)
}NO_ALIGN CPROGRESS;//31byte

//----------------    7.数字控件  ---------------------//
typedef struct
{
	u8 layer;
    u16 x;					//起始x坐标
	u16 y;					//起始y坐标
	u16 width;				//控件宽度
	u16 height;				//控件高度
    u16 num_id[10];			//数字0-9的图片ID号
    u16 num_bits;			//数值有效位数
}NO_ALIGN CNUMBER;

//-----------------   8.单选控件   --------------------//
typedef struct
{
	u8 layer;
	u16 x;					//x坐标
	u16 y;					//y坐标
	u16 width;				//控件宽度
	u16 height;				//控件高度
	PIC sel;				//选中图标
	PIC unsel;				//未选中图标
	PIC strid;				//字符串id号
}NO_ALIGN CRADIO;//27

//----------------    9.多选控件  ---------------------//
typedef struct
{
	u8 layer;
	u16 x;					//x坐标
	u16 y;					//y坐标
	u16 width;				//控件宽度
	u16 height;				//控件高度
	PIC sel;				//选中图标
	PIC unsel;				//未选中图标
	PIC strid;				//字符串id号
	u8 status;				//默认是选中状态还是未选中状态
}NO_ALIGN CHECKBOX;//28

//----------------    10.按钮控件 ----------------------//
typedef struct
{
	u8 layer;
	u16 x;					//x坐标
	u16 y;					//y坐标
	u16 width;				//控件宽度
	u16 height;				//控件高度
	PIC picid;				//按钮图片
	PIC strid;				//按钮文本
}NO_ALIGN CBUTTON;

//------------------  11.区域  ------------------//
typedef struct
{
    u8 layer;
	u16 x;
    u16 y;
    u16 width;
    u16 height;
    u32 color;
}NO_ALIGN CREGION;

//------------------  12.图层  ------------------//
typedef struct
{
	u8 /*LAYER_TYPE*/type;	//图层序号
	u8 alpha;				//图层透明度，64 level(0~63)
	u16 x;					//x坐标
	u16 y;					//y坐标
	u16 width;				//图层宽度
	u16 height;				//图层高度
}NO_ALIGN LAYER;//10 bytes

#ifdef WIN32
#pragma pack(pop)
#endif


#endif
