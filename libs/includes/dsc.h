#ifndef __DSC_H__
#define __DSC_H__


#include "imc_if.h"
#include "imb_module.h"
#include "isp_dev.h"




typedef struct _disp_iparm
{
    u8 * drc;           //��ʾԴ�豸����    ��isp0����isp1��"rep"
    u16 x;              //��ʾ��X����
    u16 y;              //��ʾ��Y����
    u16 des_w;          //��ʾ������
    u16 des_h;          //��ʾ���߶�
    u8 * des;           //��ʾĿ���豸      "lcd" "avout"
    int (*frame_cb)(void *);    //֡�ص�����
    u8 * dsc_buf;       //�Դ��ַ

    u32 yuv_val;

}disp_iparm_t;




typedef struct _dsc_hdl
{
    disp_iparm_t info;
    void * isp;
    IMGLAYER *image;
    u8 src;
    pic_size_t src_size;
    u8 imc_ch;
    volatile u8 status;

}dsc_hdl_t;


enum{
    DSC_IDLE,
    DSC_OPEN,
    DSC_START,
    DSC_STOP,
    DSC_CLOSE,

};


//dsc_hdl_t * dsc_open(disp_iparm_t * info);
dsc_hdl_t * dsc_open(disp_iparm_t * info, IMGLAYER *image);
u32 dsc_close(dsc_hdl_t * hdl);
u32 dsc_start(dsc_hdl_t * hdl);
u32 dsc_stop(dsc_hdl_t * hdl);

#endif


