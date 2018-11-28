
/*******************************************************************************************
 File name:ves.h

 Version: 1.00

 Discription: ��Ƶ����ϵͳͷ�ļ���


 Author:yulin deng

 Email :flowingfeeze@163.com

 Date:������, 07-04-2014  04:01:13����



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

//#define VES_VIDEO_TYPE_H264    0                // h264 ��Ӧmov �ļ���װ
//#define VES_VIDEO_TYPE_MJPGE   1                //  mjpeg ��Ӧ avi �ļ���װ

//#define VES_AUDIO_TYPE_PCM      0               // ��Ƶ���Ϊpcm ���ݸ�ʽ
//#define VES_AUDIO_TYPE_ADPCM    1               // ��Ƶ���Ϊadpcm ���ݸ�ʽ


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

void *pipe2 ;//������Ƶ��֡buf

u8 type         ;              //ves ����   // 0  ¼�� 1 ��ʾ  2 ����
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
    u32 is_drop_frame;//��ʶ�Ƿ�һ֡

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






//¼�������ݽӿ�
typedef struct VES_ENCODE_DATA
{
void *fp;//�ļ������
u8 *vedio ; //��ƵԴ�豸����    ��isp0��,��isp1��
u8 *audio ;// ��Ƶ�豸���ơ�    ��adc0��
u16 w;   //ʵ�ʵĿ��
u16 h;    //ʵ�ʵĸ߶�

u16 fake_w;   //��Ҫ�Ŀ��
u16 fake_h;   //��Ҫ�ĸ߶�

u16 samplerate ;//������
u8 format ;  // ��Ƶ�����ʽ
u8 channel; // ��Ƶͨ����
u8 q; // ��Ƶ����������0-8 9����Խ������Խ�ã�����Խ��

u8 drop_frame_en;
u8 org_fps;//ԭʼ֡��;org_fps��fr����ɱ�����ϵ!!!

u8 fr ;// ��Ƶ֡�ʡ�Ŀ��֡��
u8 abr_en;
u32 abr_kbps;
u32 vsize ;   // ��Ƶ�����С
u32 asize ;   // ��Ƶ�����С
void *vpipe; //frame buf ;
void *apipe ; // audio buf ;
u8 osd_en;
u8 * vid_buf;

u8 dynamic_huffman_en;
u8 dynamic_huffman_prio;
u8 osd_size ;//osd�����С 0 - С�� 1 -��  2 -��
u8 osd_format;  //  osd 0-3
u32 osd_yuv;//osd ������ɫ

//ע�⣺������ַ�����ַ������ȫ�ֵ�,Ȼ������yyyy������nn������dd��ʱ��hh������mm������ss,�����ַ�
//��Ӣ����ĸ�ͷ���

char * osd_str;//�û��Զ����ʽ������ "yyyy-nn-dd\hh:mm:ss" ���� "hh:mm:ss"
u8 osd_w;//�û��Զ��������С,����Ϊ8�ı���
u8 osd_h;
char * osd_matrix;//�û��Զ�����ģ�ַ���,���硰abcd....0123..��
char * osd_matrix_addr;//�û��Զ�����ģ����ʼ��ַ
u32 total_len;//�û��Զ�����ģ����ĳ���

u32 time_lapse_photography_ms;

u32 discard_aud;

}ves_encode_data_t ;


//��Ƶ�����ʽ��������

#define VES_FORMAT_AVI   0x00
#define VES_FORMAT_JPEG  0x01
#define VES_FORAMT_YUV   0x02
#define VES_FORAMT_JPG_BITS   0x03





//��ʾ�����ݽӿ�


//typedef struct VES_DISP_DATA
//{
//u8 *vedio;  //��ƵԴ�豸����    ��isp0��,��isp1��
//u16 x;           // ��ʾ��X����
//u16 y;           // ��ʾ��X����
//u16 w;           //��ʾ�����
//u16 h;           //��ʾ���߶�
//u8 fr           //��ʾ֡�ʡ�
//
//}ves_disp_data_t;


// ���������ݽӿ�

typedef struct VES_PHOTO_DATA
{
void *fp;//�ļ������
u8 *vedio ; //��ƵԴ�豸����    ��isp0��,��isp1��
u16 w;   //��Ƭ���
u16 h;    //��Ƭ�߶�
u8 format ;  // ��Ƶ�����ʽ

}ves_photo_data_t ;



#define VES_TYPE_ENC    0                   //¼������
#define VES_TYPE_DISP   1                   //��ʾ����
#define VES_TYPE_PHOTO  2                    //��������

#define VES_CMD_ENC_BASE               0x0000
#define VES_CMD_DISP_BASE              0x1000
#define VES_CMD_PHOTO_BASE             0x2000
#define VES_CMD_ERR_BASE               0xf000



//������������

// ¼��
#define VES_CMD_ENC_OPEN             (VES_CMD_ENC_BASE+0)      //�򿪱���ͨ��
#define VES_CMD_ENC_STOP             (VES_CMD_ENC_BASE+1)      //ֹͣ����ͨ�������ұ�������
#define VES_CMD_ENC_SAVE_FILE        (VES_CMD_ENC_BASE+2)      //����������ݣ����Ҳ�ֹͣ����
#define VES_CMD_ENC_RESTART          (VES_CMD_ENC_BASE+3)      //���¿�ʼ����
#define VES_CMD_ENC_START            (VES_CMD_ENC_BASE+4)
#define VES_CMD_ENC_CLOSE            (VES_CMD_ENC_BASE+5)
#define VES_CMD_ENC_PREPARE          (VES_CMD_ENC_BASE+6)
#define VES_CMD_ENC_OSD_EN           (VES_CMD_ENC_BASE+7)
#define VES_CMD_ENC_OSD_DIS          (VES_CMD_ENC_BASE+8)
#define VES_CMD_ENC_SET_FPS          (VES_CMD_ENC_BASE+9)
#define VES_CMD_ENC_DIS_VOICE        (VES_CMD_ENC_BASE+10)
#define VES_CMD_ENC_FRAME_NUM         (VES_CMD_ENC_BASE+11)
#define VES_CMD_ENC_AVI_FNUM         (VES_CMD_ENC_BASE+12)





//// ��ʾ
//#define VES_CMD_DISP_OPEN            (VES_CMD_DISP_BASE+0)
//#define VES_CMD_DISP_CLOSE           (VES_CMD_DISP_BASE+1)
//#define VES_CMD_DISP_OSD_OPEN        (VES_CMD_DISP_BASE+2)
//#define VES_CMD_DISP_OSD_CLOSE       (VES_CMD_DISP_BASE+3)




//����
#define VES_CMD_PHOTO_TACK         (VES_CMD_PHOTO_BASE+0)   // ����
#define VES_CMD_MANUAL_TAKE_PHOTO  (VES_CMD_PHOTO_BASE+1)
#define VES_CMD_JPEG_MANUAL_STATUS (VES_CMD_PHOTO_BASE+2)
/*--- ��������������USB����ͷ ---*/
#define VES_CMD_JPEG_OUT_START     (VES_CMD_PHOTO_BASE+3)
#define VES_CMD_JPEG_OUT_STOP      (VES_CMD_PHOTO_BASE+4)
#define VES_CMD_JPEG_OUT           (VES_CMD_PHOTO_BASE+5)


#define VES_ERR_NONE                  (VES_CMD_ERR_BASE + 0)
#define VES_ERR_UNSUPPORTED_FORMAT    (VES_CMD_ERR_BASE + 1)  //��֧�ֵ�¼���ʽ
#define VES_ERR_UNSUPPORTED_CMD       (VES_CMD_ERR_BASE + 2)  //��֧�ֵ�¼���ʽ
#define VES_ERR_TYPE_ERR              (VES_CMD_ERR_BASE + 3)  //�������ʹ���
#define VES_ERR_NO_MEM                (VES_CMD_ERR_BASE + 4)
#define VES_ERR_REOPEN                (VES_CMD_ERR_BASE + 5)  //�ظ���
#define VES_ERR_HAVE_NO_DISP_CHANNLE  (VES_CMD_ERR_BASE + 6)  // û����ʾͨ��
#define VES_ERR_HAVE_NO_ENC_CHANNLE   (VES_CMD_ERR_BASE + 7)  // û�б���ͨ��
#define VES_ERR_OPEN_SENSER_ERR       (VES_CMD_ERR_BASE + 8)  // ������ͷ����
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
void * sensor;                  //sensor�豸���
void * isp;                     // isp �豸���
void * imc_enc ;                // imc �����豸���
u8 imc_enc_ch ;                 // ����ģ��ͨ��
u8 isp_ch ;                     // ����ͷͨ��

LADC_VAR * ladc_var;
u32 old_frame_cnt;

}ves_enc_mng_t ;


//typedef struct  ves_disp_mng
//{
//void * sensor;                  //sensor�豸���
//void * isp;                     // isp �豸���
//void * imc_dsc ;                // imc ��ʾ
//u8 imc_dsc_ch ;                 // ����ģ��ͨ��
//u8 isp_ch ;                     // ����ͷͨ��
//
//}ves_disp_mng_t ;

typedef struct __ves_tack_photo_parm
{
void *ptr ;
void *fp ;

}ves_tack_photo_parm_t ;

extern u8 g_dis_voice;


#endif
