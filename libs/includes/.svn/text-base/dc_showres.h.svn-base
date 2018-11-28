#ifndef __DC_SHOWRES_H__
#define __DC_SHOWRES_H__

#include "typedef.h"

#ifdef WIN32
 #pragma pack(push)
 #pragma pack(1)
#endif

#ifdef WIN32
 #define NO_ALIGN
#else
 #define NO_ALIGN __attribute__((packed))
#endif


#define YUV_TABLE	0
#define RGB_TABLE	1

typedef struct
{
	u8  magic[4];       //'R', 'U', '2', 0x19
	u16 counts;         //资源的个数,暂时保留
	u32 lang;	        //字符串包含的语言,1 bit表示一种语言,共22种语言
	u16 sindex;         //字符串开始索引
} NO_ALIGN  RES_HEAD_T; //10字节

/*资源类型索引表目录项的数据结构*/
typedef struct
{
	u32 dwOffset;       //资源内容索引表的偏移
	u16 wCount; 	    //资源类型总数
	u8  bItemType;      //'P'--PIC Table,'S'--String Table,'X' -- XML File
	u8  reserve;
} NO_ALIGN  RES_ENTRY_T; //8字节

enum
{
	RGB_1BIT,
	RGB_8BIT,
	RGB_16BIT,
	RGB_24BIT,
	YUV420,
	JPEG,
};

typedef struct
{
	u16 nNum;		    //调色板包含颜色数
	u32 dwOffset;	    //调色板数据区在文件内偏移
	u32 dwLength;	    //调色板数据大小
} NO_ALIGN  RES_PAL_T;  //6字节

typedef struct
{
	u8 wType; 		    //图片类型，如上所示
	u16 wWidth; 		//图片宽度(以像素为单位)
	u16 wHeight; 	    //图片高度(以像素为单位)
	u32 dwOffset; 	    //数据区在文件内偏移,4 bytes
	u32 dwLength; 	    //图片数据大小,最大4G,4 bytes
} NO_ALIGN  RES_PIC_T;  //13字节

typedef struct
{
	u16 wWidth; 		//字符串总宽度(以像素为单位)
	u16 wHeight; 	    //字符串高度(以像素为单位)
	u32 dwOffset; 	    //数据区在文件内偏移,4 bytes
	u32 dwLength; 	    //字符串数据大小,最大4G，4 bytes
} NO_ALIGN RES_STR_T;   //12字节
#ifdef WIN32
#pragma pack(pop)
#endif

typedef enum
{
	ALIGN_LEFT,
	ALIGN_CENTER,
	ALIGN_RIGHT,
}ALIGN_MODE;

bool dc_init(u8 *resfilename);
void dc_set_screen_scale(u8 x_scale,u8 y_scale);
u8   get_x_scale();
u8   get_y_scale();
bool dc_drawpic(u8 layer_index,u16 id,u16 x,u16 y,u8 osd_color);
//bool dc_drawpic_area(u8 layer_index,u16 id,u16 x,u16 y,u16 spec_width,u16 spec_height,u8 osd_color);
bool dc_drawpic_area(u8 layer_index,u16 id,u16 x,u16 y,u16 spec_x,u16 spec_y,u16 spec_width,u16 spec_height,u8 flags);
bool dc_drawstring(u8 layer_index,u16 id,u16 x,u16 y,u16 limit_width,u8 align_mode,u8 step);
void dc_set_color(u8 forecolor,u8 backcolor);
//void dc_get_color(OSDCOLOR *color);
void dc_restore_color();
u16  get_picture_height(u16 id);
u16  get_picture_width(u16 id);
//bool dc_osd_palette(u8 osdlayer,u32 *pal);
bool dc_osd_palette(u8 osdlayer,u8 type,u32 *pal);
u16  get_string_width(u16 id);
u16  get_string_height(u16 id);

#endif