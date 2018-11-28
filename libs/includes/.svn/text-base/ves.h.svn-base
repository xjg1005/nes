
/*******************************************************************************************
 File name:ves.h

 Version: 1.00

 Discription: 视频编码系统头文件。


 Author:yulin deng

 Email :flowingfeeze@163.com

 Date:星期五, 07-04-2014  04:01:13下午



 Copyright:(c)JIELI  2014  @ , All Rights Reserved.
*******************************************************************************************/
#ifndef __ves_h
#define __ves_h
#include "list.h"
#include "typedef.h"
#include "if_video_ctrl.h"
#include "yuv_ram.h"
#include "dsc_ram.h"
#include "jpeg_enc_if.h"
#include "avi_pkg_if.h"
#include "wfile_serve.h"
#include "audio.h"





#define PKG_STATUS_UNINIT       0x00
#define PKG_STATUS_INIT         0x01
#define PKG_STATUS_RUNING       0x02
#define PKG_STATUS_STOP         0x03
#define PKG_STATUS_READY_STOP   0x04
#define PKG_STATUS_STOPING      0x05
#define PKG_STATUS_CLR_MSG      0x06


#define PKG_STK_SIZE    0x2000


#define AVI_PKG_PRIO    18

//#define VES_VIDEO_TYPE_H264    0                // h264 对应mov 文件封装
//#define VES_VIDEO_TYPE_MJPGE   1                //  mjpeg 对应 avi 文件封装

//#define VES_AUDIO_TYPE_PCM      0               // 音频轨道为pcm 数据格式
//#define VES_AUDIO_TYPE_ADPCM    1               // 音频轨道为adpcm 数据格式


//#define JPEG_PKG_AVI     0
//#define JPEG_PKG_JPEG    1



#define VES_STATUS_UNINIT       0x00
#define VES_STATUS_INITED       0x01
#define VES_STATUS_STARTED      0x02
#define VES_STATUS_STOPED       0x03
#define VES_STATUS_SAVEED       0x04




typedef struct  __pkg_mng
{
if_pkg_io_t  *io ;
if_avipkg_ops_t * avi_pkg_ops;
void  * fd;
u8 * stk ;
void * tcb ;
u8 *ptr ;
void *fp ;
u8 is_cycle_mode;
u8 status ;
OS_SEM sem ;
} pkg_mng_t ;



typedef struct __pkg_enc_data_out
{
int (*output_frame_end)(void *priv, void *ptr);
void *priv;
fb_pipe_t *pipe ;

}pkg_enc_data_out_t ;

typedef struct __venc_mng
{
void *hd ;
JPEG_OPERATE  *info;
JPEG_ENC_OPS_T * jpeg_ops;
}venc_mng_t ;


typedef struct __ves
{
void *mng ;
void *fbmng ;
void *pipe0 ;
void *pipe1 ;

void *pipe2 ;//传输音频的帧buf

u8 type         ;              //ves 类型   // 0  录像， 1 显示  2 拍照
u8 status ;
u8 id ;
}ves_t ;



typedef struct __ves_mng_t
{
 struct list_head list;
 char name[8];
 void* ptr ;
}ves_mng_t;



typedef struct _ves_jpg_manual_info
{
    void* priv ;
    u8 *y_addr;
    u8 *u_addr;
    u8 *v_addr;
}ves_jpg_manual_info;

typedef struct _ves_jpg_bits_t
{
    void* priv ;
    u8 *is_frame_end;
    u8 *buf;
    u32 len;
    u32 is_drop_frame;//标识是否丢一帧

}ves_jpg_bits_t;


u32 ves_init() ;

ves_t *ves_open() ;

u32 ves_ioctrl(ves_t * ves,void *parm ,u32 cmd) ;

u32 ves_close(ves_t *ves);
u32 ves_avin_init();


u32 mutex_f_Write(void  *pFile, void  *pBuf, u32 len);
u32 mutex_f_Read(void   * pFile ,void   *pBuf , u32 btr);
tbool mutex_f_Seek (void   * pFile, u32 type, u32 offsize);
u32 mutex_f_Tell(void   * pFile);
void mutex_f_Close(void  *pFile);






//录像用数据接口
typedef struct VES_ENCODE_DATA
{
void *fp;//文件句柄。
u8 *vedio ; //视频源设备名称    “isp0”,”isp1”
u8 *audio ;// 音频设备名称。    “adc0”
u16 w;   //实际的宽度
u16 h;    //实际的高度

u16 fake_w;   //需要的宽度
u16 fake_h;   //需要的高度

u16 samplerate ;//采样率
u8 format ;  // 视频编码格式
u8 channel; // 音频通道数
u8 q; // 视频编码质量，0-8 9级，越大质量越好，码流越大

u8 drop_frame_en;
u8 org_fps;//原始帧率;org_fps和fr必须成倍数关系!!!

u8 fr ;// 视频帧率。目标帧率
u8 abr_en;
u32 abr_kbps;
u32 vsize ;   // 视频缓存大小
u32 asize ;   // 音频缓存大小
void *vpipe; //frame buf ;
void *apipe ; // audio buf ;
u8 osd_en;
u8 * vid_buf;

u8 dynamic_huffman_en;
u8 dynamic_huffman_prio;
u8 osd_size ;//osd字体大小 0 - 小， 1 -中  2 -大
u8 osd_format;  //  osd 0-3
u32 osd_yuv;//osd 字体颜色

//注意：下面的字符串地址必须是全局的,然后年是yyyy，月是nn，日是dd，时是hh，分是mm，秒是ss,其他字符
//是英文字母和符号

char * osd_str;//用户自定义格式，例如 "yyyy-nn-dd\hh:mm:ss" 或者 "hh:mm:ss"
u8 osd_w;//用户自定义字体大小,必须为8的倍数
u8 osd_h;
char * osd_matrix;//用户自定义字模字符串,例如“abcd....0123..”
char * osd_matrix_addr;//用户自定义字模的起始地址
u32 total_len;//用户自定义字模数组的长度

u32 time_lapse_photography_ms;

u32 discard_aud;

}ves_encode_data_t ;


//视频编码格式定义如下

#define VES_FORMAT_AVI   0x00
#define VES_FORMAT_JPEG  0x01
#define VES_FORAMT_YUV   0x02
#define VES_FORAMT_JPG_BITS   0x03





//显示用数据接口


//typedef struct VES_DISP_DATA
//{
//u8 *vedio;  //视频源设备名称    “isp0”,”isp1”
//u16 x;           // 显示起X坐标
//u16 y;           // 显示起X坐标
//u16 w;           //显示器宽度
//u16 h;           //显示器高度
//u8 fr           //显示帧率。
//
//}ves_disp_data_t;


// 拍照用数据接口

typedef struct VES_PHOTO_DATA
{
void *fp;//文件句柄。
u8 *vedio ; //视频源设备名称    “isp0”,”isp1”
u16 w;   //照片宽度
u16 h;    //照片高度
u8 format ;  // 视频编码格式

}ves_photo_data_t ;



#define VES_TYPE_ENC    0                   //录像类型
#define VES_TYPE_DISP   1                   //显示类型
#define VES_TYPE_PHOTO  2                    //拍照类型

#define VES_CMD_ENC_BASE               0x0000
#define VES_CMD_DISP_BASE              0x1000
#define VES_CMD_PHOTO_BASE             0x2000
#define VES_CMD_ERR_BASE               0xf000



//包括如下命令

// 录像
#define VES_CMD_ENC_OPEN             (VES_CMD_ENC_BASE+0)      //打开编码通道
#define VES_CMD_ENC_STOP             (VES_CMD_ENC_BASE+1)      //停止编码通道，并且保存数据
#define VES_CMD_ENC_SAVE_FILE        (VES_CMD_ENC_BASE+2)      //保存编码数据，并且不停止编码
#define VES_CMD_ENC_RESTART          (VES_CMD_ENC_BASE+3)      //重新开始编码
#define VES_CMD_ENC_START            (VES_CMD_ENC_BASE+4)
#define VES_CMD_ENC_CLOSE            (VES_CMD_ENC_BASE+5)
#define VES_CMD_ENC_PREPARE          (VES_CMD_ENC_BASE+6)
#define VES_CMD_ENC_OSD_EN           (VES_CMD_ENC_BASE+7)
#define VES_CMD_ENC_OSD_DIS          (VES_CMD_ENC_BASE+8)
#define VES_CMD_ENC_SET_FPS          (VES_CMD_ENC_BASE+9)
#define VES_CMD_ENC_DIS_VOICE        (VES_CMD_ENC_BASE+10)
#define VES_CMD_ENC_FRAME_NUM         (VES_CMD_ENC_BASE+11)
#define VES_CMD_ENC_AVI_FNUM         (VES_CMD_ENC_BASE+12)





//// 显示
//#define VES_CMD_DISP_OPEN            (VES_CMD_DISP_BASE+0)
//#define VES_CMD_DISP_CLOSE           (VES_CMD_DISP_BASE+1)
//#define VES_CMD_DISP_OSD_OPEN        (VES_CMD_DISP_BASE+2)
//#define VES_CMD_DISP_OSD_CLOSE       (VES_CMD_DISP_BASE+3)




//拍照
#define VES_CMD_PHOTO_TACK         (VES_CMD_PHOTO_BASE+0)   // 拍照
#define VES_CMD_MANUAL_TAKE_PHOTO  (VES_CMD_PHOTO_BASE+1)
#define VES_CMD_JPEG_MANUAL_STATUS (VES_CMD_PHOTO_BASE+2)
/*--- 这三个命令用于USB摄像头 ---*/
#define VES_CMD_JPEG_OUT_START     (VES_CMD_PHOTO_BASE+3)
#define VES_CMD_JPEG_OUT_STOP      (VES_CMD_PHOTO_BASE+4)
#define VES_CMD_JPEG_OUT           (VES_CMD_PHOTO_BASE+5)


#define VES_ERR_NONE                  (VES_CMD_ERR_BASE + 0)
#define VES_ERR_UNSUPPORTED_FORMAT    (VES_CMD_ERR_BASE + 1)  //不支持的录像格式
#define VES_ERR_UNSUPPORTED_CMD       (VES_CMD_ERR_BASE + 2)  //不支持的录像格式
#define VES_ERR_TYPE_ERR              (VES_CMD_ERR_BASE + 3)  //命令类型错误
#define VES_ERR_NO_MEM                (VES_CMD_ERR_BASE + 4)
#define VES_ERR_REOPEN                (VES_CMD_ERR_BASE + 5)  //重复打开
#define VES_ERR_HAVE_NO_DISP_CHANNLE  (VES_CMD_ERR_BASE + 6)  // 没有显示通道
#define VES_ERR_HAVE_NO_ENC_CHANNLE   (VES_CMD_ERR_BASE + 7)  // 没有编码通道
#define VES_ERR_OPEN_SENSER_ERR       (VES_CMD_ERR_BASE + 8)  // 打开摄像头错误
#define VES_ERR_OPEN_PKG_ERR          (VES_CMD_ERR_BASE + 9)
#define VES_ERR_OPEN_JPEG_ENC_ERR      (VES_CMD_ERR_BASE + 10)
#define VES_ERR_IMC_INIT_ERR           (VES_CMD_ERR_BASE + 11)
#define VES_ERR_UNINIT                 (VES_CMD_ERR_BASE + 12)
#define VES_ERR_UNSTART               (VES_CMD_ERR_BASE + 13)
#define VES_ERR_INV_FILE_HANDLE           (VES_CMD_ERR_BASE + 14)
#define VES_ERR_CREAT_TASK           (VES_CMD_ERR_BASE + 15)
#define VES_ERR_KSTART_AUD           (VES_CMD_ERR_BASE + 16)
#define VES_ERR_STATUS               (VES_CMD_ERR_BASE + 17)


typedef struct ves_enc_mng
{
pkg_mng_t  *pkg ;
venc_mng_t *venc ;
void * sensor;                  //sensor设备句柄
void * isp;                     // isp 设备句柄
void * imc_enc ;                // imc 编码设备句柄
u8 imc_enc_ch ;                 // 缩放模块通道
u8 isp_ch ;                     // 摄像头通道

LADC_VAR * ladc_var;
u32 old_frame_cnt;

}ves_enc_mng_t ;


//typedef struct  ves_disp_mng
//{
//void * sensor;                  //sensor设备句柄
//void * isp;                     // isp 设备句柄
//void * imc_dsc ;                // imc 显示
//u8 imc_dsc_ch ;                 // 缩放模块通道
//u8 isp_ch ;                     // 摄像头通道
//
//}ves_disp_mng_t ;

typedef struct __ves_tack_photo_parm
{
void *ptr ;
void *fp ;

}ves_tack_photo_parm_t ;

extern u8 g_dis_voice;


#endif
