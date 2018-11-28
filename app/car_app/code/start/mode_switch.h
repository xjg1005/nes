#ifndef __MODE_SWITCH_H__
#define __MODE_SWITCH_H__


#include "config.h"



typedef struct  __mode_sw_t
{
    u32 cur_mode;
    u8 mode_index;
    u8 last_index;

}MODE_SW_T ;

typedef struct _system_status_t
{
    u8 menu_status; //on off
    u8 sys_err_status;
    u8 sys_gsensor_wakeup;
    u8 gsensor_online;
    u8 screen_off_cnt;
    u8 powerdown_flag;
}
SYS_STATUS_T;

enum
{
    SYS_MODE = BIT(0),
    ENC_MODE = BIT(1),//rec
    TPH_MODE = BIT(2),//take photo
    DEC_MODE = BIT(3),
    USB_MODE = BIT(4),

};



extern volatile u8 imc_isr_stop_flag ;
extern MODE_SW_T mode_ctrl;
extern SYS_STATUS_T sys_status;

void mode_sched(void *p_arg);
void ui_disp_task(void * p_arg);
void ui_process_task(void *p_arg);

u8 get_current_mode(void);
void init_sys_status(void);

void start_run(void);
void sw_key_msg(u8 key_msg);

void check_imc_isr(void *ptr);


#endif


