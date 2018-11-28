
/*******************************************************************************************
 File name:dms.h

 Version: 1.00

 Discription: 音频管理系统模块头文件


 Author:yulin deng

 Email :flowingfeeze@163.com

 Date:星期五, 06-20-2014  09:12:30上午



 Copyright:(c)JIELI  2014  @ , All Rights Reserved.
*******************************************************************************************/
#ifndef __dms_h
#define __dms_h
#include "typedef.h"
#include "decoder_ctrl.h"
#include "os_api.h"
#include "circular_buf.h"
#define INPUT_BUFFER_SIZE 0x2000
#define FAM_STK_SIZE      0x200

#define DMS_ERR_NULL_PTR    0x0001

#define DMS_CTRL      0x0001
#define DMS_SET_FILE  0x0002

#define FDATA_TASK_PRIO   21 

typedef struct __file_analysis
{


}file_analysis_t ;


typedef struct __dms
{
	decoder_mng_t *dec ;

  u16 status ; 
	//file_analysis_t fa ;

}dms_t ;

u32 dms_init() ;




#endif
