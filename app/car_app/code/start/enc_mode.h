#ifndef __ENC_MODE_H__
#define __ENC_MODE_H__


#include "config.h"

typedef struct  __ENC_CTRL
{
    u8 enc_type;
    u8 volatile enc0_status;
    u8 volatile enc1_status;
    u8 is_cycle_rec0;
    u8 is_cycle_rec1;
    u8 enc1_en;
    u8 crec0_time;//循环录像的时间
    u8 crec1_time;
    u8 osd_en;
    u8 pic_osd_en;
    u8 vid_fps;
    u8 dis_voice;        //是否录声音 1:不录声音，0:录声音
    u8 motion_detect;    //是否使能移动侦测
    u8 motion_start_rec; //移动帧测是否已经开始录像
    u8 volatile lock_current_file;

    u8 usb_charge_cnt;
    u8 usb_charge_flag;

    volatile void * ves_enc;

    u8 pic_inout;
    u8 enc_inout;

    u8 skip_flag;
    u8 skip_cnt;
    u8 skip_fps;

    u8 discard_aud;
    u32 time_lapse_photography_ms;

}ENC_CTRL_T ;



enum{
    ENC_APP_IDLE,
    ENC_APP_OPEN,
    ENC_APP_CLOSE,
    ENC_APP_RUNNING,
    ENC_APP_SAVE,
    ENC_APP_STOP,
    ENC_APP_ABORT,
};



typedef struct  __AV_IN_CTRL
{
    u8 status;
    u32 cnt;
    u8 parking_status;
    u32 p_cnt;
}AV_IN_CTRL_T ;



extern ENC_CTRL_T enc_ctrl;

void enc_app_task(void * p_arg);



void mic_mute_fun(u8 x);
void motion_det_if(u8 x);
void disable_voice(u8 x);
u8 get_parking_status(void);
#endif


