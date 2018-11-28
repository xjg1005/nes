#ifndef __DEC_MODE_H__
#define __DEC_MODE_H__

#include "config.h"
#include "ui_con.h"


typedef struct  __DEC_CTRL
{
    void * vds;
    volatile u8 ksframe_en;
    u8 dec_type;
    u8 dec_status;
    volatile u8 ff_fr_status;
    u8 is_del_all;

    u32 dec_fps_cnt;
    u32 dec_fps;
    u32 dec_isr_val;
    u8 dec_inout;
    u8 ff_fr_disable;//是否禁止快进快退

}DEC_CTRL_T ;



enum{
    DEC_APP_IDLE,
    DEC_APP_OPEN,
    DEC_APP_CLOSE,
    DEC_APP_READY,
    DEC_APP_RUNNING,
    DEC_APP_PAUSE,
    DEC_APP_FF,
    DEC_APP_FB,
    DEC_APP_STOP,
    DEC_APP_ABORT,//�쳣
};

extern DEC_CTRL_T dec_ctrl;


void dec_app_task(void *p_arg);

//u32 get_current_frame(void);
//u32 get_current_decfile_inf(u8 type);
TIME  get_fieltotal_time(void);
TIME  get_flie_current_palytime(void);
u32 get_current_decPic_inf(void);
void set_del_allfile_mark(u8 x);
u8 get_del_allfile_mark(void);
#endif


