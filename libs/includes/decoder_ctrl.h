
/*******************************************************************************************

Copyright (C), 2014 -2014, JIELI Tech. Co., Ltd.
File name:decoder_ctrl.h
Author:	Version:	Date: // 作者、版本及完成日期
邓玉林  1.0             2014-06-19   09:01:20上午
Description:

Others:
Function List:


1. ....
History:


1. Date: Author: Modification:
2. ...

*************************************************/
#ifndef __decoder_ctrl_h
#define __decoder_ctrl_h


#include "typedef.h"

#include "list.h"
#include "os_api.h"
#include "if_decoder_ctrl.h"
#define DECODER_ERR_NO_MEM                 0x0001
#define DECODER_ERR_NAME_TO_LONG           0x0002
#define DECODER_ERR_DECODER_ISRUN          0x0003
#define DECODER_ERR_DECODER_HAVE_REG       0x0004
#define DECODER_ERR_NO_DECODER             0x0005



/*
解码系统控制类型

*/
#define DCT_CTRL                          0x0001     // 播放控制命令，包括 play ，stop ，pause ，ff ，fb
#define DCT_SET_FILE                      0x0002     // 设置播放文件。



typedef struct decoder_mng
{
 struct list_head list;
 decoder_ops_t  ops ;
 OS_TCB  *tcb;
 u32 *stk ;
 volatile u32 status ;
 u8 *ptr ;
 u32 size ;
 void *fsv ;
 void *dec_io ;
}decoder_mng_t ;


#endif





