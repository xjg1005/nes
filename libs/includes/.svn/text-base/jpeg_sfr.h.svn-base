
/*******************************************************************************************
 File name:jpeg.h

 Version: 1.00

 Discription:   jpeg 编解码sfr 定义文件


 Author:yulin deng

 Email :flowingfeeze@163.com

 Date:星期五, 03-20-2015  11:11:00上午



 Copyright:(c)JIELI  2014  @ , All Rights Reserved.
*******************************************************************************************/
#ifndef __jpeg_h
#define __jpeg_h

#include "config.h"

#define JPEG0_SFR_BEGIN  &JPG0_CON0
#define JPEG1_SFR_BEGIN  &JPG1_CON0


typedef struct JPEG_SFR
{
volatile u32 CON0            ;
volatile u32 CON1            ;
volatile u32 CON2            ;
volatile u32 YDCVAL          ;        //write only
volatile u32 UDCVAL          ;        //write only
volatile u32 VDCVAL          ;        //write only
volatile u32 YPTR0           ;        //write only
volatile u32 UPTR0           ;        //write only
volatile u32 VPTR0           ;        //write only
volatile u32 YPTR1           ;        //write only
volatile u32 UPTR1           ;        //write only
volatile u32 VPTR1           ;        //write only
volatile u32 BADDR           ;        //write only
volatile u32 BCNT            ;
volatile u32 MCUCNT          ;
volatile u32 PRECNT          ;        //write only
volatile u32 YUVLINE         ;        //write only
volatile u32 CFGRAMADDR      ;        //write only
//u32 rev[0x1c00-0x12] ;
volatile u32 CFGRAMVAL       ;  //0x1c00*4

}JPEG_SFR ;



#endif
