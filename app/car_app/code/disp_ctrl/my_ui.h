#ifndef __MY_UI_H__
#define __MY_UI_H__

#include "typedef.h"
#include "os_api.h"
#include "ui_con.h"

typedef struct
{
	u8 reso_ratio;		//分辨率
	u8 loop_video;		//循环录影
	u8 date_label;		//日期标签
	u8 wdr;             //夜视增强
	u8 gravity_sensor;  //重力感应
	u8 key_voice;		//按键音
	u8 auto_poweroff;	//自动关机
	u8 language;		//语言
	u8 light_freq;		//光源频率
	u8 screen_protect;	//屏幕保护
	u8 photo_reso_ratio;//分辨率(拍照)
	u8 photo_date_label;//日期标签(拍照)
	u8 mic_mute;       //录音开关；
	u8 parking_guard;  //停车守卫
	u8 flashlight_mode; //补光灯模式
	u8 while_balance;  //白平衡
	u8 exposure;         //曝光度
    u8 color_effect;     //色彩特效
    u8 moving_detect; //移动侦测
    u8 TV_mode; // 电视制式
    u8 lane_dect_en;
    u16 lane_dect_carHead;
    u16 lane_dect_vanish_y;
	u8 para_flag;       //参数写入标志
	//u8
}SYS_INF;
typedef struct
{
    u8 unlock_all_flag;
    u8 note_protect_full;
	u8 protect_next_file;
    u8 is_recording;
    //u32 protect_file_num;///Burgess_151210
    //u32 protect_file_num1;
    u32 total_protect_size;
}USER_DEFINE;
//vfs_device_info[0].total_sectors[0]/(2*1024)Îª¿¨µÄ×ÜÈÝÁ¿
#define ALLOCATE_40_PER_FOR_PROTECT  vfs_device_info[0].total_sectors[0]/(5*1024)
#define ALLOCATE_30_PER_FOR_PROTECT  vfs_device_info[0].total_sectors[0]*3/(20*1024)
extern USER_DEFINE user_define;

OS_EVENT *Delfile_Semp;

extern u8 g_language_mode;
extern SYS_INF sys_info;
tbool dc_SetLanguage(u8 language);
void sys_info_reset(void);
extern bool save_sysinfo(SYS_INF *sys_info);
u8 get_gravity_sensor(void);
u8 get_parking_guard(void);
u8 get_cycle_time(void);
void draw_confirm_dialog(u8 dialog,u8 mode,void *parm, MVAR menu_var);
#endif
