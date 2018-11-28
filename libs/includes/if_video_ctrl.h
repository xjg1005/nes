
/*******************************************************************************************
 File name:if_video_ctrl.h

 Version: 1.00

 Discription: 视频编码器对外的接口。


 Author:yulin deng

 Email :flowingfeeze@163.com

 Date:星期五, 07-04-2014  06:44:27下午



 Copyright:(c)JIELI  2014  @ , All Rights Reserved.
*******************************************************************************************/
#ifndef __if_video_ctrl_h
#define __if_video_ctrl_h

#include "typedef.h"


typedef struct _MEDIA_DEV_INF
{
	//与文件操作相关的部分
	void  * pf;
	u32 (*fs_write)(void  *pFile, void  *pBuf, u32 len);
	u32 (*fs_read)(void   * pFile ,void   *pBuf , u32 btr);
	u32 (*fs_seek)(void * pFile, u32 type, u32 offsize);
	u32 (*fs_tell)(void * pFile);//获取封装文件当前位置
    void (*fs_close)(void  *pFile);

}MEDIA_DEV_INF;

typedef struct __if_pkg_io
{
MEDIA_DEV_INF dev_inf ;

}if_pkg_io_t ;


#endif
