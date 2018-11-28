#include "mode_switch.h"
#include "dev_ctrl.h"
#include "drv_manage.h"
#include "app_cfg.h"
#include "msg.h"
#include "dsc.h"
#include "enc_mode.h"
#include "dec_mode.h"
#include "my_ui.h"
#include "lcd.h"
#include "ui.h"


extern disp_iparm_t disp0_info ,disp1_info;
extern dsc_hdl_t * g_dsc0_hdl;
extern dsc_hdl_t * g_dsc1_hdl;

MODE_SW_T mode_ctrl;
SYS_STATUS_T sys_status;
static const u8 mode_list[] =
{
    SYS_MODE,
    ENC_MODE,
    TPH_MODE,
    DEC_MODE,
    //USB_MODE,
};


volatile u8 imc_isr_stop_flag = 0;
static u8 enc0_isr_cnt = 0;
static u8 dsc0_isr_cnt = 0;
void check_imc_isr(void *ptr)
{
    if( mode_ctrl.cur_mode & (ENC_MODE | TPH_MODE) )
    {
        if(!imc_isr_stop_flag)
        {
            if(enc_ctrl.ves_enc && (ves_is_enc0_isr(enc_ctrl.ves_enc) < 2)){

                enc0_isr_cnt++;
                if(enc0_isr_cnt > 1){
                    imc_isr_stop_flag |= BIT(0);
                    enc0_isr_cnt = 0;
                }
            }else{
                enc0_isr_cnt = 0;
            }

            if(g_dsc0_hdl && (ves_is_dsc0_isr(g_dsc0_hdl->imc_ch) < 2)){
                dsc0_isr_cnt++;
                if(dsc0_isr_cnt > 1){
                    imc_isr_stop_flag |= BIT(1);
                    dsc0_isr_cnt = 0;
                }
            }else{
                dsc0_isr_cnt = 0;
            }

            if(ves_isp0_isr() < 2){
                imc_isr_stop_flag |= BIT(2);
            }

            if(imc_isr_stop_flag){
 //               put_msg_fifo(MSG_RESTORE_ISP ,MSG_TO_PROCESS ,MODE_SW_N) ;//20151223 会照成会面出阴影
            }
        }
        else
        {
          enc0_isr_cnt = 0;
          dsc0_isr_cnt = 0;
        }
    }
    else
    {
        imc_isr_stop_flag = 0;
        enc0_isr_cnt = 0;
        dsc0_isr_cnt = 0;
    }
}


#include "FileSelecter.h"
#include "menu.h"
void unlock_all_file_ctrl()
{

    if(get_parking_status())
        return;
    if(g_startup_finish == 0)
        return;
    if (get_menu_statu())
        return;
    if(sys_status.powerdown_flag)
        return;

    if(is_sd_online() == FALSE)
    {
        display_note(M156); //无SD卡
        return;
    }
    static u8 flag = FALSE;
    if (flag)
        return;

    flag = TRUE;

    if (mode_ctrl.cur_mode & ENC_MODE)
    {
        if ((enc_ctrl.enc0_status == ENC_APP_RUNNING) || (enc_ctrl.enc1_status == ENC_APP_RUNNING))
        {
            user_define.is_recording = 1;
            put_msg_fifo(MSG_KEY_OK ,MSG_TO_PROCESS ,MODE_SW_N) ;
        }
        put_msg_fifo(MSG_KEY_OK_3S ,MSG_TO_PROCESS ,UI_DISP_N) ;
    }

    flag = FALSE;
}

void mode_sched(void *p_arg)
{
    u32 err;
    u32 msg ;
    msg_t *msg_d;
    msg_devicechange_t *devsta;
    u16 scr_w,scr_h;
    u8 tmp;

    err = dev_reg_msg(NULL);


    //mode_ctrl.cur_mode |= SYS_MODE;

    while(1)
    {
        msg = get_msg();

        if(msg  > (u32)0x2000000)
        {
            msg_d = msg ;
            msg = msg_d->type ;
        }


        switch(msg)
        {
            case MSG_KEY_OK_3S:
                unlock_all_file_ctrl();
                break;

            case MSG_KEY_PREV_LONG:   //20160108 san
                if (get_menu_statu())
                {
                    break;
                }
                if (mode_ctrl.cur_mode & ENC_MODE)
                {
                    if (sys_info.moving_detect)
                    {
                        sys_info.moving_detect = 0;
                    }
                    else
                    {
                        sys_info.moving_detect = 1;
                    }
                    motion_det_if(sys_info.moving_detect);
                    save_sysinfo(&sys_info);
                    puts("\n ggggggmmmm\n");
                    updata_movingdet_pic();
                }
            break;    //20160108 san

//            case MSG_KEY_NEXT_LONG:  //JOHNSON 20160531 old
//                if (get_menu_statu())
//                {
//                    break;
//                }
//                if (get_current_mode() & USB_MODE)
//                {
//                    break;
//                }
//                //puts("\n llllllllllllllllllllllllllong\n");
//                if (get_current_mode() & ENC_MODE)
//                {
//                    if (sys_info.parking_guard)
//                    {
//                        sys_info.parking_guard = 0;
//                    }
//                    else
//                    {
//                        sys_info.parking_guard = 1;
//                    }
//                    save_sysinfo(&sys_info);
//                    puts("\n ggggggggg\n");
//                    updata_parking_guard_pic();
//                }
//            break;

            case MSG_RESTORE_ISP:
                if(imc_isr_stop_flag)
                {
                    key_disable(1);
                    puts("\n");
                    printf_buf(&isp0_sen_con,10*4);
                    puts("\n");
                    printf_buf(&isp0_out_hst,6*4);
                    puts("\n");
                    printf_buf(&imc_pnd_con,24*4);
                    puts("\n");
                    printf_buf(&imc_ch2_sca0_con,12*4);
                    puts("\n");


                    tmp = (imc_isr_stop_flag & BIT(0)) ? 0 : 1;
                    if(enc_ctrl.ves_enc){
                        if(ves_stop_imc_enc0(enc_ctrl.ves_enc, tmp)){
                            puts("\ncloseimc err\n");
                        }
                    }
                    if(g_dsc0_hdl){
                        if(dsc_stop(g_dsc0_hdl) == FALSE){
                            puts("\nrstop dsc err\n");
                        }
                    }

                    if(isp0_unmount() != DEV_ERR_NONE){
                        imc_isr_stop_flag = 0;
                        key_disable(0);
                        isp_not_update(0);
                        puts("\nisp app unmount err\n");
                        break;
                    }

                    //这个时候，ISP后台不更新，相当于进入ISP的临界区,必须成对的出现
                    isp_not_update(1);

                    if(ves_dsensor_init() != 0){
                        isp0_reset_mount();
                        puts("\nisp0 mount app err\n");
                        isp_not_update(0);
                        os_time_dly(5);

                    }else{
                        reset_isp_cfg();//挂载成功，才重新配置ISP
                    }
                    isp_not_update(0);


                    if(g_dsc0_hdl){
                        if(dsc_start(g_dsc0_hdl) == FALSE){
                            puts("\nrstart dsc err\n");
                            dsc_reset_start(g_dsc0_hdl);
                        }
                    }

                    if(enc_ctrl.ves_enc){
                        if(ves_start_imc_enc0(enc_ctrl.ves_enc)){
                            puts("\nopenimc err\n");
                        }
                    }

                    imc_isr_stop_flag = 0;
                    key_disable(0);

                    puts("\nrestore over\n");
                }
            break;

#if LANE_DECT_EN
            case MSG_LDETE_WARNING:
                dac_api_play_wtone();//报警
            break;
#endif

            case MSG_DEVICECHANGE:
                screen_on();

                free(msg_d);
            break;

            case MSG_AV_IN_ONLINE:
                screen_on();
                if(mode_ctrl.cur_mode & (ENC_MODE | TPH_MODE) ){
                    put_msg_fifo(MSG_AV_IN_ONLINE ,MSG_TO_PROCESS ,ENC_TASK_N) ;
                }else{
                    enc_ctrl.enc1_en = 1;
                }

            break;

            case MSG_AV_IN_OFFLINE:
                screen_on();
                if(mode_ctrl.cur_mode & (ENC_MODE | TPH_MODE) ){
                    put_msg_fifo(MSG_AV_IN_OFFLINE ,MSG_TO_PROCESS ,ENC_TASK_N) ;
                }else{
                    enc_ctrl.enc1_en = 0;
                }

            break;

        #if 1 //电源键控制补光灯 JOHNSON 20160607 add
            case MSG_KEY_START:
                if(mode_ctrl.cur_mode & USB_MODE){
                    break;
                }else{

                    #ifdef FLASH_LIGHT
                        flash_light_ctrl();
                    #endif
                }

            break;
            case MSG_KEY_CLOSE:
//                if(mode_ctrl.cur_mode & USB_MODE){
//                    break;
//                }
                puts("power off\n");
                power_off();
            break;

        #else //电源键控制屏保 old

            case MSG_KEY_START:
                sys_status.screen_off_cnt = 0;
                ALCD_BL_OFF();
            break;
            case MSG_KEY_CLOSE:
                if(sys_status.screen_off_cnt < 1)
                {

                    ALCD_BL_ON();
                    sys_status.screen_off_cnt = 1;
                }
                if(mode_ctrl.cur_mode & USB_MODE){
                    break;
                }
                puts("power off\n");
                power_off();
            break;
        #endif

            case MSG_KEY_PARK:   //停车守卫快捷键 JOHNSON 20160531 ADD
               if (get_menu_statu())
                {
                    break;
                }
                if (get_current_mode() & USB_MODE)
                {
                    break;
                }
                puts("\n llllllllllllllllllllllllllong\n");
                if (get_current_mode() & ENC_MODE)
                {
                    if (sys_info.parking_guard)
                    {
                        sys_info.parking_guard = 0;
                    }
                    else
                    {
                        sys_info.parking_guard = 1;
                    }
                    save_sysinfo(&sys_info);
                    puts("\n ggggggggg\n");
                    updata_parking_guard_pic();
                }
            break;
//                if (get_menu_statu())
//                {
//                    break;
//                }
//                puts("\n llllllllllllllllllllllllllong\n");
//                if (mode_ctrl.cur_mode & ENC_MODE)
//                {
//                    if (sys_info.parking_guard)
//                    {
//                        sys_info.parking_guard = 0;
//                    }
//                    else
//                    {
//                        sys_info.parking_guard = 1;
//                    }
//                    save_sysinfo(&sys_info);
//                    puts("\n ggggggggg\n");
//                    updata_parking_guard_pic();
//                }
//                break;

            case MSG_KEY_MENU:

                if(sys_status.menu_status==1){
                    enc_ctrl.usb_charge_flag = 0;//防止出入充电器开机的情况下，按菜单键退出之后，还会开始录像
                    enc_ctrl.usb_charge_cnt = 0;
                    put_msg_fifo(MSG_KEY_MENU ,MSG_TO_PROCESS ,UI_DISP_N) ;
                    break;
                }
                if( (enc_ctrl.enc0_status == ENC_APP_RUNNING
					|| ((dec_ctrl.dec_status == DEC_APP_RUNNING
                        || dec_ctrl.dec_status == DEC_APP_FF
                        || dec_ctrl.dec_status == DEC_APP_PAUSE
                        || dec_ctrl.dec_status == DEC_APP_FB)
                        &&  dec_ctrl.dec_type==PKG_TYPE_AVI)  )
                   && (mode_ctrl.cur_mode & TPH_MODE) ==0)

                {
                    puts("ENC OR DEC_APP_RUNNING ");
                    break;
                }
                if(mode_ctrl.cur_mode & USB_MODE){
                    break;
                }
                enc_ctrl.usb_charge_flag = 0;//防止出入充电器开机的情况下，按菜单键退出之后，还会开始录像
                enc_ctrl.usb_charge_cnt = 0;
                put_msg_fifo(MSG_KEY_MENU ,MSG_TO_PROCESS ,UI_DISP_N) ;

            break;

            case MSG_KEY_OK:

                if(sys_status.menu_status == 1){
                    put_msg_fifo(MSG_KEY_OK ,MSG_TO_PROCESS ,UI_DISP_N) ;
                    break;
                }

                if(mode_ctrl.cur_mode & USB_MODE){
                    if(sys_status.sys_gsensor_wakeup != 1)
                    {
                        put_msg_fifo(MSG_KEY_OK ,MSG_TO_PROCESS ,UI_DISP_N) ;
                    }
                    else
                        sys_status.sys_gsensor_wakeup = 0;

                    break;
                }

                if(mode_ctrl.cur_mode & (ENC_MODE | TPH_MODE) ){
                    if((mode_ctrl.cur_mode & TPH_MODE) && (enc_ctrl.pic_inout)){
                        break;
                    }
                    if(enc_ctrl.enc_inout == 1){
                        break;
                    }
                    enc_ctrl.enc_inout = 1;

                    if(mode_ctrl.cur_mode & TPH_MODE){
                        dac_api_stop_ktone();//防止第一次拍照时，同时发出按键音和拍照音
                    }

                    put_msg_fifo(MSG_INIT_FILE ,MSG_TO_PROCESS ,ENC_TASK_N) ;
                }

                if(mode_ctrl.cur_mode & DEC_MODE ){
                    put_msg_fifo(MSG_DECODE_RUN ,MSG_TO_PROCESS ,DEC_TASK_N) ;
                }
            break;

            case MSG_KEY_NEXT:

                if(sys_status.menu_status == 1){
                    put_msg_fifo(MSG_KEY_NEXT ,MSG_TO_PROCESS ,UI_DISP_N) ;
                    break;
                }

                if(mode_ctrl.cur_mode & USB_MODE){
                    put_msg_fifo(MSG_KEY_NEXT ,MSG_TO_PROCESS ,UI_DISP_N) ;
                    break;
                }

                if(mode_ctrl.cur_mode & (ENC_MODE | TPH_MODE) ){
                    put_msg_fifo(MSG_KEY_NEXT ,MSG_TO_PROCESS ,ENC_TASK_N) ;

                  #if 1 //下键控制屏保 JOHNSON 20160607 ADD
                    if(sys_status.screen_off_cnt == 0){
                        ALCD_BL_ON();
                        sys_status.screen_off_cnt = 1;
                    }else{
                        ALCD_BL_OFF();
                        sys_status.screen_off_cnt = 0;
                    }
                  #endif

                }

                if(mode_ctrl.cur_mode & DEC_MODE ){
                    if(dec_ctrl.dec_inout == 1){
                        break;
                    }
                    dec_ctrl.dec_inout = 1;

                    put_msg_fifo(MSG_KEY_NEXT ,MSG_TO_PROCESS ,DEC_TASK_N) ;
                }
            break;

            case MSG_KEY_PREV:

                if(sys_status.menu_status == 1){
                    put_msg_fifo(MSG_KEY_PREV ,MSG_TO_PROCESS ,UI_DISP_N) ;
                    break;
                }


                if(mode_ctrl.cur_mode & USB_MODE){
                    put_msg_fifo(MSG_KEY_PREV ,MSG_TO_PROCESS ,UI_DISP_N) ;
                    break;
                }

//            #if 0 //old
                if(mode_ctrl.cur_mode & (ENC_MODE | TPH_MODE) ){
//                    put_msg_fifo(MSG_KEY_PREV ,MSG_TO_PROCESS ,ENC_TASK_N) ;
                    put_msg_fifo(MSG_KEY_NEXT ,MSG_TO_PROCESS ,ENC_TASK_N) ;
                }
//            #else //上键复用为红外灯快捷键 JOHNSON 20160531
//                if(mode_ctrl.cur_mode & (ENC_MODE | TPH_MODE) ){
//                     printf("\nred light shoule be on\n");
//                    #ifdef FLASH_LIGHT
//                        flash_light_ctrl();
//                    #endif
//                    put_msg_fifo(MSG_KEY_PREV ,MSG_TO_PROCESS ,ENC_TASK_N) ;
//                }
//            #endif

                if(mode_ctrl.cur_mode & DEC_MODE ){
                    if(dec_ctrl.dec_inout == 1){
                        break;
                    }
                    dec_ctrl.dec_inout = 1;
                    put_msg_fifo(MSG_KEY_PREV ,MSG_TO_PROCESS ,DEC_TASK_N);
                }
            break;

         #if 1
            case MSG_KEY_LOCK:  //加锁快捷键 JOHNSON 20160531 ADD
                if(mode_ctrl.cur_mode & USB_MODE){
                    break;
                }
                if(mode_ctrl.cur_mode & ENC_MODE )// 录像过程 按mode键锁文件
                {
                    if (enc_ctrl.enc0_status == ENC_APP_RUNNING)
                    {
                        enc_ctrl.lock_current_file = 1;
                        break;
                    }
                }
                break;

            case MSG_KEY_MODE:  //模式键不再复用加锁键 JOHNSON 20160531
                if(mode_ctrl.cur_mode & USB_MODE){
                    break;
                }
//                if(mode_ctrl.cur_mode & ENC_MODE )// 录像过程 按mode键锁文件
//                {
//                    if (enc_ctrl.enc0_status == ENC_APP_RUNNING)
//                    {
//                        enc_ctrl.lock_current_file = 1;
//                        break;
//                    }
//                }

        #else //old 录影时按模式键加锁

            case MSG_KEY_MODE:
                if(mode_ctrl.cur_mode & USB_MODE){
                    break;
                }
                if(mode_ctrl.cur_mode & ENC_MODE )// 录像过程 按mode键锁文件
                {
                    if (enc_ctrl.enc0_status == ENC_APP_RUNNING)
                    {
                        enc_ctrl.lock_current_file = 1;
                        break;
                    }
                }
        #endif

            case MSG_MODE_SW:

                if(sys_status.menu_status == 1){
                    put_msg_fifo(MSG_KEY_MODE ,MSG_TO_PROCESS ,UI_DISP_N) ;
                    break;
                }

                if((mode_ctrl.cur_mode & SYS_MODE) == 0){
                    puts("还没上电完成！\n");
                    break;
                }
                if(mode_ctrl.cur_mode & (ENC_MODE | TPH_MODE) )
                {
                    if(enc_ctrl.enc0_status == ENC_APP_CLOSE)
                    {
                        if( (mode_ctrl.cur_mode & ENC_MODE) && enc_ctrl.enc1_en ){
                            if( (enc_ctrl.enc1_status) && (enc_ctrl.enc1_status != ENC_APP_CLOSE) ){
                                put_msg_fifo(MSG_MODE_SW ,MSG_TO_PROCESS ,ENC_TASK_N) ;
                                break;
                            }
                        }
                        enc_ctrl.enc0_status = ENC_APP_IDLE;
                        enc_ctrl.enc1_status = ENC_APP_IDLE;

                        if(g_dsc0_hdl)
                        {
                            if(dsc_stop(g_dsc0_hdl) == FALSE){
                                puts("stop dsc err\n");
                            }else{
                                if(dsc_close(g_dsc0_hdl) == FALSE){
                                    puts("close dsc err\n");
                                }else{
                                    g_dsc0_hdl = NULL;
                                }
                            }
                        }
                        if(g_dsc1_hdl)
                        {
                            if(dsc_stop(g_dsc1_hdl) == FALSE){
                                puts("stop dsc err\n");
                            }else{
                                if(dsc_close(g_dsc1_hdl) == FALSE){
                                    puts("close dsc err\n");
                                }else{
                                    g_dsc1_hdl = NULL;
                                }
                            }
                        }

                    }
                    else
                    {
                        if(enc_ctrl.enc0_status != ENC_APP_IDLE){
                            put_msg_fifo(MSG_MODE_SW ,MSG_TO_PROCESS ,ENC_TASK_N) ;
                            break;
                        }
                        enc_ctrl.enc0_status = ENC_APP_IDLE;
                        enc_ctrl.enc1_status = ENC_APP_IDLE;

                        enc_ctrl.usb_charge_flag = 0;
                        enc_ctrl.usb_charge_cnt = 0;


                        if(g_dsc0_hdl)
                        {
                            if(dsc_stop(g_dsc0_hdl) == FALSE){
                                puts("stop dsc err\n");
                            }else{
                                if(dsc_close(g_dsc0_hdl) == FALSE){
                                    puts("close dsc err\n");
                                }else{
                                    g_dsc0_hdl = NULL;
                                }
                            }
                        }
                        if(g_dsc1_hdl)
                        {
                            if(dsc_stop(g_dsc1_hdl) == FALSE){
                                puts("stop dsc err\n");
                            }else{
                                if(dsc_close(g_dsc1_hdl) == FALSE){
                                    puts("close dsc err\n");
                                }else{
                                    g_dsc1_hdl = NULL;
                                }
                            }
                        }
                    }
                }

                if(mode_ctrl.cur_mode & DEC_MODE )
                {
                    if(dec_ctrl.dec_status == DEC_APP_CLOSE){
                        dec_ctrl.dec_status = DEC_APP_IDLE;
                        dec_ctrl.ksframe_en = 0;
                    }else{
                        if(dec_ctrl.dec_status != DEC_APP_IDLE){
                            put_msg_fifo(MSG_MODE_SW ,MSG_TO_PROCESS ,DEC_TASK_N) ;
                            break;
                        }
                        dec_ctrl.dec_status = DEC_APP_IDLE;
                        dec_ctrl.ksframe_en = 0;
                    }
#if LANE_DECT_EN
                    if(g_ldet_ptr && sys_info.lane_dect_en)
                    {
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

                if(usb_is_chargeing())
                {
                    puts("\n usb_is_chargeing \n");
                    if(mode_ctrl.cur_mode & USB_MODE)
                    {
                        mode_ctrl.cur_mode &= ~mode_list[mode_ctrl.last_index];
                        put_msg_fifo(MSG_MODE_SW ,MSG_TO_PROCESS ,USB_TASK_N) ;
                        break;
                    }
                }


                mode_ctrl.mode_index++;
                if(mode_ctrl.mode_index >= sizeof(mode_list)){
                    mode_ctrl.mode_index = 1;
                }

                mode_ctrl.cur_mode &= ~mode_list[mode_ctrl.last_index];
                mode_ctrl.cur_mode |= mode_list[mode_ctrl.mode_index];
                mode_ctrl.last_index = mode_ctrl.mode_index;

                if( mode_ctrl.cur_mode & (ENC_MODE | TPH_MODE) ){

                    put_msg_fifo(MSG_MODE_SW ,MSG_TO_PROCESS ,ENC_TASK_N) ;

                    if(get_parking_status())
                    {
                        puts("\n\n\nparking enc mode\n\n\n");
                        lcd_get_size(&scr_w, &scr_h);

                        if(g_dsc1_hdl == NULL){
                                    disp1_info.des_w = scr_w;
                                    disp1_info.des_h = scr_h;
                                    disp1_info.x = 0;
                                    disp1_info.dsc_buf = dsc1_all_ptr;
                                    g_dsc1_hdl = dsc_open(&disp1_info,(IMGLAYER *)dsc_imb_open_tmp(&disp1_info));
                                    if(g_dsc1_hdl){
                                        if(dsc_start(g_dsc1_hdl) == FALSE){
                                            puts("\n----dsc1 start err\n");
                                            break;
                                        }
                                    }else{
                                        puts("\n----dsc1 open err\n");
                                        break;
                                    }
                        g_double_dsc_status = DOUB_DSC_BACK_WIN;
                          puts("\n-----------  \n");
                        }
                        break;
                    }

                    if(g_dsc0_hdl == NULL)
                    {
                        lcd_get_size(&scr_w, &scr_h);

                        disp0_info.des_w = scr_w;
                        disp0_info.des_h = scr_h;

                        g_dsc0_hdl = dsc_open(&disp0_info,(IMGLAYER *)dsc_imb_open_tmp(&disp0_info));
                        if(g_dsc0_hdl){
                            if(dsc_start(g_dsc0_hdl) == FALSE){
                                puts("\n----dsc0 start err\n");
                            }
                        }else{
                            puts("\n----dsc0 open err\n");
                        }
                        updata_menu_cfg();
                    }
                    g_double_dsc_status = DOUB_DSC_FRONT_WIN;

                    if(mode_ctrl.cur_mode & ENC_MODE){
                        if(enc_ctrl.enc1_en && (g_dsc1_hdl == NULL)){
                            disp1_info.des_w = SMALL_WIN_WIDTH;
                            disp1_info.des_h = SMALL_WIN_HEIGH;
                            disp1_info.x = SMALL_WIN_START_X;
                            disp1_info.dsc_buf = dsc1_small_ptr;
                            g_dsc1_hdl = dsc_open(&disp1_info,(IMGLAYER *)dsc_imb_open_tmp(&disp1_info));
                            if(g_dsc1_hdl){
                                if(dsc_start(g_dsc1_hdl) == FALSE){
                                    puts("\n----dsc1 start err\n");
                                }
                            }else{
                                puts("\n----dsc1 open err\n");
                            }
                            g_double_dsc_status = DOUB_DSC_BIG_SMALL;
                        }
                    }

                    put_msg_fifo(MSG_KEY_MODE,  MSG_TO_PROCESS,UI_DISP_N);
                }
                if(mode_ctrl.cur_mode & DEC_MODE){

#if LANE_DECT_EN
                    if(g_ldet_ptr){//切换到解码模式时，停止车道检测
                        lane_dect_stop(g_ldet_ptr);
                    }
#endif

                    put_msg_fifo(MSG_MODE_SW ,MSG_TO_PROCESS ,DEC_TASK_N) ;
                }

            break;
            case MSG_PARKING_SWITCH:
                if(get_parking_status())
                {
                    key_disable(1);
                    screen_on();
                    if(mode_ctrl.cur_mode & DEC_MODE)
                    {
                         if(dec_ctrl.dec_type == PKG_TYPE_AVI)
                         {
                             if((dec_ctrl.dec_status == DEC_APP_RUNNING )
                                || dec_ctrl.dec_status == DEC_APP_FF
                                || dec_ctrl.dec_status == DEC_APP_FB
                                )
                                {
                                    u8 cnt = 0;
                                    put_msg_fifo(MSG_MODE_SW ,MSG_TO_PROCESS ,DEC_TASK_N);
//                                    os_time_dly(10);
                                    while((dec_ctrl.dec_status != DEC_APP_READY) && (cnt<20))
                                    {
                                        cnt++;
                                        os_time_dly(50);
                                    }

                                }

                         }
                         os_time_dly(20);
                    }
                    if(mode_ctrl.cur_mode & ENC_MODE)
                    {
                        lcd_get_size(&scr_w, &scr_h);
                        if(g_dsc0_hdl)
                        {
                            if(dsc_stop(g_dsc0_hdl) == FALSE){
                                puts("stop dsc err\n");
                                break;
                            }else{
                                if(dsc_close(g_dsc0_hdl) == FALSE){
                                    puts("close dsc err\n");
                                    break;
                                }else{
                                    g_dsc0_hdl = NULL;
                                }
                            }
                        }
                        if(g_dsc1_hdl)
                        {
                            if(dsc_stop(g_dsc1_hdl) == FALSE){
                                puts("stop dsc err\n");
                                break;
                            }else{
                                if(dsc_close(g_dsc1_hdl) == FALSE){
                                    puts("close dsc err\n");
                                    break;
                                }else{
                                    g_dsc1_hdl = NULL;
                                }
                            }
                        }
                        if(g_dsc1_hdl == NULL){
                            disp1_info.des_w = scr_w;
                            disp1_info.des_h = scr_h;
                            disp1_info.x = 0;
                            disp1_info.dsc_buf = dsc1_all_ptr;
                            g_dsc1_hdl = dsc_open(&disp1_info,(IMGLAYER *)dsc_imb_open_tmp(&disp1_info));
                            if(g_dsc1_hdl){
                                if(dsc_start(g_dsc1_hdl) == FALSE){
                                    puts("\n----dsc1 start err\n");
                                    break;
                                }
                            }else{
                                puts("\n----dsc1 open err\n");
                                break;
                            }
                            g_double_dsc_status = DOUB_DSC_BACK_WIN;
                            puts("\n-----------  \n");
                        }
                    }
                    else
                    {
                        mode_ctrl.mode_index = 0;
                        put_msg_fifo(MSG_MODE_SW ,MSG_TO_SELF ,0);
                    }
                    put_msg_fifo(MSG_PARKING_SWITCH ,MSG_TO_PROCESS ,UI_DISP_N) ;
                }
                else
                {
                    key_disable(0);
                    if((mode_ctrl.cur_mode & ENC_MODE) )//&& enc_ctrl.enc0_status == ENC_APP_RUNNING)
                    {
                        put_msg_fifo(MSG_KEY_PREV,MSG_TO_SELF,0) ;
                        os_time_dly(10);
                        put_msg_fifo(MSG_KEY_MODE ,MSG_TO_PROCESS ,UI_DISP_N) ;
                    }
                    else
                    {
                        mode_ctrl.mode_index = 0;
                        put_msg_fifo(MSG_MODE_SW ,MSG_TO_SELF ,0);
                    }
                    put_msg_fifo(MSG_PARKING_SWITCH ,MSG_TO_PROCESS ,UI_DISP_N) ;
                }
            break;
            case MSG_DEBUG_LCD:
                if(g_dsc0_hdl == NULL){
                    lcd_get_size(&scr_w, &scr_h);
                    disp0_info.des_w = scr_w;
                    disp0_info.des_h = scr_h;
                    g_dsc0_hdl = dsc_open(&disp0_info,(IMGLAYER *)dsc_imb_open_tmp(&disp0_info));
                    if(g_dsc0_hdl){
                        if(dsc_start(g_dsc0_hdl) == FALSE){
                            puts("\n----dsc1 start err\n");
                            break;
                            }
                        }else{
                            puts("\n----dsc1 open err\n");
                            break;
                        }
                        g_double_dsc_status = DOUB_DSC_FRONT_WIN;
                        puts("\n-----------  \n");
                }
                else
                {
                    if(dsc_stop(g_dsc0_hdl) == FALSE){
                        puts("stop dsc err\n");
                        break;
                    }else{
                        if(dsc_close(g_dsc0_hdl) == FALSE){
                            puts("close dsc err\n");
                            break;
                        }else{
                            g_dsc0_hdl = NULL;
                        }
                    }
                }
            break;
        }
    }


}


void sw_key_msg(u8 key_msg)
{
    if( (mode_ctrl.cur_mode & TPH_MODE) == 0 ){
        dac_api_play_ktone();
    }else{
        if(sys_status.menu_status == 1){
            dac_api_play_ktone();
        }else if(key_msg != MSG_KEY_OK){
            dac_api_play_ktone();
        }
    }

    put_msg_fifo(key_msg ,MSG_TO_PROCESS ,MODE_SW_N) ;
}


u8 get_current_mode(void)
{
    return mode_ctrl.cur_mode;
}

void init_sys_status(void)
{
    sys_status.menu_status = 0;
    sys_status.sys_err_status = 0;
    sys_status.screen_off_cnt = 1;
    sys_status.gsensor_online = 1;
    memset(&mode_ctrl, 0x00, sizeof(MODE_SW_T));

//    mode_ctrl.cur_mode |= SYS_MODE;
//    mode_ctrl.cur_mode |= ENC_MODE;//开机默认模式
//    mode_ctrl.mode_index = 1;
//    mode_ctrl.last_index = mode_ctrl.mode_index;

}

void start_run(void)
{
    mode_ctrl.cur_mode |= SYS_MODE;
    mode_ctrl.cur_mode |= ENC_MODE;//开机默认模式
    mode_ctrl.mode_index = 1;
    mode_ctrl.last_index = mode_ctrl.mode_index;
}










