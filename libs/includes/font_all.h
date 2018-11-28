#ifndef __FONT_ALL_H__
#define __FONT_ALL_H__

#include "typedef.h"
#include "sdfile.h"
//#include "font_gbk.h"
//#include "font_big5.h"
//#include "font_sjis.h"
//#include "font_ksc.h"
//#include "font_other_language.h"

//¡À¨º????
#define GET_WIDTH   0x01
#define SHOW_CHAR   0x02

#ifdef WIN32
#pragma pack(push)
#pragma pack(1)
#endif

typedef struct
{
	sdfile_t pixfile;
	u8 flag;
	u8 font_size;
	u8 font_nbytes;
}FONT_ATTR;


#ifdef WIN32
#pragma pack(pop)
#endif

extern FONT_ATTR fontinfo[3];
extern FONT_ATTR ascfontinfo[3];
extern FONT_ATTR *font;
extern FONT_ATTR *ascfont;
extern u8 g_amplify;
extern u8 g_language_mode;
extern bool IsGB2312;
extern u8  pixBuf[100];
extern sdfile_t tabfile;

//#define my_ntohl(x) (unsigned long)((x>>24)|((x>>8)&0xff00)|(x<<24)|((x&0xff00)<<8))
#define my_ntoh(x) (unsigned short int )((x>>8&0x00ff)|x<<8&0xff00)


#endif
