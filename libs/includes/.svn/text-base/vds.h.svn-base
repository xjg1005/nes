
/*******************************************************************************************
 File name:vds.h

 Version: 1.00

 Discription:视频解码系统头文件。


 Author:yulin deng

 Email :flowingfeeze@163.com

 Date:星期一, 07-14-2014  07:27:18下午



 Copyright:(c)JIELI  2014  @ , All Rights Reserved.
*******************************************************************************************/
#ifndef __vds_h
#define __vds_h
#include "typedef.h"
#include "list.h"
#include "avi_unpkg_if.h"
#include "dsc.h"


#define UNPKG_STK_SIZE  0x2000
#define UNPKG_TASK_PRIO  20

#define JPG_DEC_STK_SIZE   0x2000
#define JPG_DEC_TASK_PRIO  22


//vds类型 : 0  AVI解码  1 图片解码
#define VDS_TYPE_AVI            0
#define VDS_TYPE_JPG            1

#define VDS_CMD_DEC_BASE                0x9000
#define VDS_CMD_PIC_BASE                0xa000
#define VDS_CMD_ERR_BASE                0xb000


// avi解码
#define VDS_DEC_OPEN                 (VDS_CMD_DEC_BASE + 0x00)
#define VDS_DEC_START                (VDS_CMD_DEC_BASE + 0x01)
#define VDS_DEC_PAUSE                (VDS_CMD_DEC_BASE + 0x02)
#define VDS_DEC_STOP                 (VDS_CMD_DEC_BASE + 0x03)
#define VDS_DEC_FF                   (VDS_CMD_DEC_BASE + 0x04)
#define VDS_DEC_FR                   (VDS_CMD_DEC_BASE + 0x05)
#define VDS_DEC_SET_AVI              (VDS_CMD_DEC_BASE + 0x06)
#define VDS_DEC_GET_AVI_INF          (VDS_CMD_DEC_BASE + 0x07)
#define VDS_DEC_CUR_FRAME            (VDS_CMD_DEC_BASE + 0x08)
#define VDS_DEC_UNPKG_RUN            (VDS_CMD_DEC_BASE + 0x09)
#define VDS_DEC_CONTINUE             (VDS_CMD_DEC_BASE + 0x0A)
#define VDS_DEC_JPG_FBG              (VDS_CMD_DEC_BASE + 0x0B)
#define VDS_DEC_KFBG                 (VDS_CMD_DEC_BASE + 0x0C)
#define VDS_DEC_PREPARE              (VDS_CMD_DEC_BASE + 0x0D)
#define VDS_DEC_PREVIEW              (VDS_CMD_DEC_BASE + 0x0E)
#define VDS_DEC_CLOSE                (VDS_CMD_DEC_BASE + 0x0F)
#define VDS_DEC_GET_AVI_FRAME        (VDS_CMD_DEC_BASE + 0x10)
#define VDS_DEC_GET_AVI_HDL          (VDS_CMD_DEC_BASE + 0x11)
#define VDS_DEC_CLR_MSG              (VDS_CMD_DEC_BASE + 0x12)


// 图片解码
#define VDS_JPG_OPEN                (VDS_CMD_PIC_BASE + 0)
#define VDS_JPG_DEC_PIC             (VDS_CMD_PIC_BASE + 1)
#define VDS_JPG_CLOSE               (VDS_CMD_PIC_BASE + 2)
#define VDS_JPG_DEC_BUF_PIC         (VDS_CMD_PIC_BASE + 3)
#define VDS_JPG_GET_SIZE            (VDS_CMD_PIC_BASE + 4)


#define VDS_ERR_OK                  (VDS_CMD_ERR_BASE + 0x00)
#define VDS_ERR_NULL_PTR            (VDS_CMD_ERR_BASE + 0x01)
#define VDS_ERR_SET_FILE_FAILED     (VDS_CMD_ERR_BASE + 0x02)
#define VDS_ERR_UNKNOW_CMD          (VDS_CMD_ERR_BASE + 0x03)
#define VDS_ERR_UNPKG_UNINIT        (VDS_CMD_ERR_BASE + 0x04)
#define VDS_ERR_FILE_NOT_READY      (VDS_CMD_ERR_BASE + 0x05)
#define VDS_ERR_NOT_OPEN            (VDS_CMD_ERR_BASE + 0x06)
#define VDS_ERR_FF_FR               (VDS_CMD_ERR_BASE + 0x07)
#define VDS_ERR_NO_MEM              (VDS_CMD_ERR_BASE + 0x08)
#define VDS_ERR_HEAD                (VDS_CMD_ERR_BASE + 0x09)
#define VDS_ERR_TYPE                (VDS_CMD_ERR_BASE + 0x0A)
#define VDS_ERR_IMC                 (VDS_CMD_ERR_BASE + 0x0B)
#define VDS_ERR_JPG_START           (VDS_CMD_ERR_BASE + 0x0C)
#define VDS_ERR_STATUS              (VDS_CMD_ERR_BASE + 0x0D)
#define VDS_ERR_TIMEOUT             (VDS_CMD_ERR_BASE + 0x0E)




#define UNPKG_STATUS_INIT           0x00
#define UNPKG_STATUS_STOP           0x01
#define UNPKG_STATUS_RUN            0x02
#define UNPKG_STATUS_PAUSE          0x03
#define UNPKG_STATUS_FF_FR          0x04
#define UNPKG_STATUS_END            0x05
#define UNPKG_STATUS_ERR            0x06
#define UNPKG_STATUS_CLOSE          0x07
#define UNPKG_STATUS_CLR_MSG        0x08


#define JPG_DEC_STATUS_INIT         0x00
#define JPG_DEC_STATUS_STOP         0x01
#define JPG_DEC_STATUS_RUN          0x02
#define JPG_DEC_STATUS_PAUSE        0x03
#define JPG_DEC_STATUS_FF_FR        0x04
#define JPG_DEC_STATUS_END          0x05
#define JPG_DEC_STATUS_ERR          0x06
#define JPG_DEC_STATUS_STOPING      0x07
#define JPG_DEC_STATUS_CLOSE        0x08
#define JPG_DEC_STATUS_CLR_MSG      0x09



typedef struct __pic_info
{
    u8 * jpg_buf;
    u32 jpg_len;
    u32 (*yuv_out)(void * priv);
}pic_info_t ;


typedef struct dec_info
{
u32 bufsize ;           //视频解码循环BUF大小
u32 abufsize ;          //音频解码循环BUF大小
u16 x;                  //显示起X坐标
u16 y;                  //显示起X坐标
u16 des_w;              //显示器宽度
u16 des_h;              //显示器高度
u8 * des;               //显示目标设备      "lcd" "avout"
u8 * dsc_buf;           //显存
u8 * vid_buf;           //视频帧buf
u8 * aud_buf;           //音频帧buf
u32 yuv_val;
}dec_info_t ;



typedef struct _jpeg_dec_mng_t
{
u8 *stk ;
void *task ;
volatile u8 status ;
volatile u8 fs_flag;
void *dec_ops ;
void * jpg_hdl;
}jpeg_dec_mng_t ;



typedef struct __unpkg_io
{
void *fp ;
u32 (*fread)(void *fp,u8 *buf ,u32 len) ;
u32 (*fseek)(void *fp ,u32 len,u32 set) ;
u32 (*fteel)(void *fp) ;

fb_pipe_t *vpipe ;

}unpkg_io_t ;

typedef struct __unpkg_mng
{
unpkg_io_t *io ;

AVI_UNPKG_FD_T * avi_fd;

void *tcb ;
u8 *stk ;
volatile u8 status ;

jpeg_dec_mng_t * jpg_mng;
//void * disp;
//u8 imb_layer;
//u8 imc_ch;

dsc_hdl_t * dsc_hdl;

dec_info_t dec_info;

}unpkg_mng_t ;


typedef struct  __vds
{
void *fb_mng ;          //视频帧BUF
void *afb_mng ;         //音频帧BUF

fb_pipe_t *vpipe ;
fb_pipe_t *apipe ;

void *mng ;
u8 type ;              //vds类型 : 0  AVI解码 1 显示  2 jpg解码

}vds_t ;

typedef struct __list_mng_t
{
 struct list_head list;
 char name[8];
 void* ptr ;
}list_mng_t;



u32 vds_init() ;

vds_t *vds_open() ;

u32 vds_close(vds_t *vds);

u32 vds_ioctrl(vds_t *vds ,void *pram ,u32 cmd);

u32 vds_uninit() ;

u32 mutex_f_Write(void  *pFile, void  *pBuf, u32 len);
u32 mutex_f_Read(void   * pFile ,void   *pBuf , u32 btr);
tbool mutex_f_Seek (void   * pFile, u32 type, u32 offsize);
u32 mutex_f_Tell(void   * pFile);
u32 mutex_f_Size(void   * pFile);
void mutex_f_Close(void  *pFile);


#endif
