/*******************************************************************************************
 File Name: jpeg_encode.h
 Version: 1.00
 Discription:
 Author:yulin deng
 Email :flowingfeeze@163.com
 Date: 11 2013
 Copyright:(c)JIELI  2011  @ , All Rights Reserved.
*******************************************************************************************/
#ifndef  __JPEG_ENC_IF_H__
#define  __JPEG_ENC_IF_H__

#include "config.h"

typedef struct _JPEG_OPERATE
{
	u16 width;        //图像宽度
	u16 height;       //图像高度
	u8 encode_format; //编码格式
	u8 mode;    //数据输入方式，从摄像头或者指定地址
	u8 q ;    //编码质量，0-8 9级，越大质量越好，码流越大

    u8 abr_en;
	u8 fps; //帧率
	u32 abr_kbps; //目标码率

    u8 dynamic_huffman_en;
    u8 dynamic_huffman_prio;
	void *fbpipe;
//	void * jpeg_fd;
	struct video_ram * vram;
	//int (*output)(void *priv, void *buf, u32 len);
	int (*output_frame_end)(void *priv,void *ptr);
	void *priv;

	u8 imc_ch ;
	u8 imc_cmd;
	s32 (*imc_ioctrl)(void *parm ,u32 cmd);

	u32 drop_frame_en;
	u32 org_fps;//org_fps和des_fps必须成倍数关系!!!
	u32 des_fps;
	u32 timer_val;
    u32 timer_cnt;
	u32 drop_frame_cnt;
	u32 need_add;
	u32 df_cnt;

	u32 time_lapse_photography_ms;
	u32 time_lapse_photography_time_hdl;

}JPEG_OPERATE;


typedef struct _JPEG_ENC_OPS_T
{
	char *name ;               // 编码码器名称
	void* (*init)(JPEG_OPERATE *jpeg_io);
	int (*open)(void *hd);
//	int (*start)(void *hd);
//	void (*stop)(void * priv);
	void (*close)(void *hd);
	s32(*ioctrl)(void *priv,void *parm , u32 cmd);
//	int (*manual_kstart)(void *hd, u8 *y_addr,u8 *u_addr,u8 *v_addr);
//	int (*manual_status)(void *hd);
//	u8 (*jump_frame)(void * priv);
//	int (*is_stop)(void *priv);
    u32 initnum ;  // 中断号
}JPEG_ENC_OPS_T ;


typedef struct _jpg_manual_info
{
    void* priv ;
    u8 *y_addr;
    u8 *u_addr;
    u8 *v_addr;
}jpg_manual_info;



enum
{
    READ_FROM_BUF,
    READ_FROM_SENSOR,
};


//只在 READ_FROM_BUF模式下有效!!!
enum
{
    JPEG_ENC_MCU_IDLE,
    JPEG_ENC_MCU_START,
    JPEG_ENC_MCU_END,
    JPEG_ENC_FRAME_END,
};


enum
{
    JPEG_CMD_ENC_START,
    JPEG_CMD_ENC_STOP,
    JPEG_CMD_ENC_MKSTART,
    JPEG_CMD_ENC_GET_STATUS,
    JPEG_CMD_ENC_JUMP_FRAME,
    JPEG_CMD_ENC_IS_STOP,
    JPEG_CMD_ENC_GET_VRAM,
    JPEG_CMD_ENC_IMC_FRAME_END,
    JPEG_CMD_ENC_IMC_PARM,
    JPEG_CMD_ENC_RESTART,
    JPEG_CMD_ENC_FREE_FB,
    JPEG_CMD_ENC_GET_FRAME_CNT,

};


#define JPEG_FMT_YUV444    1
#define JPEG_FMT_YUV422    2
#define JPEG_FMT_YUV420    3


void* jpeg_enc_init(JPEG_OPERATE *jpeginfo);
int jpeg_enc_open(void *hd);
void jpeg_enc_close(void *hd);
int jpeg_enc_start(void  *hd);
int jpeg_enc_stop(void *priv);
int jpeg_enc_is_stop(void *priv);

int jpeg_enc_manual_kstart(void *hd, u8 *ptr);
int jpeg_enc_manual_status(void *hd);

#endif
