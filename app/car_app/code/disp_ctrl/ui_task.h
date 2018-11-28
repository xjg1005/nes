#ifndef __UI_TASK_H__
#define __UI_TASK_H__

#include "app_cfg.h"
#include "config.h"
#include "ui_con.h"

void clear_rec_cnt(void);
u16 get_rec_cnt(void);
void disp_startlogo(void);
#if DOUBLE_UI_A
void play_screen(u8 play_status, TIME *time1, TIME *time2);
#endif
void disp_file_info(void);
//void config_disp(void);  //old
void config_disp(u8 mode);  //AVOUT JOHNSON 20160530
void updata_formate_pic(u8 index);
void ui_disp_task(void * p_arg);
void ui_process_task(void *p_arg);
void updata_mic_pic(void);
void updata_parking_guard_pic(void);
void updata_movingdet_pic(void);
void updata_flashlight_mode_pic(void);
void disp_vm_init();
void updata_menu_cfg(void);
u8 get_menu_statu(void);
void screen_on(void);
void ui_updata_timer_unreg(void);
void ui_close_imb();

extern volatile u8 usb_out_status;  //1ÒÑ°Îusb 0²åÈëusb
#endif


