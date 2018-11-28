/*******************************************************************************************
 File Name: devcie.h

 Version: 1.00

 Discription:设备状态，操作函数定义和宏定义

 Author:dengyulin

 Email ：flowingfeeze@163.com

 Date:2010.09.09

 Copyright:(c) 2010 , All Rights Reserved.
*******************************************************************************************/
/*
version history

2010.09.09 v1.0 建立初始版本

*/

#ifndef __DEVCIE_H
#define __DEVCIE_H

#include "typedef.h"

#define ntol_u16(a)    ((a)=((a)>>8) | (((a)<<8)&0xff00))
#define ntol_u32(a)    ((a)=((a)>>24)|((a)<<24)|(((a)>>8)&0xff00)|(((a)<<8)&0x00ff0000))

typedef struct _SDFILE
{
    u32 addr;
    u32 len;
    u32 ptr;
    u16 crc ;
}sdfile_t ,*psdfile_t;


typedef struct _FLASHHEADER
{
	u16 crc16;
	u16 crcfileheaddata;
	u8 info[8];
	u32 filenum;
	u32 version;
	u32 version1;
	u8 chiptype[8];
}FLASHHEADER;


typedef struct _FILEHEAD
{
	u8 filetype;
	u8 reserv;
	u16 crc16;
	u32 addr;
	u32 len;
	u32 version;
	u8 name[16];
}FLASHFILEHEAD ,*PFLASHPFILEHEAD ;




typedef struct _BANKCB
{
	u16 bankNum;
	u16 size;
	u32 bankaddr;
	u32 addr;
	u16 crc;
	u16 crc1;
}BANKCB;

typedef struct _SDF_IO
{
    void *param;
    u32 addOffset;
    u32 devOnline;
    tbool (*disk_read)(void *param, void *, u32, u32);
}SDF_IO;

#define  SDDEVICE_SPIFLASH 0x00
#define  SDDEVICE_SDCARD   0x01

#define  SD_SEEK_SET  0x00

void *devHandle;
tbool   sd_fopen(const u8 *filename,sdfile_t *file);

tbool   sd_fopenbyindex(u8 index,sdfile_t *file);

void    sd_phyread(tbool isDecEn, void * buf,u32 addr,u16  len);

u32     sd_fread(sdfile_t *fp, void *buf, u32 len);

u32     sd_fseek(sdfile_t *fp,u8 seek,u32 len );

u16     Crc16(void *ptr, u16  len);

u32 sdfile_init(void * parm, void *parm2);

#endif

