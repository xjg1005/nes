#include "mode_switch.h"
#include "dev_ctrl.h"
#include "drv_manage.h"
#include "app_cfg.h"
#include "msg.h"
#include "font_area_ctl.h"
#include "ui.h"
#include "ui_con.h"
#include "menu.h"
#include "ui_com.h"
#include "vds.h"
//#include "jpeg.h"
#include "jpeg_dec_if.h"
//#include "ui_struct.h"
#include "timer.h"
#include "enc_mode.h"
#include "ui_task.h"
#include "car_fs_api.h"
#include "dec_mode.h"
#include "vm_api.h"
#include "my_ui.h"
#include "dc_showres.h"
#include "usb_mode.h"
#include "irtc.h"
#include "isp_customize.h"
#include "powercheck.h"
#include "lcd.h"

extern MENU_ARRAY menu_array0[];
extern MENU_ARRAY menu_array1[];
extern MENU_ARRAY menu_array2[];
extern MENU_ARRAY menu_array2_lock[];
extern MENU_ARRAY menu_array2_unlock[];

u8 get_menu_statu(void);
void get_remain_time_rec(void);//剩余可录像时间
void screen_protect_control(void);
#define UPDATE_TIME    0x0001
#define DISP_MASK	   0x0002

RTC_TIME system_time;


#define YUV_TABLE	0
#define RGB_TABLE	1
typedef struct
{
	u32 update_flag;
	u8 sd_online;		//SD卡是否在线
	u8 battery_level;   //电量 4:正在充电 0~3:电量
	u8 lock;			//是否加锁
	u8 disp_mode;		//显示模式
	u8 encode_fmt;		//编码格式
	TIME *time;
}SYSTEM_VAL;

#define MODE_VIDEO_RECORD	0
#define MODE_TAKE_PHOTO		1
#define MODE_REPLAY			2

static SYSTEM_VAL sval;

static u16 rec_cnt_seconds = 0; //录像计时
static u8 note_mark = 0;

volatile u8 usb_out_status = 0;  //1已拔usb 0插入usb

u8 get_sdnote_status(void)
{
    return note_mark;
}
void reset_sdnote_status(void)
{
    note_mark = 0;
}


void display_note(u16 id)
{
       if (get_menu_statu())
        return;

    if(g_startup_finish == 0)
        return;
    if(sys_status.powerdown_flag)
        return;

    if(id == M155 ||id == M156 || id == M154)
    {
        if(note_mark == 0)
            note_mark = 1;
        else
            return;
    }
#if DOUBLE_UI_A
   	Rect rect;
   	if(id == M148)
   	{
   	    ui_get_info(TYPE_PIC,Img_3_3,&rect);
   	}
   	else
   	{
        ui_get_info(TYPE_MENU,Menu_2_1,&rect);
   	}

	ui_clear_screen(LAYER_OSD1_WIN0,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);
	ui_clear_screen(LAYER_OSD0_WIN0,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);
	ui_pic(Img_3_3,-1,-1,USE_ORIG_COLOR);
	dc_drawstring(rect.layer,id,rect.x,rect.y+(rect.height-get_string_height(id))/2,rect.width,ALIGN_CENTER,0);
#endif //DOUBLE_UI_A


}

void clear_display_noto(void)
{
    if(sys_status.powerdown_flag)
        return;

    if (get_menu_statu())
        return;

//    ui_clear_controls(TYPE_PIC,Img_3_3);
//    ui_clear_screen(LAYER_OSD1_WIN0, 150, 50, 244, 150, COLOR_LUCENCY);
#if DOUBLE_UI_A
    Rect rect;
    if (!get_parking_status())
    {
    ui_get_info(TYPE_PIC,Img_3_3,&rect);
    ui_clear_screen(LAYER_OSD0_WIN0,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);
    ui_clear_screen(LAYER_OSD1_WIN0,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);
    }
#endif //DOUBLE_UI_A

    reset_sdnote_status();
}
void clear_SD_note(void)
{
    if(sys_status.powerdown_flag)
        return;
    static u8 cnt=0;
    if(g_startup_finish == 0)
        return;
    if(get_sdnote_status())
    {
        cnt++;
        if(cnt>=8)
        {
            cnt = 0;
            if((mode_ctrl.cur_mode & (ENC_MODE | TPH_MODE) ) && get_menu_statu()==0)
                clear_display_noto();
            reset_sdnote_status();
        }
    }
}

void clear_rec_cnt(void) //录像计时清零
{
    rec_cnt_seconds = 0;

//    disp_REC_cnt(0,ERASE_ONLY);//擦除数字区
//    get_remain_time_rec();
}

u16 get_rec_cnt(void) //录取录像计时时长
{
    return rec_cnt_seconds;
}

void updata_parking_guard_pic(void)
{
#if DOUBLE_UI_A
    if(sys_info.parking_guard)
    {
        ui_clear_controls(TYPE_PIC,Img_1_46);
        ui_pic(Img_1_45,-1,-1,USE_ORIG_COLOR);
    }
    else
    {
        ui_clear_controls(TYPE_PIC,Img_1_45);
 //       ui_pic(Img_1_46,-1,-1,USE_ORIG_COLOR);
    }
#endif //DOUBLE_UI_A

}
void updata_movingdet_pic(void)
{
#if DOUBLE_UI_A
    if(sys_info.moving_detect)
    {
        ui_pic(Img_1_48,-1,-1,USE_ORIG_COLOR);
    }
    else
    {
        ui_clear_controls(TYPE_PIC,Img_1_48);
    }
#endif //DOUBLE_UI_A


}
#ifdef FLASH_LIGHT
void updata_flashlight_mode_pic(void)
{
#if DOUBLE_UI_A
    ui_clear_controls(TYPE_PIC,Img_1_9);
    ui_clear_controls(TYPE_PIC,Img_1_23);
    ui_clear_controls(TYPE_PIC,Img_1_33);
     if(sys_info.flashlight_mode ==/*0*/1) //开 //JOHNSON 20160618
        ui_pic(Img_1_9,-1,-1,USE_ORIG_COLOR);
    else if(sys_info.flashlight_mode ==/*1*/0)//关
        ui_pic(Img_1_23,-1,-1,USE_ORIG_COLOR);
    else//自动
        ui_pic(Img_1_33,-1,-1,USE_ORIG_COLOR);

#endif //DOUBLE_UI_A
}
#endif

void updata_mic_pic(void)
{
#if DOUBLE_UI_A
    if(sys_info.mic_mute == 0)
    {
        ui_clear_controls(TYPE_PIC,Img_1_25);
        ui_pic(Img_1_26,-1,-1,USE_ORIG_COLOR);
    }
    else
    {
        ui_clear_controls(TYPE_PIC,Img_1_26);
        ui_pic(Img_1_25,-1,-1,USE_ORIG_COLOR);
    }
#endif //DOUBLE_UI_A


}
void photomode_icon_switch(u8 mark)
{
#if DOUBLE_UI_A
    ui_pic(Img_1_16,-1,-1,ERASE_ONLY);
    ui_pic(Img_1_7,-1,-1,USE_ORIG_COLOR);
    os_time_dly(50);
    ui_pic(Img_1_7,-1,-1,ERASE_ONLY);
    ui_pic(Img_1_16,-1,-1,USE_ORIG_COLOR);
#endif

}

void vediomode_icon_switch(u8 mark)
{
#if 0
    static u8 temp = 1;
    if(mark == 0)
        temp = mark;
    else
    {
        if(temp==0)
            temp =1;
    }

#if DOUBLE_UI_A
    if(sys_info.moving_detect)
    {
        ui_pic(Img_1_32,-1,-1,ERASE_ONLY);
        ui_pic(Img_1_38,-1,-1,ERASE_ONLY);
        ui_pic(Img_1_39,-1,-1,ERASE_ONLY);
        if(temp == 0)
        {
            ui_pic(Img_1_32,-1,-1,USE_ORIG_COLOR);
        }
        else if(temp==1)
        {
            temp =2;
            ui_pic(Img_1_38,-1,-1,USE_ORIG_COLOR);
        }
        else
        {
            temp =1;
            ui_pic(Img_1_39,-1,-1,USE_ORIG_COLOR);
        }
    }
    else
    {
        ui_pic(Img_1_17,-1,-1,ERASE_ONLY);
        ui_pic(Img_1_34,-1,-1,ERASE_ONLY);
        ui_pic(Img_1_35,-1,-1,ERASE_ONLY);
        if(temp == 0)
        {
            ui_pic(Img_1_17,-1,-1,USE_ORIG_COLOR);
        }
        else if(temp==1)
        {
            temp =2;
            ui_pic(Img_1_34,-1,-1,USE_ORIG_COLOR);
        }
        else
        {
            temp =1;
            ui_pic(Img_1_35,-1,-1,USE_ORIG_COLOR);
        }
    }
#endif //DOUBLE_UI_A

#endif
}

const u16 fmt[]={
#ifndef DOUBLE_VEDIO
	    Img_1_12,
#endif
	    Img_1_12,
	    Img_1_13,
#ifdef DOUBLE_VEDIO
	    Img_1_12,
#endif
	    Img_1_28,
	    Img_1_15,
	    Img_1_30,
//	    Img_1_29
	    }; //

//    const u16 fmt[]={Img_1_13,Img_1_14,Img_1_12,Img_1_28,Img_1_15,Img_1_30,Img_1_29};


void updata_formate_pic(u8 index)
{
#if DOUBLE_UI_A
    Rect rect1,rect2;

//    const u16 fmt[]={Img_1_12,Img_1_13,Img_1_14,Img_1_28,Img_1_15,Img_1_30,Img_1_29}; //
    u16 tmp = -1;
    tmp = fmt[index];
#if 0
    printf("\nformate_pic index %d\n",index);
    printf("\nformate_pic %d\n",tmp);
    GUI_POINT point = {0,0};
    ui_pic(Img_1_8,-1,-1,USE_ORIG_COLOR);
//	if(tmp != tmp0)
	{
//		tmp0 = tmp;
		ui_get_info(TYPE_PIC,Img_1_8,&rect1);
		ui_get_info(TYPE_PIC,tmp,&rect2);
		point.x = rect1.x + (rect1.width-rect2.width)/2;
		point.y = rect1.y + (rect1.height-rect2.height)/2;
	}
	ui_pic(tmp,point.x,point.y,USE_ORIG_COLOR);
#endif
    if (((tmp == Img_1_12) || (tmp == Img_1_13)) && (mode_ctrl.cur_mode & ENC_MODE))
    {
        ui_pic(tmp,-1,-1, ERASE_ONLY);
        ui_pic(tmp,-1,-1,USE_NEW_COLOR);
    }
    else if (((tmp == Img_1_28) || (tmp == Img_1_15) || (tmp == Img_1_30)) && (mode_ctrl.cur_mode & TPH_MODE))
    {
        ui_pic(tmp,-1,-1, ERASE_ONLY);
        ui_pic(tmp,-1,-1,USE_NEW_COLOR);
    }
#endif //DOUBLE_UI_A

}
#if DOUBLE_UI_A
const u16 battery_icon[]={Img_1_20,Img_1_19,Img_1_4,Img_1_21,Img_1_18};
#endif //DOUBLE_UI_A



void main_frame(SYSTEM_VAL *val)
{
#if DOUBLE_UI_A
	Rect rect1,rect2;

	static u16 tmp = -1,tmp0 = -1;
	static GUI_POINT point = {0,0};

	//窗口框架
//	ui_pic(Img_1_6,-1,-1,USE_ORIG_COLOR);
	//ui_pic(Img_1_22,-1,-1,USE_ORIG_COLOR);

//	if(val->update_flag & DISP_MASK)
//		ui_pic(Img_1_3,-1,-1,USE_ORIG_COLOR);
//	else
//		ui_pic(Img_1_3,-1,-1,ERASE_ONLY);

//	ui_pic(Img_1_1,-1,-1,USE_ORIG_COLOR);
//	ui_pic(Img_1_2,-1,-1,USE_ORIG_COLOR);
	ui_pic(battery_icon[val->battery_level],-1,-1,USE_ORIG_COLOR);

//	ui_pic(Img_1_7,-1,-1,ERASE_ONLY);
	ui_pic(Img_1_17,-1,-1,ERASE_ONLY);
	ui_pic(Img_1_16,-1,-1,ERASE_ONLY);

	if(val->sd_online)
		ui_pic(Img_1_5,-1,-1,USE_ORIG_COLOR);
    else
        ui_pic(Img_1_27,-1,-1,USE_ORIG_COLOR);

	if(val->disp_mode == MODE_VIDEO_RECORD)
	{
        if(enc_ctrl.enc0_status != ENC_APP_RUNNING)
            get_remain_time_rec();

	    if(sys_info.moving_detect)
            ui_pic(Img_1_48,-1,-1,USE_ORIG_COLOR);
//	    else
            ui_pic(Img_1_17,-1,-1,USE_ORIG_COLOR);

#if LANE_DECT_EN
        if(sys_info.lane_dect_en)
            ui_pic(Img_1_46,-1,-1,USE_ORIG_COLOR);//车道检测
#endif

	}
	else if(val->disp_mode == MODE_TAKE_PHOTO)
		ui_pic(Img_1_16,-1,-1,USE_ORIG_COLOR);
//	else if(val->disp_mode == MODE_REPLAY)
//	{
//#if LANE_DECT_EN
//        ui_pic(Img_1_46,-1,-1,ERASE_ONLY);
//#endif
//	    ui_pic(Img_1_7,-1,-1,USE_ORIG_COLOR);
//	}


//	ui_pic(Img_1_8,-1,-1,USE_ORIG_COLOR);

	if(val->lock)
		ui_pic(Img_1_47,-1,-1,USE_ORIG_COLOR);

//	tmp = fmt[val->encode_fmt];
//
//	printf("\n val->encode_fmt 1 %d\n",val->encode_fmt);
//	printf("\n val->encode_fmt 2 %d\n",tmp);
//	if(tmp != tmp0)
//	{
//		tmp0 = tmp;
//		ui_get_info(TYPE_PIC,Img_1_8,&rect1);
//		ui_get_info(TYPE_PIC,tmp,&rect2);
//		point.x = rect1.x + (rect1.width-rect2.width)/2;
//		point.y = rect1.y + (rect1.height-rect2.height)/2;
//	}
//	ui_pic(tmp,point.x,point.y,USE_ORIG_COLOR);

	if(val->update_flag & UPDATE_TIME)
	{
		val ->update_flag &=~ UPDATE_TIME;
//		ui_time1(Txt_3_1,val->time,TIME_YYYY_MM_DD_HH_MM_SS,-1,-1,USE_ORIG_COLOR);
	}

////ÏÔÊ¾Â¼Ïñ·Ö±æÂÊ
//	if (get_current_mode() & ENC_MODE)
//    {
//        if (sys_info.reso_ratio == 0)
//            ui_pic(Img_1_12, -1, -1, USE_NEW_COLOR);
//        else
//            ui_pic(Img_1_13, -1, -1, USE_NEW_COLOR);
//    }

////ÏÔÊ¾ÅÄÕÕ·Ö±æÂÊ
//    if (get_current_mode() & TPH_MODE)
//    {
//        if (sys_info.photo_reso_ratio == 0)
//            ui_pic(Img_1_28,-1,-1,USE_NEW_COLOR);
//        else if (sys_info.photo_reso_ratio == 1)
//            ui_pic(Img_1_15,-1,-1,USE_NEW_COLOR);
//        else if (sys_info.photo_reso_ratio == 2)
//            ui_pic(Img_1_30,-1,-1,USE_NEW_COLOR);
//    }

//   补光灯参数
#ifdef FLASH_LIGHT
   if(get_current_mode() & ENC_MODE)
   {
        ui_clear_controls(TYPE_PIC,Img_1_9);
        ui_clear_controls(TYPE_PIC,Img_1_23);
        ui_clear_controls(TYPE_PIC,Img_1_33);
       if(sys_info.flashlight_mode ==/*0*/1 ) //开 //JOHNSON 20160618
        {
            ui_pic(Img_1_9,-1,-1,USE_ORIG_COLOR);
        }
        else if(sys_info.flashlight_mode ==/*1*/0) //关
            ui_pic(Img_1_23,-1,-1,USE_ORIG_COLOR);
        else
            ui_pic(Img_1_33,-1,-1,USE_ORIG_COLOR);
   }
#endif

    if(get_current_mode() & ENC_MODE)
    {
        if(sys_info.mic_mute)
            ui_pic(Img_1_25,-1,-1,USE_ORIG_COLOR);//¾²ÒôÉèÖÃ ¹Ø
        else
            ui_pic(Img_1_26,-1,-1,USE_ORIG_COLOR);//¿ª

        if(sys_info.parking_guard)
            ui_pic(Img_1_45,-1,-1,ERASE_DRAW);//ui_pic(Img_1_24,-1,-1,USE_ORIG_COLOR);//停车守卫//jm
//        else
//            ui_pic(Img_1_46,-1,-1,USE_ORIG_COLOR);//ui_clear_controls(TYPE_PIC,Img_1_24);//jm
    }

    if(get_current_mode() & DEC_MODE)
    {
       if(dec_ctrl.dec_type == PKG_TYPE_AVI)
        {
            ui_pic(Img_4_15, -1, -1, ERASE_ONLY);
            ui_pic(Img_4_16, -1, -1, USE_ORIG_COLOR);
        }
         else if(dec_ctrl.dec_type == PKG_TYPE_JPG)
        {
            ui_pic(Img_4_16, -1, -1, ERASE_ONLY);
            ui_pic(Img_4_15, -1, -1, USE_ORIG_COLOR);
        }
    }

#endif //DOUBLE_UI_A

}


void updata_mode_pic(u8 mode)
{
#if DOUBLE_UI_A

    switch(mode)
    {
    case MODE_VIDEO_RECORD:
        ui_pic(Img_1_17,-1,-1,USE_ORIG_COLOR);
        break;
    case MODE_TAKE_PHOTO:
        ui_pic(Img_1_16,-1,-1,USE_ORIG_COLOR);
        break;
    case MODE_REPLAY:
        ui_pic(Img_1_7,-1,-1,USE_ORIG_COLOR);
        break;
    }
#endif //DOUBLE_UI_A

}
void disp_closelogo(void)
{
    g_ss_flag = 1;//注意：只有在显示开关机图片的时候，才使能!!!

#if DOUBLE_UI_A
    ui_pic(Img_1_22,-1,-1,USE_ORIG_COLOR);

#endif //DOUBLE_UI_A

    g_ss_flag = 0;
}

void disp_startlogo(void)
{
    g_ss_flag = 1;//注意：只有在显示开关机图片的时候，才使能!!!

#if DOUBLE_UI_A
    ui_pic(Img_1_6,-1,-1,USE_NEW_COLOR);

#endif //DOUBLE_UI_A

    g_ss_flag = 0;

}

void up_data_uipic(void)
{
#if DOUBLE_UI_A
    ui_pic(Img_1_23,-1,-1,USE_ORIG_COLOR);//补光灯参数

    os_time_dly(10);
    ui_pic(Img_1_25,-1,-1,USE_ORIG_COLOR);//静音设置
    os_time_dly(10);
    ui_pic(Img_1_45,-1,-1,USE_ORIG_COLOR);//停车守卫
#endif //DOUBLE_UI_A


}

#define MODE_PLAY		0
#define MODE_PAUSE		1
#define MODE_ERASE		2


u8 fileNum2Str(u32 index,u32 total,u8 *pbuf)
{
    u8 i;
    u8 len1 =1,len2 =1;
    if(index >9999)
        len1 =5;
    else if(index >999)
        len1 = 4;
    else if(index >99)
        len1 = 3;
    else if(index >9)
        len1 =2;
    i= 0;
    while(len1)
    {
        pbuf[len1-1]= (index%(10)) + 0x30;
        index = index/(10);
        len1--;
        i++;
    }
    len1 = i;
    pbuf[i++] = '/';
    if(total >9999)
        len2 =5;
    else if(total >999)
        len2 = 4;
    else if(total >99)
        len2 = 3;
    else if(total >9)
        len2 =2;
    while(len2)
    {
        pbuf[len1 + len2]= (total%(10)) + 0x30;
        total = total/(10);
        len2--;
        i++;
    }
    return i;
}

void updata_curfile_num(void)
{
    static  u16 current_file_indexA = 0 ,current_file_indexB = 0;
    static  u16 index_A,index_B;
#if DOUBLE_UI_A
    ui_clear_controls(TYPE_TEXT,Txt_4_4);
#endif //DOUBLE_UI_A

    static u8 Str_buf[12];
    u32 current_file_index,total_file_num;

    total_file_num = pb_fctrl.foladerA_file_num+ pb_fctrl.foladerB_file_num;

    if(pb_fctrl.current_folder == 0) //B
    {
        if(pb_fctrl.cur_findex_B == pb_fctrl.foladerB_endFile_index)
        {
           index_B = pb_fctrl.foladerB_file_num;
        }
        else if(pb_fctrl.cur_findex_B == pb_fctrl.foladerB_startFile_index)
        {
           index_B = 1;
        }
        else
        {
            if(current_file_indexB>pb_fctrl.cur_findex_B)
            {
                 index_B--;
            }
            else if(current_file_indexB<pb_fctrl.cur_findex_B)
            {
                index_B++;
            }


        }
        current_file_indexB =   pb_fctrl.cur_findex_B;
        current_file_index = index_B;
    }
    else
    {
        if(pb_fctrl.cur_findex_A == pb_fctrl.foladerA_endFile_index)
        {
           index_A = pb_fctrl.foladerB_file_num +pb_fctrl.foladerA_file_num;
        }
        else if(pb_fctrl.cur_findex_A == pb_fctrl.foladerA_startFile_index)
        {
            index_A = pb_fctrl.foladerB_file_num +1;
        }
        else
        {
            if(current_file_indexA > pb_fctrl.cur_findex_A)
            {
                index_A--;
            }
            else if(current_file_indexA < pb_fctrl.cur_findex_A)
            {
                index_A++;
            }

        }
        current_file_indexA =   pb_fctrl.cur_findex_A;
        current_file_index =index_A;
    }


	DTEXT text_temp;
	text_temp.flags = ANSI;
    text_temp.buf = Str_buf;
    text_temp.len = fileNum2Str(current_file_index,total_file_num,Str_buf);
    printf("Str_buf :  %s \n",Str_buf);
#if DOUBLE_UI_A
	ui_text(Txt_4_4,&text_temp);
#endif //DOUBLE_UI_A

}
#if DOUBLE_UI_A
void play_screen(u8 play_status, TIME *time1, TIME *time2)
#endif

{
#if DOUBLE_UI_A
	u32 curr_second,total_second;
	static u32 old_curr_second = 0,old_total_second = 0;
	static u8 old_play_status = 0xFF;
	Rect rect;
	float percent;
	static Rect rect1 = {0,0,0,0,0};


    if(play_status == MODE_ERASE)
    {
//        ui_clear_controls(TYPE_TEXT,Txt_4_3);
//        ui_clear_controls(TYPE_TEXT,Txt_4_4);
        //ui_clear_screen(LAYER_OSD1_WIN0,29,3,72,16,COLOR_LUCENCY);//JOHNSON 20160321
        old_play_status = MODE_ERASE;
//        ui_clear_screen(LAYER_OSD0_WIN0,125,200,330,45,COLOR_LUCENCY);
//        ui_clear_screen(LAYER_OSD1_WIN0,125,200,330,45,COLOR_LUCENCY);
        return;
    }
//	DTEXT text_temp;
//	text_temp.flags = ANSI;
//    text_temp.buf = &video_path[strlen((const char *)root_dir)-5];
//	text_temp.len = strlen((const char *)video_path) - strlen((const char *)root_dir)+5;
//	ui_text(Txt_4_3,&text_temp);


//	ui_clear_screen(LAYER_OSD0_WIN0,125,200,330,45,COLOR_BLACK);


//    puts("\n play_status: \n");
//    put_u8hex(play_status);
//	if(play_status == MODE_PLAY)
//	{
//	     ui_clear_controls(TYPE_PIC,Img_4_3);
//	     ui_pic(Img_4_2,-1,-1,USE_ORIG_COLOR);
//	}
//	else if(play_status == MODE_PAUSE)
//	{
//	    ui_clear_controls(TYPE_PIC,Img_4_2);
//	    ui_pic(Img_4_3,-1,-1,USE_ORIG_COLOR);
//	}


	curr_second = time1->hour*3600 + time1->minute*60 + time1->second;
	total_second = time2->hour*3600 + time2->minute*60 + time2->second;
	if(old_play_status == play_status)
	{
        if(old_total_second == total_second && curr_second == old_curr_second)
            return;
	}
	old_play_status = play_status;
    old_total_second = total_second;
    old_curr_second = curr_second;
	percent = curr_second*1.0/total_second;

//	ui_time1(Txt_4_1,time1,TIME_HH_MM_SS,-1,-1,ERASE_DRAW);
//	ui_time1(Txt_4_2,time2,TIME_HH_MM_SS,-1,-1,ERASE_DRAW);
    //ui_time(Time_4_1,time1,-1,-1,SHOW_ALL); //JOHNSON 20160531

//	if(!rect1.width)
//		ui_get_info(TYPE_PIC,Img_4_8,&rect1);
//	ui_clear_screen(LAYER_OSD1_WIN0,rect1.x,rect1.y,rect1.width,rect1.height,USE_ORIG_COLOR);
//
//	ui_pic(Img_4_4,-1,-1,USE_ORIG_COLOR);
//	ui_get_info(TYPE_PIC,Img_4_5,&rect);
//	ui_pic_spec_area(Img_4_5,-1,-1,(u32)rect.width*percent,-1,USE_ORIG_COLOR);
//
//	ui_pic(Img_4_1,-1,-1,USE_ORIG_COLOR);
//	ui_pic(Img_4_7,-1,-1,USE_ORIG_COLOR);
//
//
//
//
//	ui_get_info(TYPE_PIC,Img_4_4,&rect);
//	ui_pic(Img_4_8,rect.x+(u32)(rect.width*percent)-rect1.width/2,-1,USE_ORIG_COLOR);
//	rect1.x = rect.x+(u32)(rect.width*percent)-rect1.width/2;
#endif //DOUBLE_UI_A

}

const char encode_mode_str[][5]=
{
#ifndef DOUBLE_VEDIO
    "1080P",
#endif
    "720P",
    "VGA",
#ifdef DOUBLE_VEDIO
    "VGA",
#endif
    "1M",
    "2M",
    "3M",
    "5M"
};



void disp_file_info(void)
{
#if DOUBLE_UI_A
//    if(is_sd_online() == FALSE || pb_fctrl.no_file == 1)
    if(is_phy_sd_online() == FALSE || pb_fctrl.no_file == 1)
        return;

    updata_curfile_num();
    if(dec_ctrl.dec_status == DEC_APP_ABORT) //异常文件 不清除提示，背景清黑
    {
        ui_clear_controls(TYPE_PIC,Img_1_47);
        ui_clear_controls(TYPE_PIC,Img_4_5);
        ui_clear_controls(TYPE_PIC,Img_4_3);
        ui_clear_screen(LAYER_IMAGE0, 0,0, LCD_W, LCD_H, IMAGE_COLOR_BLACK);
        return;
    }
    else
    {
        if(sys_status.menu_status == 0)
          clear_display_noto();

    }

    play_screen(MODE_ERASE,NULL,NULL);
//    puts("----- \n");
    ui_clear_controls(TYPE_TEXT,Txt_4_3);
	DTEXT text_temp;
	text_temp.flags = ANSI;
    text_temp.buf = &video_path[strlen((const char *)root_dir)-5];
	text_temp.len = strlen((const char *)video_path) - strlen((const char *)root_dir)+5;
	ui_text(Txt_4_3,&text_temp);
    u32 Twidth = 0;
	if(pb_fctrl.ftype == PKG_TYPE_JPG)
    {
        puts("\n PKG_TYPE_JPG \n");
        if(sys_status.menu_status == 0)
            ui_pic(Img_1_11,-1,-1,ERASE_ONLY);
        os_time_dly(10);
        Twidth = get_current_decPic_inf();
        puts("\n decPic_inf: \n");
        put_u32hex(Twidth);
        if(Twidth <=720)
            sval.encode_fmt = 3;
        else if(Twidth <=912)
            sval.encode_fmt = 4;
        else if(Twidth <=1088)
            sval.encode_fmt = 5;
        else
            sval.encode_fmt = 6;

    }
    else
    {
         Twidth = get_current_decfile_inf(1);
        puts("\n PKG_TYPE_AVI \n");
        put_u32hex(Twidth);

#ifndef DOUBLE_VEDIO
        if(Twidth <= 720)
            sval.encode_fmt = 2;
        else if(Twidth <= 1280)
            sval.encode_fmt = 1;
        else
            sval.encode_fmt = 0;
#else
        if(Twidth <= 720)
            sval.encode_fmt = 1;
        else if(Twidth <= 1280)
            sval.encode_fmt = 0;
        else
            sval.encode_fmt = 2;
#endif

        if(sys_status.menu_status == 0 && dec_ctrl.dec_status != DEC_APP_ABORT)
            ui_pic(Img_1_11,-1,-1,USE_ORIG_COLOR);
    }
//    text_temp.buf = encode_mode_str[sval.encode_fmt];
//	text_temp.len = 4;
//	ui_clear_controls(TYPE_TEXT,Txt_4_5);
//    ui_text(Txt_4_5,&text_temp);
    updata_formate_pic(sval.encode_fmt);
//    puts("\n disp_file_info out \n");
    if(dec_file_pro())
        ui_pic(Img_1_47,-1,-1,USE_ORIG_COLOR);  //±£»¤ÊôÐÔ
    else
        ui_clear_controls(TYPE_PIC,Img_1_47);
#endif //DOUBLE_UI_A

}

void clear_file_info(void)
{
    ui_clear_controls(TYPE_PIC,Img_1_47);
    if(is_phy_sd_online() == FALSE)
        return;
//    if((get_current_mode() & TPH_MODE)==0)
    {
        ui_pic(Img_1_11,-1,-1,ERASE_ONLY);
        if((get_current_mode() & TPH_MODE)== 0)
            ui_clear_controls(TYPE_TEXT,Txt_4_3);
        ui_clear_controls(TYPE_TEXT,Txt_4_4);
//        ui_clear_controls(TYPE_TEXT,Txt_4_5);
    }

}

u8 not_open_dsc_flag;
extern u8 not_open_dsc_flag2;
static u8 last_mode_tmp = 0;
IMGLAYER *image0;
IMGLAYER *image1;

//static u8 k_osd0_buf[LCD_W * LCD_H]  ALIGNE(32); //old
//static u8 k_osd1_buf[LCD_W * LCD_H]  ALIGNE(32);
static u8 k_osd0_buf[720 * 576]  ALIGNE(32); //AVOUT JOHNSON 20160530
static u8 k_osd1_buf[720 * 576]  ALIGNE(32); //AVOUT JOHNSON 20160530
void layer_init()
{
    u8 *layer_buf;
    u8 layer_id[]= {Layer_1_1,Layer_1_2,Layer_1_3};
    u8 i;
    LAYER_INFO layer;//½«LAYER¸ÄÎªLAYER_INFO
    u8 x_scale = get_x_scale();
    u8 y_scale = get_y_scale();

    for(i=0; i<sizeof(layer_id); i++)
    {
        if(ui_layer(layer_id[i],&layer))
        {
            layer.x *= x_scale;
            layer.y *= y_scale;
            layer.width *= x_scale;
            layer.height *= y_scale;

            if(layer.type <= LAYER_IMAGE1)
            {
#ifndef N_BOOT_SCR
                image0 = dsc_imb_open_tmp(&disp0_info);
                image1 = dsc_imb_open_tmp(&disp1_info);
                extern void ui_close_imb1();
                ui_close_imb1();
//#endif
//
//#if 1
#else
                //layer_buf = (u8*)malloc(layer.width*layer.height*3/2 * 3);
                if(layer.type == LAYER_IMAGE0)
                {
                    if(disp0_info.dsc_buf){
                        layer_buf = disp0_info.dsc_buf;
                    }else{
                        layer_buf = (u8*)malloc(layer.width*layer.height*3/2 * 3);
                    }


                }
                else
                {
                    if(disp1_info.dsc_buf){
                        layer_buf = disp1_info.dsc_buf;
                    }else{
                        layer_buf = (u8*)malloc(layer.width*layer.height*3/2 * 3);
                    }


                }

                if(layer_buf == NULL)
                {
                    printf("\nimage layer malloc err!");
                    return FALSE;
                }

                IMGLAYER *image = imb_get_image_hdl(layer.type);
                image->enable = TRUE;
                image->alpha = layer.alpha;
                image->x = layer.x;
                image->y = layer.y;
                image->width = layer.width;
                image->height = layer.height;
                image->buff_width = layer.width ;
                image->buff_height = layer.height ;
                image->buf = layer_buf;
                image->buf_num = 3;
                imb_image_cfg(image);

                ui_clear_screen(layer.type,
                                layer.x/x_scale,
                                layer.y/y_scale,
                                layer.width/x_scale,
                                layer.height/y_scale,
                                IMAGE_COLOR_BLACK);
//                                0xFF8080);
#endif
            }
            else
            {
                //u8 type = layer.type - LAYER_OSD0_WIN0;

                if(layer.type == LAYER_OSD0_WIN0){
                    layer_buf = k_osd0_buf;
                }else if(layer.type == LAYER_OSD1_WIN0){
                    layer_buf = k_osd1_buf;
                }else{
                    continue;
                }
//                layer_buf = (u8*)malloc(1*layer.width*layer.height);
                if(layer_buf == NULL)
                {
                    printf("\nosd layer malloc err! ");
                    return FALSE;
                }

                OSDWIN *osd = imb_get_osd_hdl(layer.type);
                osd->enable = TRUE;

                if(layer.type == LAYER_OSD0_WIN0){
                    osd->alpha = 10;
                }else if(layer.type == LAYER_OSD1_WIN0){
                    osd->alpha = layer.alpha;
                }
                //                osd->alpha = 10;//layer.alpha;
                osd->x = layer.x;
                osd->y = layer.y;
                osd->width = layer.width;
                osd->height = layer.height;

                //osd->buf_disp = layer_buf;
                //osd->buf_nodisp = NULL;//&layer_buf[layer.width*layer.height];
                //memset(&layer_buf[layer.width*layer.height],0x00,layer.width*layer.height);
                osd->buf = layer_buf;

                imb_osd_win_cfg(osd);
                ui_clear_screen(layer.type,
                                 layer.x/x_scale,
                                 layer.y/y_scale,
                                 layer.width/x_scale,
                                 layer.height/y_scale,0);
            }
        }
    }
}

extern void font_get_color(OSDCOLOR *color);
extern u8 font_get_layer();

extern void my_putchar(u8 *buf,u16 width,u16 height,u16 x,u16 y);
// 显示相关的 初始化接口调用 LCD imb
//************************************************************
#if 1 //AVOUT JOHNSON 20160530
void config_disp(u8 mode)
{
    u16 width;
    u16 heigh;

    LCD_MODE = mode;
    lcd_init(mode);

    lcd_get_size(&width, &heigh);
    if (mode == LCD_AVOUT)
    {
//        puts("set screen scale****************1\n");
        width = 320;
        heigh = 240;
    }

    dc_set_screen_scale(1,1);

//    if(!font_init(my_putchar,Chinese_Simplified))
//    if(!font_init(my_putchar,Chinese_Traditional))
//put_u16hex(g_language_mode);
//put_u16hex(Chinese_Simplified);

    if(!font_init(my_putchar,g_language_mode))
    {
        printf("\nfont init fail\n");
    }

    if (mode != LCD_AVOUT)
    {
        puts("**************************************************************\n");
        if(!ui_init((u8*)"ui_sty.sty",width, heigh))
        {
            printf("\nui init fail");
            return FALSE;
        }
    }
    else if (mode == LCD_AVOUT)
    {
        if (sys_info.TV_mode == 1)//ntsc
        {
            puts("*******************************************ui_sty_ntsc.sty\n");
            if(!ui_init((u8*)"ui_sty_ntsc.sty",width, heigh))
//            if(!ui_init((u8*)"ui_sty.sty",width, heigh))//test**********
            {
                printf("\nui init fail");
                return FALSE;
            }
        }
        else
            if (sys_info.TV_mode == 0)//pal
            {
                puts("*******************************************ui_sty_pal.sty\n");
                if(!ui_init((u8*)"ui_sty_pal.sty",width, heigh))
                {
                    printf("\nui init fail");
                    return FALSE;
                }
            }
    }

    if(!dc_init((u8*)"menu.res"))
    {
        printf("\ndc init fail");
        return FALSE;
    }
    if (mode == LCD_AVOUT)
    {
        dc_set_screen_scale(2,2);
    }

    layer_init();


	u32 *paltable = (u32*)malloc(256*sizeof(u32));

	if(paltable == NULL){
        puts("\nconfig_disp no buf\n");
        return ;
	}

	dc_osd_palette(1,YUV_TABLE,paltable);
	imb_osd_pal_cfg(OSD0, paltable, 256);
	dc_osd_palette(2,YUV_TABLE,paltable);
	imb_osd_pal_cfg(OSD1,paltable,256);
	free(paltable);
//    os_time_dly(50);
    imb_start();
    printf("\nimb start");
}

#else //old ******************************************************

//void config_disp(void)  //old
void config_disp(u8 mode) //AVOUT JOHNSON 20160530
{
    u16 width;
    u16 heigh;

    //lcd_init();  //old
    lcd_init(mode); //AVOUT JOHNSON 20160530
    printf("\nlcd init succ");

    lcd_get_size(&width, &heigh);

    dc_set_screen_scale(1,1);
//    if(!font_init(my_putchar,Chinese_Simplified))
//    if(!font_init(my_putchar,Chinese_Traditional))
put_u16hex(g_language_mode);
put_u16hex(Chinese_Simplified);

    if(!font_init(my_putchar,g_language_mode))
    {
        printf("\nfont init fail");
//        return FALSE;
    }
//    dc_SetLanguage(g_language_mode);
    printf("\nfont init succ");
    if(!ui_init((u8*)"ui_sty.sty",width, heigh))
    {
        printf("\nui init fail");
        return FALSE;
    }
    printf("\nui init succ");
    if(!dc_init((u8*)"menu.res"))
    {
        printf("\ndc init fail");
        return FALSE;
    }
    printf("\ndc init succ");

    layer_init();
    printf("\nlayer init succ");

	u32 *paltable = (u32*)malloc(256*sizeof(u32));

	if(paltable == NULL){
        puts("\nconfig_disp no buf\n");
        return ;
	}

	dc_osd_palette(1,YUV_TABLE,paltable);
//	printf("\npal 1:");
//	printf_buf(paltable,256*4);
	imb_osd_pal_cfg(OSD0, paltable, 256);
	dc_osd_palette(2,YUV_TABLE,paltable);
//	printf("\npal 2:");
//	printf_buf(paltable,256*4);
	imb_osd_pal_cfg(OSD1,paltable,256);
	free(paltable);
	printf("\npalette config succ");
//    ui_pic(Img_1_6,-1,-1,USE_ORIG_COLOR);
    os_time_dly(50);
//    imb_image_ctl(LAYER_IMAGE0,FALSE);
//    imb_image_ctl(LAYER_IMAGE1,FALSE);
    imb_start();
    printf("\nimb start");
}
#endif //**************************************************

void updata_timer(void *p_arg)
{
    if(get_del_allfile_mark() == 0){
        put_msg_fifo(MSG_ONE_SECOND ,MSG_TO_PROCESS ,UI_DISP_N) ;
    }
}


void disp_sys_time(u8 mark)
{
	static u8 temp_mark = 1;
	read_sys_time(&system_time);//读取系统时间
	ui_time(Time_3_3,(TIME *)&system_time,-1,-1,SHOW_ALL);
    if(sys_status.menu_status == 0 && ((get_current_mode() & DEC_MODE)== 0))
    {
        if(temp_mark)
        {
            temp_mark = 0;
            ui_clear_screen(LAYER_OSD1_WIN0,0,240,150,32,COLOR_LUCENCY);
//            ui_clear_controls(TYPE_TEXT,Txt_3_1);
        }
        else
            ui_time(Time_3_4,(TIME *)&system_time,-1,-1,SHOW_ALL);
//            ui_time1(Txt_3_1,(TIME*) &system_time,TIME_YYYY_MM_DD_HH_MM_SS,-1,-1,SHOW_ALL);
    }
    else
        temp_mark  = 1;
}


void update_battery_pic(void )
{
    static u8 coruscate_status = 0;
    static u8 lowpower_mark = 0 ;
    static u8 lowpower_cnt = 0;
    u8 temp_power;
    if(usb_is_chargeing() == TRUE) //充电
    {
        if(lowpower_mark)
        {
            clear_display_noto();
            lowpower_mark = 0;
            lowpower_cnt = 0;
        }

        if(sval.battery_level != 4 ) //
        {
            ui_clear_controls(TYPE_PIC,battery_icon[sval.battery_level]);
            sval.battery_level = 4;
            ui_pic(battery_icon[sval.battery_level],-1,-1,USE_ORIG_COLOR);
        }

        if (coruscate_status == 0){
            coruscate_status = 1;
            ui_pic(battery_icon[sval.battery_level],-1,-1,USE_ORIG_COLOR);
        }
        else {
            coruscate_status = 0;
            ui_pic(battery_icon[sval.battery_level],-1,-1,ERASE_ONLY);
        }
    }
    else
    {
        temp_power = get_power_level();

        if(temp_power ==0)//超低电 3.5v 直接关机
        {
            put_msg_fifo(MSG_KEY_CLOSE ,MSG_TO_PROCESS ,MODE_SW_N);
            return;
        }
        else
            temp_power -=1;

        if(temp_power == 0xff)
            return;
//        puts("\n power_level: \n");
//        put_u8hex(temp_power);
//      if(sval.battery_level != 3)  // 电量有变化
        if(sval.battery_level != temp_power)  // 电量有变化
        {
            ui_clear_controls(TYPE_PIC,battery_icon[sval.battery_level]);
            sval.battery_level = temp_power;
            ui_pic(battery_icon[sval.battery_level],-1,-1,USE_ORIG_COLOR);
        }
        if(temp_power ==0)//低电
        {

            if(sys_status.menu_status == 1)
                return;
            lowpower_cnt++;
            if(lowpower_mark != 1)
            {
               display_note(M148);
               lowpower_mark = 1;
            }
            else
            {
                clear_display_noto();
                lowpower_mark = 2;
            }
            if(lowpower_cnt >= 10)
            {
                put_msg_fifo(MSG_KEY_CLOSE ,MSG_TO_PROCESS ,MODE_SW_N) ;
            }
        }
        else
        {
            if(lowpower_mark)
            {
                clear_display_noto();
                lowpower_mark = 0;
                lowpower_cnt = 0;
            }
        }
    }


}

 u8 temp_rec_status =0; //for test rec count
void get_remain_time_rec(void)//剩余可录像时间
{
    /**/
    u32 free_space_SDcard = 0;
    u32 remain_time_for_video = 0;
     free_space_SDcard = mute_f_get_free_space('B');
    free_space_SDcard >>= 1; //返回的为空闲的sec数,转成K

    //每秒视频数据估算（往大了算）：720P大约1.5M，480P约800K
    //每秒总数为2.3M，即2400K
    //last_time_for_video *= 60;
    remain_time_for_video = free_space_SDcard /3000;

//    disp_time_count(last_time_for_video,ERASE_BACKGROUND);

    TIME time;
    time.year = 2016;
    time.month = 1;
    time.day = 19;
    time.hour = (remain_time_for_video/3600)%24;
    time.minute = (remain_time_for_video/60)%60;
    time.second = (remain_time_for_video%60);


    ui_time1(Txt_1_2,&time,TIME_HH_MM_SS,-1,-1,ERASE_DRAW);

}

static u32 last_file_num = 0;

void num_of_last_photo(void)
{
#if 0
    u32 free_space_for_photo;
    if(last_file_num == 0)
    {
        free_space_for_photo = mute_f_get_free_space('B');//get_free_space('B');
        free_space_for_photo = (free_space_for_photo >> 1);   //单位BYTE

//        free_space_for_photo /= 1200;   //按照解析度来设置
        free_space_for_photo /= ((sys_info.photo_reso_ratio+1)*400);   //按照解析度来设置
        last_file_num = free_space_for_photo;

    }
    ui_number(Number_1_1, last_file_num, -1, -1, USE_NEW_COLOR);
//    ui_number(Number_3_2,time.month,-1,-1,USE_NEW_COLOR);
#endif
}

void num_of_photo_cutdown(void)
{
#if 0
    if(last_file_num)
        last_file_num--;
    ui_clear_controls(TYPE_NUMBER,Number_1_1);
    num_of_last_photo();
#endif
}
void reset_num_of_photo(void)
{
#if 0
    ui_clear_controls(TYPE_NUMBER,Number_1_1);
    last_file_num = 0;
    num_of_last_photo();
#endif
}
void disp_REC_cnt( u16 seconds,u8 erase_mark) //录像计时
{
    TIME time;
    time.year = 2016;
	time.month = 1;
	time.day = 19;
	time.hour = (seconds/3600)%24;
	time.minute = (seconds/60)%60;
	time.second = (seconds%60);
//    if( temp_rec_status ==0 && seconds ==0)
//        ui_time1(Txt_1_1,&time,TIME_HH_MM_SS,ERASE_ONLY);
//    else if(temp_rec_status == 1)
    ui_time1(Txt_1_1,&time,TIME_MM_SS,-1,-1,erase_mark);


}
#if DOUBLE_UI_A
const u16 usb_opt_str[]= {M136,M137,M173};
const u16 usb_opt_pic[]= {Img_4_6,Img_4_10,Img_4_11};
#endif //DOUBLE_UI_A


void disp_usb_option(u8 select)
{
    u8 i;
    Rect rect;


    for(i =0;i<3;i++)
    {
        if(i == select)
        {
            dc_set_color(COLOR_WHITE,COLOR_LUCENCY);
            ui_get_info(TYPE_PIC,usb_opt_pic[i],&rect);
            ui_pic(Img_4_9,rect.x,rect.y,USE_ORIG_COLOR);
            dc_drawstring(LAYER_OSD1_WIN0,usb_opt_str[i],rect.x,rect.y+(rect.height-get_string_height(usb_opt_str[i]))/2,rect.width,ALIGN_CENTER,0);
            dc_restore_color();
        }
        else
        {
            dc_set_color(COLOR_BLACK,COLOR_LUCENCY);
            ui_get_info(TYPE_PIC,usb_opt_pic[i],&rect);
            ui_pic(usb_opt_pic[i],rect.x,rect.y,USE_ORIG_COLOR);
            dc_drawstring(LAYER_OSD1_WIN0,usb_opt_str[i],rect.x,rect.y+(rect.height-get_string_height(usb_opt_str[i]))/2,rect.width,ALIGN_CENTER,0);
            dc_restore_color();
        }
    }

}

#if LANE_DECT_EN

static u8 ldete_warning_mark =0;
void ldect_warning_remind(void)
{
    static u8 temp = 0;
    if((get_current_mode() & ENC_MODE) ||  (get_current_mode() & TPH_MODE))
    {
        if(ldete_warning_mark)
        {
            if(temp)
            {
                temp = 0;
                ui_pic(Img_1_45,-1,-1,ERASE_ONLY);
                ui_pic(Img_1_46,-1,-1,USE_ORIG_COLOR);
            }
            else
            {
                temp = 1;
                ui_pic(Img_1_46,-1,-1,ERASE_ONLY);
                ui_pic(Img_1_45,-1,-1,USE_ORIG_COLOR);
            }
        }
    }
}
#endif

#if (UART_DEBUG_EN == 0)
void video_running_led()
{
    PORTH_DIR &= ~BIT(14);
    PORTH_OUT ^= BIT(14);
}

void video_running_led_off()
{
    PORTH_DIR &= ~BIT(14);
    PORTH_OUT &= ~BIT(14);
}
#endif

extern void usb_charge_status (void);

u16 get_loop_video_status()
{
    return sys_info.loop_video;
}
extern volatile u8 usb_charg_status;
static timer_handle ui_updata_timer_hd;

void disp_unlock_all_file()
{
    static u8 flag = FALSE;
    if (flag == FALSE)
    {
        flag = TRUE;
        display_note(M146);
    }
    else
    {
        flag = FALSE;
        clear_display_noto();
    }
}

void usb_out_stop_enc(void)
{
    static u8 usb_chargeing_status = 0;
    static u8 usb_out_cnt = 0;

    if (usb_is_chargeing())
    {
        usb_chargeing_status = 1;
        usb_out_status = 0;
        if (usb_out_cnt)
        {
            if(get_current_mode() & USB_MODE)
                return;

                clear_display_noto();
        }

    }
    else
    {
        usb_out_status = 1;
        if (usb_chargeing_status == 1)
        {
            if (usb_out_status == 0)
            {
                usb_out_cnt = 0;
                return ;
            }
            if ((usb_out_cnt == 0) || (usb_out_cnt == 1))
            {
                display_note(M186);
            }


            if (usb_out_status >= 1)
                usb_out_cnt++;
            if (usb_out_cnt >= 5)//10 //拔USB后5s关机 JOHNSON 20160321
            {
                usb_out_status = 0;
                usb_out_cnt = 0;
                put_msg_fifo(MSG_KEY_CLOSE,MSG_TO_PROCESS ,MODE_SW_N) ;
            }
        }
        else
        {
            usb_out_status = 0;
        }
    }
}

#ifdef FLASH_LIGHT  //JOHNSON 20160531
void flash_light_ctrl()
{

  #if 0 //JOHNSON 20160616 error
    if (get_flashlight_mode() == 0)
    {
        printf("\n HL_LED_OPEN 111111111111");
        HL_LED_OPEN(); //HL_LED_CLOSE();
        set_flashlight_mode() == 1;
    }
    else if (get_flashlight_mode() == 1)
    {
        printf("\n HL_LED_CLOSE 2222222222222");
        HL_LED_CLOSE(); //HL_LED_OPEN();
        set_flashlight_mode() == 0;
    }

  #else //old

    if (sys_info.flashlight_mode == 0)  //JOHNSON 20160618
    {
        printf("\n HL_LED_CLOSE 111111111111");
        HL_LED_OPEN();
        sys_info.flashlight_mode = 1;
    }
    else if (sys_info.flashlight_mode == 1)
    {
        printf("\n HL_LED_OPEN 2222222222222");
        HL_LED_CLOSE();
        sys_info.flashlight_mode = 0;
    }
  #endif

    updata_flashlight_mode_pic();
}
#endif

static timer_handle ui_updata_timer_hd;

#if 1 //AVOUT JOHNSON 20160530 ADD
extern IMGLAYER *image0;
extern IMGLAYER *image1;
extern u8 g_dbuf[400 * 240 * 3 / 2 * 3 + SMALL_WIN_WIDTH * SMALL_WIN_HEIGH * 3 / 2 * 3];
#endif
extern bool check_tvout_flag;
void ui_disp_task(void *p_arg)
{
    u32 err;
    u32 msg ;
    msg_t *msg_d;
    msg_devicechange_t *devsta;
    err = dev_reg_msg(NULL);

	TIME time;
	time.year = 2016;
	time.month = 1;
	time.day = 19;
	time.hour = 15;
	time.minute = 19;
	time.second = 0;

//	sval.battery_level = 3;
    sval.battery_level = get_power_level(); //解决开机时电池电量有时显示满问题
	sval.disp_mode = MODE_VIDEO_RECORD;
//	sval.disp_mode = MODE_TAKE_PHOTO;
//	sval.disp_mode = MODE_REPLAY;
	sval.encode_fmt = sys_info.reso_ratio;
//	sval.sd_online = TRUE;
//	sval.sd_online = is_sd_online();
	sval.sd_online = is_phy_sd_online();
	sval.lock = 0;
	sval.time = &time;
	sval.update_flag = UPDATE_TIME;


	u8 status;
	u8 finish_flag;
	TIME time1,time2;
	time1.hour   = 0;
	time1.minute = 0;
	time1.second = 0;
	time2.hour   = 0;
	time2.minute = 3;
	time2.second = 38;

	status = MODE_PLAY;
	finish_flag = FALSE;

//    os_time_dly(50);
    sval.update_flag = UPDATE_TIME|DISP_MASK;

//	MPARAM mvar;
    MENU mvar;
    mvar.array = menu_array0;
    mvar.listcnt =3;
    mvar.timeout = 10;
    mvar.act_color.forecolor = COLOR_BLUE;
    mvar.act_color.backcolor = COLOR_LUCENCY;
    mvar.unact_color.forecolor = COLOR_WHITE;
    mvar.unact_color.backcolor = COLOR_LUCENCY;
#if DOUBLE_UI_A
    mvar.effect_en = FALSE;
    mvar.item_vertical_center = FALSE;
    mvar.show_title = TRUE;
#endif //DOUBLE_UI_A
//	mvar.head_activepic = PNG3;

//	mvar.titlepic1 = PNG42;
    mvar.callback = draw_confirm_dialog;

    if(check_tvout_flag){   //xjg add  AVOUT  ui位置的修改
        mvar.item_point_x[0] = 30;
        mvar.item_point_x[1] = 70;
        mvar.item_point_x[2] = 310;//435; //菜单栏右边选中框位置 JOHNSON 20160531
        mvar.item_point_x[3] = M32; //ÓÃÓÚÅÐ¶ÏÊÇ¿ª¹ØÏî»¹ÊÇÆäËû
        mvar.item_point_x[4] = 320;//445; //菜单栏右边选中框位置 JOHNSON 20160531
        mvar.item_point_x[5] = NEXT;
    }else{
        mvar.item_point_x[0] = 10;
        mvar.item_point_x[1] = 50;
        mvar.item_point_x[2] = 360;//360; //菜单栏右边选中框位置 JOHNSON 20160531
        mvar.item_point_x[3] = M32; //ÓÃÓÚÅÐ¶ÏÊÇ¿ª¹ØÏî»¹ÊÇÆäËû
        mvar.item_point_x[4] = 370;//370; //菜单栏右边选中框位置 JOHNSON 20160531
        mvar.item_point_x[5] = NEXT;
    }

    mvar.item_point_x[6] = M107; //°æ±¾ºÅ²Ëµ¥Ïî±êÊ¶
    mvar.item_point_x[7] = M13; //°æ±¾ºÅ²Ëµ¥
    mvar.item_point_x[8] = CLEAR; //°æ±¾ºÅ²Ëµ¥
    mvar.item_point_x[9] = CLOSE1;
//	ui_menu(Menu_2_1,&mvar,MENU_CMD_INIT);
    printf("\ngui menu...");

    while(!g_startup_finish)
    {
        os_time_dly(2);
    }
    sval.battery_level = get_power_level();//解决开机时电池电量有时显示满问题
	main_frame(&sval);
	printf("\n main frame...");


    ui_updata_timer_hd = timer_reg(500 , updata_timer , NULL); //1秒
    u8 usb_select_item = 0;
    static u8 rec_clear_mark = 0;
    while(1)
    {
        msg = get_msg();
//        puts("\n ui_task \n");
//        printf("\nmsg:%x",msg);
        if(msg  > (u32)0x2000000)
        {
            msg_d = msg ;
            msg = msg_d->type ;
        }
//        os_time_dly(1000);

        switch(msg)
        {

            case MSG_RW_FILE_ERR:
                {
                    if (get_menu_statu())
                        break;

                        display_note(M185);
                }
                break;

//            #ifdef GUNTHER
//            case MSG_KEY_OK_3S:
//                //提示 正在查询受保护文件，请等待
//                disp_unlock_all_file();
//                break;
//            #endif
			case MSG_KEY_PREV:
                printf("\nMSG KEY PREV");
                if(mode_ctrl.cur_mode & USB_MODE) //USB模式选择
                {
                    if(!usb_ctrl.enter_usb)
                        break;
                    if(usb_select_item)
                        usb_select_item--;
                    else
                        usb_select_item = 2;
                    disp_usb_option(usb_select_item);
                    break;
                }
//                if(mode_ctrl.cur_mode& DEC_MODE && sys_status.menu_status == 0) //回放文件切换
//                {
//                    os_time_dly(10);//等待解码完成
//                    disp_file_info();
//                    break;
//                }
#if DOUBLE_UI_A
				ui_menu(Menu_2_1,&mvar,MENU_CMD_PREV); //菜单选择
#endif //DOUBLE_UI_A

				break;

			case MSG_KEY_NEXT:
                printf("\nMSG KEY NEXT");
                if(mode_ctrl.cur_mode & USB_MODE) //USB模式选择
                {
                    if(!usb_ctrl.enter_usb)
                        break;
                    usb_select_item++;
                    if(usb_select_item > 2)
                        usb_select_item = 0;

                    disp_usb_option(usb_select_item);
                    break;
                }
//                if(mode_ctrl.cur_mode& DEC_MODE && sys_status.menu_status == 0) //回放文件切换
//                {
//                    os_time_dly(10);//等待解码完成
//                    disp_file_info();
//                    break;
//                }
#if DOUBLE_UI_A
				ui_menu(Menu_2_1,&mvar,MENU_CMD_NEXT); //菜单选择
#endif //DOUBLE_UI_A

				break;

			case MSG_KEY_MENU:
                printf("\nMSG KEY MENU");
#if DOUBLE_UI_A
//                printf("\n sys_status.menu_status %d ;\n",sys_status.menu_status);
//                printf("\n ui_get_node %d ,%d;\n",get_counter(),ui_get_node());
                if(sys_status.menu_status == 0)
                {
                    sys_status.menu_status = 1;

                    if(sval.disp_mode == MODE_VIDEO_RECORD)
                    {
                        mvar.listcnt = 1;
                        mvar.array = menu_array0;
                    }
                    else if(sval.disp_mode == MODE_TAKE_PHOTO)
                    {
                        mvar.listcnt = 1;
                        mvar.array = menu_array1;
                    }
                    else if(sval.disp_mode == MODE_REPLAY)
                    {
//                        if(is_sd_online() == FALSE || pb_fctrl.no_file == 1)
                        if(is_phy_sd_online() == FALSE || pb_fctrl.no_file == 1)
                        {
                            sys_status.menu_status = 0;
                            break;
                        }
                        play_screen(MODE_ERASE,&time1,&time2);
                        mvar.listcnt = 1;
                        mvar.array = menu_array2;
//                        if(PKG_TYPE_AVI == pb_fctrl.ftype)
//                        {
//                            if(pb_fctrl.file_protect)
//                            {
//                                mvar.array = menu_array2_unlock;
//                            }
//                            else
//                            {
//                                mvar.array = menu_array2_lock;
//                            }
//                        }
//                        else
//                        {
//                            mvar.array = menu_array2;
//                        }
                    }
                    clear_display_noto();
                    ui_pic(Img_1_49,-1,-1,ERASE_DRAW);
                    ui_menu(Menu_2_1,&mvar,MENU_CMD_INIT);
//                    ui_pic(Img_1_2,-1,-1,USE_ORIG_COLOR);
                }
                else
                {
                    ui_menu(Menu_2_1,&mvar,MENU_CMD_SWITCH);
                    os_time_dly(10);
                    printf("\n get_select_item %d ,%d;\n",get_select_item(),get_counter());
                    if(ui_get_active_menu() == mvar.listcnt )
                    {
                        sys_status.menu_status = 0;
                        vm_api_write(SYS_SET_INFO,&sys_info); //写入参数

//                        ui_pic(Img_1_3,-1,-1,ERASE_ONLY);
                        ui_clear_screen(LAYER_OSD0_WIN0,0,48,80,100,COLOR_LUCENCY);//侧边栏
                        ui_clear_screen(LAYER_OSD1_WIN0,0,48,120,100,COLOR_LUCENCY);//侧边栏
//                        ui_pic(Img_1_2,-1,-1,USE_ORIG_COLOR);

                        if(sval.disp_mode == MODE_REPLAY)
                        {
                            ui_pic(Img_1_49,-1,-1,ERASE_ONLY);
//                            if(is_sd_online() == FALSE || pb_fctrl.no_file == 1)
                            if(is_phy_sd_online() == FALSE || pb_fctrl.no_file == 1)
                            {
                                if(is_phy_sd_online() == FALSE)
                                    display_note(M152);
                                else
                                    display_note(M149);
                                ui_clear_controls(TYPE_TEXT,Txt_4_3);
                                ui_clear_controls(TYPE_TEXT,Txt_4_4);
//                                ui_clear_controls(TYPE_TEXT,Txt_4_5);
                            }
                            else
                            {
                                disp_file_info();
                                if(dec_ctrl.dec_status == DEC_APP_READY && dec_ctrl.dec_type != PKG_TYPE_JPG )
                                {
                                    ui_pic(Img_1_11,-1,-1,USE_ORIG_COLOR);
                                }
                                else if(dec_ctrl.dec_status == DEC_APP_PAUSE && dec_ctrl.dec_type != PKG_TYPE_JPG )
                                {
                                    status =  MODE_PAUSE;
                                    play_screen(status,&time1,&time2);
                                }
                                if(dec_ctrl.dec_type == PKG_TYPE_AVI)
                                {
                                    ui_pic(Img_4_15, -1, -1, ERASE_ONLY);
                                    ui_pic(Img_4_16, -1, -1, USE_ORIG_COLOR);
                                }
                                 else if(dec_ctrl.dec_type == PKG_TYPE_JPG)
                                {
                                    ui_pic(Img_4_16, -1, -1, ERASE_ONLY);
                                    ui_pic(Img_4_15, -1, -1, USE_ORIG_COLOR);
                                }
                            }
                        }
                        else if(sval.disp_mode == MODE_VIDEO_RECORD)
                        {
                            disp_sys_time(1);
                            ui_pic(Img_1_49,-1,-1,ERASE_ONLY);
                            ui_pic(Img_1_17,-1,-1,USE_ORIG_COLOR);
                        }
                        else if(sval.disp_mode == MODE_TAKE_PHOTO)
                        {
                            disp_sys_time(1);
                            ui_pic(Img_1_49,-1,-1,ERASE_ONLY);
                            ui_pic(Img_1_16,-1,-1,USE_ORIG_COLOR);
                        }
                    }
                }
#endif //DOUBLE_UI_A

				break;

			case MSG_KEY_OK:
                printf("\nMSG KEY OK");
#if DOUBLE_UI_A
                if(mode_ctrl.cur_mode & USB_MODE )
                {
                    if(!usb_ctrl.enter_usb)
                        break;
                    if(usb_select_item ==2 )
                    {
                        ui_clear_screen(LAYER_OSD1_WIN0,0,0,LCD_W,LCD_H,COLOR_LUCENCY);
                        put_msg_fifo(MSG_USB_FLAG ,MSG_TO_PROCESS ,USB_TASK_N);
                    }
                    else
                    {
                        usb_ctrl.usb_class = usb_select_item + 1;
                        put_msg_fifo(MSG_MODE_SW ,MSG_TO_PROCESS ,MODE_SW_N) ;
                    }
                    break;
                }
                if(sys_status.menu_status == 0)
                {
                     if(get_current_mode() & ENC_MODE)
                     {

                     }
                     else if(get_current_mode() & TPH_MODE)
                     {
                        //¸üÐÂÊ£ÓàÅÄÕÕÊ£ÓàÊýÄ¿ ??£»
                     }
                     else if(get_current_mode() & DEC_MODE)
                     {
                          if(pb_fctrl.ftype == PKG_TYPE_JPG)
                            break;
                         os_time_dly(10);
                         if(dec_ctrl.dec_status == DEC_APP_RUNNING)
                         {
                             ui_pic(Img_1_11,-1,-1,ERASE_ONLY);
                             status =  MODE_PLAY;
                             time1 = get_flie_current_palytime();
                             time2 = get_fieltotal_time();
                            play_screen(status,&time1,&time2);
                            puts("\n MODE_PLAY \n");
                         }
                         else
                         {
                             if(dec_ctrl.dec_status == DEC_APP_PAUSE)
                             {
                                status =  MODE_PAUSE;
                                time1 = get_flie_current_palytime();
                                time2 = get_fieltotal_time();
                                play_screen(status,&time1,&time2);
                                puts("\n MODE_PAUSE \n");
                             }
                         }
                     }

                }
                else
                    ui_menu(Menu_2_1,&mvar,MENU_CMD_CONFIRM);
#endif //DOUBLE_UI_A

				break;
			case MSG_KEY_MODE:
                printf("\nMSG KEY MODE");
#if DOUBLE_UI_A
                if(sys_status.menu_status==0)
                {
                    puts("\n set get_current_mode \n");
                    put_u8hex(get_current_mode());
                    if(get_current_mode() & ENC_MODE)
                    {
                        ui_pic(Img_1_28,-1,-1,ERASE_ONLY);
//                        if(pb_fctrl.no_file == 1)
                        clear_display_noto();
                        ui_clear_controls(TYPE_TEXT,Txt_4_4);
                        play_screen(MODE_ERASE,&time1,&time2);
                        clear_file_info();
                        sval.disp_mode = MODE_VIDEO_RECORD;
                        sval.encode_fmt = sys_info.reso_ratio;
                        if(enc_ctrl.enc0_status != ENC_APP_RUNNING)
                            get_remain_time_rec();
                        disp_sys_time(1);
                    }
                    else if(get_current_mode() & TPH_MODE)
                    {
                        ui_pic(Img_1_12,-1,-1,ERASE_ONLY);
                        ui_pic(Img_1_26,-1,-1,ERASE_ONLY);
                        ui_pic(Img_1_46,-1,-1,ERASE_ONLY);
                        ui_pic(Img_1_48,-1,-1,ERASE_ONLY);
                        clear_display_noto();
                        clear_file_info();
                        sval.disp_mode = MODE_TAKE_PHOTO;
                        sval.encode_fmt = sys_info.photo_reso_ratio +3;
                        disp_REC_cnt(0,ERASE_ONLY);//²Á³ýÊý×ÖÇø
//                        num_of_last_photo();
                        reset_num_of_photo();
                    }
                    else if(get_current_mode() & DEC_MODE)
                    {
                        ui_pic(Img_1_28, -1, -1, ERASE_ONLY);
                        ui_pic(Img_1_9,-1,-1,ERASE_ONLY);
                        ui_clear_screen(LAYER_OSD1_WIN0,0,240,250,32,COLOR_LUCENCY);
                        if(is_phy_sd_online() == FALSE || pb_fctrl.no_file == 1)
                        {
                            puts("\n is_sd_online \n");
                            if(is_phy_sd_online() == FALSE)
                                display_note(M152);
                            else
                                display_note(M149);
                            sval.encode_fmt = 1;
                        }
                        else
                        {
                            os_time_dly(10);
                        }
                        sval.disp_mode = MODE_REPLAY;
//                        disp_file_info();
                        disp_REC_cnt(0,ERASE_ONLY);//擦除数字区
                    }
                    else
                    {
                        puts("\n---------222-----\n");

                    }
                    sval.sd_online = is_phy_sd_online();
                    main_frame(&sval);
                }
                else
                {
                     put_msg_fifo(MSG_MENU_EXIT ,MSG_TO_PROCESS ,UI_DISP_N);
                }
#endif //DOUBLE_UI_A

				break;
            case MSG_MENU_EXIT:
#if DOUBLE_UI_A
                if(sys_status.menu_status ==1 )
                {
                    puts("\n MSG_MENU_EXIT \n");
                    ui_menu(Menu_2_1,&mvar,MENU_CMD_EXIT);
                    sys_status.menu_status = 0;
                    ui_pic(Img_1_3,-1,-1,ERASE_ONLY);
                    ui_clear_screen(LAYER_OSD0_WIN0,0,48,80,100,COLOR_LUCENCY);//侧边栏
                    ui_clear_screen(LAYER_OSD1_WIN0,0,48,120,100,COLOR_LUCENCY);//侧边栏
//                    ui_pic(Img_1_2,-1,-1,USE_ORIG_COLOR);
                    if(sval.disp_mode == MODE_REPLAY)
                    {
                        if(is_phy_sd_online() == FALSE || pb_fctrl.no_file == 1)
                        {
                            if(is_phy_sd_online() == FALSE)
                                display_note(M152);
                            else
                                display_note(M149);
                            ui_clear_controls(TYPE_TEXT,Txt_4_3);
//                            ui_clear_controls(TYPE_TEXT,Txt_4_5);
                            ui_clear_controls(TYPE_TEXT,Txt_4_4);
                            ui_clear_screen(LAYER_IMAGE0, 0,0, LCD_W, LCD_H, IMAGE_COLOR_BLACK);
                        }
                        else
                        {
                            disp_file_info();

                            if(dec_ctrl.dec_status == DEC_APP_READY && dec_ctrl.dec_type != PKG_TYPE_JPG )
                            {
                                ui_pic(Img_1_11,-1,-1,USE_ORIG_COLOR);
                            }
                            else if(dec_ctrl.dec_status == DEC_APP_PAUSE && dec_ctrl.dec_type != PKG_TYPE_JPG )
                            {
                                status =  MODE_PAUSE;
                                play_screen(status,&time1,&time2);
                            }

                            if(dec_ctrl.dec_type == PKG_TYPE_AVI)
                            {
                                ui_pic(Img_4_15, -1, -1, ERASE_ONLY);
                                ui_pic(Img_4_16, -1, -1, USE_ORIG_COLOR);
                            }
                             else if(dec_ctrl.dec_type == PKG_TYPE_JPG)
                            {
                                ui_pic(Img_4_16, -1, -1, ERASE_ONLY);
                                ui_pic(Img_4_15, -1, -1, USE_ORIG_COLOR);
                            }
                        }
                    }

                sval.sd_online = is_phy_sd_online();
                if (mode_ctrl.cur_mode & ENC_MODE)
                    sval.encode_fmt = sys_info.reso_ratio;
                else if (mode_ctrl.cur_mode & TPH_MODE)
                    sval.encode_fmt = sys_info.photo_reso_ratio+3;
                main_frame(&sval);
                }
#endif //DOUBLE_UI_A

                break;
            case MSG_DEC_INFORMATION:
                if(get_keystatus() == 0)
                    disp_file_info();
                break;

           case MSG_KEY_OK_3S:
                              //提示 正在查询受保护文件，请等待
                    {

                    Rect rect;
                    key_disable(1);
//                    ui_get_info(TYPE_MENU,Menu_1_1,&rect);
//                    ui_clear_screen(rect.layer,rect.x,rect.y,rect.width,rect.height+4,COLOR_LUCENCY);
                    #if (DOUBLE_UI_A == 1)  //SAN
//                    ui_get_info(TYPE_PIC,Img_2_29,&rect); //san 160302
//                    ui_clear_screen(rect.layer,rect.x,rect.y,rect.width,rect.height,COLOR_BLUE);
//                    dc_drawstring(rect.layer,M181,rect.x,rect.y+(rect.height-get_string_height(M181))/2,rect.width,ALIGN_CENTER,0);
                      //display_note(M182); //去掉在录像过程中按模式键会出现正在解锁全部文件弹窗 JOHNSON 20160531
                    #endif

//                    dc_restore_color();
                    os_time_dly(10);

                    user_define.unlock_all_flag = 1;
                    }
                break;

            case MSG_ONE_SECOND:

                if(get_current_mode() & TPH_MODE)
                {
                    user_isp_cfg();    //检测更新isp配置
                }

                usb_out_stop_enc();

                #if (UART_DEBUG_EN == 0)
                if ((get_current_mode() & ENC_MODE) == 0)
                {
                    video_running_led_off();//非录像模式关闭 录像状态led  20151202
                }
                #endif

#if DOUBLE_UI_A
                if(get_current_mode() & ENC_MODE)
                {
                    usb_charging_status();  //20160313 san
                    usb_off_stop_video_cnt();  //20160313 san
                    user_isp_cfg();    //检测更新isp配置
//                    put_u16hex(enc_ctrl.enc0_status);
//                    put_u32hex(rec_cnt_seconds);
                    if(enc_ctrl.enc0_status != ENC_APP_RUNNING && (rec_cnt_seconds != 0 || rec_clear_mark ==1 ))
                    {
                        rec_clear_mark = 0;
                        if((mode_ctrl.cur_mode & USB_MODE) == 0)
                        {
                            vediomode_icon_switch(0);
                            rec_cnt_seconds = 0;
                            disp_REC_cnt(rec_cnt_seconds,ERASE_ONLY);
                            get_remain_time_rec();
                        //    if(sval.lock)
                            {
                                sval.lock=0;
                                ui_clear_controls(TYPE_PIC,Img_1_47);
                            }
                        }

                    }
                    else if(enc_ctrl.enc0_status == ENC_APP_RUNNING)
                    {
                        if(get_parking_status()== 0)  //非倒车模式
                            vediomode_icon_switch(1);
                        if(sval.lock != enc_ctrl.lock_current_file && get_parking_status()== 0) //变换锁图标
                        {
                            puts("\n lock_current_file pic \n ");
                            sval.lock = enc_ctrl.lock_current_file;
                            if(enc_ctrl.lock_current_file)
//                                ui_pic(Img_1_47,-1,-1,USE_NEW_COLOR);  //保护属性
                            {
                                if(user_define.total_protect_size >ALLOCATE_30_PER_FOR_PROTECT)
                                {
                                    if(user_define.note_protect_full < 2)
                                    {
                                        ui_pic(Img_1_47,-1,-1,USE_ORIG_COLOR);  //保护属性
    //                                    if(rec_cnt_seconds >= (get_cycle_time()>5))//如果发生保护的时间大于5/6，则下一个文件也保护

                                        if(rec_cnt_seconds >= (get_cycle_time()-10))//如果发生保护的时间大于5/6，则下一个文件也保护
                                        {
                                            user_define.protect_next_file = 1;
    //                                        puts("###########################################################\n");
                                        }
                                    }
                                    else if(user_define.note_protect_full ==2)
                                    {
                                        enc_ctrl.lock_current_file = 0;//不再保护文件
                                        sval.lock = 0;
                                        display_note(M184);
    //                                    puts("no longer protect file \n");
                                    }

                                    if(user_define.note_protect_full == 0)
                                    {
                                        display_note(M183);
                                        user_define.note_protect_full = 1;
    //                                    puts("protect file 30%  \n");
                                    }
                                    else if((user_define.note_protect_full == 1) && (user_define.total_protect_size >ALLOCATE_40_PER_FOR_PROTECT))
                                    {
                                        display_note(M183);
                                        user_define.note_protect_full = 2;
    //                                    puts("protect file 40%  \n");
                                    }
                                }
                                else
                                {
                                    ui_pic(Img_1_47,-1,-1,USE_ORIG_COLOR);  //保护属性
                                    if(rec_cnt_seconds >= (get_cycle_time()-10))//如果发生保护的时间大于5/6，则下一个文件也保护
                                    {
                                        user_define.protect_next_file = 1;
    //                                    puts("###########################################################\n");
                                    }
                                }
                            }
                            else
                            {
//                                clear_display_noto();
                                ui_clear_controls(TYPE_PIC,Img_1_47);
//                                ui_clear_controls(TYPE_PIC,Img_1_50);
//                                ui_clear_controls(TYPE_PIC,Img_1_40);
//                                ui_clear_controls(TYPE_PIC,Img_1_54);
   //                         disp_local_backgrounp(TYPE_PIC, Img_1_1, TYPE_PIC, Img_1_54,  USE_ORIG_COLOR);
                            }

                        }

                        if(get_parking_status()== 0)  //非倒车模式
                            disp_REC_cnt(rec_cnt_seconds,SHOW_ALL);
//                            disp_REC_cnt(rec_cnt_seconds,ERASE_DRAW);
                        rec_cnt_seconds++;
                        if(sys_status.sys_gsensor_wakeup == 1)  //停车守卫 默认录像30S
                        {
                            if(rec_cnt_seconds >= 30)
                            {
 //                                put_msg_fifo(MSG_KEY_OK ,MSG_TO_PROCESS ,MODE_SW_N) ;
                                if (usb_is_chargeing())
                                {
                                    sys_status.sys_gsensor_wakeup = 0;
                                    rec_cnt_seconds = 0;
                                    rec_clear_mark = 1;
                                    put_msg_fifo(MSG_ENC_SAVE_FILE ,MSG_TO_PROCESS ,ENC_TASK_N);
                                }
                                else
                                {
                                    put_msg_fifo(MSG_KEY_OK ,MSG_TO_PROCESS ,MODE_SW_N);
                                }
                            }


//                            put_msg_fifo(MSG_KEY_OK ,MSG_TO_PROCESS ,MODE_SW_N);
                        }
                        else
                        {
    //                        if(  enc_ctrl.is_cycle_rec0 !=0 && rec_cnt_seconds == enc_ctrl.is_cycle_rec0*60) //循环录影
                        if ((get_usb_charg_status() ==2) && (usb_is_chargeing() == 0))
                        {
                            if((rec_cnt_seconds >= sys_info.loop_video*60) || (rec_cnt_seconds >= 60*3))
                            {//拔掉usb之后，在10s种以内，如果刚好录好一个1min,2min或者3min的视频，就去停止录像，并关机
//                                puts("\n gggggggggggggggg 3\n");
                                rec_cnt_seconds = 0;
                                rec_clear_mark = 1;
                                usb_charg_off_stop_video();
                            }
                        }
                        else
                        {
                            if(sys_info.loop_video)
                            {
                                if(rec_cnt_seconds >= sys_info.loop_video*60)
                                {
                                    rec_cnt_seconds = 0;
                                    rec_clear_mark = 1;
                                    put_msg_fifo(MSG_ENC_SAVE_FILE ,MSG_TO_PROCESS ,ENC_TASK_N) ;
                                }
                            }
                            else
                            {
                                if(rec_cnt_seconds >= 60*3)//3分钟
                                {
                                    rec_cnt_seconds = 0;
                                    rec_clear_mark = 1;
    //                                put_msg_fifo(MSG_ENC_SAVE_FILE ,MSG_TO_PROCESS ,ENC_TASK_N);
//                                    put_msg_fifo(MSG_KEY_OK ,MSG_TO_PROCESS ,MODE_SW_N);
                                    if (usb_is_chargeing())
                                    {
                                        put_msg_fifo(MSG_ENC_SAVE_FILE ,MSG_TO_PROCESS ,ENC_TASK_N);
                                    }
                                    else
                                    {
                                        put_msg_fifo(MSG_KEY_OK ,MSG_TO_PROCESS ,MODE_SW_N);
                                    }
                                }
                            }
                        }
                    }
                      #if (UART_DEBUG_EN == 0)
                      video_running_led();//录像状态灯20151202
                      #endif

                    }


                if((is_sd_online() == TRUE) && (get_menu_statu()==0) && ((get_current_mode() & USB_MODE) == 0)
                   && user_define.unlock_all_flag == 0)//需要放在上面判断user_define.unlock_all_flag == 0
                {
//                    printf("ALLOCATE_30_PER_FOR_PROTECT = %d\n",ALLOCATE_30_PER_FOR_PROTECT);
//                    printf("user_define.protect_file_num = %d\n",user_define.protect_file_num);
//                    printf("user_define.protect_file_num1 = %d\n",user_define.protect_file_num1);
//                    printf("user_define.note_protect_full = %d\n",user_define.note_protect_full);
                    if(user_define.note_protect_full == 0)
                    {
                        Rect rect;
                        ui_get_info(TYPE_PIC,Img_1_50,&rect);   //160302 san
                        ui_clear_screen(LAYER_OSD0_WIN0,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);  //SAN
                        ui_clear_screen(LAYER_OSD1_WIN0,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);  //SAN
                        ui_get_info(TYPE_PIC,Img_1_40,&rect);   //160302 san
                        ui_clear_screen(LAYER_OSD0_WIN0,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);  //SAN
                        ui_clear_screen(LAYER_OSD1_WIN0,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);  //SAN
                    }
                    else if(user_define.note_protect_full == 1)///Burgess_151210
                    {
                        screen_on();
                        display_note(M183);
                    }
                    else if(user_define.note_protect_full == 2)
                    {
                        screen_on();
                        display_note(M184);
                    }
                }

                //发消息解锁全部文件
                if((user_define.unlock_all_flag == 1)&&(enc_ctrl.enc0_status != ENC_APP_RUNNING)&& (rec_cnt_seconds == 0))
                {
                    user_define.unlock_all_flag = 0;
                    puts("***********************4\n");
                    puts("***********************4\n");
                    put_msg_fifo(MSG_KEY_OK_3S ,MSG_TO_PROCESS ,DEC_TASK_N);
                }
                if(user_define.unlock_all_flag == 2)//解锁完成，清除提示
                {
                    user_define.unlock_all_flag = 0;
                    key_disable(0);

                    Rect rect;
                    ui_get_info(TYPE_PIC,Img_1_50,&rect);   //160302 san
                    ui_clear_screen(LAYER_OSD0_WIN0,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);  //SAN
                    ui_clear_screen(LAYER_OSD1_WIN0,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);  //SAN
                    ui_get_info(TYPE_PIC,Img_1_40,&rect);   //160302 san
                    ui_clear_screen(LAYER_OSD0_WIN0,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);  //SAN
                    ui_clear_screen(LAYER_OSD1_WIN0,rect.x,rect.y,rect.width,rect.height,COLOR_LUCENCY);  //SAN
                }

                }
                if(get_current_mode() & DEC_MODE )
                {
                    static u8 temp_mark = 0;
                    if(pb_fctrl.ftype != PKG_TYPE_JPG)
                    {
                        if(dec_ctrl.dec_status == DEC_APP_RUNNING ) //for test
                        {
                            time1 = get_flie_current_palytime();
                            play_screen(status,&time1,&time2);

                        }
                        else if( dec_ctrl.dec_status == DEC_APP_READY )
                        {
                            if(sys_status.menu_status == 0)
                            {
    //                            play_screen(MODE_ERASE,&time1,&time2);
    //                            if(is_sd_online())
    //                                ui_pic(Img_1_11,-1,-1,USE_ORIG_COLOR);
                            }
                        }
                        static temp_m = 0;
                        if(dec_ctrl.dec_status == DEC_APP_ABORT && is_sd_online() == FALSE )//异常状态
                        {
                            if(temp_m == 0)
                            {
                                temp_m =1;
                                ui_clear_screen(LAYER_IMAGE0, 0,0, LCD_W, LCD_H, IMAGE_COLOR_BLACK);
                                play_screen(MODE_ERASE,&time1,&time2);
                                if(is_sd_online() == FALSE)
                                {
                                    ui_clear_controls(TYPE_TEXT,Txt_4_3);
//                                    ui_clear_controls(TYPE_TEXT,Txt_4_5);
                                    display_note(M149);
                                }
                            }
                        }
                        else
                            {
                                if(temp_m)
                                    temp_m = 0;
                            }
                    }
                }
                if((mode_ctrl.cur_mode & USB_MODE) == 0 && get_parking_status() == 0){
                   disp_sys_time(1);
                   update_battery_pic();
                }
                clear_SD_note();
                auto_power_off_ctrl();
                screen_protect_control();
#endif //DOUBLE_UI_A
#if LANE_DECT_EN
                ldect_warning_remind();
#endif

				break;
			case MSG_USB_MENU:
                if(get_parking_status()) //倒车过程 连接电脑-置为充电模式
                {
                    put_msg_fifo(MSG_USB_FLAG ,MSG_TO_PROCESS ,USB_TASK_N);
                    break;
                }
                puts("\n MSG_USB_MENU \n");

                ui_clear_controls(TYPE_PIC,Img_1_1);//清除USB模式选择界面上方的黑条 JOHNSON 20160530 add

                ui_clear_screen(LAYER_OSD1_WIN0,0,0,LCD_W,LCD_H,COLOR_BLACK);
                usb_select_item = 0;
                disp_usb_option(usb_select_item);
                break;
            case  MSG_USB_MODE:

                ui_clear_screen(LAYER_OSD1_WIN0,0,0,LCD_W,LCD_H,COLOR_BLACK);
                if(usb_select_item)
                {
#if DEBUG_LCD
                        ui_clear_screen(LAYER_OSD1_WIN0,0,0,LCD_W,LCD_H,COLOR_LUCENCY);
                        ui_clear_screen(LAYER_OSD0_WIN0,0,0,LCD_W,LCD_H,COLOR_LUCENCY);
#else
                        ui_pic(Img_4_13,-1,-1,USE_ORIG_COLOR);
#endif
                }
                else
                    ui_pic(Img_4_12,-1,-1,USE_ORIG_COLOR);
                break;
            case MSG_PARKING_SWITCH:
                if(get_keystatus())
                {
                    ui_clear_screen(LAYER_OSD0_WIN0,0,0,LCD_W,LCD_H,COLOR_LUCENCY);
                    ui_clear_screen(LAYER_OSD1_WIN0,0,0,LCD_W,LCD_H,COLOR_LUCENCY);
                    ui_pic(Img_3_1,-1,-1,USE_ORIG_COLOR);
                }
                else
                   ui_clear_controls(TYPE_PIC,Img_3_1);

                break;

#if LANE_DECT_EN
            case MSG_LDETE_WARNING:
                if((get_current_mode() & ENC_MODE) ||  (get_current_mode() & TPH_MODE))
                {
                    if(ldete_warning_mark) //关闭警报提示
                    {
                        ldete_warning_mark = 0;
                        ui_pic(Img_1_45,-1,-1,ERASE_ONLY);
                        ui_pic(Img_1_46,-1,-1,USE_ORIG_COLOR);
                    }
                    else//打开警报提示
                    {
                        ldete_warning_mark = 1;
                        ui_pic(Img_1_46,-1,-1,ERASE_ONLY);
                        ui_pic(Img_1_45,-1,-1,USE_ORIG_COLOR);
                    }
                }
                break;
#endif

            case MSG_DEVICECHANGE:
//                puts("\n ui task MSG_DEVICECHANGE \n");
#if DOUBLE_UI_A
                devsta = msg_d->ptr;
                if(devsta->hdev->dev_io->device_type == DEV_LOGIC_DISK)
                {
                    if(devsta->sta ==DEV_ONLINE )
                    {
                        puts("----DEV_ONLINE \n");

                        if((mode_ctrl.cur_mode & USB_MODE) == 0 && (get_parking_status()== 0))
                        {
                            if(get_menu_statu()== 0)
                                clear_display_noto();
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
                            else if(get_current_mode() & DEC_MODE)
                            {
    //                            ui_clear_screen(LAYER_OSD1_WIN0, 150, 50, 244, 150, COLOR_LUCENCY);
                                if(get_menu_statu()==0)
                                    clear_display_noto();
                            }
                        }
                    }
                    else if(devsta->sta ==DEV_OFFLINE )
                    {
                        puts("---DEV_OFFLINE \n");
                        if((mode_ctrl.cur_mode & USB_MODE) == 0 && (get_parking_status()== 0))
                        {
                            if(sys_status.menu_status==0)
                                clear_display_noto();
                            ui_clear_controls(TYPE_PIC, Img_1_5);
                            ui_pic(Img_1_27,-1,-1,USE_ORIG_COLOR);
                            if(get_current_mode() & ENC_MODE)
                            {
                                get_remain_time_rec();
                            }
                            else if(get_current_mode() & TPH_MODE)
                            {
                                disp_REC_cnt(0,ERASE_ONLY);//²Á³ýÊý×ÖÇø
//                                num_of_last_photo();
                                reset_num_of_photo();
                            }
                            else if(get_current_mode() & DEC_MODE)
                            {
//
                                puts("\n DEV_OFFLINE ui_clear_screen \n");
                                ui_clear_controls(TYPE_PIC,Img_1_47);
                                if(sys_status.menu_status)
                                    put_msg_fifo(MSG_MENU_EXIT,MSG_TO_SELF,0) ;
                                else
                                {
                                    ui_clear_controls(TYPE_TEXT,Txt_4_4);
                                    if(pb_fctrl.ftype == PKG_TYPE_JPG)
                                    {
                                        ui_clear_screen(LAYER_IMAGE0, 0,0, LCD_W, LCD_H, IMAGE_COLOR_BLACK);
                                        ui_clear_controls(TYPE_TEXT,Txt_4_3);
//                                        ui_clear_controls(TYPE_TEXT,Txt_4_5);
                                        display_note(M149);
                                    }
                                    else
                                    {
                                        if(dec_ctrl.dec_status == DEC_APP_READY)
                                        {
                                            ui_clear_controls(TYPE_TEXT,Txt_4_3);
//                                            ui_clear_controls(TYPE_TEXT,Txt_4_5);
                                            display_note(M149);
                                        }
                                        else if(dec_ctrl.dec_status == DEC_APP_ABORT)
                                        {
                                            display_note(M149);
                                        }
                                    }

                                }
                            }
                        }
                    }
                }
                else
                {
                    if (DEV_OFFLINE == devsta->sta && devsta->hdev->dev_io->device_type == DEV_USB_SLAVE ) //usbÀëÏß
                    {
//                        ui_clear_screen(LAYER_OSD0_WIN0,0,0,480,272,COLOR_LUCENCY);
                        if(usb_select_item != 2) //非充电模式
                            ui_clear_screen(LAYER_OSD1_WIN0,0,0,LCD_W,LCD_H,COLOR_LUCENCY);
                        printf("\n find dev  %s--%d \n",devsta->hdev->dev_io->name, devsta->sta);
                        puts("----2222222----");
                    }
                    else
                    {
                        puts("----111111111----");
                    }
                    puts("----33333333333----");
                }
#endif //SINGLE_UI_A

                free(msg_d);
				break;


#if 1 //AVOUT JOHNSON 20160530 ADD
                case  MSG_SCR_DISP_CHANGE:
                if((get_current_mode() & ENC_MODE))
                {
                        enc_ctrl.crec0_time = 0;
                        enc_ctrl.crec1_time = 0;
                        enc_ctrl.is_cycle_rec0 = 0;
                        enc_ctrl.is_cycle_rec1 = 0;  //禁止循环录影
                        enc_ctrl.usb_charge_cnt = 0;
                        enc_ctrl.usb_charge_flag = 0;
                        os_time_dly(10);
                        put_msg_fifo(MSG_MODE_SW ,MSG_TO_PROCESS ,ENC_TASK_N) ;
                }
                ui_updata_timer_unreg();
                if(g_dsc1_hdl)
                {
                    puts("stop g_dsc1_hdl \n");
                        if(dsc_stop(g_dsc1_hdl) == FALSE){
                                puts("stop dsc err\n");
                        }
                        else{
                            if(dsc_close(g_dsc1_hdl) == FALSE){
                                puts("close dsc err\n");
                            }else{
                                g_dsc1_hdl = NULL;
                            }
                        }
                }
                if(g_dsc0_hdl)
                {
                    if(dsc_stop(g_dsc0_hdl) == FALSE){
                        puts("stop dsc err\n");
                    }else{
                         puts("stop dsc ok \n");
                        if( g_double_dsc_status == DOUB_DSC_FRONT_WIN || g_double_dsc_status ==DOUB_DSC_BIG_SMALL || g_double_dsc_status==DOUB_DSC_IDLE)
                            disp_closelogo();
                        else
                        {
                            if(dsc_close(g_dsc0_hdl) == FALSE){
                                puts("close dsc err\n");
                            }else{
                                g_dsc0_hdl = NULL;
                            }
                        }
                    }
                }
                ui_clear_screen(LAYER_IMAGE0, 0, 0, LCD_W, LCD_H, IMAGE_COLOR_BLACK);
                ui_clear_screen(LAYER_IMAGE1, 0, 0, LCD_W, LCD_H, IMAGE_COLOR_BLACK);
                #if (DOUBLE_UI_C != 1)
                ui_clear_screen(LAYER_OSD0_WIN0, 0, 0, LCD_W, LCD_H, COLOR_LUCENCY);
                #endif
                ui_clear_screen(LAYER_OSD1_WIN0, 0, 0, LCD_W, LCD_H, COLOR_LUCENCY);

                CPU_SR_ALLOC();
                OS_ENTER_CRITICAL();
                imb_close();
                OS_EXIT_CRITICAL();

                if (avout_det_io())
                {//avout
                    LCD_MODE = LCD_AVOUT;
                    ALCD_BL_OFF();
                    if (sys_info.TV_mode == 0)//pal
                    {
                        puts("tv mode pal*******************1\n");
                        lcd_size_init(720, 576);
                    }
                    else
                        if (sys_info.TV_mode == 1)//ntsc
                        {
                            puts("tv mode ntsc*******************1\n");
                            lcd_size_init(720, 480);
                        }
                }
                else
                {//普通屏幕显示，通过使能 LCD_TYPE 得到对应的屏幕类型

                    LCD_MODE = LCD_ILI9325;
                    lcd_size_init(400, 240);//(320, 240);
                }

                u16 scr_w, scr_h;
                lcd_get_size(&scr_w, &scr_h);

                dsc1_small_ptr = g_dbuf + scr_w * scr_h * 3 / 2 * 3;
                dsc1_half_ptr = g_dbuf + (scr_w / 2) * scr_h * 3 / 2 * 3;
                dsc1_all_ptr = g_dbuf;

                //disp0
                disp0_info.drc = isp0_name;
                disp0_info.x = 0;//0;
                disp0_info.y = 0;
                disp0_info.des_w = scr_w ;
                disp0_info.des_h = scr_h - 0;
                disp0_info.des = lcd_name;
                disp0_info.frame_cb = NULL;

                disp0_info.yuv_val = lcd_get_bottom_color();

                disp0_info.dsc_buf = g_dbuf;
                if(disp0_info.dsc_buf == NULL){
                    puts("\ndsc0 buf null\n");
                }

                //disp1
                disp1_info.drc = isp1_name;
               //     disp1_info.x = 0;
                disp1_info.x = SMALL_WIN_START_X;
                disp1_info.y = SMALL_WIN_START_Y;
                disp1_info.des_w = SMALL_WIN_WIDTH;
                disp1_info.des_h = SMALL_WIN_HEIGH;
                disp1_info.des = lcd_name;
                disp1_info.frame_cb = NULL;

                disp1_info.yuv_val = lcd_get_bottom_color();

                disp1_info.dsc_buf = dsc1_small_ptr;//malloc(scr_w * scr_h *3/2*3);//先申请满屏的显存
                if(disp1_info.dsc_buf == NULL){
                    puts("\ndsc1 buf null\n");
                }
                config_disp(LCD_MODE);
                if(LCD_MODE == LCD_ILI9325)
                {
                    ALCD_BL_ON();
                }
                main_frame(&sval);

                g_dsc0_hdl = dsc_open(&disp0_info,image0);
                if(g_dsc0_hdl){
                    if(dsc_start(g_dsc0_hdl) == FALSE){
                        puts("\n----dsc0 start err\n");
                    }
                }else{
                    puts("\n----dsc0 open err 1\n");
                }

                if(enc_ctrl.enc1_en)
                {
                    g_dsc1_hdl = dsc_open(&disp1_info,image1);
                    if(g_dsc1_hdl){
                        if(dsc_start(g_dsc1_hdl) == FALSE){
                            puts("\n----dsc1 start err\n");
                        }
                    }else{
                        puts("\n----dsc1 open err\n");
                    }
                    g_double_dsc_status = DOUB_DSC_BIG_SMALL;
                }
                ui_updata_timer_hd = timer_reg(500 , updata_timer , NULL); //1秒

                break;

#endif


        }
    }
}

//void open_iamge0(void)
//{
//    u32 cpu_sr;
//    IMGLAYER *image;
//OS_ENTER_CRITICAL();
//    image = imb_get_image_hdl(LAYER_IMAGE0);
//    if(image->enable == TRUE){
//        if(image->buf){
//            free(image->buf);
//        }
//    }
//    image->enable = TRUE;
//    imb_image_ctl(image->layer,image->enable);
//OS_EXIT_CRITICAL();
//}

void ui_close_imb()
{
    u32 cpu_sr;
    IMGLAYER *image;
    u8 * buf0 = NULL;
    u8 * buf1 = NULL;

OS_ENTER_CRITICAL();
    image = imb_get_image_hdl(LAYER_IMAGE0);
    if(image->enable == TRUE){
        if(image->buf){
//            free(image->buf);
            buf0 = image->buf;
        }
    }
    image->enable = FALSE;
OS_EXIT_CRITICAL();
    imb_image_ctl(image->layer,image->enable);

OS_ENTER_CRITICAL();
    image = imb_get_image_hdl(LAYER_IMAGE1);
    if(image->enable == TRUE){
        if(image->buf){
            //free(image->buf);
            buf1 = image->buf;
        }
    }
    image->enable = FALSE;
OS_EXIT_CRITICAL();
    imb_image_ctl(image->layer,image->enable);


//    if(buf0){
//        free(buf0);
//    }
//    if(buf1){
//        free(buf1);
//    }

}
void ui_close_imb1()
{
    u32 cpu_sr;
    IMGLAYER *image;
    u8 * buf0 = NULL;
    u8 * buf1 = NULL;

OS_ENTER_CRITICAL();
//    image = imb_get_image_hdl(LAYER_IMAGE0);
//    if(image->enable == TRUE){
//        if(image->buf){
////            free(image->buf);
//            buf0 = image->buf;
//        }
//    }
//    image->enable = FALSE;
//    imb_image_ctl(image->layer,image->enable);

    image = imb_get_image_hdl(LAYER_IMAGE1);
    if(image->enable == TRUE){
        if(image->buf){
            //free(image->buf);
            buf1 = image->buf;
        }
    }
    image->enable = FALSE;
    OS_EXIT_CRITICAL();
    imb_image_ctl(image->layer,image->enable);


//    if(buf0){
//        free(buf0);
//    }
//    if(buf1){
//        free(buf1);
//    }

}


void ui_open_imb()
{
    u32 cpu_sr;
    IMGLAYER *image;
    u8 * buf0 = NULL;
    u8 * buf1 = NULL;

OS_ENTER_CRITICAL();
//    image = imb_get_image_hdl(LAYER_IMAGE0);
//    if(image->enable == TRUE){
//        if(image->buf){
////            free(image->buf);
//            buf0 = image->buf;
//        }
//    }
//    image->enable = FALSE;
//    imb_image_ctl(image->layer,image->enable);

    image = imb_get_image_hdl(LAYER_IMAGE1);
    if(image->enable == TRUE){
        if(image->buf){
            //free(image->buf);
            buf1 = image->buf;
        }
    }
    image->enable = TRUE;
    OS_EXIT_CRITICAL();
    imb_image_ctl(image->layer,image->enable);


//    if(buf0){
//        free(buf0);
//    }
//    if(buf1){
//        free(buf1);
//    }

}

extern u8 language_talbe[];
void disp_vm_init()
{
int ret;
    ret = vm_api_read(SYS_SET_INFO,&sys_info);
    if((ret == VM_ERR_FLASH_OLD) || (sys_info.para_flag != UI_PARA_REFRESH))
    {
    puts("\n rewrite SYS_SET_INFO \n");
//    }
//    if(sys_info.para_flag != UI_PARA_REFRESH)
//    {
        vm_eraser();
        sys_info_reset();
        ret = vm_api_write(SYS_SET_INFO,&sys_info);
        if(ret != VM_ERR_NONE)
        {
            puts("\n vm_api_write err \n");
        }

//        TIME time;  //old
//        time.year = 2016;  //系统时间初始化
//        time.month = 5;
//        time.day = 1;
//        time.hour = 0;
//        time.minute = 0;
//        time.second = 0;
//        write_sys_time((RTC_TIME*)&time);

//        g_language_mode = sys_info.language;
    }

  #if 1 //JOHNSON 20160530 ADD
        TIME time;
        time.year = 2016;  //系统时间初始化
        time.month = 6;
        time.day = 18;
        time.hour = 0;
        time.minute = 0;
        time.second = 0;
        write_sys_time((RTC_TIME*)&time);
  #endif

    g_language_mode = language_talbe[sys_info.language];
    printf("g_language_mode :%d ",g_language_mode);
    printf("sys_info.language :%d ",sys_info.language);
    printf("sys_info.lane_dect_carHead :%d ",sys_info.lane_dect_carHead);
    printf("sys_info.lane_dect_vanish_y :%d ",sys_info.lane_dect_vanish_y);
}
const u8 wb_table[] =
{
 WB_PRESET_DISABLE,
 WB_PRESET_DAYLIGHT,
 WB_PRESET_CLOUNDY,
 WB_PRESET_TUNGSTEN,
 WB_PRESET_FLUORESCENT1,
 WB_PRESET_FLUORESCENT2,

};

const u8 ae_bftype[] =
{
    AE_BF_50HZ,
    AE_BF_60HZ,
};
void updata_menu_cfg(void)
{
    float tmp1,tmp2,tmp3;
    u16 scr_w,scr_h;

    if(imc_isr_stop_flag == 0)
    {
        //这个时候，ISP后台不更新，相当于进入ISP的临界区,必须成对的出现

        ispt_preset_wb(wb_table[sys_info.while_balance]); //白平衡
        ispt_customize_ev(sys_info.exposure); //曝光补偿
        //颜色特效
    //    if(sys_info.moving_detect)     //运动侦测 精度等级设置
        ispt_customize_mdLevel(2); // [0-4]个等级 越小越灵敏
        //mic_mute_fun(sys_info.mic_mute);  //录音开关
        set_AE_bf_type(ae_bftype[sys_info.light_freq]);

    }

    dac_api_enable_ktone(sys_info.key_voice); //按键音使能
#if LANE_DECT_EN
    //车道检测
    if(sys_info.lane_dect_en && g_ldet_ptr)
    {
        g_ldet_info.vanish_y = sys_info.lane_dect_vanish_y;
        g_ldet_info.carHead = sys_info.lane_dect_carHead;

        lcd_get_size(&scr_w, &scr_h);

        g_vanish_y = sys_info.lane_dect_vanish_y; //暂存参数
        g_carHead = sys_info.lane_dect_carHead;

        tmp1 = scr_h;
        tmp2 = LDET_HEIGH;

        tmp3 = g_vanish_y;
        tmp3 = (tmp2 * tmp3) / tmp1;
        g_ldet_info.vanish_y = (int)tmp3;
        g_ldet_info.vanish_y = (g_ldet_info.vanish_y > LDET_HEIGH) ? LDET_HEIGH : g_ldet_info.vanish_y;

        tmp3 = g_carHead;
        tmp3 = (tmp2 * tmp3) / tmp1;
        g_ldet_info.carHead = (int)tmp3;
        g_ldet_info.carHead = (g_ldet_info.carHead > LDET_HEIGH) ? LDET_HEIGH : g_ldet_info.carHead;

        if((g_ldet_info.carHead - g_ldet_info.vanish_y) < (LDET_HEIGH / 4)){
            g_ldet_info.carHead += 2;
        }

        g_ldet_info.len_factor = 0;
        if(lane_dect_stop(g_ldet_ptr) == 0){  //关
            puts("\nlane_dect_stop err\n");
        }
        if(lane_dect_start(g_ldet_ptr, &g_ldet_info) == 0){
            puts("\nlane_dect_start err\n");
            put_u32hex(g_ldet_info.vanish_y);
            put_u32hex(g_ldet_info.carHead);
        }else{
            puts("lane_dect_start succ\n");
        }
    }
#endif
}

void moving_process(void)
{
    if(enc_ctrl.enc0_status == ENC_APP_IDLE || enc_ctrl.enc0_status ==ENC_APP_STOP)
    {
        if(sys_info.moving_detect)
        {
            static u8 mark = 0 ;
            mark = mark<<1;
            mark |= ispt_customize_mdStatus();
            if((mark&0xF) == 0xf)
            {
                put_msg_fifo(MSG_INIT_FILE ,MSG_TO_PROCESS ,ENC_TASK_N) ;
            }
        }
    }
}

u8 get_menu_statu(void)
{
    return sys_status.menu_status;
}

const u16 screen_protect_cnt[] = {
    0,30,60,120
};
void screen_protect_control(void)
{
    if(get_parking_status())   //倒车过程不屏保
        return;
    if(sys_info.screen_protect == 0)
        return;
    if(sys_status.screen_off_cnt == 0)
        return;

    sys_status.screen_off_cnt++;
    if(sys_status.screen_off_cnt >= screen_protect_cnt[sys_info.screen_protect])
    {
        sys_status.screen_off_cnt = 0;
        ALCD_BL_OFF();
    }

}

void screen_on(void)
{
    if(sys_status.screen_off_cnt < 1)
        ALCD_BL_ON();

    sys_status.screen_off_cnt = 1;
}

void ui_updata_timer_unreg(void)
{
    timer_unreg(ui_updata_timer_hd);
}
