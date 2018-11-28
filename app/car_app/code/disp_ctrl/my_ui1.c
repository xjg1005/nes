#include "ui_con.h"
//#include "ui_struct.h"
#include "menu.h"
#include "msg.h"
//#include "ui_msg.h"
#include "ui.h"
#include "dc_showres.h"
#include "my_ui.h"
#include "my_msg.h"
#include "ui_com.h"
#include "os_api.h"
#include "vm_api.h"
//#include "my_ui.h"
#include "app_cfg.h"
#include "irtc.h"
#include "FileSelecter.h"
#include "mode_switch.h"
#include "car_fs_api.h"
#include "gravity_sensor_manage.h"
SYS_INF sys_info;
USER_DEFINE user_define;
//#define save_sysinfo(a)
#define Sleep(a)        os_time_dly(a)
#define app_get_msg(a)  get_msg(a)
extern const u8 wb_table[];
extern const u8 ae_bftype[];
extern void set_select_item(u8 item);
extern u8 get_select_item();
extern u16 get_string_height(u16 id);

//extern bool save_sysinfo(SYS_INF *sys_info);
extern bool load_sysinfo(SYS_INF *sys_info);
extern u8 cb_play_setting(u16 id,u8 mode);
extern u8 cb_delfile(u16 id,u8 mode);
extern u8 cb_protect_file(u16 id,u8 mode);
extern u8 cb_delallfile(u16 id,u8 mode);

extern u8 cb_lockfile(u16 id,u8 mode);
extern u8 cb_unlockfile(u16 id,u8 mode);
extern u8 cb_unlock_all_file(u16 id,u8 mode);

extern u8 cb_setting(u16 id,u8 mode);

extern u8 cb_normal_setting(u16 id,u8 mode);
extern u8 cb_perfer_setting(u16 id,u8 mode);
extern u8 cb_safety_assist_setting(u16 id,u8 mode);
extern u8 cb_system_setting(u16 id,u8 mode);

extern u8 cb_while_balance(u16 id,u8 mode);
extern u8 cb_sennor_exposure(u16 id,u8 mode);
extern u8 cb_color_effect(u16 id,u8 mode);
extern u8 cb_moving_detect(u16 id,u8 mode);

extern u8 cb_formatting(u16 id,u8 mode);
extern u8 cb_restore_factory_settings(u16 id,u8 mode);
extern u8 cb_reso_ratio(u16 id,u8 mode);
extern u8 cb_loop_video(u16 id,u8 mode);
extern u8 cb_mic_mute(u16 id, u8 mode);
extern u8 cb_parking_guard(u16 id,u8 mode);
extern u8 cb_date_label(u16 id,u8 mode);
extern u8 cb_gravity_reaction(u16 id,u8 mode);
extern u8 cb_key_voice(u16 id,u8 mode);
extern u8 cb_wdr_function(u16 id,u8 mode);
extern u8 cb_auto_power(u16 id,u8 mode);
extern u8 cb_language_set(u16 id,u8 mode);
extern u8 cb_light_freq(u16 id,u8 mode);
extern u8 cb_TV_mode(u16 id,u8 mode);
#ifdef FLASH_LIGHT
extern u8 cb_flashlight_mode(u16 id,u8 mode);
#endif

extern u8 cb_screen_protect(u16 id,u8 mode);
extern u8 cb_photo_setting(u16 id,u8 mode);
extern u8 cb_photo_reso_ratio(u16 id,u8 mode);
extern u8 cb_photo_date_label(u16 id,u8 mode);




extern void disp_startlogo(void);
extern void disp_closelogo(void);
//------------------------------------------------------------视频--------------------------------------------------------------------//
u8 language_talbe[] = {Chinese_Simplified, Chinese_Traditional,English,German,Japanese,Russian};

bool save_sysinfo(SYS_INF *sys_info)
{
    vm_api_write(SYS_SET_INFO,sys_info);
}



#if 0
const S_MENU setting[] =
{
    {M39,	P17,	1}, //白平衡
    {M42,	P17,	2}, //曝光度
    {M40,	P17,	3}, //色彩
    {M04,	P17,	4}, //移动侦测

    {M171,	P17,	5}, //录音使能
	{M172,	P12,	6}, //停车守卫
	{M06,	P36,	7}, //日期标签
	{M103,	P37,	8}, //重力感应
	{M96,	P38,	9}, //按键声音
	{M79,	P18,	OTHER_SCREEN}, //日期/时间
	{M95,	P40,	11}, //自动关机
	{M97,	P39,	12}, //语言设置
	{M99,	P15,	13}, //光源频率

	{M47,	P15,	14}, //电视制式
	{M46,	P15,	15}, //补光灯


	{M102,	P43,	16},//屏幕保护
	{M105,	P46,	17},//格式化
	{M106,	P42,	18},//默认设置
	{M107,  P42,    OTHER_SCREEN},//版本
};
#else
const S_MENU setting[] =
{
    {M35,	P88,	1}, //解释度
    {M37,	P89,	2}, //图像质量
    {M02,	P90,	3}, //循环录影

    {M39,	P92,	4}, //白平衡
    {M42,	P94,	5}, //曝光度
//    {M40,	P17,	3}, //色彩
    {M04,	P91,	6}, //移动侦测

    {M171,	P97,	7}, //录音使能
	{M172,	P98,	8}, //停车守卫
	{M06,	P102,	9}, //日期标签
	{M103,	P99,	10}, //重力感应
	{M96,	P93,	11}, //按键声音
	{M181,	P109,	12}, //WDR
	{M79,	P100,	OTHER_SCREEN}, //日期/时间
	{M95,	P106,	13}, //自动关机
	{M97,	P101,	14}, //语言设置
	{M99,	P107,	15}, //光源频率

//	{M47,	P108,	15}, //电视制式

#if 0//def FLASH_LIGHT //JOHNSON 20160618
	{M46,	P15,	16}, //补光灯

	{M102,	P96,	17},//屏幕保护
	{M105,	P103,	18},//格式化
	{M106,	P104,	19},//默认设置
	{M107,  P105,    OTHER_SCREEN},//版本
#else
	{M102,	P96,	16},//屏幕保护
	{M105,	P103,	17},//格式化
	{M106,	P104,	18},//默认设置
	{M107,  P105,    OTHER_SCREEN},//版本
#endif

};
#endif

//分辨率
const S_MENU reso_ratio[] =
{
#ifndef DOUBLE_VEDIO
	{M09,	0,  NO_MENU},//1080P 1440x1080
#endif
	{M10,	0,  NO_MENU},// 1680X960
	{M12,	0,  NO_MENU},// 1280x720
};
//循环录影
const S_MENU loop_video[] =
{
	{M14,   0,  NO_MENU},//关
	{M15,   0,  NO_MENU},//1分钟
	{M16,   0,  NO_MENU},//2分钟
	{M17,   0,  NO_MENU},//3分钟
};

//白平衡
const S_MENU while_balance[] =
{
	{M64,	0,  NO_MENU},//自动
	{M65,	0,  NO_MENU},// 日光
	{M66,	0,  NO_MENU},//阴天
	{M67,	0,  NO_MENU},//钨丝灯
	{M68,	0,  NO_MENU},//荧光灯
};


//曝光度
const S_MENU sennor_exposure[] =
{
	{M18,	0,  NO_MENU},// -3
	{M19,	0,  NO_MENU},// -2
	{M20,	0,  NO_MENU},// -1
	{M21,	0,  NO_MENU},//  0
	{M22,	0,  NO_MENU},//  1
	{M23,	0,  NO_MENU},//  2
	{M24,	0,  NO_MENU},//  3
};

//色彩特效
const S_MENU color_effect[] =
{
	{M69,	0,  NO_MENU},// 无特效
	{M70,	0,  NO_MENU},// 黑白
	{M71,	0,  NO_MENU},// 怀旧
	{M72,	0,  NO_MENU},//  负片
	{M73,	0,  NO_MENU},//  暴晒
};

//移动侦测
const S_MENU moving_detect[] =
{
	{M32,   0,  NO_MENU},//关
    {M31,   0,  NO_MENU},//开
};
//日期标签
const S_MENU date_label[]=
{
    {M32,   0,  NO_MENU},//关
	{M31,   0,  NO_MENU},//开
};

//录音使能
const S_MENU mic_mute[] =
{
    {M32, 0,  NO_MENU},
    {M31, 0,  NO_MENU},
};

//停车守卫
const S_MENU parking_guard[]=
{
    {M32,   0,  NO_MENU},//关
	{M31,   0,  NO_MENU},//开
};

//重力感应
const S_MENU gravity_reaction[]=
{
	{M32,    0,   NO_MENU},//关
	{M129,   0,   NO_MENU},//高
	{M130,   0,   NO_MENU},//中
	{M131,   0,   NO_MENU},//低
};
//按键音
const S_MENU key_voice[] =
{
    {M32,   0,    NO_MENU},//关
	{M31,   0,    NO_MENU},//开

};
//wdr
const S_MENU wdr_function[] =
{
    {M32,   0,    NO_MENU},//关
	{M31,   0,    NO_MENU},//开

};
//自动关机
const S_MENU auto_power[] =
{
#ifdef LINGDUPRJ
    {M32,  0,    NO_MENU},//关
    {M15,  0,    NO_MENU},//1分
    {M16,  0,    NO_MENU},//2分
    {M17,  0,    NO_MENU},//3分
#else
    {M32,   0,    NO_MENU},//关
    {M169,  0,    NO_MENU},//30秒
    {M15,  0,    NO_MENU},//1分
    {M16,  0,    NO_MENU},//2分
#endif
};

//语言设置
const S_MENU language_set[]=
{
	{M114,   0,   NO_MENU},//简体中文
	{M113,   0,   NO_MENU},//繁体中文
	{M119,   0,   NO_MENU},//English
	{M116,   0,   NO_MENU},//English
	{M111,   0,   NO_MENU},//日语
	{M112,   0,   NO_MENU},//俄语
};
//光源频率
const S_MENU light_freq[]=
{
	{M122,   0,   NO_MENU},//50Hz
	{M123,   0,   NO_MENU},//60Hz
};


//电视制式
const S_MENU TV_mode[]=
{
	{M48,   0,   NO_MENU},//PAL
	{M49,   0,   NO_MENU},//NTSC

};


//补光灯
const S_MENU flashlight_mode[]=
{
	{M32,   0,  NO_MENU},//关
    {M31,   0,  NO_MENU},//开
    //{M125,   0,  NO_MENU},//自动

};


//屏幕保护
const S_MENU screen_protect[]=
{
    {M32,    0,   NO_MENU},//关
    {M167,   0,   NO_MENU},//10S
    {M169,   0,   NO_MENU},//30s
    {M15,   0,   NO_MENU},//60s
};

//确认操作
const S_MENU confirm[]=
{
	{M140,   0,   NO_MENU},//取消
    {M141,   0,   NO_MENU},//确认
};
//分辨率
const S_MENU photo_reso_ratio[]=
{
	{M170,   P26,   NO_MENU},//0.9M 1280x720
	{M55,   P26,   NO_MENU},//2M 1600*900
	{M54,   P26,   NO_MENU},//3M 1920*1080
//	{M53,   P26,   NO_MENU},//5M 2592x1944
};
///<菜单列表
S_MENULIST  menu_video_list[] =
{
	/* 0*/{NO_MENU,     NULL,  STYLE_NORMAL,   setting, 	cb_setting,	 sizeof(setting) / sizeof(setting[0])   },
	 /* 1*/{0,     M35,  STYLE_NORMAL,   reso_ratio, 	    cb_reso_ratio,	 sizeof(reso_ratio) / sizeof(reso_ratio[0]) },
	/* 2*/ {0,    M35,   STYLE_NORMAL,  photo_reso_ratio,		cb_photo_reso_ratio,	 sizeof(photo_reso_ratio) / sizeof(photo_reso_ratio[0]) },
	 /* 3*/{0,     M02,  STYLE_NORMAL,   loop_video,		cb_loop_video,	 sizeof(loop_video) / sizeof(loop_video[0]) },
	/* 4*/{0,     M39,  STYLE_NORMAL,   while_balance, 	    cb_while_balance,	 sizeof(while_balance) / sizeof(while_balance[0]) },//白平衡
	/* 5*/{0,     M42,  STYLE_NORMAL,   sennor_exposure,		cb_sennor_exposure,	 sizeof(sennor_exposure) / sizeof(sennor_exposure[0]) },
//	/* 3*/{0,     M40,  STYLE_NORMAL,   color_effect,		cb_color_effect,	 sizeof(color_effect) / sizeof(color_effect[0]) },
	/* 6*/{0,     M04,  STYLE_NORMAL,   moving_detect,		cb_moving_detect,	 sizeof(moving_detect) / sizeof(moving_detect[0]) },


	/* 7*/{0,     M171,  STYLE_NORMAL,   mic_mute, 	    cb_mic_mute,	 sizeof(mic_mute) / sizeof(mic_mute[0]) },
	/* 8*/{0,     M172,  STYLE_NORMAL,   parking_guard,		cb_parking_guard,	 sizeof(parking_guard) / sizeof(parking_guard[0]) },

	/* 9*/{0,     M06,  STYLE_NORMAL,   date_label,       cb_date_label,	 sizeof(date_label) / sizeof(date_label[0]) },

	/* 10*/{0,     M103,  STYLE_NORMAL,   gravity_reaction,cb_gravity_reaction,	 sizeof(gravity_reaction) / sizeof(gravity_reaction[0]) },
	/* 11*/{0,     M96,  STYLE_NORMAL,   key_voice,		cb_key_voice,	 sizeof(key_voice) / sizeof(key_voice[0]) },
	/* 11*/{0,     M181,  STYLE_NORMAL,   wdr_function,		cb_wdr_function,	 sizeof(wdr_function) / sizeof(wdr_function[0]) },
	/* 12*/{0,     M95,  STYLE_NORMAL,   auto_power,  	    cb_auto_power,	 sizeof(auto_power) / sizeof(auto_power[0]) },
	/* 13*/{0,     M97,  STYLE_NORMAL,   language_set,		cb_language_set,	 sizeof(language_set) / sizeof(language_set[0]) },
	/* 14*/{0,     M99,  STYLE_NORMAL,   light_freq,		cb_light_freq,	 sizeof(light_freq) / sizeof(light_freq[0]) },


//	/* 15*/{0,     M47,  STYLE_NORMAL,   TV_mode,		cb_TV_mode,	 sizeof(TV_mode) / sizeof(TV_mode[0]) }, //電視制式
#if 0//def FLASH_LIGHT //JOHNSON 20160618
	/* 16*/{0,     M46,  STYLE_NORMAL,   flashlight_mode,		cb_flashlight_mode,	 sizeof(flashlight_mode) / sizeof(flashlight_mode[0]) }, //背光
#endif
	/* 17*/{0,     M102,  STYLE_NORMAL,   screen_protect,       cb_screen_protect,	 sizeof(screen_protect) / sizeof(screen_protect[0]) },
	/*18*/{0,     M105, STYLE_CONFIRM,        confirm,		    cb_formatting,	 sizeof(confirm) / sizeof(confirm[0]) },
	/*19*/{0,     M142, STYLE_CONFIRM,        confirm, 	        cb_restore_factory_settings,	 sizeof(confirm) / sizeof(confirm[0]) },
};
S_MENULIST  menu_videoMP_list[] =
{
    {NO_MENU,     M35,  STYLE_NORMAL,   reso_ratio, 	    cb_reso_ratio,	 sizeof(reso_ratio) / sizeof(reso_ratio[0]) },
};
S_MENULIST  menu_videolp_list[] =
{
    {NO_MENU,     M02,  STYLE_NORMAL,   loop_video,		cb_loop_video,	 sizeof(loop_video) / sizeof(loop_video[0]) },
};


const S_MENU  normal_setting[] =
{
    {M35,	P17,	1}, //分辨率
    {M02,	P17,	2}, //循环录像
    {M95,	P40,	3}, //自动关机
    {M96,	P38,	4}, //按键音
    {M171,	P17,	5}, //录音使能
//    {M,	P17,	6}, //WDR开关
    {M46,	P15,	6}, //补光灯
};
const S_MENU  prefer_setting[]=
{
    {M39,	P17,	1}, //白平衡
    {M42,	P17,	2}, //曝光度
//    {M40,	P17,	3}, //色彩
};
const S_MENU safety_assist_setting[] =
{
    {M04,	P17,	1}, //移动侦测
    {M172,	P12,	2}, //停车守卫
    {M103,	P37,	3}, //重力感应
};
const S_MENU  system_setting[] =
{
    {M06,	P36,	1}, //日期标签
    {M79,	P18,	OTHER_SCREEN}, //日期/时间
    {M97,	P39,	2}, //语言设置
	{M99,	P15,	3}, //光源频率
	{M47,	P15,	4}, //电视制式
    {M102,	P43,	5},//屏幕保护
    {M106,	P42,	6},//默认设置
	{M105,	P46,	7},//格式化
	{M107,  P42,    OTHER_SCREEN},//版本
};

S_MENULIST  menu_video_normallist[] =
{
/* 0*/{NO_MENU,     NULL,  STYLE_NORMAL,   normal_setting, 	cb_normal_setting,	 sizeof(normal_setting) / sizeof(normal_setting[0])   },
/* 1*/{0,     M35,  STYLE_NORMAL,   reso_ratio, 	    cb_reso_ratio,	 sizeof(reso_ratio) / sizeof(reso_ratio[0]) },
/* 2*/{0,     M02,  STYLE_NORMAL,   loop_video,		cb_loop_video,	 sizeof(loop_video) / sizeof(loop_video[0]) },
/* 3*/{0,     M95,  STYLE_NORMAL,   auto_power,  	    cb_auto_power,	 sizeof(auto_power) / sizeof(auto_power[0]) },
/* 4*/{0,     M96,  STYLE_NORMAL,   key_voice,		cb_key_voice,	 sizeof(key_voice) / sizeof(key_voice[0]) },
/* 5*/{0,     M171,  STYLE_NORMAL,   mic_mute, 	    cb_mic_mute,	 sizeof(mic_mute) / sizeof(mic_mute[0]) },
/* 6*/ //WDR
#if 0//def FLASH_LIGHT //JOHNSON 20160618
/* 6*/{0,     M46,  STYLE_NORMAL,   flashlight_mode,		cb_flashlight_mode,	 sizeof(flashlight_mode) / sizeof(flashlight_mode[0]) },
#endif
};

S_MENULIST menu_video_preferlist[] =
{
/* 0*/{NO_MENU,     NULL,  STYLE_NORMAL,   prefer_setting, 	cb_perfer_setting,	 sizeof(prefer_setting) / sizeof(prefer_setting[0])   },
/* 1*/{0,     M39,  STYLE_NORMAL,   while_balance, 	    cb_while_balance,	 sizeof(while_balance) / sizeof(while_balance[0]) },//白平衡
/* 2*/{0,     M42,  STYLE_NORMAL,   sennor_exposure,		cb_sennor_exposure,	 sizeof(sennor_exposure) / sizeof(sennor_exposure[0]) },
//	/* 3*/{0,     M40,  STYLE_NORMAL,   color_effect,		cb_color_effect,	 sizeof(color_effect) / sizeof(color_effect[0]) },
};

S_MENULIST menu_video_safetyassistlist[] =
{
/* 0*/{NO_MENU,     M177,  STYLE_NORMAL,   safety_assist_setting, 	cb_safety_assist_setting,	 sizeof(safety_assist_setting) / sizeof(safety_assist_setting[0])   },
/* 1*/{0,     M04,  STYLE_NORMAL,   moving_detect,		cb_moving_detect,	 sizeof(moving_detect) / sizeof(moving_detect[0]) },
/* 2*/{0,     M172,  STYLE_NORMAL,   parking_guard,		cb_parking_guard,	 sizeof(parking_guard) / sizeof(parking_guard[0]) },
/* 3*/{0,     M103,  STYLE_NORMAL,   gravity_reaction,cb_gravity_reaction,	 sizeof(gravity_reaction) / sizeof(gravity_reaction[0]) },
};
S_MENULIST menu_video_systemlist[] =
{
/* 0*/{NO_MENU,     M178,  STYLE_NORMAL,   system_setting, 	cb_system_setting,	 sizeof(system_setting) / sizeof(system_setting[0])   },
/* 1*/{0,     M06,  STYLE_NORMAL,   date_label,       cb_date_label,	 sizeof(date_label) / sizeof(date_label[0]) },
/* 2*/{0,     M97,  STYLE_NORMAL,   language_set,		cb_language_set,	 sizeof(language_set) / sizeof(language_set[0]) },
/* 3*/{0,     M99,  STYLE_NORMAL,   light_freq,		cb_light_freq,	 sizeof(light_freq) / sizeof(light_freq[0]) },
/* 4*/{0,     M47,  STYLE_NORMAL,   TV_mode,		cb_TV_mode,	 sizeof(TV_mode) / sizeof(TV_mode[0]) }, //電視制式
/* 5*/{0,     M102,  STYLE_NORMAL,   screen_protect,       cb_screen_protect,	 sizeof(screen_protect) / sizeof(screen_protect[0]) },
/*6*/{0,     M142, STYLE_CONFIRM,        confirm, 	        cb_restore_factory_settings,	 sizeof(confirm) / sizeof(confirm[0]) },
/*7*/{0,     M143, STYLE_CONFIRM,        confirm,		    cb_formatting,	 sizeof(confirm) / sizeof(confirm[0]) },

};
//------------------------------------------------------------拍照--------------------------------------------------------------------//
//拍照
const S_MENU photo_setting[]=
{
    {M39,	P17,	1}, //白平衡
    {M42,	P17,	2}, //曝光度
    {M40,	P17,	3}, //色彩
	{M06,   P27,    4},//日期标签
	{M97,	P39,	5}, //语言设置
	{M99,	P15,	6}, //光源频率
	{M79,	P18,	OTHER_SCREEN}, //日期/时间
	{M105,	P46,	7},//格式化
	{M106,	P42,	8},//默认设置
};



//日期标签
const S_MENU photo_date_label[]=
{
	{M32,   0,   NO_MENU},//关
	{M94,   0,   NO_MENU},//时期/时间
};


///<系统设置
S_MENULIST  menu_photo_list[] =
{
	/* 0*/{NO_MENU,    NULL,   STYLE_NORMAL,  photo_setting, 	cb_photo_setting,	 sizeof(photo_setting) / sizeof(photo_setting[0])   },
    /* 1*/{0,     M39,  STYLE_NORMAL,   while_balance, 	    cb_while_balance,	 sizeof(while_balance) / sizeof(while_balance[0]) },//白平衡
	/* 2*/{0,     M42,  STYLE_NORMAL,   sennor_exposure,		cb_sennor_exposure,	 sizeof(sennor_exposure) / sizeof(sennor_exposure[0]) },
	/* 3*/{0,     M40,  STYLE_NORMAL,   color_effect,		cb_color_effect,	 sizeof(color_effect) / sizeof(color_effect[0]) },
	/* 4*/  //{0,   M35,   STYLE_NORMAL,  photo_reso_ratio,		cb_photo_reso_ratio,	 sizeof(photo_reso_ratio) / sizeof(photo_reso_ratio[0]) },
	/* 5*/{0,   M45,   STYLE_NORMAL,  photo_date_label,		cb_photo_date_label,	 sizeof(photo_date_label) / sizeof(photo_date_label[0]) },
    /* 6*/{0,     M97,  STYLE_NORMAL,   language_set,		cb_language_set,	 sizeof(language_set) / sizeof(language_set[0]) },
    /* 7*/{0,     M99,  STYLE_NORMAL,   light_freq,		cb_light_freq,	 sizeof(light_freq) / sizeof(light_freq[0]) },
    /*8*/{0,     M143, STYLE_CONFIRM,        confirm,		    cb_formatting,	 sizeof(confirm) / sizeof(confirm[0]) },
	/*9*/{0,     M142, STYLE_CONFIRM,        confirm, 	        cb_restore_factory_settings,	 sizeof(confirm) / sizeof(confirm[0]) },
};
S_MENULIST  menu_photoMP_list[] =
{
//    {NO_MENU,     M35,  STYLE_NORMAL,   reso_ratio, 	    cb_reso_ratio,	 sizeof(reso_ratio) / sizeof(reso_ratio[0]) },
    {NO_MENU,   M35,   STYLE_NORMAL,  photo_reso_ratio,		cb_photo_reso_ratio,	 sizeof(photo_reso_ratio) / sizeof(photo_reso_ratio[0]) },
};



//------------------------------------------------------------回放--------------------------------------------------------------------//

const S_MENU lookback[]=
{
    {M84,   P109/*P44*/,  1},//删除当前  //更改图标 JOHNSON 20160531
    {M85,   P110/*P44*/,  2},//全部删除
	{M86,   P108/*P45*/,  3},
	{M87,   P104/*P45*/,  4},
	{M89,   P96/*P45*/,  5},
};
const S_MENU lookback_lock[]=
{
	{M84,   P44,  1},//删除当前// //jm {M84,   P44,  1},//删除当前
	{M85,   P45,  2},//全部删除//{M85,   P45,  2},//全部删除
};
const S_MENU lookback_unlock[]=
{
	{M84,   P44,  1},//删除当前// //jm {M84,   P44,  1},//删除当前
	{M85,   P45,  2},//全部删除//{M85,   P45,  2},//全部删除
};
/*
const S_MENU deletefile[]=
{
	{M141,   0,   NO_MENU},//确认
	{M140,   0,   NO_MENU},//取消
};
*/

///<回放
S_MENULIST  menu_play_list[] =
{
//	/*0*/{NO_MENU,    M81,   STYLE_NORMAL,  lookback, 	NULL,	 sizeof(lookback) / sizeof(lookback[0])   },
	/*0*/{NO_MENU,    NULL,   STYLE_NORMAL,  lookback, 	cb_play_setting,	 sizeof(lookback) / sizeof(lookback[0])   },
	/*1*/{0,	      M144,   STYLE_CONFIRM,  confirm, 		cb_delfile,	 sizeof(confirm) / sizeof(confirm[0])   },
	/*2*/{0,          M145,   STYLE_CONFIRM,  confirm, 		cb_delallfile,	 sizeof(confirm) / sizeof(confirm[0])   },
    /*3*/{0,          M86,   STYLE_CONFIRM,  confirm, 		cb_lockfile,	 sizeof(confirm) / sizeof(confirm[0])   },
    /*4*/{0,          M87,   STYLE_CONFIRM,  confirm, 		cb_unlockfile,	 sizeof(confirm) / sizeof(confirm[0])   },
    /*4*/{0,          M89,   STYLE_CONFIRM,  confirm, 		cb_unlock_all_file,	 sizeof(confirm) / sizeof(confirm[0])   },
};
S_MENULIST  menu_play_list_lock[] =
{
//	/*0*/{NO_MENU,    M81,   STYLE_NORMAL,  lookback, 	NULL,	 sizeof(lookback) / sizeof(lookback[0])   },
	/*0*/{NO_MENU,    NULL,   STYLE_NORMAL,  lookback_lock, 	cb_play_setting,	 sizeof(lookback) / sizeof(lookback[0])   },
	/*1*/{0,	      M144,   STYLE_CONFIRM,  confirm, 		cb_delfile,	 sizeof(confirm) / sizeof(confirm[0])   },
	/*2*/{0,          M145,   STYLE_CONFIRM,  confirm, 		cb_delallfile,	 sizeof(confirm) / sizeof(confirm[0])   },
	/*2*/{0,          M86,   STYLE_CONFIRM,  confirm, 		cb_lockfile,	 sizeof(confirm) / sizeof(confirm[0])   },
};
S_MENULIST  menu_play_list_unlock[] =
{
//	/*0*/{NO_MENU,    M81,   STYLE_NORMAL,  lookback, 	NULL,	 sizeof(lookback) / sizeof(lookback[0])   },
	/*0*/{NO_MENU,    NULL,   STYLE_NORMAL,  lookback_unlock, 	cb_play_setting,	 sizeof(lookback) / sizeof(lookback[0])   },
	/*1*/{0,	      M144,   STYLE_CONFIRM,  confirm, 		cb_delfile,	 sizeof(confirm) / sizeof(confirm[0])   },
	/*2*/{0,          M145,   STYLE_CONFIRM,  confirm, 		cb_delallfile,	 sizeof(confirm) / sizeof(confirm[0])   },
	/*2*/{0,          M87,   STYLE_CONFIRM,  confirm, 		cb_unlockfile,	 sizeof(confirm) / sizeof(confirm[0])   },
};

 MENU_ARRAY menu_array0[] =
{
//	{PNG21,menu_videoMP_list},
//	{PNG23,menu_videolp_list},
	{NULL,menu_video_list},
};

MENU_ARRAY menu_array1[] =
{
//    {PNG21,menu_photoMP_list},
	{NULL,menu_video_list},
};

MENU_ARRAY menu_array2[] =
{
	{NULL,menu_play_list},
};
MENU_ARRAY menu_array2_lock[] =
{
	{NULL,menu_play_list_lock},
};
MENU_ARRAY menu_array2_unlock[] =
{
	{NULL,menu_play_list_unlock},
};


/*--------------------------------------------------------------------------------------------
								          回调函数
--------------------------------------------------------------------------------------------*/
void disp_msgbox(u16 id,u16 ms)
{
#if DOUBLE_UI_A
	Rect rect;
	ui_get_info(TYPE_MENU,Menu_2_1,&rect);
	ui_clear_screen(rect.layer,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);
	ui_clear_screen(LAYER_OSD0_WIN0,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);
	ui_pic(Img_3_3,-1,-1,USE_ORIG_COLOR);
	dc_drawstring(rect.layer,id,rect.x,rect.y+(rect.height-get_string_height(id))/2,rect.width,ALIGN_CENTER,0);

	if(ms)
	{
        Sleep(ms);
        ui_get_info(TYPE_PIC,Img_3_3,&rect);
        ui_clear_screen(rect.layer,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);
        ui_clear_screen(LAYER_OSD1_WIN0,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);
	}
#endif //DOUBLE_UI_A

}

enum
{
	TIME_YEAR = 0,
	TIME_MONTH,
	TIME_DAY,
	TIME_HOUR,
	TIME_MIN,
	TIME_SEC,
};


u16 day_to_month(u16 year, u16 month)
{
    u16 day;
    if(month==1 || month==3 || month==5 || month==7 || month==8 || month==10 || month==12)
    {
        day = 31;
    }
    else if(month==4 || month==6 || month==9 || month==11)
    {
        day = 30;
    }
    else if((year%4==0 && year%100!=0) || year%400==0)
    {
        day = 29;
    }
    else
    {
        day=28;
    }

    return day;
}
u8 cb_normal_setting(u16 id,u8 mode)
{
    if(mode == MENU_INIT)
	{

	}
	else if(mode == MENU_SELECT)
	{
	}
	return TRUE;
}
u8 cb_perfer_setting(u16 id,u8 mode)
{
    if(mode == MENU_INIT)
	{

	}
	else if(mode == MENU_SELECT)
	{
	}
	return TRUE;
}
u8 cb_safety_assist_setting(u16 id,u8 mode)
{
    if(mode == MENU_INIT)
	{

	}
	else if(mode == MENU_SELECT)
	{
	}
	return TRUE;
}


u8 cb_system_setting(u16 id,u8 mode)
{
	Rect rect;
//	u8 update = TRUE;
	u8 update = FALSE;
	if(mode == MENU_SELECT)
	{
		printf("\ncb_setting");
		if(id == M107)
		{
#if DOUBLE_UI_A
			ui_get_info(TYPE_MENU,Menu_2_1,&rect);
#endif //DOUBLE_UI_A


			ui_clear_screen(rect.layer,rect.x-10,rect.y-10,rect.width+20,rect.height+20,COLOR_LUCENCY);
			ui_clear_screen(LAYER_OSD0_WIN0,rect.x-10,rect.y-10,rect.width+20,rect.height+20,COLOR_LUCENCY);

			ui_pic(Img_3_3,-1,-1,USE_ORIG_COLOR);

			dc_set_color(COLOR_WHITE,COLOR_LUCENCY);

            DTEXT text_temp;
            text_temp.flags = ANSI;
            text_temp.buf = SOFTWARE_VERSION;
            text_temp.len = 18;//14;
            ui_text(Txt_3_3,&text_temp);
            text_temp.len = 18;//14;
            //text_temp.buf = SENNOR_VERSION;
            //ui_text(Txt_3_2,&text_temp);

//			ui_pic(Img_3_1,-1,-1,USE_NEW_COLOR);
			Sleep(800);
			ui_clear_screen(rect.layer,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);
			ui_clear_screen(LAYER_OSD0_WIN0,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);

			ui_get_info(TYPE_PIC,Img_3_3,&rect);
			ui_clear_screen(rect.layer,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);
		}
		else if(id == M79)
		{
			TIME time;
//			time.year = 2015;
//			time.month = 6;
//			time.day = 24;
//			time.hour = 1;
//			time.minute = 10;
//			time.second = 30;
            read_sys_time((RTC_TIME*)&time);

			u8 pos = TIME_YEAR;
			u8 pos_old = TIME_YEAR;
#if DOUBLE_UI_A
			ui_get_info(TYPE_MENU,Menu_2_1,&rect);
#endif //DOUBLE_UI_A


			ui_clear_screen(rect.layer,rect.x-10,rect.y-10,rect.width+20,rect.height+20,COLOR_LUCENCY);
			ui_clear_screen(LAYER_OSD0_WIN0,rect.x-10,rect.y-10,rect.width+20,rect.height+20,COLOR_LUCENCY);

            ui_get_info(TYPE_PIC,Img_3_2,&rect);
            ui_clear_screen(rect.layer,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);

            ui_pic(Img_3_3,-1,-1,USE_ORIG_COLOR);

//            dc_set_color(COLOR_WHITE,COLOR_LUCENCY);
            dc_set_color(COLOR_RED,COLOR_LUCENCY);
            ui_number(Number_3_1,time.year,-1,-1,USE_NEW_COLOR);
            dc_restore_color();

            ui_pic(Img_3_6,-1,-1,USE_NEW_COLOR);
            ui_number(Number_3_2,time.month,-1,-1,USE_NEW_COLOR);

            ui_pic(Img_3_7,-1,-1,USE_NEW_COLOR);
            ui_number(Number_3_3,time.day,-1,-1,USE_NEW_COLOR);

            ui_number(Number_3_4,time.hour,-1,-1,USE_NEW_COLOR);
            ui_pic(Img_3_4,-1,-1,USE_NEW_COLOR);
            ui_number(Number_3_5,time.minute,-1,-1,USE_NEW_COLOR);
            ui_pic(Img_3_5,-1,-1,USE_NEW_COLOR);
            ui_number(Number_3_6,time.second,-1,-1,USE_NEW_COLOR);
            u32 msg ;//= app_get_msg();
            msg_t *msg_d;
            msg_devicechange_t *devsta;
			while(1)
			{
				if(update)
				{
					update = FALSE;
					dc_set_color(COLOR_RED,COLOR_LUCENCY);
                    switch(pos)
                    {
                        case TIME_YEAR:
                            ui_clear_controls(TYPE_NUMBER,Number_3_1);
                            ui_number(Number_3_1,time.year,-1,-1,USE_NEW_COLOR);
                            if(pos_old != pos)
                            {
                               pos_old =  pos;
                               ui_clear_controls(TYPE_NUMBER,Number_3_6);
                               dc_set_color(COLOR_WHITE,COLOR_LUCENCY);
                               ui_number(Number_3_6,time.second,-1,-1,USE_NEW_COLOR);
                            }
                            break;
                        case TIME_MONTH:
                            ui_clear_controls(TYPE_NUMBER,Number_3_2);
                            ui_number(Number_3_2,time.month,-1,-1,USE_NEW_COLOR);
                            if(pos_old != pos)
                            {
                               pos_old =  pos;
                               ui_clear_controls(TYPE_NUMBER,Number_3_1);
                               dc_set_color(COLOR_WHITE,COLOR_LUCENCY);
                               ui_number(Number_3_1,time.year,-1,-1,USE_NEW_COLOR);
                            }
                            break;
                        case TIME_DAY:
                            ui_clear_controls(TYPE_NUMBER,Number_3_3);
                            ui_number(Number_3_3,time.day,-1,-1,USE_NEW_COLOR);
                            if(pos_old != pos)
                            {
                               pos_old =  pos;
                               ui_clear_controls(TYPE_NUMBER,Number_3_2);
                               dc_set_color(COLOR_WHITE,COLOR_LUCENCY);
                               ui_number(Number_3_2,time.month,-1,-1,USE_NEW_COLOR);
                            }
                            break;
                        case TIME_HOUR:
                            ui_clear_controls(TYPE_NUMBER,Number_3_4);
                            ui_number(Number_3_4,time.hour,-1,-1,USE_NEW_COLOR);
                            if(pos_old != pos)
                            {
                               pos_old =  pos;
                               ui_clear_controls(TYPE_NUMBER,Number_3_3);
                               dc_set_color(COLOR_WHITE,COLOR_LUCENCY);
                               ui_number(Number_3_3,time.day,-1,-1,USE_NEW_COLOR);
                            }
                            break;
                        case TIME_MIN:
                            ui_clear_controls(TYPE_NUMBER,Number_3_5);
                            ui_number(Number_3_5,time.minute,-1,-1,USE_NEW_COLOR);
                            if(pos_old != pos)
                            {
                               pos_old =  pos;
                               ui_clear_controls(TYPE_NUMBER,Number_3_4);
                               dc_set_color(COLOR_WHITE,COLOR_LUCENCY);
                               ui_number(Number_3_4,time.hour,-1,-1,USE_NEW_COLOR);
                            }
                            break;
                        case TIME_SEC:
                            ui_clear_controls(TYPE_NUMBER,Number_3_6);
                            ui_number(Number_3_6,time.second,-1,-1,USE_NEW_COLOR);
                            if(pos_old != pos)
                            {
                               pos_old =  pos;
                               ui_clear_controls(TYPE_NUMBER,Number_3_5);
                               dc_set_color(COLOR_WHITE,COLOR_LUCENCY);
                               ui_number(Number_3_5,time.minute,-1,-1,USE_NEW_COLOR);
                            }
                            break;
                    }
                    dc_set_color(COLOR_WHITE,COLOR_LUCENCY);
				}
                msg= app_get_msg();
//                printf("\nmsg:%x",msg);
//                puts("\n ui sys time seting 1 \n");
                if(msg  > (u32)0x2000000)
                {
                    msg_d = msg ;
                    msg = msg_d->type ;
                }
				switch(msg)
				{
				case MSG_KEY_MENU:
//				case MSG_KEY_OK:

             #if 1 //JOHNSON 20160321 add
                case MSG_KEY_OK:
                    ui_get_info(TYPE_PIC,Img_3_3,&rect);
                    ui_clear_screen(rect.layer,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);
                    ui_clear_screen(LAYER_OSD1_WIN0,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);
                    write_sys_time((RTC_TIME*)&time);
                    disp_sys_time(0);
                    return TRUE;
             #else //old
                case MSG_KEY_MODE:  //20151120 san mode键取消设置时间

					ui_get_info(TYPE_PIC,Img_3_3,&rect);
					ui_clear_screen(rect.layer,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);
					ui_clear_screen(LAYER_OSD1_WIN0,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);
					//write_sys_time((RTC_TIME*)&time);
					//disp_sys_time(0);
					return TRUE;
			 #endif

				case MSG_KEY_NEXT:
					switch(pos)
					{
					case TIME_YEAR:
						time.year--;
						if(time.year < 1900)
							time.year = 2099;
						break;
					case TIME_MONTH:
						time.month--;
						if(time.month < 1)
							time.month = 12;
						break;
					case TIME_DAY:
						time.day--;
						if(time.day < 1)
							time.day = day_to_month(time.year, time.month);

						break;
					case TIME_HOUR:
						time.hour--;
						if(time.hour == 0xff)
							time.hour = 23;
						break;
					case TIME_MIN:
						time.minute--;
						if(time.minute == 0xff)
							time.minute = 59;
						break;
					case TIME_SEC:
						time.second--;
						if(time.second == 0xff)
							time.second = 59;
						break;
					}
					update = TRUE;
					break;
				case MSG_KEY_PREV:
					switch(pos)
					{
					case TIME_YEAR:
						time.year++;
						if(time.year > 2099)
							time.year = 1900;
						break;
					case TIME_MONTH:
						time.month++;
						if(time.month > 12)
							time.month = 1;
						break;
					case TIME_DAY:
						time.day++;
						if(time.day > day_to_month(time.year, time.month))
							time.day = 1;
						break;
					case TIME_HOUR:
						time.hour++;
						if(time.hour > 23)
							time.hour = 0;
						break;
					case TIME_MIN:
						time.minute++;
						if(time.minute > 59)
							time.minute = 0;
						break;
					case TIME_SEC:
						time.second++;
						if(time.second > 59)
							time.second = 0;
						break;
					}
					update = TRUE;
					break;

			 #if 1 //JOHNSON 20160321 add
			    case MSG_KEY_MODE:
                    pos ++;
                    if(pos > TIME_SEC)
                        pos = TIME_YEAR;
                    update = TRUE;
                    break;
             #else //old
                case MSG_KEY_OK:  //20151120 san mode键取消设置时间

                    pos ++;
                    if(pos > TIME_SEC)
                    {
 //                        pos = TIME_YEAR;
                        ui_get_info(TYPE_PIC,Img_3_3,&rect);
                        ui_clear_screen(rect.layer,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);
                        ui_clear_screen(LAYER_OSD1_WIN0,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);
                        write_sys_time((RTC_TIME*)&time);
                        disp_sys_time(0);
                        return TRUE;
                    }

                    update = TRUE;
                    break;
             #endif

//				case MSG_KEY_MODE:
//					pos ++;
//					if(pos > TIME_SEC)
//						pos = TIME_YEAR;
//					update = TRUE;
//					break;
                case MSG_MENU_EXIT:
                    put_msg_fifo(MSG_KEY_MENU,MSG_TO_SELF,0) ;
                    put_msg_fifo(MSG_MENU_EXIT,MSG_TO_SELF,0) ;
                    break;
                case MSG_ONE_SECOND:
                    disp_sys_time(0);
                    break;
                case MSG_DEVICECHANGE:
//                puts("\n ui sys time seting 2 \n");
                    devsta = msg_d->ptr;
                     if(devsta->hdev->dev_io->device_type == DEV_LOGIC_DISK)
                     {
                        if(devsta->sta ==DEV_ONLINE )
                        {
                            ui_clear_controls(TYPE_PIC, Img_1_27);
                            ui_pic(Img_1_5,-1,-1,USE_ORIG_COLOR);
                            if(get_current_mode() & ENC_MODE)
                            {
                                get_remain_time_rec();
                            }
                            else if(get_current_mode() & TPH_MODE)
                            {
                                disp_REC_cnt(0,ERASE_ONLY);//擦除数字区
                                num_of_last_photo();
                            }
                        }
                        else if(devsta->sta ==DEV_OFFLINE )
                        {
                            ui_clear_controls(TYPE_PIC, Img_1_5);
                            ui_pic(Img_1_27,-1,-1,USE_ORIG_COLOR);
                            if(get_current_mode() & ENC_MODE)
                            {
                                get_remain_time_rec();
                            }
                            else if(get_current_mode() & TPH_MODE)
                            {
                                disp_REC_cnt(0,ERASE_ONLY);//擦除数字区
                                reset_num_of_photo();
                            }
                        }
                     }
                     else if (DEV_ONLINE == devsta->sta && devsta->hdev->dev_io->device_type == DEV_USB_SLAVE ) //usb上线
                     {
                         puts("\n DEV_USB_SLAVE \n");
                        put_msg_fifo(MSG_KEY_MENU,MSG_TO_SELF,0) ;
                        put_msg_fifo(MSG_MENU_EXIT,MSG_TO_SELF,0) ;
                        put_msg_fifo(MSG_USB_MENU,MSG_TO_SELF,0) ;
                     }
                    free(msg_d);
                    break;
				}
			}

		}
	}
	return TRUE;
}

u8 cb_setting(u16 id,u8 mode)
{
	Rect rect;
	u8 update = FALSE;
	u32 menu_outmark = 0;
	u32 second_cnt = 0;
	    u32 msg;
    msg_t *msg_d;
    msg_devicechange_t *devsta;
	if(mode == MENU_SELECT)
	{
		printf("\ncb_setting");
		if(id == M107)
		{
#if DOUBLE_UI_A
			ui_get_info(TYPE_MENU,Menu_2_1,&rect);
#endif //DOUBLE_UI_A


			ui_clear_screen(rect.layer,rect.x-10,rect.y-10,rect.width+20,rect.height+20,COLOR_LUCENCY);
			ui_clear_screen(LAYER_OSD0_WIN0,rect.x-10,rect.y-10,rect.width+20,rect.height+20,COLOR_LUCENCY);

			ui_pic(Img_3_3,-1,-1,USE_ORIG_COLOR);

			dc_set_color(COLOR_WHITE,COLOR_LUCENCY);

            DTEXT text_temp;
            text_temp.flags = ANSI;
            text_temp.buf = SOFTWARE_VERSION;
            text_temp.len = 18;//14;
            ui_text(Txt_3_3,&text_temp);
            text_temp.len = 18;//14;
            //text_temp.buf = SENNOR_VERSION;
            //ui_text(Txt_3_2,&text_temp);


            while(1)
            {
                putbyte('a');
                msg= app_get_msg();
                if(msg  > (u32)0x2000000)
                {
                    msg_d = msg ;
                    msg = msg_d->type ;
                }
                switch(msg)
                {
                    case MSG_KEY_MENU:
                    case MSG_KEY_OK:
                        menu_outmark = 1;
                        break;
                    case MSG_KEY_MODE:
                        menu_outmark = 1;
                        put_msg_fifo(MSG_MENU_EXIT,MSG_TO_SELF,0);
                        break;
                    case MSG_ONE_SECOND:
                        second_cnt++;
                        break;
                    case MSG_DEVICECHANGE:
                        devsta = msg_d->ptr;
                         if(devsta->hdev->dev_io->device_type == DEV_LOGIC_DISK)
                         {
                            if(devsta->sta ==DEV_ONLINE )
                            {
                                puts("\n 000000000000000\n");
                                ui_clear_controls(TYPE_PIC, Img_1_27);
                                ui_pic(Img_1_5,-1,-1,USE_ORIG_COLOR);// sd pic
                                if(get_current_mode() & ENC_MODE)
                                {
                                    get_remain_time_rec();
                                }
                                else if(get_current_mode() & TPH_MODE)
                                {
                                    disp_REC_cnt(0,ERASE_ONLY);//擦除数字区
                                    num_of_last_photo();
                                }
                            }
                            else if(devsta->sta ==DEV_OFFLINE )
                            {
                                puts("\n 1111111111111111 \n");
                                ui_clear_controls(TYPE_PIC, Img_1_5);
                                ui_pic(Img_1_27,-1,-1,USE_ORIG_COLOR);
                                if(get_current_mode() & ENC_MODE)
                                {
                                    get_remain_time_rec();
                                }
                                else if(get_current_mode() & TPH_MODE)
                                {
                                    disp_REC_cnt(0,ERASE_ONLY);//擦除数字区
                                    reset_num_of_photo();
                                }
                            }
                         }
                         else if (DEV_ONLINE == devsta->sta && devsta->hdev->dev_io->device_type == DEV_USB_SLAVE ) //usb上线
                         {
                             puts("\n ====================  DEV_USB_SLAVE \n");
//                            put_msg_fifo(MSG_KEY_MENU,MSG_TO_SELF,0) ;
                             menu_outmark = 1;
                            put_msg_fifo(MSG_MENU_EXIT,MSG_TO_SELF,0) ;
                            put_msg_fifo(MSG_USB_MENU,MSG_TO_SELF,0) ;
                         }
                        free(msg_d);
                        break;
                }
                putbyte('b');
                if(second_cnt>4)
                    menu_outmark = 1;
                if(menu_outmark)
                {
                    ui_clear_screen(rect.layer,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);
                     ui_clear_screen(LAYER_OSD0_WIN0,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);

                    ui_get_info(TYPE_PIC,Img_3_3,&rect);
                    ui_clear_screen(rect.layer,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);
                    return TRUE;
                }
                putbyte('c');

                os_time_dly(1);

            }


////			ui_pic(Img_3_1,-1,-1,USE_NEW_COLOR);
//			Sleep(800);
//			ui_clear_screen(rect.layer,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);
//			ui_clear_screen(LAYER_OSD0_WIN0,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);
//
//			ui_get_info(TYPE_PIC,Img_3_3,&rect);
//			ui_clear_screen(rect.layer,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);
		}
		else if(id == M79)
		{
			TIME time;
//			time.year = 2015;
//			time.month = 6;
//			time.day = 24;
//			time.hour = 1;
//			time.minute = 10;
//			time.second = 30;
            read_sys_time((RTC_TIME*)&time);

			u8 pos = TIME_YEAR;
			u8 pos_old = TIME_YEAR;
#if DOUBLE_UI_A
//			ui_get_info(TYPE_MENU,Menu_2_1,&rect);
			ui_get_info(TYPE_PIC,Img_3_2,&rect);
#endif //DOUBLE_UI_A



			ui_clear_screen(rect.layer,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);
//			ui_clear_screen(LAYER_OSD0_WIN0,rect.x-10,rect.y-10,rect.width+20,rect.height+20,COLOR_LUCENCY);


//            ui_get_info(TYPE_PIC,Img_3_2,&rect);
//            ui_clear_screen(rect.layer,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);

            ui_pic(Img_3_3,-1,-1,USE_ORIG_COLOR);

            dc_drawstring(rect.layer,id,rect.x,rect.y+ (rect.height/3 - get_string_height(id))/2,rect.width,1,0);


//            dc_set_color(COLOR_WHITE,COLOR_LUCENCY);
            dc_set_color(COLOR_RED,COLOR_LUCENCY);
            ui_number(Number_3_1,time.year,-1,-1,USE_NEW_COLOR);
            dc_restore_color();

            ui_pic(Img_3_7,-1,-1,USE_NEW_COLOR);
            ui_number(Number_3_2,time.month,-1,-1,USE_NEW_COLOR);

            ui_pic(Img_3_6,-1,-1,USE_NEW_COLOR);
            ui_number(Number_3_3,time.day,-1,-1,USE_NEW_COLOR);

            ui_number(Number_3_4,time.hour,-1,-1,USE_NEW_COLOR);
            ui_pic(Img_3_4,-1,-1,USE_NEW_COLOR);
            ui_number(Number_3_5,time.minute,-1,-1,USE_NEW_COLOR);
            ui_pic(Img_3_5,-1,-1,USE_NEW_COLOR);
            ui_number(Number_3_6,time.second,-1,-1,USE_NEW_COLOR);
            u32 msg ;//= app_get_msg();
            msg_t *msg_d;
            msg_devicechange_t *devsta;
			while(1)
			{
				if(update)
				{
					update = FALSE;
					dc_set_color(COLOR_RED,COLOR_LUCENCY);
                    switch(pos)
                    {
                        case TIME_YEAR:
                            ui_clear_controls(TYPE_NUMBER,Number_3_1);
                            ui_number(Number_3_1,time.year,-1,-1,USE_NEW_COLOR);
                            if(pos_old != pos)
                            {
                               pos_old =  pos;
                               ui_clear_controls(TYPE_NUMBER,Number_3_6);
                               dc_set_color(COLOR_WHITE,COLOR_LUCENCY);
                               ui_number(Number_3_6,time.second,-1,-1,USE_NEW_COLOR);
                            }
                            break;
                        case TIME_MONTH:
                            ui_clear_controls(TYPE_NUMBER,Number_3_2);
                            ui_number(Number_3_2,time.month,-1,-1,USE_NEW_COLOR);
                            if(pos_old != pos)
                            {
                               pos_old =  pos;
                               ui_clear_controls(TYPE_NUMBER,Number_3_1);
                               dc_set_color(COLOR_WHITE,COLOR_LUCENCY);
                               ui_number(Number_3_1,time.year,-1,-1,USE_NEW_COLOR);
                            }
                            break;
                        case TIME_DAY:
                            ui_clear_controls(TYPE_NUMBER,Number_3_3);
                            ui_number(Number_3_3,time.day,-1,-1,USE_NEW_COLOR);
                            if(pos_old != pos)
                            {
                               pos_old =  pos;
                               ui_clear_controls(TYPE_NUMBER,Number_3_2);
                               dc_set_color(COLOR_WHITE,COLOR_LUCENCY);
                               ui_number(Number_3_2,time.month,-1,-1,USE_NEW_COLOR);
                            }
                            break;
                        case TIME_HOUR:
                            ui_clear_controls(TYPE_NUMBER,Number_3_4);
                            ui_number(Number_3_4,time.hour,-1,-1,USE_NEW_COLOR);
                            if(pos_old != pos)
                            {
                               pos_old =  pos;
                               ui_clear_controls(TYPE_NUMBER,Number_3_3);
                               dc_set_color(COLOR_WHITE,COLOR_LUCENCY);
                               ui_number(Number_3_3,time.day,-1,-1,USE_NEW_COLOR);
                            }
                            break;
                        case TIME_MIN:
                            ui_clear_controls(TYPE_NUMBER,Number_3_5);
                            ui_number(Number_3_5,time.minute,-1,-1,USE_NEW_COLOR);
                            if(pos_old != pos)
                            {
                               pos_old =  pos;
                               ui_clear_controls(TYPE_NUMBER,Number_3_4);
                               dc_set_color(COLOR_WHITE,COLOR_LUCENCY);
                               ui_number(Number_3_4,time.hour,-1,-1,USE_NEW_COLOR);
                            }
                            break;
                        case TIME_SEC:
                            ui_clear_controls(TYPE_NUMBER,Number_3_6);
                            ui_number(Number_3_6,time.second,-1,-1,USE_NEW_COLOR);
                            if(pos_old != pos)
                            {
                               pos_old =  pos;
                               ui_clear_controls(TYPE_NUMBER,Number_3_5);
                               dc_set_color(COLOR_WHITE,COLOR_LUCENCY);
                               ui_number(Number_3_5,time.minute,-1,-1,USE_NEW_COLOR);
                            }
                            break;
                    }
                    dc_set_color(COLOR_WHITE,COLOR_LUCENCY);
				}
                msg= app_get_msg();
//                printf("\nmsg:%x",msg);
//                puts("\n ui sys time seting 1 \n");
                if(msg  > (u32)0x2000000)
                {
                    msg_d = msg ;
                    msg = msg_d->type ;
                }
				switch(msg)
				{
				case MSG_KEY_MENU:

			#if 1 //JOHNSON 20160321 ADD
			     case MSG_KEY_OK:
                    ui_get_info(TYPE_PIC,Img_3_3,&rect);
                    ui_clear_screen(rect.layer,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);
                    ui_clear_screen(LAYER_OSD1_WIN0,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);
                    write_sys_time((RTC_TIME*)&time);
                    disp_sys_time(0);
                    return TRUE;
			#else //old
				case MSG_KEY_MODE:
					//ui_get_info(TYPE_PIC,Img_3_3,&rect);
					//ui_clear_screen(rect.layer,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);
					//ui_clear_screen(LAYER_OSD1_WIN0,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);
//					write_sys_time((RTC_TIME*)&time);
//					disp_sys_time(0);
					return TRUE;
			#endif

				case MSG_KEY_NEXT:
					switch(pos)
					{
					case TIME_YEAR:
						time.year--;
						if(time.year < 1900)
							time.year = 2099;
						break;
					case TIME_MONTH:
						time.month--;
						if(time.month < 1)
							time.month = 12;
						break;
					case TIME_DAY:
						time.day--;
						if(time.day < 1)
							time.day = day_to_month(time.year, time.month);

						break;
					case TIME_HOUR:
						time.hour--;
						if(time.hour == 0xff)
							time.hour = 23;
						break;
					case TIME_MIN:
						time.minute--;
						if(time.minute == 0xff)
							time.minute = 59;
						break;
					case TIME_SEC:
						time.second--;
						if(time.second == 0xff)
							time.second = 59;
						break;
					}
					update = TRUE;
					break;
				case MSG_KEY_PREV:
					switch(pos)
					{
					case TIME_YEAR:
						time.year++;
						if(time.year > 2099)
							time.year = 1900;
						break;
					case TIME_MONTH:
						time.month++;
						if(time.month > 12)
							time.month = 1;
						break;
					case TIME_DAY:
						time.day++;
						if(time.day > day_to_month(time.year, time.month))
							time.day = 1;
						break;
					case TIME_HOUR:
						time.hour++;
						if(time.hour > 23)
							time.hour = 0;
						break;
					case TIME_MIN:
						time.minute++;
						if(time.minute > 59)
							time.minute = 0;
						break;
					case TIME_SEC:
						time.second++;
						if(time.second > 59)
							time.second = 0;
						break;
					}
					update = TRUE;
					break;

			#if 1 //JOHNSON 20160321 ADD
                case MSG_KEY_MODE:
                    pos ++;
                    if(pos > TIME_SEC)
                        pos = TIME_YEAR;
                    update = TRUE;
                    break;
			#else //old
				case MSG_KEY_OK:
					pos ++;
					if(pos > TIME_SEC)
					{
                        write_sys_time((RTC_TIME*)&time);
                        disp_sys_time(0);

//					    put_msg_fifo(MSG_KEY_MENU,MSG_TO_SELF,0) ;
					    return TRUE;
					}
//					pos = TIME_YEAR;
					update = TRUE;
					break;
			#endif

                case MSG_MENU_EXIT:
                    put_msg_fifo(MSG_KEY_MENU,MSG_TO_SELF,0) ;
                    put_msg_fifo(MSG_MENU_EXIT,MSG_TO_SELF,0) ;
                    break;
                case MSG_ONE_SECOND:
                    disp_sys_time(0);
                    break;
                case MSG_DEVICECHANGE:
//                puts("\n ui sys time seting 2 \n");
                    devsta = msg_d->ptr;
                     if(devsta->hdev->dev_io->device_type == DEV_LOGIC_DISK)
                     {
                        if(devsta->sta ==DEV_ONLINE )
                        {
                            ui_clear_controls(TYPE_PIC, Img_1_27);
                            ui_pic(Img_1_5,-1,-1,USE_ORIG_COLOR);// sd pic
                            if(get_current_mode() & ENC_MODE)
                            {
                                get_remain_time_rec();
                            }
                            else if(get_current_mode() & TPH_MODE)
                            {
                                disp_REC_cnt(0,ERASE_ONLY);//擦除数字区
                                num_of_last_photo();
                            }
                        }
                        else if(devsta->sta ==DEV_OFFLINE )
                        {
                            ui_clear_controls(TYPE_PIC, Img_1_5);
                            ui_pic(Img_1_27,-1,-1,USE_ORIG_COLOR);
                            if(get_current_mode() & ENC_MODE)
                            {
                                get_remain_time_rec();
                            }
                            else if(get_current_mode() & TPH_MODE)
                            {
                                disp_REC_cnt(0,ERASE_ONLY);//擦除数字区
                                reset_num_of_photo();
                            }
                        }
                     }
                     else if (DEV_ONLINE == devsta->sta && devsta->hdev->dev_io->device_type == DEV_USB_SLAVE ) //usb上线
                     {
                         puts("\n DEV_USB_SLAVE \n");
                        put_msg_fifo(MSG_KEY_MENU,MSG_TO_SELF,0) ;
                        put_msg_fifo(MSG_MENU_EXIT,MSG_TO_SELF,0) ;
                        put_msg_fifo(MSG_USB_MENU,MSG_TO_SELF,0) ;
                     }
                    free(msg_d);
                    break;
				}
			}

		}
	}
	return TRUE;
}

u8 cb_reso_ratio(u16 id,u8 mode)
{
    u8 temp_sle;
	if(mode == MENU_INIT)
	{
		set_select_item(sys_info.reso_ratio);
	}
	else if(mode == MENU_SELECT)
	{
		temp_sle = get_select_item();
		if(temp_sle != sys_info.reso_ratio)
		{

		    sys_info.reso_ratio = temp_sle;
		    put_msg_fifo(MSG_SW_SIZE ,MSG_TO_PROCESS ,ENC_TASK_N) ;
		    updata_formate_pic(temp_sle);
//		    put_u16hex(temp_sle);
		}
		save_sysinfo(&sys_info);
	}
	return TRUE;
}

u8 cb_loop_video(u16 id,u8 mode)
{
	if(mode == MENU_INIT)
	{
//	    put_u16hex(sys_info.loop_video );
		set_select_item(sys_info.loop_video);
	}
	else if(mode == MENU_SELECT)
	{
	   u8 temp_sle = get_select_item();
	    if(sys_info.loop_video != temp_sle)
	    {
            sys_info.loop_video = temp_sle;
            save_sysinfo(&sys_info);
	    }

	}
	return TRUE;
}


u8 cb_while_balance(u16 id, u8 mode)
{
	if(mode == MENU_INIT)
	{
		set_select_item(sys_info.while_balance);
	}
	else if(mode == MENU_SELECT)
	{
	    u8 temp = get_select_item();
	    if(temp != sys_info.while_balance)
	    {
            sys_info.while_balance =  temp;
            save_sysinfo(&sys_info);
            ispt_preset_wb(wb_table[sys_info.while_balance]);
	    }

	}
	return TRUE;
}

u8 cb_sennor_exposure(u16 id, u8 mode)
{
    if(mode == MENU_INIT)
	{
		set_select_item(sys_info.exposure);
	}
	else if(mode == MENU_SELECT)
	{
	    s8 temp = get_select_item();
	    if(temp != sys_info.exposure)
	    {
            sys_info.exposure = temp;
            save_sysinfo(&sys_info);
            ispt_customize_ev(3-temp);
	    }

	}
	return TRUE;
}



u8 cb_color_effect(u16 id, u8 mode)
{
	if(mode == MENU_INIT)
	{
		set_select_item(sys_info.color_effect);
	}
	else if(mode == MENU_SELECT)
	{
	    u8 temp = get_select_item();
	    if(sys_info.color_effect != temp)
	    {
            sys_info.color_effect = temp;
            save_sysinfo(&sys_info);
	    }

	}
	return TRUE;
}

u8 cb_moving_detect(u16 id, u8 mode)
{
	if(mode == MENU_INIT)
	{
		set_select_item(sys_info.moving_detect);
	}
	else if(mode == MENU_SELECT)
	{
	    u8 temp = get_select_item();
	    if(temp != sys_info.moving_detect)
	    {
            sys_info.moving_detect = temp;
            updata_movingdet_pic();
            motion_det_if(sys_info.moving_detect);
            save_sysinfo(&sys_info);
	    }
	}
	return TRUE;
}

u8 cb_mic_mute(u16 id, u8 mode)
{
	if(mode == MENU_INIT)
	{
		set_select_item(sys_info.mic_mute);
	}
	else if(mode == MENU_SELECT)
	{
        u8 temp = get_select_item();
	    if(temp != sys_info.mic_mute)
	    {
//            sys_info.mic_mute = temp;
            mic_mute_fun(sys_info.mic_mute);
//            save_sysinfo(&sys_info);
	    }

	}
	return TRUE;
}

u8 cb_parking_guard(u16 id,u8 mode)
{
 	if(mode == MENU_INIT)
	{
		set_select_item(sys_info.parking_guard);
	}
	else if(mode == MENU_SELECT)
	{
        u8 temp = get_select_item();
	    if(temp != sys_info.parking_guard)
	    {
            sys_info.parking_guard = temp;
            save_sysinfo(&sys_info);
            updata_parking_guard_pic();
	    }

	}
	return TRUE;
}

u8 cb_date_label(u16 id,u8 mode)
{
	if(mode == MENU_INIT)
	{
		set_select_item(sys_info.date_label);
	}
	else if(mode == MENU_SELECT)
	{
	    u8 temp = get_select_item();
	    if(temp != sys_info.date_label)
	    {
            sys_info.date_label = temp;
            save_sysinfo(&sys_info);
            set_video_osd_en(sys_info.date_label);
	    }

	}
	return TRUE;
}
u8 cb_gravity_reaction(u16 id,u8 mode)
{
	if(mode == MENU_INIT)
	{
		set_select_item(sys_info.gravity_sensor);
	}
	else if(mode == MENU_SELECT)
	{
	    u8 temp = get_select_item();
	    if(temp != sys_info.gravity_sensor)
	    {
            sys_info.gravity_sensor = temp;
            save_sysinfo(&sys_info);
            if (sys_status.gsensor_online)
                gravity_dev_con.gravity_sensor_sensity(sys_info.gravity_sensor,0xFF);
	    }

	}
	return TRUE;
}

u8 cb_key_voice(u16 id,u8 mode)
{
	if(mode == MENU_INIT)
	{
		set_select_item(sys_info.key_voice);
	}
	else if(mode == MENU_SELECT)
	{
	    u8 temp = get_select_item();
	    if(temp != sys_info.key_voice)
	    {
            sys_info.key_voice = temp;
            dac_api_enable_ktone(sys_info.key_voice);
            save_sysinfo(&sys_info);
	    }

	}
	return TRUE;
}

u8 cb_wdr_function(u16 id,u8 mode)
{
    u8 lv_value;
	if(mode == MENU_INIT)
	{
		set_select_item(sys_info.wdr);
	}
	else if(mode == MENU_SELECT)
	{
	    u8 temp = get_select_item();
	    if(temp != sys_info.wdr)
	    {
            sys_info.wdr = temp;
            save_sysinfo(&sys_info);

            if (sys_info.wdr == 0)
            {
                ispt_customize_drcEnable(0);
            }
            else
            {
                lv_value = ispt_get_lv();
                if(lv_value <=6)// && lv_value_pre>=10) //白天->夜晚 WDR THRD 8 / 6
                {
                    ispt_customize_drcEnable(sys_info.wdr);
                    //lv_value_pre = lv_value;
                }
            }
	    }

	}
	return TRUE;
}
u8 cb_auto_power(u16 id,u8 mode)
{
	if(mode == MENU_INIT)
	{
		set_select_item(sys_info.auto_poweroff);
	}
	else if(mode == MENU_SELECT)
	{
        u8 temp = get_select_item();
	    if(temp != sys_info.auto_poweroff)
	    {
            sys_info.auto_poweroff = temp;
            save_sysinfo(&sys_info);
	    }
	}
	return TRUE;
}

 tbool dc_SetLanguage(u8 language)
{
    tbool ret = FALSE;
    g_language_mode = language;
//    g_language_flag = FALSE;

    ret = InitFont_ASCII();
    //printf("\nlanguage %x init:%x",g_language_mode,g_language_flag);
    return ret;
}
u8 cb_language_set(u16 id,u8 mode)
{
	if(mode == MENU_INIT)
	{
		set_select_item(sys_info.language);
	}
	else if(mode == MENU_SELECT)
	{
        u8 temp = get_select_item();
	    if(temp != sys_info.language)
	    {
            sys_info.language = temp;
            g_language_mode = language_talbe[sys_info.language];
            dc_SetLanguage(g_language_mode);
            save_sysinfo(&sys_info);
	    }

	}
	return TRUE;
}
u8 cb_light_freq(u16 id,u8 mode)
{
	if(mode == MENU_INIT)
	{
		set_select_item(sys_info.light_freq);
	}
	else if(mode == MENU_SELECT)
	{
        u8 temp = get_select_item();
	    if(temp != sys_info.light_freq)
	    {
            sys_info.light_freq = temp;
            set_AE_bf_type(ae_bftype[sys_info.light_freq]);
            save_sysinfo(&sys_info);
	    }
	}
	return TRUE;
}

u8 cb_TV_mode(u16 id,u8 mode)
{
	if(mode == MENU_INIT)
	{
		set_select_item(sys_info.TV_mode);
	}
	else if(mode == MENU_SELECT)
	{
        u8 temp = get_select_item();
	    if(temp != sys_info.TV_mode)
	    {
            sys_info.TV_mode = temp;
            save_sysinfo(&sys_info);
	    }
	}
	return TRUE;
}
#ifdef FLASH_LIGHT
u8 cb_flashlight_mode(u16 id,u8 mode)
{
	if(mode == MENU_INIT)
	{
		set_select_item(sys_info.flashlight_mode);
	}
	else if(mode == MENU_SELECT)
	{

        u8 temp = get_select_item();
	    if(temp != sys_info.flashlight_mode)
	    {
            sys_info.flashlight_mode = temp;
            save_sysinfo(&sys_info);
            updata_flashlight_mode_pic();
	    }
	}
	return TRUE;
}
#endif


u8 cb_screen_protect(u16 id,u8 mode)
{
	if(mode == MENU_INIT)
	{
		set_select_item(sys_info.screen_protect);
	}
	else if(mode == MENU_SELECT)
	{
        u8 temp = get_select_item();
	    if(temp != sys_info.screen_protect)
	    {
            sys_info.screen_protect = temp;
            save_sysinfo(&sys_info);
	    }
	}
	return TRUE;
}
u8 cb_photo_setting(u16 id,u8 mode)
{
    Rect rect;
//	u8 update = TRUE;
	u8 update = FALSE;
	if(mode == MENU_INIT)
	{

	}
	else if(mode == MENU_SELECT)
	{
	    if(id == M79)
		{
			TIME time;
            read_sys_time((RTC_TIME*)&time);

			u8 pos = TIME_YEAR;
			u8 pos_old = TIME_YEAR;
#if DOUBLE_UI_A
			ui_get_info(TYPE_MENU,Menu_2_1,&rect);
#endif //DOUBLE_UI_A


			ui_clear_screen(rect.layer,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);
			ui_clear_screen(LAYER_OSD0_WIN0,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);

            ui_get_info(TYPE_PIC,Img_3_2,&rect);
            ui_clear_screen(rect.layer,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);

            ui_pic(Img_3_3,-1,-1,USE_ORIG_COLOR);

//            dc_set_color(COLOR_WHITE,COLOR_LUCENCY);
            dc_set_color(COLOR_RED,COLOR_LUCENCY);
            ui_number(Number_3_1,time.year,-1,-1,USE_NEW_COLOR);
            dc_restore_color();

            ui_pic(Img_3_6,-1,-1,USE_NEW_COLOR);
            ui_number(Number_3_2,time.month,-1,-1,USE_NEW_COLOR);

            ui_pic(Img_3_7,-1,-1,USE_NEW_COLOR);
            ui_number(Number_3_3,time.day,-1,-1,USE_NEW_COLOR);

            ui_number(Number_3_4,time.hour,-1,-1,USE_NEW_COLOR);
            ui_pic(Img_3_4,-1,-1,USE_NEW_COLOR);
            ui_number(Number_3_5,time.minute,-1,-1,USE_NEW_COLOR);
            ui_pic(Img_3_5,-1,-1,USE_NEW_COLOR);
            ui_number(Number_3_6,time.second,-1,-1,USE_NEW_COLOR);
            u32 msg ;//= app_get_msg();
            msg_t *msg_d;
            msg_devicechange_t *devsta;
			while(1)
			{
				if(update)
				{
					update = FALSE;
					dc_set_color(COLOR_RED,COLOR_LUCENCY);
                    switch(pos)
                    {
                        case TIME_YEAR:
                            ui_clear_controls(TYPE_NUMBER,Number_3_1);
                            ui_number(Number_3_1,time.year,-1,-1,USE_NEW_COLOR);
                            if(pos_old != pos)
                            {
                               pos_old =  pos;
                               ui_clear_controls(TYPE_NUMBER,Number_3_6);
                               dc_set_color(COLOR_WHITE,COLOR_LUCENCY);
                               ui_number(Number_3_6,time.second,-1,-1,USE_NEW_COLOR);
                            }
                            break;
                        case TIME_MONTH:
                            ui_clear_controls(TYPE_NUMBER,Number_3_2);
                            ui_number(Number_3_2,time.month,-1,-1,USE_NEW_COLOR);
                            if(pos_old != pos)
                            {
                               pos_old =  pos;
                               ui_clear_controls(TYPE_NUMBER,Number_3_1);
                               dc_set_color(COLOR_WHITE,COLOR_LUCENCY);
                               ui_number(Number_3_1,time.year,-1,-1,USE_NEW_COLOR);
                            }
                            break;
                        case TIME_DAY:
                            ui_clear_controls(TYPE_NUMBER,Number_3_3);
                            ui_number(Number_3_3,time.day,-1,-1,USE_NEW_COLOR);
                            if(pos_old != pos)
                            {
                               pos_old =  pos;
                               ui_clear_controls(TYPE_NUMBER,Number_3_2);
                               dc_set_color(COLOR_WHITE,COLOR_LUCENCY);
                               ui_number(Number_3_2,time.month,-1,-1,USE_NEW_COLOR);
                            }
                            break;
                        case TIME_HOUR:
                            ui_clear_controls(TYPE_NUMBER,Number_3_4);
                            ui_number(Number_3_4,time.hour,-1,-1,USE_NEW_COLOR);
                            if(pos_old != pos)
                            {
                               pos_old =  pos;
                               ui_clear_controls(TYPE_NUMBER,Number_3_3);
                               dc_set_color(COLOR_WHITE,COLOR_LUCENCY);
                               ui_number(Number_3_3,time.day,-1,-1,USE_NEW_COLOR);
                            }
                            break;
                        case TIME_MIN:
                            ui_clear_controls(TYPE_NUMBER,Number_3_5);
                            ui_number(Number_3_5,time.minute,-1,-1,USE_NEW_COLOR);
                            if(pos_old != pos)
                            {
                               pos_old =  pos;
                               ui_clear_controls(TYPE_NUMBER,Number_3_4);
                               dc_set_color(COLOR_WHITE,COLOR_LUCENCY);
                               ui_number(Number_3_4,time.hour,-1,-1,USE_NEW_COLOR);
                            }
                            break;
                        case TIME_SEC:
                            ui_clear_controls(TYPE_NUMBER,Number_3_6);
                            ui_number(Number_3_6,time.second,-1,-1,USE_NEW_COLOR);
                            if(pos_old != pos)
                            {
                               pos_old =  pos;
                               ui_clear_controls(TYPE_NUMBER,Number_3_5);
                               dc_set_color(COLOR_WHITE,COLOR_LUCENCY);
                               ui_number(Number_3_5,time.minute,-1,-1,USE_NEW_COLOR);
                            }
                            break;
                    }
                    dc_set_color(COLOR_WHITE,COLOR_LUCENCY);
				}
                msg= app_get_msg();
                if(msg  > (u32)0x2000000)
                {
                    msg_d = msg ;
                    msg = msg_d->type ;
                }
				switch(msg)
				{
				case MSG_KEY_MENU:
				case MSG_KEY_MODE:
					ui_get_info(TYPE_PIC,Img_3_3,&rect);
					ui_clear_screen(rect.layer,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);
					ui_clear_screen(LAYER_OSD1_WIN0,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);
//					write_sys_time((RTC_TIME*)&time);
//					disp_sys_time(0);
					return TRUE;
				case MSG_KEY_NEXT:
					switch(pos)
					{
					case TIME_YEAR:
						time.year--;
						if(time.year < 1900)
							time.year = 2099;
						break;
					case TIME_MONTH:
						time.month--;
						if(time.month < 1)
							time.month = 12;
						break;
					case TIME_DAY:
						time.day--;
						if(time.day < 1)
                            time.day = day_to_month(time.year, time.month);
						break;
					case TIME_HOUR:
						time.hour--;
						if(time.hour == 0xff)
							time.hour = 23;
						break;
					case TIME_MIN:
						time.minute--;
						if(time.minute == 0xff)
							time.minute = 59;
						break;
					case TIME_SEC:
						time.second--;
						if(time.second == 0xff)
							time.second = 59;
						break;
					}
					update = TRUE;
					break;
				case MSG_KEY_PREV:
					switch(pos)
					{
					case TIME_YEAR:
						time.year++;
						if(time.year > 2099)
							time.year = 1900;
						break;
					case TIME_MONTH:
						time.month++;
						if(time.month > 12)
							time.month = 1;
						break;
					case TIME_DAY:
						time.day++;
						if(time.day > day_to_month(time.year, time.month))
							time.day = 1;
						break;
					case TIME_HOUR:
						time.hour++;
						if(time.hour > 23)
							time.hour = 0;
						break;
					case TIME_MIN:
						time.minute++;
						if(time.minute > 59)
							time.minute = 0;
						break;
					case TIME_SEC:
						time.second++;
						if(time.second > 59)
							time.second = 0;
						break;
					}
					update = TRUE;
					break;

				case MSG_KEY_OK:
					pos ++;
					if(pos > TIME_SEC)
					{
                        ui_get_info(TYPE_PIC,Img_3_3,&rect);
                        ui_clear_screen(rect.layer,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);
                        ui_clear_screen(LAYER_OSD1_WIN0,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);
                        write_sys_time((RTC_TIME*)&time);
                        disp_sys_time(0);
					}
//						pos = TIME_YEAR;
					update = TRUE;
					break;
                case MSG_MENU_EXIT:
                    put_msg_fifo(MSG_KEY_MENU,MSG_TO_SELF,0) ;
                    put_msg_fifo(MSG_MENU_EXIT,MSG_TO_SELF,0) ;
                    break;
                case MSG_ONE_SECOND:
                    disp_sys_time(0);
                    break;
                case MSG_DEVICECHANGE:
                    devsta = msg_d->ptr;
                     if(devsta->hdev->dev_io->device_type == DEV_LOGIC_DISK)
                     {

                        if(devsta->sta ==DEV_ONLINE )
                        {
                            ui_clear_controls(TYPE_PIC, Img_1_27);
                            ui_pic(Img_1_5,-1,-1,USE_ORIG_COLOR);// sd pic
                            if(get_current_mode() & ENC_MODE)
                            {
                                get_remain_time_rec();
                            }
                            else if(get_current_mode() & TPH_MODE)
                            {
                                disp_REC_cnt(0,ERASE_ONLY);//擦除数字区
                                num_of_last_photo();
                            }
                        }
                        else if(devsta->sta ==DEV_OFFLINE )
                        {
                            ui_clear_controls(TYPE_PIC, Img_1_5);
                            ui_pic(Img_1_27,-1,-1,USE_ORIG_COLOR);
                            if(get_current_mode() & ENC_MODE)
                            {
                                get_remain_time_rec();
                            }
                            else if(get_current_mode() & TPH_MODE)
                            {
                                disp_REC_cnt(0,ERASE_ONLY);//擦除数字区
                                reset_num_of_photo();
                            }
                        }
                     }
                     else if (DEV_ONLINE == devsta->sta && devsta->hdev->dev_io->device_type == DEV_USB_SLAVE ) //usb上线
                     {
                        put_msg_fifo(MSG_KEY_MENU,MSG_TO_SELF,0);
                        put_msg_fifo(MSG_MENU_EXIT,MSG_TO_SELF,0);
                        put_msg_fifo(MSG_USB_MENU,MSG_TO_SELF,0);
                     }
                    free(msg_d);
                    break;

				}
			}

		}

	}
	return TRUE;
}
u8 cb_photo_reso_ratio(u16 id,u8 mode)
{
    u8 temp_sel;
	if(mode == MENU_INIT)
	{
		set_select_item(sys_info.photo_reso_ratio);
	}
	else if(mode == MENU_SELECT)
	{
		temp_sel = get_select_item();
		if(temp_sel != sys_info.photo_reso_ratio)
		{

		    sys_info.photo_reso_ratio = temp_sel;
		    put_msg_fifo(MSG_SW_SIZE ,MSG_TO_PROCESS ,ENC_TASK_N) ;
		    updata_formate_pic(temp_sel+3);
		    reset_num_of_photo();
		    put_u16hex(temp_sel);
		}
		save_sysinfo(&sys_info);
	}
	return TRUE;
}
u8 cb_photo_date_label(u16 id,u8 mode)
{
	if(mode == MENU_INIT)
	{
		set_select_item(sys_info.photo_date_label);
	}
	else if(mode == MENU_SELECT)
	{
        u8 temp_sel = get_select_item();
		if(temp_sel != sys_info.photo_date_label)
		{
            sys_info.photo_date_label = temp_sel;
            save_sysinfo(&sys_info);
            set_pic_osd_en(sys_info.photo_date_label);
		}

	}
	return TRUE;
}

u8 cb_formatting(u16 id,u8 mode)
{
	if(mode == MENU_SELECT)
	{
		if(id == M141)
		{
		    if(is_phy_sd_online() == FALSE)
		    {
		        puts("\n bdisk_ready = FALSE \n");
                disp_msgbox(M156,200);
		    }
		    else
		    {
		        //display_note(M165);
		        Rect rect;
		        ui_get_info(TYPE_PIC,Img_1_40,&rect);
                //ui_pic(Img_1_50,-1,-1,USE_ORIG_COLOR);
                dc_drawstring(rect.layer,M165,rect.x,rect.y+rect.height/2,rect.width,ALIGN_CENTER,0);//jm  提示 没有SD卡
                //dc_drawstring(rect.layer,M147,rect.x,rect.y+((rect.height/3)-get_string_height(M147))/2,rect.width,ALIGN_CENTER,0);
		        if (mute_f_mkfs_drv('B') == 0)
		        {
		            puts("\n mkfs_drv OK \n");

		            FILE_ENTER_MUTEX();
                    init_record_dir();//格式化完需重新初始化目录
                    FILE_EXIT_MUTEX();

                    get_remain_time_rec();
		        }
		        else
		        {
		        }
		        //disp_msgbox(M165,3000);
		        //clear_display_noto();
		    }

		}
		else if(id == M140)
		{

		}
	}
	return TRUE;
}
void reset_ui_process(SYS_INF old_sys_info)
{
#ifdef FLASH_LIGHT
    if(old_sys_info.flashlight_mode != sys_info.flashlight_mode)
    {
        puts("\n flashlight_mode \n");
        updata_flashlight_mode_pic();
    }
#endif
    if(old_sys_info.parking_guard != sys_info.parking_guard)
        updata_parking_guard_pic();

    if(old_sys_info.date_label != sys_info.date_label)
        set_video_osd_en(sys_info.date_label);

    if(old_sys_info.photo_date_label != sys_info.photo_date_label)
        set_pic_osd_en(sys_info.photo_date_label);

    if(get_current_mode() & ENC_MODE)
    {
        if(old_sys_info.reso_ratio != sys_info.reso_ratio)
        {
            put_msg_fifo(MSG_SW_SIZE ,MSG_TO_PROCESS ,ENC_TASK_N);
            updata_formate_pic(sys_info.reso_ratio);
        }
        if(old_sys_info.moving_detect != sys_info.moving_detect)
        {
            updata_movingdet_pic();
            motion_det_if(sys_info.moving_detect);
        }
    }
    else
    {
         if(old_sys_info.photo_reso_ratio != sys_info.photo_reso_ratio)
         {
             put_msg_fifo(MSG_SW_SIZE ,MSG_TO_PROCESS ,ENC_TASK_N);
             updata_formate_pic(sys_info.photo_reso_ratio+3);
         }

    }
    if(old_sys_info.mic_mute != sys_info.mic_mute)
    {
        sys_info.mic_mute = old_sys_info.mic_mute; // 在使能任务里再设置
        mic_mute_fun(sys_info.mic_mute);
    }
    else
    {
        if(memcmp(&old_sys_info,&sys_info,sizeof(SYS_INF)))
        {
            save_sysinfo(&sys_info);
//            puts("\n save_sysinfo \n");
        }
    }


}

u8 cb_restore_factory_settings(u16 id,u8 mode)
{
	if(mode == MENU_SELECT)
	{
		if(id == M141)
		{
		    //display_note(M164);
		    Rect rect;
            ui_get_info(TYPE_PIC,Img_1_40,&rect);
            //ui_pic(Img_1_50,-1,-1,USE_ORIG_COLOR);
            dc_drawstring(rect.layer,M164,rect.x,rect.y+rect.height/2,rect.width,ALIGN_CENTER,0);//jm  提示 没有SD卡
            //dc_drawstring(rect.layer,M147,rect.x,rect.y+((rect.height/3)-get_string_height(M147))/2,rect.width,ALIGN_CENTER,0);
		    SYS_INF old_sys_info;
		    memcpy(&old_sys_info,&sys_info,sizeof(SYS_INF));
		    sys_info_reset();
			updata_menu_cfg();
			g_language_mode = language_talbe[sys_info.language];
			reset_ui_process(old_sys_info);
			//clear_display_noto();
			os_time_dly(10);
//			memcmp()
//			vm_api_write(SYS_SET_INFO,&sys_info);
		}
		else if(id == M140)
		{

		}
	}
	return TRUE;
}
u8 cb_play_setting(u16 id,u8 mode)
{
	if(mode == MENU_INIT)
	{

	}
	else if(mode == MENU_SELECT)
	{
	}
	return TRUE;
}

u8 cb_delfile(u16 id,u8 mode)
{
   //清菜单控件
    Rect rect;
	if(mode == MENU_SELECT)
	{
		printf("\ncb_delfile");
		if(id == M141)
		{
		    if(pb_fctrl.no_file == 1)
		    {
                //disp_msgbox(M149,200);
                ui_get_info(TYPE_PIC,Img_1_40,&rect);
                //ui_pic(Img_1_50,-1,-1,USE_ORIG_COLOR);
                dc_drawstring(rect.layer,M149,rect.x,rect.y+rect.height/2,rect.width,ALIGN_CENTER,0);
		    }
		    else
            {
                if(dec_file_pro())
                {
//                    disp_msgbox(M86,200);
                    ui_get_info(TYPE_PIC,Img_1_40,&rect);
                    //ui_pic(Img_1_50,-1,-1,USE_ORIG_COLOR);
                    dc_drawstring(rect.layer,M86,rect.x,rect.y+rect.height/2,rect.width,ALIGN_CENTER,0);//jm  提示 没有SD卡
                }

                else
                {
                    set_del_allfile_mark(0);
                    put_msg_fifo(MSG_DEC_DEL,MSG_TO_PROCESS,DEC_TASK_N) ;
                    //disp_msgbox(M159,200);
                    ui_get_info(TYPE_PIC,Img_1_40,&rect);
                    //ui_pic(Img_1_50,-1,-1,USE_ORIG_COLOR);
                    dc_drawstring(rect.layer,M159,rect.x,rect.y+rect.height/2,rect.width,ALIGN_CENTER,0);
                }
		    }
		}
		else if(id == M140)
		{

		}
	}

	return TRUE;
}

u8 cb_delallfile(u16 id,u8 mode)
{
   //清菜单控件
    Rect rect;
	if(mode == MENU_SELECT)
	{
		printf("\n cb_delfile");
		if(id == M141)
		{
            if(pb_fctrl.no_file == 1)
		    {
                //disp_msgbox(M149,200);
                ui_get_info(TYPE_PIC,Img_1_40,&rect);
                //ui_pic(Img_1_50,-1,-1,USE_ORIG_COLOR);
                dc_drawstring(rect.layer,M149,rect.x,rect.y+rect.height/2,rect.width,ALIGN_CENTER,0);
		    }
		    else
		    {
                set_del_allfile_mark(1);
                key_disable(1);
//                os_sem_set(Delfile_Semp,0);
                put_msg_fifo(MSG_DEC_DEL,MSG_TO_PROCESS,DEC_TASK_N) ;
                //display_note(M166);
                ui_get_info(TYPE_PIC,Img_1_40,&rect);
                //ui_pic(Img_1_50,-1,-1,USE_ORIG_COLOR);
                dc_drawstring(rect.layer,M166,rect.x,rect.y+rect.height/2,rect.width,ALIGN_CENTER,0);
//                while(os_sem_pend(Delfile_Semp,0) != OS_NO_ERR );
//                while(os_sem_pend(Delfile_Semp,500) == OS_TIMEOUT );
                while(get_del_allfile_mark() == 1)
                {
                   os_time_dly(20);
                   //puts("A");
                }

                puts("\n --------Delfile_Semp---------\n");
                key_disable(0);
                //clear_display_noto();
		    }

		}
		else if(id == M140)
		{

		}
	}

	return TRUE;
}
u8 cb_lockfile(u16 id,u8 mode)
{
   //清菜单控件
    printf("pb_fctrl.current_folder = %d\n",pb_fctrl.current_folder );
	if(mode == MENU_SELECT)
	{
		printf("\ncb_delfile");
		if(id == M141)
		{
		    if(pb_fctrl.no_file == 1)
		    {
                disp_msgbox(M149,200);
		    }
		    else
            {
                puts(" MSG_LOCK_FILE \n");
                put_msg_fifo(MSG_LOCK_FILE,MSG_TO_PROCESS,DEC_TASK_N) ;
		    }
		}
		else if(id == M140)
		{

		}
	}

	return TRUE;
}
u8 cb_unlockfile(u16 id,u8 mode)
{
   //清菜单控件

	if(mode == MENU_SELECT)
	{
		printf("\ncb_delfile");
		if(id == M140)
		{
		    if(pb_fctrl.no_file == 1)
		    {
                disp_msgbox(M149,200);
		    }
		    else
            {

		    }
		}
		else if(id == M141)
		{
            puts(" MSG_UNLOCK_FILE \n");
            put_msg_fifo(MSG_UNLOCK_FILE,MSG_TO_PROCESS,DEC_TASK_N);
		}
	}

	return TRUE;
}

u8 cb_unlock_all_file(u16 id,u8 mode)
{
   //清菜单控件
	if(mode == MENU_SELECT)
	{
		if(id == M141)
		{
//		    if(is_sd_online() ==FALSE)
//                return TRUE;
            if(pb_fctrl.no_file == 1)
            {
                disp_msgbox(M149,200);
            }
            else
            {
                key_disable(1);

                put_msg_fifo(MSG_UNLOCK_ALLFILE,MSG_TO_PROCESS,DEC_TASK_N) ;
//                display_note(M181);

                key_disable(0);
//                clear_display_noto();
            }

		}
		else if(id == M140)
		{

		}
	}

	return TRUE;
}


void sys_info_reset(void)
{
    sys_info.para_flag = UI_PARA_REFRESH;
    sys_info.reso_ratio = 0;
    #ifdef LINGDUPRJ
        sys_info.loop_video = 3;
    #else
        sys_info.loop_video = 1;//2;
    #endif
    sys_info.date_label = 1;
    sys_info.gravity_sensor = 0;//2;//0;
    sys_info.key_voice = 1;
    sys_info.wdr = 1;//0;
    #ifdef LINGDUPRJ
        sys_info.auto_poweroff = 3;
    #else
        sys_info.auto_poweroff = 0;//2;
    #endif
    sys_info.language = 0;  // 0简体中文 5 俄语
    sys_info.light_freq = 0;
    sys_info.screen_protect = 0;
    sys_info.photo_date_label = 1;
    #ifdef LINGDUPRJ
    sys_info.photo_reso_ratio = 2;
    #else
     sys_info.photo_reso_ratio = 0;
    #endif
    sys_info.mic_mute = 1;
    sys_info.parking_guard = 0;
    sys_info.flashlight_mode = 0; //背光灯关闭
    sys_info.while_balance = 0;// 自动
    sys_info.exposure = 3;// 无补偿
    sys_info.color_effect = 0;//无特效
    sys_info.moving_detect = 0; //关闭
    sys_info.TV_mode = 0;//pal
}

u8 get_vedio_reso_ratio(void)
{
    return sys_info.reso_ratio;
}
void set_vedio_reso_ratio(u8 x)
{

}

u8 get_vedio_date_label(void)
{
    return sys_info.date_label;
}
void set_vedio_date_label(u8 x)
{

}


u8 get_loop_video_time(void)
{
    return sys_info.loop_video;
}
void set_loop_video_time(u8 x)
{

}
u8 get_cycle_time(void)
{
    u8 cycle_time  = 0;
    cycle_time = 60*sys_info.loop_video;
    return cycle_time;
}

u8 get_parking_guard(void)
{
    return (sys_info.parking_guard)?0:1;
}
void set_parking_guard(u8 x)
{

}

u8 get_gravity_sensor(void)
{
    if(sys_status.gsensor_online)
        return sys_info.gravity_sensor;
    else
        return 0;
}
void set_gravity_sensor(u8 x)
{

}


u8 get_flashlight_mode(void)
{
   return sys_info.flashlight_mode;
}

void set_flashlight_mode(u8 x)
{
    if(x==0)
        sys_info.flashlight_mode = 0;
    else if(x==1)
        sys_info.flashlight_mode = 1;
    else
        sys_info.flashlight_mode = 2;
}

u8 get_TV_mode(void)
{
   return sys_info.TV_mode;
}
void set_TV_mode(u8 x)
{

}
u8 get_photo_reso_ratio(void)
{
    return sys_info.photo_reso_ratio;
}
void set_photo_reso_ratio(u8 x)
{

}

u8 get_photo_date_label(void)
{
    return sys_info.photo_date_label;
}
void set_photo_date_label(u8 x)
{

}


//#ifdef DOUBLE_CAR_UI
#include "ui_dynamic_effect.h"

#if DOUBLE_UI_A
void draw_confirm_dialog(u8 dialog,u8 mode,void *parm, MVAR menu_var)
{
	DVAR *var = (DVAR *)parm;
	Rect rect;

	if(dialog == DIALOG_CONFIRM)//确认对话框
	{
		if(mode == DRAW_BACKPIC)
		{
//			ui_pic(Img_1_41,-1,-1,0);
            ui_get_info(TYPE_PIC,Img_1_40,&rect);
//            ui_get_info(TYPE_MENU,Menu_2_1,&rect);
            ui_clear_screen(LAYER_OSD1_WIN0,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);
			ui_pic(Img_1_40,-1,-1,0);
        //19和18对应的是格式化和恢复默认的菜单顺序，增加补光灯后往后移 JOHNSON 20160531
			if ((menu_var.node) == /*19*/18) //恢复默认设置的node在菜单中变化，此处也相应变化
            {
                dc_drawstring(LAYER_OSD1_WIN0,M180,rect.x,rect.y*2+10,rect.width,ALIGN_CENTER,0);
            }
            else if ((menu_var.node) == /*18*/17)//格式化的node在菜单中变化，此处也相应变化
            {
                dc_drawstring(LAYER_OSD1_WIN0,M179,rect.x,rect.y*2+10,rect.width,ALIGN_CENTER,0);
            }
		}
		else if(mode == GET_TITLE_RECT)
		{
			ui_get_info(TYPE_PIC,Img_1_41,&var->rect);
		}
		else if(mode == DRAW_ITEM)
		{
			Rect rect_left,rect_right;

			ui_get_info(TYPE_PIC,Img_1_42,&rect_left);
			ui_get_info(TYPE_PIC,Img_1_43,&rect_right);

//			ui_pic(var->is_active?Img_1_42:Img_1_43,(var->item_idx == 0)?rect_left.x:rect_right.x,-1,SHOW_ALL);
            if(var->is_active)
            {
                ui_pic(Img_1_42,(var->item_idx == 0)?rect_left.x:rect_right.x,-1,SHOW_ALL);
            }
            else
            {
                if(var->item_idx == 0)
                {
                    ui_clear_controls(TYPE_PIC,Img_1_42);
                }
                else
                {
                    ui_clear_controls(TYPE_PIC,Img_1_43);
                }
            }

			if(var->item_idx == 0)
			{
				ui_get_info(TYPE_PIC,Img_1_42,&var->rect);
			}
			else
			{
			   ui_get_info(TYPE_PIC,Img_1_43,&var->rect);
			}

		}
	}
	else if(dialog == DIALOG_BROADSIDE)//侧边栏
	{
		if(mode == DRAW_BACKPIC)
		{

		}
		else if(mode == GET_TITLE_RECT)
		{

		}
		else if(mode == DRAW_ITEM)
		{
			Rect rect,rect1;
			u16 id;
			u16 h;
			u16 w;
            puts("\n DRAW_ITEM \n");
			ui_get_info(TYPE_PIC,Img_2_9,&rect);
			ui_get_info(TYPE_PIC,Img_2_8,&rect1);
			w = rect.width;
//			h = rect1.y - rect.y;
			if(var->is_active)
				id = Img_2_9;
			else
				id = Img_2_8;
//			ui_pic(id,-1,rect.y + var->item_idx * h,SHOW_ALL);
//			ui_pic(id,rect.x+var->item_idx * w,-1,SHOW_ALL);
//			puts("\n --1-- \n");
//            put_u16hex(w);
//            put_u16hex(var->item_idx);
//            puts("\n --2-- \n");
			if(var->is_select)
			{
				ui_get_info(TYPE_PIC,Img_2_9,&rect1);
//				ui_pic(Img_2_9,-1,rect.y + (rect.height-rect1.height)/2 + var->item_idx * h,SHOW_ALL);
//				ui_pic(Img_2_9,rect.x  + var->item_idx * w,-1,SHOW_ALL);
			}

			ui_get_info(TYPE_PIC,Img_2_9,&var->rect);
//			var->rect.y = rect.y + var->item_idx * h;
			var->rect.x = rect.x + var->item_idx * w;

			//ui_pic(Img_1_17,-1,rect.y + (var->item_idx+1 )* h,SHOW_ALL);
		}
	}
	else if(dialog == DIALOG_MENU)
	{
		if(mode == DRAW_ITEM)
		{
//			ui_clear_screen(LAYER_OSD0_WIN0,var->rect.x,var->rect.y,var->rect.width-2,var->rect.height,COLOR_BLACK);
		}
        else if(mode == DRAW_BACKPIC)
		{
            ui_pic(Img_2_12,-1,-1,USE_ORIG_COLOR|SHOW_ALL);
//            ui_clear_screen(LAYER_OSD1_WIN0,var->rect.x,var->rect.y,var->rect.width-2,var->rect.height,COLOR_LUCENCY);
		}
		else if (mode == DRAW_M_BACKPIC)
		{//版本号设置
		    DTEXT text_temp;
            text_temp.flags = ANSI;
            #ifdef LINGDUPRJ
                text_temp.buf = LD_PJ_VERSION;
                text_temp.len = 20;
                ui_text(Txt_1_4,&text_temp);
                text_temp.buf = G_SENSOR_VERSION;
                text_temp.len = 12;
                ui_text(Txt_1_5,&text_temp);
                text_temp.buf = LD_SW_VERSION;
                text_temp.len = 60;
                ui_text(Txt_1_3,&text_temp);
            #else
                #if (UART_DEBUG_EN == 1)
                    text_temp.buf = SENNOR_VERSION;
                    text_temp.len = 20;
                    ui_text(Txt_1_4,&text_temp);
                #endif
                text_temp.buf = SOFTWARE_VERSION;
                text_temp.len = 80;
                ui_text(Txt_1_3,&text_temp);
            #endif

		}
	}
	else if(dialog == DIALOG_MOVE_IN)
	{

	}
	else if(dialog == DIALOG_MOVE_OUT) //退出菜单
	{
		Rect rect;
		ui_get_info(TYPE_MENU,Menu_2_1,&rect);
		ui_clear_screen(LAYER_OSD0_WIN0,rect.x,rect.y,rect.width,rect.width,COLOR_LUCENCY);
		ui_clear_screen(rect.layer,rect.x,rect.y,rect.width,rect.width,COLOR_LUCENCY);
//		ui_clear_screen(LAYER_OSD1_WIN0,0,58,103,170,COLOR_LUCENCY);
	}
	else if(dialog == DIALOG_SECOND_MENU)
	{
        if(mode == DRAW_ITEM)
        {


        }
        else if(mode == DRAW_BACKPIC)
        {
            if (get_current_mode() & DEC_MODE)
            {
                ui_get_info(TYPE_PIC,Img_1_40,&rect);
                ui_clear_screen(LAYER_OSD1_WIN0,rect.x,rect.y,rect.width,rect.width,COLOR_LUCENCY);
            }
        }
	}
}
#endif //DOUBLE_UI_A

