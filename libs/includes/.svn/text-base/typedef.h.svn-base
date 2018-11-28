/*************************************************************
File: typedef.h
Author:Juntham
Discriptor:
    数据类型重定义
Version:
Date：
*************************************************************/
#ifndef _typedef_h_
#define _typedef_h_

#ifndef NULL
#define NULL ((void *)0)
#endif
#ifdef __GNUC__
#define ALIGNE(x) __attribute__((aligned(x)))
#define sec(x) __attribute__((section(#x)))
#define AT(x) __attribute__((section(#x)))
#define SET(x) __attribute__((x))
#define _GNU_PACKED_	__attribute__((packed))
#define NO_ALIGN __attribute__((packed))
#else
#define sec(x)
#define AT(x)
#define SET(x)
#define _GNU_PACKED_
#endif

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE !FALSE
#endif

#ifndef false
#define false 0
#endif
#ifndef true
#define true !false
#endif

//#ifdef WIN32
typedef unsigned char	__u8,u8,uint8,BYTE, bool ,bit1 ;
typedef char			__s8,s8;
typedef unsigned short	__le16,__u16,u16,uint16,WORD;
typedef signed short	__s16,__be16,be16,le16,s16;
typedef unsigned int	__be32,__le32,be32,le32,__u32,u32,uint32,tu8,tu16,tbool,DWORD;
typedef signed int		__s32,s32,BOOL;
//typedef signed long long u64,__le64,le64;

typedef u32				FOURCC;

#define _xdata
#define _data
#define _idata
#define _pdata
#define _bdata
#define _code
#define _bit		bool
#define _sbit		bool
#define pu16		u16
#define pu32		u32

#define _banked_func
#define _xdata
//#define _data
#define _root
#define _no_init
#ifdef __UBOOT
#define my_memset memset
#define my_memcpy memcpy
#endif


//#endif


#define     BIT(n)	            (1 << (n))
#define     BitSET(REG,POS)     (REG |= (1 << POS))
#define     BitCLR(REG,POS)     (REG &= (~(1 << POS)))
#define     BitXOR(REG,POS)     (REG ^= (~(1 << POS)))
#define     BitCHK_1(REG,POS)   ((REG & (1 << POS)) == (1 << POS))
#define     BitCHK_0(REG,POS)   ((REG & (1 << POS)) == 0x00)
#define     testBit(REG,POS)    (REG & (1 << POS))

#define     clrBit(x,y)         x &= ~(1L << y)
#define     setBit(x,y)         x |= (1L << y)

#define ntohl(x) (u32)((x>>24)|((x>>8)&0xff00)|(x<<24)|((x&0xff00)<<8))
#define ntoh(x) (u16)((x>>8&0x00ff)|x<<8&0xff00)


#define	LD_WORD(ptr)		(WORD)(((WORD)*((BYTE*)(ptr)+1)<<8)|(WORD)*(BYTE*)(ptr))
#define	LD_DWORD(ptr)		(DWORD)(((DWORD)*((BYTE*)(ptr)+3)<<24)|((DWORD)*((BYTE*)(ptr)+2)<<16)|((WORD)*((BYTE*)(ptr)+1)<<8)|*(BYTE*)(ptr))
#define	ST_WORD(ptr,val)	*(BYTE*)(ptr)=(BYTE)(val); *((BYTE*)(ptr)+1)=(BYTE)((WORD)(val)>>8)
#define	ST_DWORD(ptr,val)	*(BYTE*)(ptr)=(BYTE)(val); *((BYTE*)(ptr)+1)=(BYTE)((WORD)(val)>>8); *((BYTE*)(ptr)+2)=(BYTE)((DWORD)(val)>>16); *((BYTE*)(ptr)+3)=(BYTE)((DWORD)(val)>>24)

//#define	LD_WORD(ptr)		(u16)(*(u16*)(u8*)(ptr))
//#define	LD_DWORD(ptr)		(u32)(*(u32*)(u8*)(ptr))
//#define	ST_WORD(ptr,val)	*(u16*)(u8*)(ptr)=(u16)(val)
//#define	ST_DWORD(ptr,val)	*(u32*)(u8*)(ptr)=(u32)(val)

//#define my_memcpy memcpy
//#define my_memset memset

#ifndef min
#define min(a,b)    ((a)<(b)?(a):(b))
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))
#endif

#ifndef MIN
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif
typedef struct S_PICTURE_SIZE
{
	u16 w;
	u16 h;
}pic_size_t;
#endif
