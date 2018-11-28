#ifndef __LANE_DECT_H__
#define __LANE_DECT_H__


#include "typedef.h"
#include "os_api.h"




typedef struct _ldet_iparm
{
    u8 src;                   //显示源 IMC_SRC_ISP0 / IMC_SRC_ISP1
    u8 * drc;                   //显示源设备名称    “isp0””isp1”
    u8 * dsc_buf;               //显存地址
    u8 buf_num;
    u8 task_prio;               //车道检测任务优先级

    int dsc_w;                  //显示器宽度
    int dsc_h;                  //显示器高度
    int vanish_y;               //马路消失点纵坐标
    int carHead;                //车头盖坐标
    int len_factor;             //镜头广角参数

}ldet_iparm_t;


typedef struct _ldet_hdl
{
    ldet_iparm_t info;
    u8 src;
    u8 imc_ch;                      //IMC_DISP_CH0 / IMC_DISP_CH1
    OS_SEM lane_sem;
    struct dsc_ram * dsc_ram;
    volatile u8 status;
    volatile u8 is_offset;
    volatile void * lane_hdl;
    volatile u8 inout_flag;
    volatile u8 cnt;
    volatile u8 run_flag;

}ldet_hdl_t;

enum{
    LDET_IDLE = 0,
    LDET_OPEN,
    LDET_CLOSE,
    LDET_START,
    LDET_STOP,

};

ldet_hdl_t * lane_dect_open(ldet_iparm_t * info);
int lane_dect_close(ldet_hdl_t * ldet_hdl);
int lane_dect_start(ldet_hdl_t * ldet_hdl, ldet_iparm_t * info);
int lane_dect_stop(ldet_hdl_t * ldet_hdl);
int lane_dect_warning(ldet_hdl_t * ldet_hdl);
int lane_dect_clr_warning(ldet_hdl_t * ldet_hdl);
void lane_dect_post(ldet_hdl_t * ldet_hdl, u8 src, u8 imc_ch);
ldet_hdl_t * get_ldet_hdl();


#endif



