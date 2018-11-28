
#ifndef __JPEG_DEC_IF_H__
#define __JPEG_DEC_IF_H__


#include "circular_buf.h"
#include "yuv_ram.h"
#include "frame_buf.h"
#define JPG_DEC_FMT_YUV444    2
#define JPG_DEC_FMT_YUV422    1
#define JPG_DEC_FMT_YUV420    0

//jpeg解码模式
#define JPG_DEC_MODE_NORMAL         0X01
#define JPG_DEC_MODE_SPECIAL        0X02


enum{
    JPG_DEC_CMD_HEAD,
    JPG_DEC_CMD_START,
    JPG_DEC_CMD_STOP,
    JPG_DEC_CMD_ABORT_STOP,
    JPG_DEC_CMD_DEAL_WAIT_DATA,
    JPG_DEC_CMD_SEM_INIT,
    JPG_DEC_CMD_POST_DAT_OK,
    JPG_DEC_CMD_IS_FEND,
    JPG_DEC_CMD_CUR_FNUM,//当前已经解码完的帧号
    JPG_DEC_CMD_SET_FNUM,
};

enum{
	JPG_DEC_ERR_NONE,
	JPG_DEC_ERR_READ_NULL,      //读取不到数据
	JPG_DEC_ERR_HEAD,			//解码头出错
	JPG_DEC_ERR_STATUS,		    //状态出错
	JPG_DEC_ERR_NOMEM,          // 没有内存
	JPG_DEC_ERR_NOT_SUPPORT,    //不支持的类型

};


typedef fbuf_t* (*JPEG_HOOK_READ)(void * priv );
//typedef u32 (*JPEG_HOOK_SEEK)(u32 seekmode,u32 offset);

typedef struct _JPEG_DEC_YUV_OUT
{
    u32 line ;
    u32 width;
    struct yuv_addr *yuv;
    u8 is_frame_begin;
    u8 mode;//jpeg解码模式
    u8 yuv_type;
}JPEG_DEC_YUV_OUT;


typedef struct _JPEG_DEC_UNPKG
{
    cbuffer_t * jpg_unpkg_cbuf;
    void * file_hdl;

}JPEG_DEC_UNPKG;


typedef struct _JPEG_DEC_INFO
{
    fb_pipe_t * pipe;
    u32 (*jpg_info_yuv_out)(void * priv);
    u32 (*jpg_info_wait_data)(void * priv);
    void * priv;
    u32 jpg_width;
    u32 jpg_heigh;
    u32 jpg_org_w;
    u32 jpg_org_h;
    u8 yuv_type;
    u8 dec_frame;//是否按帧解码

}JPEG_DEC_INFO;



typedef struct _JPEG_DEC_OPS_T
{
	char *name ;               // 解码器名称
	int (*init)();
	void * (*open)(JPEG_DEC_INFO * dec_info);
	s32 (*close)(void * priv);
	s32 (*read)(void *priv ,u8*buf, u32 addr , u32 len);
	s32 (*write)(void *priv ,u8*buf , u32 addr , u32 len);
	s32 (*ioctrl)(void *priv ,void* parm, u32 cmd );
	u32 isr_num;
}JPEG_DEC_OPS_T ;













JPEG_DEC_OPS_T *get_jpeg0_dec_ops();
JPEG_DEC_OPS_T *get_jpeg1_dec_ops();








#endif


