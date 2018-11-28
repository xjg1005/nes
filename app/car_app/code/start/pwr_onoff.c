
#include "pwr_onoff.h"
#include "app_cfg.h"
#include "irtc.h"
#include "lcd.h"
#include "gravity_sensor_manage.h"
#include "mode_switch.h"
#include "enc_mode.h"
#include "dec_mode.h"
#include "imb_module.h"
#include "dac.h"
#include "audio.h"
void pwr_key_check(void * priv);


static PWR_ONOFF_T g_pwr_ctrl;

/*----------------------------------------------------------------------------*/
/** @brief: 复位系统
    @param:
    @return:
    @author:
    @note:
*/
/*----------------------------------------------------------------------------*/
void sofware_reset(void)
{
    CPU_SR_ALLOC();
    OS_ENTER_CRITICAL();
    while(1)
    {
        puts("\n sofwart reset FF\n");
        CRC_REG=0X6EA5;
        WDT_CON =0x50;//1010
        CRC_REG=0;
        PWR_CON |= BIT(4);//SOFEWARE RESET
    }
    OS_EXIT_CRITICAL();
}


//PR2控制红外灯 JOHNSON 20160531 ADD
//#ifdef FLASH_LIGHT
#if 0
void flash_light_ctrl()
{
    u8 pr_io_dir_out;
    printf("\nget_flashlight_mode=%d\n",get_flashlight_mode());

    if (get_flashlight_mode() == 0)
    {
//           PR2 = 0
        pr_io_dir_out = read_RTC_reg(READ_REG + 2);
        //6-2=4  两者的差值必须是四 xjg add
        pr_io_dir_out &= ~BIT(6);//PR2
        pr_io_dir_out &= ~BIT(2);//PR2
        write_RTC_reg(WRITE_REG + 2, pr_io_dir_out);
        set_flashlight_mode(1);
        printf("pr2=%d\n",pr_io_dir_out-161);//读取PR2的状态
    }
    else if (get_flashlight_mode() == 1)
    {
        set_flashlight_mode(0);
        //PR2 = 1
        pr_io_dir_out = read_RTC_reg(READ_REG + 2);
        pr_io_dir_out &= ~BIT(6);//PR2
        pr_io_dir_out |= BIT(2);//PR2
        write_RTC_reg(WRITE_REG + 2, pr_io_dir_out);
        printf("pr2=%d\n",pr_io_dir_out-161);//读取PR2的状态

    }
    updata_flashlight_mode_pic();
}
#endif


void power_onoff_init()
{
    u8 pr_io_dir_out;

    PWR_IO_INIT();

if(g_chip_id > 1)
//if(0)
{
    puts("\n set pr0 \n");
    pr_io_dir_out = read_RTC_reg(READ_REG + 2);
    pr_io_dir_out &= ~BIT(4);//PR0
    pr_io_dir_out |= BIT(0);//PR0

//    pr_io_dir_out &= ~BIT(6);//PR2
//    pr_io_dir_out &= ~BIT(2);//PR2
//#if OSC_1PIN_EN
//    pr_io_dir_out &= ~BIT(7);//PR3 =1
//    pr_io_dir_out |=  BIT(3);//PR3
//#endif
    write_RTC_reg(WRITE_REG + 2, pr_io_dir_out);
}

    memset(&g_pwr_ctrl, 0, sizeof(PWR_ONOFF_T));

//    timer_reg(5 , pwr_key_check , NULL);//

}

u8 power_get_status()
{
    return g_pwr_ctrl.status;
}


void power_on()
{
    u8 pr_io_dir_out;

    //PR0 = 1
    pr_io_dir_out = read_RTC_reg(READ_REG + 2);
    pr_io_dir_out &= ~BIT(4);//PR0
    pr_io_dir_out |= BIT(0);//PR0
    write_RTC_reg(WRITE_REG + 2, pr_io_dir_out);
    APP_PWR_ON();

}

void set_pr_io(u8 mark)
{

}

void clear_wakeu_pend(void)
{
//    if (read_RTC_reg(READ_REG + 5) & BIT(2))//读取PR2 G_SENSOR wkup_pnd
//	{
//        puts("\n 2222 clear wkup_pnd\n");
//        write_RTC_reg(WRITE_REG + 5,0x40);// 清除pnd
//	}
//

//    cfg_pdflag(1);
    if (read_RTC_reg(READ_REG + 5) & BIT(1))//读取PR1 usb_wake_up wkup_pnd
	{
        puts("\n 111 clear wkup_pnd\n");
        write_RTC_reg(WRITE_REG + 5,0x20);//清除pnd

	}

	//读取去PR2输入电平
    if (read_RTC_reg(READ_REG + 1) & BIT(2))
    {
        if(get_parking_guard()== 0)
        {
          puts("\n 222 G_sensor wakeup \n");
          sys_status.sys_gsensor_wakeup = 1;
        }
    }
    extern u8 get_power_ctrl_flag();
    if (get_power_ctrl_flag()
        && (usb_is_chargeing() == FALSE)
        && (PWR_IO_READ() == FALSE))
    {//第一次接电池，pflag为高，关机

        extern u8 check_firsttime_power_tmp(void);
        check_firsttime_power_tmp();
        puts("\n fffffffjjjjjjjj\n");

    }
    else
    {
        cfg_pdflag(1);
    }

}

static u32 lp_delay(unsigned int  volatile t)
{
    while ( t--)
    {
       asm volatile (" nop  ;");
    }
    return t ;
}

#include "video_play.h"
//extern u8 g_dbuf[LCD_W * LCD_H * 3 / 2 * 3 + SMALL_WIN_WIDTH * SMALL_WIN_HEIGH * 3 / 2 * 3] ALIGNE(32);
//AVOUT JOHNSON 20160530
extern u8 g_dbuf[720 * 576 * 3 / 2 * 3 + SMALL_WIN_WIDTH * SMALL_WIN_HEIGH * 3 / 2 * 3] ALIGNE(32);
extern const u8 close_res_filename[];
void power_off()
{
    u8 close_cnt = 0;
    u8 pr_io_dir_out;
    u8 pr_io_reg;
    u8 cnt = 0;
    static u8 enter_flag = FALSE;
    if(enter_flag)
    {
        return;
    }
    enter_flag = TRUE;
    //PR0 = 0
//    pr_io_dir_out = read_RTC_reg(READ_REG + 2);
//    pr_io_dir_out &= ~BIT(4);//PR0
//    pr_io_dir_out &= ~BIT(0);//PR0
//    write_RTC_reg(WRITE_REG + 2, pr_io_dir_out);
    key_disable(1);
    sys_status.powerdown_flag = 1;
    g_sensor_timer_unreg();
    if(get_menu_statu())
    {
        put_msg_fifo(MSG_MENU_EXIT ,MSG_TO_PROCESS ,UI_DISP_N) ;
        os_time_dly(10);
    }

//    if(get_current_mode() & USB_MODE)
//    {
//        puts("\n USB_MODE \n");
//        put_msg_fifo(MSG_USB_FLAG ,MSG_TO_PROCESS ,USB_TASK_N);
//        os_time_dly(10);
//    }


#if LANE_DECT_EN
    if(g_ldet_ptr){//关机之前，必须关闭车道检测模块
        lane_dect_stop(g_ldet_ptr);
        lane_dect_close(g_ldet_ptr);
    }
#endif

    if((get_current_mode() & ENC_MODE) )
    {
        enc_ctrl.crec0_time = 0;
        enc_ctrl.crec1_time = 0;
        enc_ctrl.is_cycle_rec0 = 0;
        enc_ctrl.is_cycle_rec1 = 0;  //禁止循环录影
        enc_ctrl.usb_charge_cnt = 0;
        enc_ctrl.usb_charge_flag = 0;
        os_time_dly(20);
        if(enc_ctrl.enc0_status == ENC_APP_RUNNING)//录像过程先暂停
        {
            puts("ENC_APP_RUNNING \n");
             put_msg_fifo(MSG_INIT_FILE ,MSG_TO_PROCESS ,ENC_TASK_N) ;
            os_time_dly(20);
            while(enc_ctrl.enc0_status != ENC_APP_STOP && cnt<100)
            {
                cnt++;
                os_time_dly(10);
            }
            if(enc_ctrl.enc1_en)
            {
                cnt =0;
                while(enc_ctrl.enc1_status != ENC_APP_STOP && cnt<100)
                {
                    cnt++;
                    os_time_dly(10);
                }
            }
         os_time_dly(10);
        }
        put_msg_fifo(MSG_MODE_SW ,MSG_TO_PROCESS ,ENC_TASK_N) ;
    }
    if(get_current_mode() & TPH_MODE)
    {
        if(enc_ctrl.enc0_status == ENC_APP_RUNNING)
        {
            put_msg_fifo(MSG_MODE_SW ,MSG_TO_PROCESS ,ENC_TASK_N) ;
            os_time_dly(20);
        }
    }
    if((mode_ctrl.cur_mode & DEC_MODE)&&(dec_ctrl.dec_status)) //回放过程先暂停
    {
        if(dec_ctrl.dec_type == PKG_TYPE_AVI)
        {
            puts("\n dec stop 1\n");
            if((dec_ctrl.dec_status == DEC_APP_RUNNING ) || dec_ctrl.dec_status == DEC_APP_PAUSE )
            {
                put_msg_fifo(MSG_DECODE_PAUSE ,MSG_TO_PROCESS ,DEC_TASK_N);
                put_msg_fifo(MSG_DECODE_STOP ,MSG_TO_PROCESS ,DEC_TASK_N);
                put_msg_fifo(MSG_DEC_CLOSE ,MSG_TO_PROCESS ,DEC_TASK_N);
                os_time_dly(10);
                puts("\n dec stop 2\n");
                //while((dec_ctrl.dec_status == DEC_APP_RUNNING ) && cnt<100)
                while((dec_ctrl.dec_status != DEC_APP_CLOSE) && (cnt < 100))
                {
                    cnt++;
                    os_time_dly(10);
                }
                //os_time_dly(50);
            }
            else if(dec_ctrl.dec_status == DEC_APP_READY)
            {
                put_msg_fifo(MSG_DECODE_STOP ,MSG_TO_PROCESS ,DEC_TASK_N);
                put_msg_fifo(MSG_DEC_CLOSE ,MSG_TO_PROCESS ,DEC_TASK_N);
                os_time_dly(10);
                cnt = 0;
                while((dec_ctrl.dec_status != DEC_APP_CLOSE) && (cnt < 100))
                {
                    cnt++;
                    os_time_dly(10);
                }
            }

        }
        else
            put_msg_fifo(MSG_MODE_SW ,MSG_TO_PROCESS ,DEC_TASK_N);

    }

    wdt_ctf_ctrl(0);//关闭看门狗
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
        }else
        {
             puts("stop dsc ok \n");
            //if( g_double_dsc_status == DOUB_DSC_FRONT_WIN || g_double_dsc_status ==DOUB_DSC_BIG_SMALL || g_double_dsc_status==DOUB_DSC_IDLE){
                puts("\nuuuuu1111111111111111111\n\n\n");
                //malloc_stats();
                //os_time_dly(40);//20
//#ifndef N_BOOT_SCR
                //disp_closelogo();
//#endif
//
//#ifdef N_BOOT_SCR
//                goto _CLOSE_SCREEN;
//#endif
//            }
//
//            else
            {
                if(dsc_close(g_dsc0_hdl) == FALSE){
                    puts("close dsc err\n");
                }else{
                    g_dsc0_hdl = NULL;
                }
            }
        }
    }
    if(g_dsc0_hdl == NULL)
    {
 //       puts("\n g_dsc0_hdl == NULL \n");
        u16 scr_w,scr_h;
        lcd_get_size(&scr_w, &scr_h);
        disp0_info.des_w = scr_w;
        disp0_info.des_h = scr_h;
        g_dsc0_hdl = dsc_open(&disp0_info,(IMGLAYER *)dsc_imb_open_tmp(&disp0_info));
        os_time_dly(20);
//#ifndef N_BOOT_SCR

        puts("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee44444444444444444444\n");
        disp_closelogo();
//#endif
//
//#ifdef N_BOOT_SCR
//goto _CLOSE_SCREEN;
//#endif
    }

    ui_clear_screen(LAYER_OSD0_WIN0,0,0,LCD_W,LCD_H,COLOR_LUCENCY);
    ui_clear_screen(LAYER_OSD1_WIN0,0,0,LCD_W,LCD_H,COLOR_LUCENCY);


//#ifdef N_BOOT_SCR
//_CLOSE_SCREEN:
//    printf("\nclose screen");
//    os_time_dly(50);
//    ui_clear_screen(LAYER_OSD0_WIN0,0,0,LCD_W,LCD_H,COLOR_LUCENCY);
//    ui_clear_screen(LAYER_OSD1_WIN0,0,0,LCD_W,LCD_H,COLOR_LUCENCY);
//
//    u16 src_w,src_h;
//    lcd_get_size(&src_w,&src_h);
//    disp_buf_init(g_dbuf,3,src_w,src_h,0x008080);
//
//    IMGLAYER *img= imb_get_image_hdl(LAYER_IMAGE1);
//    img->enable = FALSE;
//    imb_image_cfg(img);
//    img = imb_get_image_hdl(LAYER_IMAGE0);
//    img->enable = TRUE;
//    img->x = 0;
//    img->y = 0;
//    img->width = src_w;
//    img->height = src_h;
//    img->buff_x = 0;
//    img->buff_y = 0;
//    img->buff_width = src_w;
//    img->buff_height = src_h;
//    imb_image_cfg(img);
//
//    if(video_init((u8*)close_res_filename,NULL))
//    {
//        video_task_create(VIDEO_TASK_PRIO,VIDEO_TASK_N);
//    }
//
//    u16 dac_sample;
//    u8 audio_samp_rate = video_get_audio_sample();
//    if(audio_samp_rate == 16)
//        dac_sample = DAC_16KHZ;
//    else if(audio_samp_rate == 8)
//        dac_sample = DAC_8KHZ;
//
//    if(video_get_audio_number())
//    {
//        audio_ctl(AUDIO_CMD_DAC_SET_SRATE,(void*)&dac_sample);
//        dac_api_play_ctone2();
//    }
//    else
//    {
//        dac_sample = DAC_8KHZ;
//        audio_ctl(AUDIO_CMD_DAC_SET_SRATE,(void*)&dac_sample);
//        dac_api_play_ctone();
//        os_time_dly(250);
//    }
//
//    video_wait_play_end();
//    video_close();
//#endif


#ifndef N_BOOT_SCR
//    dac_api_play_stone();//播放开机音
    dac_api_play_ctone();
    os_time_dly(300);
#endif
    puts("\n set PR IO \n");
   //设置PR 口 PR2 输入 ； PR1 输入
        IRTC_WKCLRPND;
        pr_io_reg = read_RTC_reg(READ_REG + 3);
        pr_io_reg &=  ~(0x06);
        pr_io_reg &=  ~(0x6<<4);
        write_RTC_reg(WRITE_REG + 3,pr_io_reg);//PR2 PR1 关闭上下拉
        delay(10);
        pr_io_reg = read_RTC_reg(READ_REG + 2);
        pr_io_reg |= (0x6<<4);
        pr_io_reg &= ~(0x6);
        write_RTC_reg(WRITE_REG + 2,pr_io_reg);//PR2 PR1 设置为输入，关闭输出
    if(get_parking_guard() == 0) //停车守卫开
//    if(get_gravity_sensor() &&  get_parking_guard() == 0) //重力传感+停车守卫开
//    if (sys_info.parking_guard)
    {
        puts("\n gravity_sensor on ppppppppppppppppppppppp \n");
//    //设置 Gsensor 低功耗模式
        if(sys_info.gravity_sensor)//根据当前灵敏度去设置停车守卫的灵敏度
            g_sensor_low_power_mode(TRUE,sys_info.gravity_sensor);
        else
            g_sensor_low_power_mode(TRUE,0xFF);
#ifdef  BORAD_V11
       if(g_chip_id > 1)
       {
           puts("\n fffffff g_chip_id 2\n");
           pr_io_reg = read_RTC_reg(READ_REG + 4);
           pr_io_reg &= ~(0x6<<4);
           pr_io_reg |= 0x6;
           write_RTC_reg(WRITE_REG + 4,pr_io_reg);//PR1  PR2 上升沿唤醒，使能wkup
       }
#endif
    }
    else
    {
        puts("\n gravity_sensor off \n");
        //设置 Gsensor  待机
        g_sensor_low_power_mode(FALSE,0xff);
#ifdef  BORAD_V11
       if(g_chip_id > 1)
       {
           pr_io_reg = read_RTC_reg(READ_REG + 4);
           pr_io_reg &= ~(0x2<<4);
           pr_io_reg |= 0x2;
           write_RTC_reg(WRITE_REG + 4,pr_io_reg);//PR1 上升沿唤醒，使能wkup
       }
#endif
    }

#ifdef  BORAD_V11
    write_RTC_reg(WRITE_REG + 5,0xF0);//清除pnd
#endif

#ifdef  BORAD_V11
    if(0)
#else
    if(usb_is_chargeing())  //假关机
#endif
    {
        wdt_ctf_ctrl(0);//关闭看门狗
        //PWR_CTL 置 0
        APP_PWR_OFF();
        ALCD_BL_OFF();
//        OSTaskDel(UI_DISP_N);
        OSTaskSuspend(UI_DISP_N);
        OSTaskSuspend(USB_TASK_N);
        OSTaskSuspend(DEC_TASK_N);
        OSTaskSuspend(ENC_TASK_N);
        key_disable(1);
        u8 power_state = 0;
        if (PWR_IO_READ())//START_KEY
        {
            power_state = 1;
        }
        while (usb_is_chargeing())
        {
            //读取去PR2输入电平 G_sennor中断是否起来
            if(sys_status.gsensor_online)
            {
                if (read_RTC_reg(READ_REG + 1) & BIT(2))
                {
                    puts("G_sennor");
                    sofware_reset();
                }
            }

            os_time_dly(1);
            if (power_state==1 && PWR_IO_READ())
            {
                os_time_dly(1);
                //关机键还没有释放

            }
            else if (power_state==0 && PWR_IO_READ())
            {
                //开机键下来
                puts("POWER_KEY");
    //            os_time_dly(10);
                if (PWR_IO_READ())
                {
                    sofware_reset();
                }
                os_time_dly(1);
            }
            else if (power_state==1 && !PWR_IO_READ())
            {
                //关机键释放
                power_state = 0;
    //            os_time_dly(10);
            }
            else if (power_state==0 && !PWR_IO_READ())
            {
                os_time_dly(1);//关机键释放后正常等待

            }
        }

         puts("\n APP_PWR_OFF uu\n");
        APP_PWR_OFF();
        cfg_pdflag(0);
    }
    else
    {
//PWR_CTL 置 0
#ifdef  BORAD_V11
        cfg_pdflag(0);
//        os_time_dly(1);
#endif
        APP_PWR_OFF();

        PWR_IO_INIT();

        static u8 close_cnt = 0;
        while (PWR_IO_READ() && (close_cnt < 60))
        {
            close_cnt++;
            if(get_gravity_sensor() &&  get_parking_guard() == 0) //重力传感+停车守卫开
            {
                puts("\n gravity_sensor on \n");
            //设置 Gsensor 低功耗模式
                g_sensor_low_power_mode(TRUE,0xFF);

            }
            else
            {
                puts("\n gravity_sensor off \n");
                //设置 Gsensor  待机
                g_sensor_low_power_mode(FALSE,0xFF);
            }
        }
    }

    puts("\nyyyy\n");
    while(1)
    {

    }
}


void pwr_key_check(void * priv)
{
    if(g_pwr_ctrl.status == 1)
    {
        if(PWR_IO_READ()){
            g_pwr_ctrl.cnt++;
        }else{
            g_pwr_ctrl.cnt = 0;
        }

        if(g_pwr_ctrl.cnt > 5){
            g_pwr_ctrl.status = 0;
            g_pwr_ctrl.cnt = 0;
            puts("power off\n");
            power_off();

        }
    }
    else
    {
        if(PWR_IO_READ()){
            g_pwr_ctrl.cnt++;
        }else{
            g_pwr_ctrl.cnt = 0;
            if(g_pwr_ctrl.status == 2){
                g_pwr_ctrl.status = 1;
            }
        }

        if(g_pwr_ctrl.cnt > 5){

            g_pwr_ctrl.status = 2;
            g_pwr_ctrl.cnt = 0;
            power_on();

            puts("power on\n");
        }

    }

}

#ifdef LINGDUPRJ
const u8 auto_poweroff_sec[5] =
{
    0,60,120,180,240
};
#else
const u8 auto_poweroff_sec[5] =
{
    0,10,30,60,120
};
#endif

void auto_power_off_ctrl(void)
{
  static u16  poweroff_count= 0;
  if(sys_info.auto_poweroff == 0) //自动关机功能关闭
  {
    poweroff_count= 0;
    return;
  }
  if(get_parking_status())   //倒车过程不关机
  {
    poweroff_count= 0;
    return;
  }
  if(sys_status.screen_off_cnt == 1 )
  {
      poweroff_count= 0;
      if(sys_info.screen_protect == 0)//屏保关闭
      {
         sys_status.screen_off_cnt = 2;
      }
      return;
  }

    if(  ((get_current_mode() & ENC_MODE) && (enc_ctrl.enc0_status == ENC_APP_RUNNING) )//录像过程
      ||  ((get_current_mode() & DEC_MODE ) && (dec_ctrl.dec_status == DEC_APP_RUNNING )) //回放过程
    )
    {
        poweroff_count = 0;
        return;
    }

    if(mode_ctrl.cur_mode & USB_MODE)
    {
        poweroff_count = 0;
        return;
    }

    poweroff_count++;

    if (poweroff_count >= 0xFFFF)
    {
        poweroff_count = 0;
    }

//    if(poweroff_count >= sys_info.auto_poweroff*10)
    if(poweroff_count >= auto_poweroff_sec[sys_info.auto_poweroff])
    {
        poweroff_count = 0;
        put_msg_fifo(MSG_KEY_CLOSE ,MSG_TO_PROCESS ,MODE_SW_N);
    }

}
void cfg_pdflag(u8 x)
{
    u8 pr_io_reg;
    pr_io_reg = read_RTC_reg(READ_REG + 0);
    if(x)
        pr_io_reg |= BIT(7);
    else
        pr_io_reg &= ~BIT(7);
    write_RTC_reg(WRITE_REG + 0,pr_io_reg);
}

u8 read_pdflag(void)
{
     u8 pr_io_reg;
    pr_io_reg = read_RTC_reg(READ_REG + 0);
    if(pr_io_reg&BIT(7))
        return 1;
    return 0;
}

void check_firsttime_power(void)
{
    USB_CHARGE_INIT();
    u8 pr_io_reg;
    if(read_pdflag()) //pdflag =1
    {
        puts("\n pdflag =1  \n");
//        if(PWR_IO_READ())
//        {
//           puts("\n start key  power on   \n");
//           return;
//        }
        u8 temp = read_RTC_reg(READ_REG + 5);
        put_u16hex(temp);
        if (temp != 0xF)//?
        {
            puts(" PR 0123 wakeup pending \n");
            return;
        }
        if(usb_is_chargeing() == FALSE) //没插充电线
        {

            puts("\n pdflag =1 close \n");
            pr_io_reg = read_RTC_reg(READ_REG + 2);
            pr_io_reg &= ~BIT(4);//PR0
            pr_io_reg |= BIT(0);//PR0
//            pr_io_reg &= ~BIT(0);//PR0

            IRTC_WKCLRPND;
            pr_io_reg = read_RTC_reg(READ_REG + 3);
            pr_io_reg &=  ~(0x06);
            pr_io_reg &=  ~(0x6<<4);
            write_RTC_reg(WRITE_REG + 3,pr_io_reg);//PR2 PR1 关闭上下拉
            delay(10);
            pr_io_reg = read_RTC_reg(READ_REG + 2);
            pr_io_reg |= (0x6<<4);
            pr_io_reg &= ~(0x6);
            write_RTC_reg(WRITE_REG + 2,pr_io_reg);//PR2 PR1 设置为输入，关闭输出


           pr_io_reg = read_RTC_reg(READ_REG + 4);
           pr_io_reg &= ~(0x2<<4);
           pr_io_reg |= 0x2;
           write_RTC_reg(WRITE_REG + 4,pr_io_reg);//PR1  上升沿唤醒

            write_RTC_reg(WRITE_REG + 5,0xF0);//清除pnd

            cfg_pdflag(0);
            puts(" aa\n");
            APP_PWR_OFF();
            puts(" A \n");

        }
    }
}



u8 check_firsttime_power_tmp(void)
{
    USB_CHARGE_INIT();
    u8 pr_io_reg;
//    if(read_pdflag()) //pdflag =1
    {
        puts("\n pdflag =1  \n");
//        if(PWR_IO_READ())
//        {
//           puts("\n start key  power on   \n");
//           return;
//        }

        u8 temp = read_RTC_reg(READ_REG + 5);
        put_u16hex(temp);
//        if (temp != 0xF)//?
        if (temp&BIT(2) && (read_pdflag() == 0))//?
        {
            puts(" PR 0123 wakeup pending \n");
            return 0;
        }
//        power_crtl_flag = TRUE;

        if(usb_is_chargeing() == FALSE) //没插充电线
        {

            puts("\n pdflag =1 close \n");
            pr_io_reg = read_RTC_reg(READ_REG + 2);
            pr_io_reg &= ~BIT(4);//PR0
            pr_io_reg |= BIT(0);//PR0
//            pr_io_reg &= ~BIT(0);//PR0

            IRTC_WKCLRPND;
            pr_io_reg = read_RTC_reg(READ_REG + 3);
            pr_io_reg &=  ~(0x06);
            pr_io_reg &=  ~(0x6<<4);
            write_RTC_reg(WRITE_REG + 3,pr_io_reg);//PR2 PR1 关闭上下拉
            delay(10);

            pr_io_reg = read_RTC_reg(READ_REG + 2);
            pr_io_reg |= (0x6<<4);
            pr_io_reg &= ~(0x6);
            write_RTC_reg(WRITE_REG + 2,pr_io_reg);//PR2 PR1 设置为输入，关闭输出


           pr_io_reg = read_RTC_reg(READ_REG + 4);
           pr_io_reg &= ~(0x2<<4);
           pr_io_reg |= 0x2;
           write_RTC_reg(WRITE_REG + 4,pr_io_reg);//PR1  上升沿唤醒

            write_RTC_reg(WRITE_REG + 5,0xF0);//清除pnd

            cfg_pdflag(0);
            puts(" aa\n");
            APP_PWR_OFF();
            puts(" A \n");

            return 1;
        }
    }

    return 0;
}
