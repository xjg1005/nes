#include "config.h"

//#if KEY_EN
#include "os_api.h"
#include "key_drv_manager.h"
#include "key_drv_ad.h"
#include "dac.h"
#include "app_cfg.h"
#include "audio.h"
#include "msg.h"
#include "mode_switch.h"
#include "lcd.h"
#include "ui_task.h"
#if KEY_EN

#define DOUBLE_GAP  40

typedef struct
{
    void    (*key_init)(void);
    u8      (*key_get_value)(void);
    u8      (*key_pressed_to_msg)(u8, u8);
} key_interface_t;


static u8 (* key_curr_pressed_to_msg)(u8, u8);


static const key_interface_t key_interface_table[]=
{
#if USE_AD_KEY0
    {ad_key0_init, ad_key0_get_value, ad_key0_pressed_to_msg},
#endif
#if USE_AD_KEY1
    {ad_key1_init, ad_key1_get_value, ad_key1_pressed_to_msg},
#endif

    {NULL, NULL, NULL}
};

static u8 key_onoff = 0;
void key_disable(u8 mark)
{
    key_onoff = (mark)?1:0;
}

u8 get_keystatus(void)
{
    return key_onoff;
}
void key_drv_init(void)
{
    key_interface_t *key = (key_interface_t*)key_interface_table;

    for(; key->key_init; key++)
    {
        key->key_init();
    }

    timer_reg(6, key_scan_process, NULL);

}


static u8  used_key = NO_KEY;
static u8  old_key = NO_KEY;
static u8  key_counter = 0;

static u8 key_filter(u8 key)
{
    if (old_key != key)
    {
        key_counter = 0;
        old_key = key;
    }
    else
    {
        key_counter++;
        if (key_counter == KEY_BASE_CNT)
        {
            used_key = key;
        }
    }

    return used_key;
}

static key_interface_t *key_scan_cnt = (key_interface_t*)key_interface_table;
static key_interface_t *key_sacn_old = (key_interface_t*)key_interface_table;

static u8 get_key_value(void)
{
    u8 keynum, value;

    do
    {
        if (key_scan_cnt->key_get_value == NULL)
        {
            break;
        }

        keynum = key_scan_cnt->key_get_value();
        value = key_filter(keynum);
        if(PWR_IO_READ())
        {
#ifdef DOUBLE_VEDIO
            value = 9; //
#else
            value = 4; //
#endif
        }
        if(value != NO_KEY)
        {
            key_curr_pressed_to_msg = key_scan_cnt->key_pressed_to_msg;
            key_sacn_old = key_scan_cnt;
            return value;
        }

        if (keynum != NO_KEY)
        {
            break;
        }

        key_scan_cnt++;
        if (key_scan_cnt->key_get_value == NULL)
        {
            key_scan_cnt = (key_interface_t*)key_interface_table;
        }
    }
    while(key_sacn_old != key_scan_cnt);

    return NO_KEY;
}



static u8  last_key = NO_KEY;
static u8  key_press_cnt = 0;
void key_scan_process(void* parm)
{
    u8 key_style;
    u8 key_val = NO_KEY;
    u8 key_msg;
    static u16 double_press_cnt = 0;//把连续快速的按两次按键给滤除
    static u8  double_press_flag = 0;



    if(sys_status.powerdown_flag)
    {
        goto __EXIT;
    }

    if(key_onoff)
    {
        goto __EXIT;
    }
    if(double_press_flag)//对应ok按键，解决快速按ok按键不停止录像
    {
        double_press_cnt++;
        if(double_press_cnt >= DOUBLE_GAP)
        {
            double_press_cnt = 0;
            double_press_flag = 0;
        }
    }
    else
    {
        double_press_cnt = 0;
    }

    key_val = get_key_value();

    if (key_val == last_key)               //长时间按键
    {
        if (key_val == NO_KEY)
        {
            goto __EXIT;
        }

        key_press_cnt++;
        if (key_press_cnt == KEY_LONG_CNT)//长按
        {
            key_style = KEY_LONG;
        }
        else if (key_press_cnt == (KEY_LONG_CNT + KEY_HOLD_CNT))//连按
        {
            key_style = KEY_HOLD;
            key_press_cnt = KEY_LONG_CNT;
        }

        else
        {
            goto __EXIT;
        }
    }
    else  //key_num = NO_KEY, 抬键
    {
        if ((key_val==NO_KEY) && (key_press_cnt>=KEY_BASE_CNT) && (key_press_cnt<KEY_LONG_CNT)) //短按抬起
        {
            key_press_cnt = 0;
            key_style = KEY_SHORT_UP;
        }
        else if((key_val==NO_KEY) && (key_press_cnt>=KEY_LONG_CNT)) //长按抬起
        {
            key_press_cnt = 0;
            key_style = KEY_LONG_UP;
        }
        else
        {
            key_press_cnt = 0;
            goto __EXIT;
        }
    }

    if(sys_status.screen_off_cnt < 1)
    {
        sys_status.screen_off_cnt = 1;
        ALCD_BL_ON();
        goto __EXIT;
    }
    else
        sys_status.screen_off_cnt = 1;

    if ((last_key == 7) //对应ok按键，解决快速按ok按键不停止录像
        || (((last_key == 4) || (last_key == 5)) && (get_menu_statu() == 0) && (get_current_mode() & DEC_MODE)))
    {
        if(double_press_flag ==1)
        {
            if(double_press_cnt < DOUBLE_GAP)
            {
                double_press_cnt = 0;
                double_press_flag = 0;
                goto __EXIT;
            }
        }
        else
            double_press_flag = 1;
    }


    //todo  key to msg
    if (key_curr_pressed_to_msg)
    {
        key_msg = key_curr_pressed_to_msg(key_style, last_key);
        if(key_msg != NO_MSG){
            usb_out_status = 0;
            sw_key_msg(key_msg);
        }
        //put_u8hex(key_curr_pressed_to_msg(key_style, last_key)) ;
    }
    //put_u8hex(key_curr_pressed_to_msg(key_style, last_key)) ;

__EXIT:
    last_key = key_val;
}

#else

void key_drv_init(void)
{

}

#endif
