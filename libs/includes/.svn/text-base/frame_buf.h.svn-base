
/*******************************************************************************************
 File name:frame_buf.h

 Version: 1.00

 Discription:


 Author:yulin deng

 Email :flowingfeeze@163.com

 Date:ÐÇÆÚÁù, 04-04-2015  09:38:42ÉÏÎç



 Copyright:(c)JIELI  2014  @ , All Rights Reserved.
*******************************************************************************************/
#ifndef __frame_buf_h
#define __frame_buf_h
#include "list.h"


#define MMM_STATUS_UNUSED   0x00
#define MMM_STATUS_EMPTY    0x01
#define MMM_STATUS_FULL     0x02
#define MMM_STATUS_USEING   0x03




typedef struct __frame_buf
{

struct list_head list;
u8* ptr ;
u32 size;
u32 presize ;
u32 used;
u32 id ;
}fbuf_t ;


typedef struct _fb_mng
{
void *mmm ;
u32 size ;
u8 * out_buf;
}fb_mng_t ;


typedef struct _bf_pipe_mng
{
struct list_head list;
fb_mng_t *mng ;
u32 ch ;
u32 totale ;
u32 size ;

}fb_pipe_t ;

#endif
