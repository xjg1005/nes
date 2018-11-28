#include "enc_mode.h"
#include "hwi.h"
#include "os_api.h"
#include "mango_dev_spi.h"
#include "mango_dev_spi0flash.h"
#include "imb_module.h"
#include "imc_if.h"
#include "jpeg_enc_if.h"
#include "dsc_ram.h"
#include "msg.h"
#include "dev_ctrl.h"
#include "drv_manage.h"
#include "app_cfg.h"
#include "car_fs_api.h"
#include "sdfile.h"
#include "vm_api.h"
#include "timer.h"
#include "ves.h"
#include "wfile_serve.h"
#include "lcd.h"
#include "mode_switch.h"
#include "dsc.h"
#include "my_ui.h"
#include "menu.h"
#include "wdt.h"

#define MOVE_STOP_SEC       30  //移动侦测时，如果静止20S，则停止录像

ENC_CTRL_T enc_ctrl;
AV_IN_CTRL_T g_av_in;


//#ifdef DOUBLE_VEDIO
//#define SPEC_VAL   (1280)
//#else
//#define SPEC_VAL   1280
//#endif

//#define SPEC_VAL   1280
#define SPEC_VAL   1920+1

#ifdef DOUBLE_VEDIO

//const u16 video_xsize[] = {SPEC_VAL, 1280,640,SPEC_VAL};
//const u16 video_ysize[] = {720, 720, 480,720};
//
//const u16 video_fake_xsize[] = {1680, 1280,640,1680};
//const u16 video_fake_ysize[] = {960, 720, 480,960};
const u16 video_xsize[] = {1920, 1280, 640, 1920};
const u16 video_ysize[] = {1088, 720, 480, 1088};

const u16 video_fake_xsize[] = {1920, 1280, 640, 1920};
const u16 video_fake_ysize[] = {1088, 720, 480, 1088};
//const u16 video_fake_xsize[] = {1280, 640,720,1280};
//const u16 video_fake_ysize[] = {720, 480, 480,720};
//#endif

#else
const u16 video_xsize[] = {1920,SPEC_VAL, 640,720,SPEC_VAL};
const u16 video_ysize[] = {1088,720, 480, 480,720};

const u16 video_fake_xsize[] = {1920,SPEC_VAL, 640,720,SPEC_VAL};
const u16 video_fake_ysize[] = {1088,720, 480, 480,720};

#endif

//最大支持1080P
//const u16 photo_xsize[] = {1280, 1920, 2048, 2592}; //1 2 3 5M
//const u16 photo_ysize[] = {720, 1080, 1536, 1944};

const u16 photo_xsize[] = {1280, 1600, 1920, 640};//1 2  3m
const u16 photo_ysize[] = {720, 912, 1080, 480};


             //更改时间水印的颜色 JOHNSON 20160321
#define SENSOR_OSD_COLOR        0xe20095//0x008080//0xe20095


//函数声明
void timetosavefile(void *ptr);
void motion_det_func(void *ptr);
void enc_usb_charge(void *ptr);
void enc_skip_frame(void *ptr);

extern void timer2_init();


#ifdef TEST_MEM

u32 g_mem_flag = 0;
u32 g_mem_offset = 0;
u32 g_mem_test_buf[0x100];
u32 g_mem_test_len[0x100];
void malloc_hook(void * addr, u32 len)
{
    u32 i;

    if(g_mem_flag == 0) return;

    for(i =0;i < 0x100;i++)
    {
        if(g_mem_test_buf[i] == 0){
            g_mem_test_buf[i] = addr;
            g_mem_test_len[i] = len;
            break;
        }
    }
}

void free_hook(void * addr)
{
    u32 i;

    if(g_mem_flag == 0) return;

    for(i =0;i < 0x100;i++)
    {
        if(g_mem_test_buf[i] == addr){
            g_mem_test_buf[i] = 0;
            break;
        }
    }
}

void malloc_printf()
{
    u32 i;
    u32 cnt = 0;

    for(i =0;i < 0x100;i++)
    {
        if(g_mem_test_buf[i]){
            put_u32hex(g_mem_test_buf[i]);
            put_u32hex(g_mem_test_len[i]);
            cnt += g_mem_test_len[i];

        }
    }

    puts("\n\ntotal cnt: ");
    put_u32hex(cnt);
    memset(g_mem_test_buf, 0 ,sizeof(g_mem_test_buf));
    memset(g_mem_test_len, 0 ,sizeof(g_mem_test_len));
    g_mem_offset = 0;
}

#else

void malloc_hook(void * addr, u32 len)
{

}

void free_hook(void * addr)
{

}

#endif

void en0_set_time_lapse_photography(u32 ms)
{
    if(enc_ctrl.time_lapse_photography_ms == ms)
        return;

    if(ms == 0)
    {
        enc_ctrl.time_lapse_photography_ms = 0;
        enc_ctrl.discard_aud = 0;
    }
    else
    {
        enc_ctrl.time_lapse_photography_ms = ms;
        enc_ctrl.discard_aud = 1;
    }

    put_msg_fifo(MSG_SW_SIZE ,MSG_TO_PROCESS ,ENC_TASK_N) ;
}

volatile u8 usb_charg_status = 0;
u8 get_usb_charg_status()
{
    return usb_charg_status;
}
void set_usb_charg_status( u8 set)
{
    usb_charg_status = set;
}
void clear_usb_charg_status()
{
//    printf("clear_usb_charg_status******\n");
    usb_charg_status = FALSE;
}

void usb_charg_off_stop_video()
{
    #if 0
//    clear_usb_charg_status();
    put_msg_fifo(MSG_KEY_OK ,MSG_TO_PROCESS ,MODE_SW_N) ;
    puts("\n 000000000 a\n");
    if(enc_ctrl.enc1_status == ENC_APP_SAVE){
        puts("\n 000000000 b\n");
        put_msg_fifo(MSG_KEY_OK ,MSG_TO_PROCESS ,MODE_SW_N) ;
    }

    #else
    put_msg_fifo(MSG_KEY_CLOSE ,MSG_TO_PROCESS ,MODE_SW_N) ;
    #endif
}

void usb_off_stop_video_cnt(void *parm)
{
    static u8 cnt = 0;

    if(usb_charg_status == 0)
    {
        cnt = 0;
        if(usb_is_chargeing())
            usb_charg_status = 1;
        else
        {
            usb_charg_status = 0;
        }
    }
    else if(usb_charg_status == 1)
    {
        if(usb_is_chargeing())
        {
            usb_charg_status = 1;
            cnt = 0;
        }

        else
        {
            usb_charg_status = 2;
        }
    }
    else if((usb_charg_status == 2)&& (usb_is_chargeing() == TRUE))
    {
        usb_charg_status = 1;
        cnt = 0;
    }
//    printf("usb_charg_status = %d\n",usb_charg_status);
//    printf("usb_charg_status = %d\n",usb_charg_status);
    if ((usb_charg_status == 2)&& (usb_is_chargeing() == FALSE)&& (get_current_mode() & ENC_MODE)
//        && ((enc_ctrl.enc1_status == ENC_APP_RUNNING)
//        || (enc_ctrl.enc0_status == ENC_APP_RUNNING))
        )
    {
//        printf("cnt =222 == %d\n",cnt);
        cnt++;
        if (cnt > 5)//10 //拔USB后5s关机 //JOHNSON 20160321
        {
            cnt = 0;
            put_msg_fifo(MSG_KEY_CLOSE ,MSG_TO_PROCESS ,MODE_SW_N) ;
        }
    }

}

///////////////////////////////////////////////////
//提示空间不足，在自动关机关的情况下，倒计时关机
static u8 disp_note_m154 = FALSE;
u8 get_disp_note_m154_status()
{
    return disp_note_m154;
}

void set_disp_note_m154_status()
{
    disp_note_m154 = TRUE;
}

void clear_disp_note_m154_status()
{
    disp_note_m154 = FALSE;
}

void disp_note_m154_power_off(void *parm)
{
    if(get_parking_status())   //倒车过程不关机
    {
        return;
    }
    if (sys_status.powerdown_flag )
        return;
    if (sys_info.auto_poweroff)
    {
        if (get_disp_note_m154_status())
            clear_disp_note_m154_status();
        return;
    }

    static u8 cnt = 0;
    if(get_current_mode() & ENC_MODE)
    {
        if (get_disp_note_m154_status())
        {
            if (cnt < 50)
                cnt++;
            if (cnt > 30)//30s
            {
                cnt = 0;
                clear_disp_note_m154_status();
                put_msg_fifo(MSG_KEY_CLOSE ,MSG_TO_PROCESS ,MODE_SW_N);
            }
        }
    }
    else
    {
        cnt = 0;
        clear_disp_note_m154_status();
    }
}

void disp_note_m154_timer_reg()
{
    timer_reg(500, disp_note_m154_power_off, NULL);
}




///////////////////////////////////////////////////

void enc_app_task(void * p_arg)
{
    u32 err;
    void * pFile;
    u32 msg ;
    u32 ret;
    msg_t *msg_d;
    msg_devicechange_t *devsta;

    u8 * cache_ptr;
    u32 i;

    wfsv_info_t wfsv_info ;
    imc_info_t imc_info;
    ves_t *ves_enc0;
    ves_t *ves_enc1 ;
    ves_t *ves_photo ;
    ves_encode_data_t enc0_info ,enc1_info ,photo_info;
    timer_handle tsave_hdl = NULL ;
    u32 vid_fps;

    u16 scr_w,scr_h;
    u32 usb_sw;
    u32 av_in_offline;
    void * tmp_fp;



    memset(&enc_ctrl,0,sizeof(ENC_CTRL_T));

    err = dev_reg_msg(NULL);

    wfsv_info.fs_write = mutex_f_Write;
    wfsv_info.wfsv_task_prio = WFILE_TASE_PRIO;
    if(wfsv_init(&wfsv_info) == FALSE){
        puts("\nwfsv init err\n");
        while(1);
    }

    //enc0
    enc0_info.asize = 100*1024 ;
    enc0_info.vsize = VID0_FBUF_SIZE;
    enc0_info.vedio = isp0_name ;
    enc0_info.format = VES_FORMAT_AVI ;

    enc0_info.drop_frame_en = 0;
    enc0_info.org_fps = 0;
    enc0_info.fr = 30;

    enc0_info.time_lapse_photography_ms = 0;
    enc0_info.discard_aud = 0;

    printf("sys_info.reso_ratio %d \n",sys_info.reso_ratio);
    enc0_info.h = video_ysize[sys_info.reso_ratio];
    enc0_info.w = video_xsize[sys_info.reso_ratio];
    if(enc0_info.w >= SPEC_VAL){

        enc0_info.w = video_xsize[sys_info.reso_ratio] - g_spec_val;

        enc0_info.fake_h = video_fake_ysize[sys_info.reso_ratio];
        enc0_info.fake_w = video_fake_xsize[sys_info.reso_ratio];
    }else{
        enc0_info.fake_h = 0;
        enc0_info.fake_w = 0;
    }

printf("enc0_info.h %d \n",enc0_info.h);
printf("enc0_info.w  %d \n",enc0_info.w );
    enc0_info.q = 5;
    enc0_info.abr_en = 1;
    enc0_info.abr_kbps = 16000;
    enc0_info.samplerate = 8000 ;
    enc0_info.osd_en = sys_info.date_label;
    enc0_info.vid_buf = g_vid0_buf;

    enc0_info.osd_size = 0;
    enc0_info.osd_format = 0;
    enc0_info.osd_yuv = SENSOR_OSD_COLOR;

    enc0_info.osd_str = 0;
    enc0_info.osd_w = 0;
    enc0_info.osd_h = 0;
    enc0_info.osd_matrix = 0;
    enc0_info.osd_matrix_addr = 0;
    enc0_info.total_len = 0;
    enc0_info.dynamic_huffman_en = 0;
    enc0_info.dynamic_huffman_prio = 0;//LIB_PRIO_DYNHUFF;

    //enc1
    memcpy(&enc1_info,&enc0_info,sizeof(enc0_info)) ;
    enc1_info.vedio = isp1_name ;
    enc1_info.h = 480 ;
    enc1_info.w = 720;
    enc1_info.fake_h = 0;
    enc1_info.fake_w = 0;
    enc1_info.abr_en = 1;
    enc1_info.abr_kbps = 4000;

    enc1_info.drop_frame_en = 0;
    enc1_info.org_fps = 0;

    enc1_info.time_lapse_photography_ms = 0;
    enc1_info.discard_aud = 0;

    enc1_info.format = VES_FORMAT_AVI ;
    enc1_info.vsize = VID1_FBUF_SIZE;
    enc1_info.osd_en = sys_info.date_label;
    enc1_info.vid_buf = g_vid1_buf;
    enc1_info.dynamic_huffman_en = 0;
    //photo
    photo_info.vsize = VID0_FBUF_SIZE;
    photo_info.vedio = isp0_name ;
    photo_info.format = VES_FORMAT_JPEG ;

    photo_info.osd_en = sys_info.photo_date_label;
    photo_info.h = photo_ysize[sys_info.photo_reso_ratio];
    photo_info.w = photo_xsize[sys_info.photo_reso_ratio];
    photo_info.fake_h = 0;
    photo_info.fake_w = 0;
    photo_info.q = 5;
    photo_info.abr_en = 0;
    photo_info.abr_kbps = 16000;
    photo_info.vid_buf = g_vid0_buf;
    photo_info.dynamic_huffman_en = 0;
    photo_info.drop_frame_en = 0;
    photo_info.org_fps = 0;
    photo_info.osd_size = 0;
    photo_info.osd_format = 0;
    photo_info.osd_yuv = SENSOR_OSD_COLOR;

    photo_info.time_lapse_photography_ms = 0;
    photo_info.discard_aud = 0;


    photo_info.osd_str = 0;
    photo_info.osd_w = 0;
    photo_info.osd_h = 0;
    photo_info.osd_matrix = 0;
    photo_info.osd_matrix_addr = 0;
    photo_info.total_len = 0;


    ves_enc0 = ves_open() ;
    ves_enc1 = ves_open() ;
    ves_photo = ves_open() ;
    lcd_get_size(&scr_w, &scr_h);

    enc_ctrl.enc_type = PKG_TYPE_AVI;
    enc_ctrl.osd_en = sys_info.date_label;
    enc_ctrl.pic_osd_en = sys_info.photo_date_label;

    enc_ctrl.crec0_time = 60;
    enc_ctrl.crec1_time = 60;
    enc_ctrl.is_cycle_rec0 = (enc_ctrl.crec0_time > 0) ? 1 : 0;
    enc_ctrl.is_cycle_rec1 = (enc_ctrl.crec1_time > 0) ? 1 : 0;

    timer_reg(500, motion_det_func ,&enc_ctrl) ;
    enc_ctrl.motion_start_rec = 0;
    enc_ctrl.motion_detect = sys_info.moving_detect;
    enc_ctrl.dis_voice = (sys_info.mic_mute)?0:1;
    ves_ioctrl(ves_enc0, &enc_ctrl.dis_voice,VES_CMD_ENC_DIS_VOICE) ;  //解决录音问题  san
    usb_sw = 0;
    av_in_offline = 0;

    enc_ctrl.skip_flag = 0;
    enc_ctrl.skip_cnt = 0;
    enc_ctrl.skip_fps = enc0_info.fr;

    timer_reg(20, enc_usb_charge ,&enc_ctrl) ;

    timer_reg(50, enc_skip_frame ,&enc_ctrl) ;

    enc_ctrl.ves_enc = ves_enc0;

    while(1)
    {
        msg = get_msg();
        if(msg  > (u32)0x2000000)
        {
            msg_d = msg ;
            msg = msg_d->type ;
            printf("---------msg = %d ----------\n",msg);
        }


        switch(msg)
        {
            case MSG_MODE_SW:

                if((mode_ctrl.cur_mode & (ENC_MODE | TPH_MODE) ) == 0){
                    break;
                }

                if(mode_ctrl.cur_mode & ENC_MODE)
                {
                    enc_ctrl.enc_type = PKG_TYPE_AVI;
                }
                else if(mode_ctrl.cur_mode & TPH_MODE)
                {
                    enc_ctrl.enc_type = PKG_TYPE_JPG;
                }

                if(enc_ctrl.enc_type == PKG_TYPE_AVI)
                {
                    if((enc_ctrl.enc0_status == ENC_APP_STOP) || (enc_ctrl.enc0_status == ENC_APP_OPEN))
                    {

                        enc_ctrl.ves_enc = NULL;

                        ret = ves_ioctrl(ves_enc0, NULL ,VES_CMD_ENC_CLOSE) ;
                        if(ret != VES_ERR_NONE){
                            puts("ves enc close err\n");
                            break;
                        }
                        enc_ctrl.enc0_status = ENC_APP_CLOSE;

                        if(ves_close(ves_enc0) == FALSE){
                            puts("ves0 close err\n");
                            break;
                        }
                        ves_enc0 = NULL;

                        puts("\nves enc close ok\n");
                    }

                    //if(enc_ctrl.enc1_en && (enc_ctrl.enc1_status == ENC_APP_STOP))
                    if((enc_ctrl.enc1_status == ENC_APP_STOP) || (enc_ctrl.enc1_status == ENC_APP_OPEN))
                    {
                        ret = ves_ioctrl(ves_enc1, NULL ,VES_CMD_ENC_CLOSE) ;
                        if(ret != VES_ERR_NONE){
                            puts("ves enc1 close err\n");
                            break;
                        }
                        enc_ctrl.enc1_status = ENC_APP_CLOSE;

                        if(ves_close(ves_enc1) == FALSE){
                            puts("ves1 close err\n");
                            break;
                        }
                        ves_enc1 = NULL;

                        puts("ves enc1 close ok\n");
                    }
                }
                else//JPG
                {
                    if( (enc_ctrl.enc0_status == ENC_APP_RUNNING) || (enc_ctrl.enc0_status == ENC_APP_OPEN) )
                    {
                        ret = ves_ioctrl(ves_photo, NULL ,VES_CMD_ENC_CLOSE) ;
                        if(ret != VES_ERR_NONE){
                            puts("ves enc close err\n");
                            break;
                        }
                        enc_ctrl.enc0_status = ENC_APP_CLOSE;

                        if(ves_close(ves_photo) == FALSE){
                            puts("ves0 close err\n");
                            break;
                        }
                        ves_photo = NULL;
                        photo_info.vedio = isp0_name ;

                        puts("\nves enc close ok\n");
                    }
                }

                if(enc_ctrl.enc0_status == ENC_APP_CLOSE){

                    //if((enc_ctrl.enc_type == PKG_TYPE_AVI) && enc_ctrl.enc1_en){
                    if((enc_ctrl.enc_type == PKG_TYPE_AVI) && (enc_ctrl.enc1_status != ENC_APP_IDLE)){
                        if(enc_ctrl.enc1_status == ENC_APP_CLOSE){

                            enc_ctrl.skip_flag = 0;
                            enc_ctrl.skip_cnt = 0;
                            enc_ctrl.motion_detect = 0;
                            enc_ctrl.motion_start_rec = 0;

                            av_in_offline = 0;
                            enc_ctrl.usb_charge_flag = 0;
                            enc_ctrl.usb_charge_cnt = 0;
                            enc_ctrl.pic_inout = 0;
                            enc_ctrl.enc_inout = 0;

                            if(usb_sw){
                                mode_ctrl.cur_mode |= USB_MODE;
                                usb_sw = 0;
                                put_msg_fifo(MSG_USB_MENU ,MSG_TO_PROCESS ,UI_DISP_N);
                            }
                            else
                                put_msg_fifo(MSG_MODE_SW ,MSG_TO_PROCESS ,MODE_SW_N) ;
                        }else{
                            break;
                        }
                    }else{

                        enc_ctrl.skip_flag = 0;
                        enc_ctrl.skip_cnt = 0;
                        enc_ctrl.motion_detect = 0;
                        enc_ctrl.motion_start_rec = 0;

                        av_in_offline = 0;
                        enc_ctrl.usb_charge_flag = 0;
                        enc_ctrl.usb_charge_cnt = 0;
                        enc_ctrl.pic_inout = 0;
                        enc_ctrl.enc_inout = 0;

                        if(usb_sw){
                            mode_ctrl.cur_mode |= USB_MODE;
                            usb_sw = 0;
                            put_msg_fifo(MSG_USB_MENU ,MSG_TO_PROCESS ,UI_DISP_N);
                        }
                        else{
                            put_msg_fifo(MSG_MODE_SW ,MSG_TO_PROCESS ,MODE_SW_N) ;
                        }

                    }
                }

                if(usb_sw){
                    if(enc_ctrl.enc0_status == ENC_APP_IDLE){
                        mode_ctrl.cur_mode |= USB_MODE;
                        usb_sw = 0;
                        av_in_offline = 0;

                        enc_ctrl.skip_flag = 0;
                        enc_ctrl.skip_cnt = 0;
                        enc_ctrl.motion_detect = 0;
                        enc_ctrl.motion_start_rec = 0;

                        enc_ctrl.usb_charge_flag = 0;
                        enc_ctrl.usb_charge_cnt = 0;
                        enc_ctrl.pic_inout = 0;
                        enc_ctrl.enc_inout = 0;

                        put_msg_fifo(MSG_USB_MENU ,MSG_TO_PROCESS ,UI_DISP_N);
//                        put_msg_fifo(MSG_MODE_SW ,MSG_TO_PROCESS ,MODE_SW_N) ;
                    }
                }
                else{
                    if(enc_ctrl.enc0_status == ENC_APP_IDLE){
                        enc_ctrl.motion_detect = sys_info.moving_detect;
                    }
                }

            break;

            case MSG_DEVICECHANGE:
                if (sys_status.powerdown_flag)   //20151118
                {
                    free(msg_d);
                    break;
                }

                if((mode_ctrl.cur_mode & (ENC_MODE | TPH_MODE) ) == 0){
                    free(msg_d);
                    break;
                }

                puts("encode dev change\n");
                devsta = msg_d->ptr;

                ret = usb_slave_msg_ctrl(devsta);
                if(ret)
                {
                    if(ret == 1){ //USB插入
                        if(get_menu_statu()) //菜单状态 先退出菜单
                            put_msg_fifo(MSG_MENU_EXIT ,MSG_TO_PROCESS ,UI_DISP_N) ;
                        put_msg_fifo(MSG_USB_FLAG ,MSG_TO_SELF ,0) ;
                    }else if(ret == 3){//充电器插入

                        if( (enc_ctrl.enc_type == PKG_TYPE_AVI) && (sys_status.sys_gsensor_wakeup != 1) )
                        {
                            if((enc_ctrl.enc0_status == ENC_APP_STOP) || (enc_ctrl.enc0_status == ENC_APP_IDLE)
                                || (enc_ctrl.enc0_status == ENC_APP_CLOSE))
                            {
                                if(g_startup_finish == 0){//插入充电器开机
                                    enc_ctrl.usb_charge_cnt = 0;
                                    enc_ctrl.usb_charge_flag = 1;

                                }else{
                                    if(get_menu_statu()==0)
                                        put_msg_fifo(MSG_KEY_OK ,MSG_TO_PROCESS ,MODE_SW_N);
                                }
                            }
                        }
                    }else if(ret == 4){//充电器拔开

                        if( (enc_ctrl.enc_type == PKG_TYPE_AVI) && (sys_status.sys_gsensor_wakeup != 1) )
                        {
                            if((enc_ctrl.enc0_status == ENC_APP_RUNNING) || (enc_ctrl.enc0_status == ENC_APP_SAVE))
                            {
                                enc_ctrl.usb_charge_cnt = 0;
                                enc_ctrl.usb_charge_flag = 0;
                                enc_ctrl.is_cycle_rec0 = 0;
                                enc_ctrl.is_cycle_rec1 = 0;
                                put_msg_fifo(MSG_KEY_OK ,MSG_TO_PROCESS ,MODE_SW_N) ;

                                if(enc_ctrl.enc1_status == ENC_APP_SAVE){
                                    put_msg_fifo(MSG_KEY_OK ,MSG_TO_PROCESS ,MODE_SW_N) ;
                                }
                            }
                        }
                    }

                    free(msg_d);
                    break;
                }

                file_sel_msg_ctrl(msg,devsta);
                printf("\nfind dev  %s--%d \n",devsta->hdev->dev_io->name, devsta->sta);
                if(devsta->sta ==DEV_ONLINE && devsta->hdev->dev_io->device_type == DEV_LOGIC_DISK  )
                {
                    //puts("begin enc \n") ;
                    //put_msg_fifo(MSG_INIT_FILE,MSG_TO_SELF,0) ;
                }
                free(msg_d);

            break;

            case MSG_USB_FLAG:

                usb_sw = 1;

                if(enc_ctrl.enc_type == PKG_TYPE_AVI)
                {
                    enc_ctrl.motion_detect = 0;
                    enc_ctrl.motion_start_rec = 0;

                    if(enc_ctrl.enc0_status == ENC_APP_RUNNING)
                    {
                        puts("stop 0\n") ;
                        ret = ves_ioctrl(ves_enc0,0,VES_CMD_ENC_STOP) ;
                        if(ret != VES_ERR_NONE){
                            puts("\nstop 0 err\n");
                            break;
                        }
                        enc_ctrl.skip_flag = 0;
                        enc_ctrl.skip_cnt = 0;

                        enc0_info.fp = 0 ;
                        enc_ctrl.is_cycle_rec0 = 0;
                        enc_ctrl.enc0_status = ENC_APP_STOP;
                        puts("\nstop 0 succ\n") ;

                    }

                    if(enc_ctrl.enc1_status == ENC_APP_RUNNING)
                    {
                        printf("stop 1\r\n") ;
                        ret = ves_ioctrl(ves_enc1,0, VES_CMD_ENC_STOP) ;
                        if(ret != VES_ERR_NONE){
                            puts("\nstop 1 err\n");
                            put_u32hex(ret);
                            break;
                        }
                        enc1_info.fp = 0 ;
                        enc_ctrl.is_cycle_rec1 = 0;
                        enc_ctrl.enc1_status = ENC_APP_STOP;
                        puts("stop 1 succ\n");
                    }
                }

                put_msg_fifo(MSG_MODE_SW ,MSG_TO_SELF ,0) ;
            break;


            case MSG_AV_IN_ONLINE:
                if(enc_ctrl.enc_type == PKG_TYPE_AVI)
                {
                    if( (enc_ctrl.enc0_status == ENC_APP_RUNNING) || (enc_ctrl.enc0_status == ENC_APP_SAVE ) )
                    {
                        put_msg_fifo(MSG_ENC_STOP,MSG_TO_SELF,0) ;
                        clear_rec_cnt();//录像计时清零
                    }
                    if( (enc_ctrl.enc0_status) && (enc_ctrl.enc0_status != ENC_APP_STOP)
                       && (enc_ctrl.enc0_status != ENC_APP_CLOSE)){
                        put_msg_fifo(MSG_INIT_FILE,MSG_TO_SELF,0) ;
                    }
#if 0
                    g_double_dsc_status = DOUB_DSC_BACK_WIN;
                    put_msg_fifo(MSG_KEY_PREV,MSG_TO_SELF,0) ;
#else
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
//
//                    if(g_dsc0_hdl == NULL)
//                    {
//                        disp0_info.des_w = scr_w;
//                        disp0_info.des_h = scr_h;
//                        g_dsc0_hdl = dsc_open(&disp0_info);
//                        if(g_dsc0_hdl){
//                            if(dsc_start(g_dsc0_hdl) == FALSE){
//                                puts("\n----dsc0 start err\n");
//                                break;
//                            }
//                        }else{
//                            puts("\n----dsc0 open err\n");
//                            break;
//                        }
//                    }
                    if(g_dsc1_hdl == NULL){
                        disp1_info.des_w = SMALL_WIN_WIDTH;
                        disp1_info.des_h = SMALL_WIN_HEIGH;
                        disp1_info.x = SMALL_WIN_START_X;
                        disp1_info.dsc_buf = dsc1_small_ptr;
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
                    }
                    g_double_dsc_status = DOUB_DSC_BIG_SMALL;
#endif

                    enc_ctrl.enc1_en = 1;
                }
                else if(enc_ctrl.enc_type == PKG_TYPE_JPG)
                {
                    enc_ctrl.enc1_en = 1;
                }
            break;

            case MSG_AV_IN_OFFLINE:
                if(enc_ctrl.enc_type == PKG_TYPE_AVI)
                {
                    if(enc_ctrl.enc_inout == 0)
                    {
                         if((enc_ctrl.enc0_status == ENC_APP_RUNNING) || (enc_ctrl.enc0_status == ENC_APP_SAVE)){
                            put_msg_fifo(MSG_ENC_STOP,MSG_TO_SELF,0) ;
                            put_msg_fifo(MSG_ENC1_STOP,MSG_TO_SELF,0) ;
                            clear_rec_cnt();//录像计时清零
                        }
                        if( (enc_ctrl.enc0_status) && (enc_ctrl.enc0_status != ENC_APP_STOP)
                           && (enc_ctrl.enc0_status != ENC_APP_CLOSE)){
                            put_msg_fifo(MSG_INIT_FILE,MSG_TO_SELF,0) ;
                        }
                    }

                    g_double_dsc_status = DOUB_DSC_HALF_WIN;
                    put_msg_fifo(MSG_KEY_PREV,MSG_TO_SELF,0) ;
                    enc_ctrl.enc1_en = 0;
                    av_in_offline = 1;
                }
                else if(enc_ctrl.enc_type == PKG_TYPE_JPG)
                {
                    if(g_double_dsc_status == DOUB_DSC_BACK_WIN){
                        put_msg_fifo(MSG_KEY_PREV,MSG_TO_SELF,0) ;
                        av_in_offline = 1;
                    }
                    enc_ctrl.enc1_en = 0;
                }

            break;

            case MSG_SW_ENC_OSD:
                puts("\n MSG_SW_ENC_OSD \n");
                if(enc_ctrl.enc_type == PKG_TYPE_AVI)
                {
                    enc1_info.osd_en = enc_ctrl.osd_en;
                    enc0_info.osd_en = enc_ctrl.osd_en;
                    if(enc_ctrl.osd_en){
                        if(enc_ctrl.enc0_status == ENC_APP_STOP){
                            puts("\n VES_CMD_ENC_OSD_EN \n");
                            ves_ioctrl(ves_enc0, &enc0_info ,VES_CMD_ENC_OSD_EN) ;
                        }
                        if(enc_ctrl.enc1_en && (enc_ctrl.enc1_status == ENC_APP_STOP)){
                            puts("\n VES_CMD_ENC_OSD_EN 2\n");
                            ves_ioctrl(ves_enc1, &enc1_info ,VES_CMD_ENC_OSD_EN);
                        }
                    }else{
                        if(enc_ctrl.enc0_status == ENC_APP_STOP){
                            puts("\n VES_CMD_ENC_OSD_DIS \n");
                            ves_ioctrl(ves_enc0, NULL ,VES_CMD_ENC_OSD_DIS) ;
                        }
                        if(enc_ctrl.enc1_en && (enc_ctrl.enc1_status == ENC_APP_STOP)){
                            puts("\n VES_CMD_ENC_OSD_DIS 2\n");
                            ves_ioctrl(ves_enc1, NULL ,VES_CMD_ENC_OSD_DIS) ;
                        }
                    }
                }
                else if(enc_ctrl.enc_type == PKG_TYPE_JPG)
                {
                    photo_info.osd_en = enc_ctrl.pic_osd_en;
                    if(enc_ctrl.pic_osd_en){
                        if(enc_ctrl.enc0_status == ENC_APP_RUNNING){
                            puts("\n VES_CMD_ENC_OSD_EN \n");
                            ves_ioctrl(ves_photo, &photo_info ,VES_CMD_ENC_OSD_EN) ;
                        }
                    }else{
                        if(enc_ctrl.enc0_status == ENC_APP_RUNNING){
                             puts("\n VES_CMD_ENC_OSD_DIS \n");
                            ves_ioctrl(ves_photo, NULL ,VES_CMD_ENC_OSD_DIS) ;
                        }
                    }
                }
            break;

            case MSG_SW_SIZE:

                if(enc_ctrl.enc_type == PKG_TYPE_AVI)
                {
                    if(enc_ctrl.enc0_status == ENC_APP_STOP)
                    {
                        ret = ves_ioctrl(ves_enc0, NULL ,VES_CMD_ENC_CLOSE) ;
                        if(ret != VES_ERR_NONE){
                            puts("ves enc close err\n");
                            break;
                        }
                        enc_ctrl.enc0_status = ENC_APP_CLOSE;
                        puts("\nves enc close ok\n");
                    }

                    //if(enc_ctrl.enc1_en && (enc_ctrl.enc1_status == ENC_APP_STOP))
                    if(enc_ctrl.enc1_status == ENC_APP_STOP)
                    {
                        ret = ves_ioctrl(ves_enc1, NULL ,VES_CMD_ENC_CLOSE) ;
                        if(ret != VES_ERR_NONE){
                            puts("ves enc1 close err\n");
                            break;
                        }
                        enc_ctrl.enc1_status = ENC_APP_CLOSE;
                        puts("ves enc1 close ok\n");
                    }
                     if (AV_IN_INSERT() == 0)   //如果没插后拉为1080p 160304 san
                    {//

//                        puts("\n ffffffffffffffffffffffffffffffffff 0\n");
                        enc0_info.h = video_ysize[sys_info.reso_ratio];
                        enc0_info.w = video_xsize[sys_info.reso_ratio];
//                            printf("\n enc0_info.w %d\n",enc0_info.w);
//                        printf("\n sys_info.reso_ratio %d\n",sys_info.reso_ratio);
                    }
                    else
                    {
                        static u8 tmp=0;
                        if (sys_info.reso_ratio == 0)
                        {
                            tmp = 1;
                        }
                        else
                        {
                            tmp = sys_info.reso_ratio;
                        }
//                        puts("\n ffffffffffffffffffffffffffffffffff 3\n");

                        enc0_info.h = video_ysize[tmp];
                        enc0_info.w = video_xsize[tmp];
//                         printf("\n enc0_info.w %d\n",enc0_info.w);
//                        printf("\n sys_info.reso_ratio %d\n",sys_info.reso_ratio);

                    }
//                    enc0_info.h = video_ysize[sys_info.reso_ratio];
//                    enc0_info.w = video_xsize[sys_info.reso_ratio];  //如果没插后拉为1080p 160304 san
                    if(enc0_info.w >= SPEC_VAL){

                        enc0_info.w = video_xsize[sys_info.reso_ratio] - g_spec_val;

                        enc0_info.fake_h = video_fake_ysize[sys_info.reso_ratio];
                        enc0_info.fake_w = video_fake_xsize[sys_info.reso_ratio];
                    }else{
                        enc0_info.fake_h = 0;
                        enc0_info.fake_w = 0;
                    }

                    if(enc0_info.abr_en)
                    {
                        if( (enc0_info.w == 640) || (enc0_info.w == 720) ){
                            enc0_info.abr_kbps = 4000;
                        }else{
                            enc0_info.abr_kbps = 16000;
                        }
                    }
                    enc0_info.discard_aud = enc_ctrl.discard_aud;
                    enc0_info.time_lapse_photography_ms = enc_ctrl.time_lapse_photography_ms;
                }
                else if(enc_ctrl.enc_type == PKG_TYPE_JPG)//JPG
                {
                    if(enc_ctrl.enc0_status == ENC_APP_RUNNING)
                    {
                        ret = ves_ioctrl(ves_photo, NULL ,VES_CMD_ENC_CLOSE) ;
                        if(ret != VES_ERR_NONE){
                            puts("ves enc close err\n");
                            break;
                        }
                        enc_ctrl.enc0_status = ENC_APP_CLOSE;
                        puts("\nves enc close ok\n");
                    }
                    photo_info.h = photo_ysize[sys_info.photo_reso_ratio];
                    photo_info.w = photo_xsize[sys_info.photo_reso_ratio];

                    if(strcmp(photo_info.vedio, isp1_name) == 0){
                        photo_info.w = 720;
                        photo_info.h = 480;
                    }
                }
            break;


            case MSG_RW_FILE_ERR:
                puts("\nmsg read write file err \n") ;

                put_msg_fifo(MSG_RW_FILE_ERR ,MSG_TO_PROCESS ,UI_DISP_N) ;
                put_msg_fifo(MSG_ENC_STOP ,MSG_TO_SELF ,0) ;
                if(enc_ctrl.enc1_en){
                    put_msg_fifo(MSG_ENC1_STOP ,MSG_TO_SELF ,0) ;
                }

            break ;

            case MSG_INIT_FILE :

                if(is_phy_sd_online() == FALSE){
                    puts("here\n");
                    if (!get_parking_status())
                    display_note(M156); //无SD卡

                    enc_ctrl.enc_inout = 0;

                    break;
                }

                if(is_sd_online()==FALSE)
                {
                     puts("here 2 \n");
                    if(enc_ctrl.enc_type != PKG_TYPE_JPG)
                    {
                        display_note(M155); //族异常

                        enc_ctrl.enc_inout = 0;
                        break;
                    }

                }
                puts("msg init file \n") ;

                if(enc_ctrl.enc_type == PKG_TYPE_JPG){

                    if( (enc_ctrl.enc0_status == ENC_APP_RUNNING)
                       || (enc_ctrl.enc0_status == ENC_APP_OPEN) ){
                        put_msg_fifo(MSG_TACK_PHOTO_BEGIN,MSG_TO_SELF,0) ;
                    }else{
                        put_msg_fifo(MSG_TACK_PHOTO_INIT,MSG_TO_SELF,0) ;
                    }
                }else{

                    if (sys_info.reso_ratio == 0)
                    {//选择1080分辨率时候，不差后拉，录1080p，插后拉录720p  160304 san
                        put_msg_fifo(MSG_SW_SIZE ,MSG_TO_PROCESS ,ENC_TASK_N);
                        if((enc_ctrl.enc0_status == ENC_APP_RUNNING) || (enc_ctrl.enc0_status == ENC_APP_SAVE)){
                            enc_ctrl.is_cycle_rec0 = 0;
                            put_msg_fifo(MSG_ENC_STOP,MSG_TO_SELF,0) ;
                        }
                        else
                        {
                            if( (enc_ctrl.enc0_status == ENC_APP_IDLE)
                               || (enc_ctrl.enc0_status == ENC_APP_CLOSE)
                               || (enc_ctrl.enc0_status == ENC_APP_STOP))
                               {
                                   put_msg_fifo(MSG_ENC_OPEN,MSG_TO_SELF,0) ;
    //                               puts("\n ffffffffffffffff333fffffffffffff enc open\n");
                               }
                        }

                        if(enc_ctrl.enc1_en)
                        {
                            if((enc_ctrl.enc1_status == ENC_APP_RUNNING) || (enc_ctrl.enc1_status == ENC_APP_SAVE)){
                                enc_ctrl.is_cycle_rec1 = 0;
                                put_msg_fifo(MSG_ENC1_STOP,MSG_TO_SELF,0) ;
                            }
                            else
                            {
                                if( (enc_ctrl.enc1_status == ENC_APP_IDLE)
                                   || (enc_ctrl.enc1_status == ENC_APP_CLOSE)
                                   || (enc_ctrl.enc1_status == ENC_APP_STOP))
                                   {
    //                                   puts("\n fffffffffffffffffffffs333ffffffffff enc1 open\n");
                                       put_msg_fifo(MSG_ENC1_OPEN,MSG_TO_SELF,0) ;
                                   }
                            }
                        }

                    }
                    else
                    {
                        if((enc_ctrl.enc0_status == ENC_APP_RUNNING) || (enc_ctrl.enc0_status == ENC_APP_SAVE)){

                        enc_ctrl.is_cycle_rec0 = 0;

                        put_msg_fifo(MSG_ENC_STOP,MSG_TO_SELF,0) ;
                    }else if(enc_ctrl.enc0_status == ENC_APP_STOP){
                        put_msg_fifo(MSG_ENC_START,MSG_TO_SELF,0) ;
                    }else{
                        if( (enc_ctrl.enc0_status == ENC_APP_IDLE)
                           || (enc_ctrl.enc0_status == ENC_APP_CLOSE))
                       {
                           put_msg_fifo(MSG_ENC_OPEN,MSG_TO_SELF,0) ;
                       }
                    }

                        if(enc_ctrl.enc1_en && (enc0_info.w < 1920))
                        {

                        if((enc_ctrl.enc1_status == ENC_APP_RUNNING) || (enc_ctrl.enc1_status == ENC_APP_SAVE)){

                            enc_ctrl.is_cycle_rec1 = 0;

                            put_msg_fifo(MSG_ENC1_STOP,MSG_TO_SELF,0) ;
                        }else if(enc_ctrl.enc1_status == ENC_APP_STOP){
                            put_msg_fifo(MSG_ENC1_START,MSG_TO_SELF,0) ;
                        }else{
                            if( (enc_ctrl.enc1_status == ENC_APP_IDLE)
                               || (enc_ctrl.enc1_status == ENC_APP_CLOSE))
                           {
                               put_msg_fifo(MSG_ENC1_OPEN,MSG_TO_SELF,0) ;
                           }
                        }
                    }
                }
                }

                enc_ctrl.enc_inout = 0;
            break ;

         #if 1 //解决开机录音一直为关 JOHNSON 20160531
            case MSG_KEY_NEXT:
                if(enc_ctrl.enc_type == PKG_TYPE_AVI){

                    enc_ctrl.dis_voice = !enc_ctrl.dis_voice;
                    sys_info.mic_mute = !sys_info.mic_mute;
                    disable_voice(sys_info.mic_mute);
                    if(ves_enc0)
                    {
                        enc_ctrl.dis_voice = (sys_info.mic_mute)?0:1;
                        ves_ioctrl(ves_enc0, &enc_ctrl.dis_voice,VES_CMD_ENC_DIS_VOICE) ;
                    }
                }
            break;

        #else //old

            case MSG_KEY_NEXT:
//                if(enc_ctrl.enc_type == PKG_TYPE_AVI){
//
//                    enc_ctrl.dis_voice = !enc_ctrl.dis_voice;
//                    disable_voice(enc_ctrl.dis_voice);
//                    if(ves_enc0)
//                    {
//                        ves_ioctrl(ves_enc0, &enc_ctrl.dis_voice,VES_CMD_ENC_DIS_VOICE) ;
//                    }
//                }
                if(enc_ctrl.enc_type == PKG_TYPE_AVI){

//                    enc_ctrl.dis_voice = !enc_ctrl.dis_voice;
                    sys_info.mic_mute = !sys_info.mic_mute;
                    disable_voice(sys_info.mic_mute);
                    //if(ves_enc0)
                    {
                        enc_ctrl.dis_voice = (sys_info.mic_mute)?0:1;
                        ves_ioctrl(ves_enc0, &enc_ctrl.dis_voice,VES_CMD_ENC_DIS_VOICE) ;
                    }
                }
            break;
        #endif

            case MSG_KEY_PREV:

                if(g_startup_finish == 0){
                    break;
                }

                if( (enc_ctrl.enc1_en == 0) && (av_in_offline == 0) ){
                    break;
                }
                av_in_offline = 0;

                if(enc_ctrl.enc_type == PKG_TYPE_AVI)
                {
                    if(g_double_dsc_status == DOUB_DSC_BIG_SMALL)
                    {
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

                        if(g_dsc0_hdl == NULL)
                        {
                            disp0_info.des_w = scr_w / 2;
                            disp0_info.des_h = scr_h;
                            puts("\nd0 :");
                            put_u32hex(disp0_info.des_w);
                            put_u32hex(disp0_info.des_h);

                            g_dsc0_hdl = dsc_open(&disp0_info,(IMGLAYER *)dsc_imb_open_tmp(&disp0_info));
                            if(g_dsc0_hdl){
                                if(dsc_start(g_dsc0_hdl) == FALSE){
                                    puts("\n----dsc0 start err\n");
                                    break;
                                }
                            }else{
                                puts("\n----dsc0 open err\n");
                                break;
                            }
                        }
                        if(g_dsc1_hdl == NULL){
                            disp1_info.des_w = scr_w / 2;
                            disp1_info.des_h = scr_h;
                            disp1_info.x = scr_w / 2;
                            disp1_info.dsc_buf = dsc1_half_ptr;

                            puts("\nd1 :");
                            put_u32hex(disp1_info.des_w);
                            put_u32hex(disp1_info.des_h);

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
                        }
                        g_double_dsc_status = DOUB_DSC_HALF_WIN;
                    }
                    else if(g_double_dsc_status == DOUB_DSC_HALF_WIN)
                    {
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
                        if(g_dsc0_hdl == NULL)
                        {
                            disp0_info.des_w = scr_w;
                            disp0_info.des_h = scr_h;
                            g_dsc0_hdl = dsc_open(&disp0_info,(IMGLAYER *)dsc_imb_open_tmp(&disp0_info));
                            if(g_dsc0_hdl){
                                if(dsc_start(g_dsc0_hdl) == FALSE){
                                    puts("\n----dsc0 start err\n");
                                    break;
                                }
                            }else{
                                puts("\n----dsc0 open err\n");
                                break;
                            }
                        }

                        g_double_dsc_status = DOUB_DSC_FRONT_WIN;
                    }
                    else if(g_double_dsc_status == DOUB_DSC_FRONT_WIN)
                    {
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
                        }

                        g_double_dsc_status = DOUB_DSC_BACK_WIN;
                    }
                    else if(g_double_dsc_status == DOUB_DSC_BACK_WIN)
                    {
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
                        if(g_dsc0_hdl == NULL)
                        {
                            disp0_info.des_w = scr_w;
                            disp0_info.des_h = scr_h;
                            g_dsc0_hdl = dsc_open(&disp0_info,(IMGLAYER *)dsc_imb_open_tmp(&disp0_info));
                            if(g_dsc0_hdl){
                                if(dsc_start(g_dsc0_hdl) == FALSE){
                                    puts("\n----dsc0 start err\n");
                                    break;
                                }
                            }else{
                                puts("\n----dsc0 open err\n");
                                break;
                            }
                        }
                        if(g_dsc1_hdl == NULL){
                            disp1_info.des_w = SMALL_WIN_WIDTH;
                            disp1_info.des_h = SMALL_WIN_HEIGH;
                            disp1_info.x = SMALL_WIN_START_X;
                            disp1_info.dsc_buf = dsc1_small_ptr;
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
                        }

                        g_double_dsc_status = DOUB_DSC_BIG_SMALL;
                    }
                }
                else if(enc_ctrl.enc_type == PKG_TYPE_JPG)
                {
                    if(g_double_dsc_status == DOUB_DSC_BACK_WIN){
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
                        if(g_dsc0_hdl == NULL)
                        {
                            disp0_info.des_w = scr_w;
                            disp0_info.des_h = scr_h;
                            g_dsc0_hdl = dsc_open(&disp0_info,(IMGLAYER *)dsc_imb_open_tmp(&disp0_info));
                            if(g_dsc0_hdl){
                                if(dsc_start(g_dsc0_hdl) == FALSE){
                                    puts("\n----dsc0 start err\n");
                                    break;
                                }
                            }else{
                                puts("\n----dsc0 open err\n");
                                break;
                            }

                            photo_info.vedio = isp0_name ;
                            put_msg_fifo(MSG_SW_SIZE,MSG_TO_SELF,0) ;
                        }
                        g_double_dsc_status = DOUB_DSC_FRONT_WIN;
                    }else{
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
                            photo_info.vedio = isp1_name ;
                            put_msg_fifo(MSG_SW_SIZE,MSG_TO_SELF,0) ;
                        }
                        g_double_dsc_status = DOUB_DSC_BACK_WIN;
                    }
                }

            break;

            case MSG_SW_FPS:

                if(enc_ctrl.enc_type == PKG_TYPE_AVI){
                    if((enc_ctrl.enc0_status != ENC_APP_STOP) && (enc_ctrl.enc0_status != ENC_APP_IDLE)){
                        break;
                    }
                    if(enc_ctrl.enc1_en){
                        if((enc_ctrl.enc1_status != ENC_APP_STOP) && (enc_ctrl.enc1_status != ENC_APP_IDLE)){
                            break;
                        }
                    }
                }
                else if(enc_ctrl.enc_type == PKG_TYPE_JPG){
                    if((enc_ctrl.enc0_status != ENC_APP_RUNNING) && (enc_ctrl.enc0_status != ENC_APP_IDLE)){
                        break;
                    }
                }

                enc_ctrl.skip_flag = 0;
                enc_ctrl.skip_cnt = 0;
                enc0_info.fr = enc_ctrl.vid_fps;
                enc_ctrl.skip_fps = enc0_info.fr;

                vid_fps = enc_ctrl.vid_fps;
                ves_ioctrl(ves_enc0, &vid_fps ,VES_CMD_ENC_SET_FPS) ;

            break;

            case MSG_ENC_OPEN:

                if(ves_enc0 == NULL){
                    ves_enc0 = ves_open() ;
                    if(!ves_enc0){
                        puts("ves0 open err\n");
                        break;
                    }
                    enc_ctrl.ves_enc = ves_enc0;
                }

                ret = ves_ioctrl(ves_enc0, &enc0_info ,VES_CMD_ENC_OPEN) ;
                if(ret != VES_ERR_NONE)
                {
                     printf("ves enc open err %x \r\n",ret ) ;
                     if(ret == VES_ERR_REOPEN)
                     {
                        put_msg_fifo(MSG_ENC_START,MSG_TO_SELF,0) ;
//                        if(tsave_hdl == NULL){
//                           tsave_hdl = timer_reg(30000, timetosavefile ,&enc_ctrl) ;
//                        }
                     }
                }
                else
                {
                    put_msg_fifo(MSG_ENC_START,MSG_TO_SELF,0) ;
//                    if(tsave_hdl == NULL){
//                       tsave_hdl = timer_reg(30000, timetosavefile ,&enc_ctrl) ;
//                    }
                    enc_ctrl.enc0_status = ENC_APP_OPEN;
                }

            break ;

            case MSG_ENC_CLOSE:

                ret = ves_ioctrl(ves_enc0, NULL ,VES_CMD_ENC_CLOSE) ;
                if(ret != VES_ERR_NONE){
                    puts("ves enc close err\n");
                    break;
                }
                enc_ctrl.enc0_status = ENC_APP_CLOSE;
                puts("\nves enc close ok\n");

            break;

            case MSG_ENC_START:
                if(enc_ctrl.enc0_status == ENC_APP_RUNNING){
                    printf(" enc0 restart0 \r\n ") ;
                    break ;
                }


                if(enc0_info.fp ==0){

                    enc0_info.fp = file_io_init(0) ;

                    if(!enc0_info.fp){
                        put_msg_fifo(MSG_ENC_STOP,MSG_TO_SELF,0) ;
                        display_note(M154);
                        set_disp_note_m154_status();
                        puts("\n\nfile io is null  \n\n") ;
                        break ;
                    }
                }else{
                     printf(" enc0 restart1 \r\n ") ;
                     //break ;
                }

                enc_ctrl.is_cycle_rec0 = (enc_ctrl.crec0_time > 0) ? 1 : 0;

                ret = ves_ioctrl(ves_enc0,enc0_info.fp, VES_CMD_ENC_START) ;
                if(ret != VES_ERR_NONE){
                    printf("\nstart  enc0 err %x \r\n" ,ret ) ;
                }else{
                    if(enc_ctrl.enc0_status != ENC_APP_SAVE){
                        enc_ctrl.skip_cnt = 0;
                        enc_ctrl.skip_flag = 1;
                    }

                    enc_ctrl.enc0_status = ENC_APP_RUNNING;
                    printf("\n-------------------------ves enc0 start ok \r\n") ;
                }
            break ;

            case MSG_ENC_STOP:
                //防止在循环录像最后1秒按OK按键导致状态机混乱
                if( (enc_ctrl.enc0_status == ENC_APP_SAVE) && (enc0_info.fp) ) {
                    puts("\n\n\nenc0 lijie\n\n\n");
                    put_msg_fifo(MSG_KEY_OK ,MSG_TO_PROCESS ,MODE_SW_N) ;
                    break;
                }

                puts("stop 0\n") ;
                if(enc0_info.fp)
                {
                    ves_ioctrl(ves_enc0, &i, VES_CMD_ENC_AVI_FNUM) ;
//                    puts("\n\n\nvv\n\n\n");
//                    put_u32hex(i);
                    if( (i != (-1)) && (i == 0)){
                        os_time_dly(2);
                    }
                }

                tmp_fp = enc0_info.fp;

                ret = ves_ioctrl(ves_enc0,0,VES_CMD_ENC_STOP) ;
                if(ret == VES_ERR_NONE){

                    enc_ctrl.motion_start_rec = 0;

                    enc0_info.fp = 0 ;
                    enc_ctrl.is_cycle_rec0 = 0;
                    enc_ctrl.enc0_status = ENC_APP_STOP;
                    printf("\n enc_ctrl.enc0_status : %d \n",enc_ctrl.enc0_status);
                    puts("\nstop 0 succ\n") ;
                }

                enc_ctrl.skip_flag = 0;
                enc_ctrl.skip_cnt = 0;

                if((enc_ctrl.lock_current_file != 0)||(user_define.protect_next_file !=0)){
//                if((enc_ctrl.lock_current_file != 0)){
                    if(tmp_fp){
                        protect_current_file(0);
//                        user_define.protect_file_num ++;///Burgess_151210
                        //printf("mutex_f_size((u8 *)video_path = %d\n",mutex_f_size((u8 *)video_path));
                        user_define.total_protect_size += (mutex_f_size((u8 *)video_path)>>20);
                        //printf("\n user_define.total_protect_size **************== %d\n",user_define.total_protect_size);
                    }
                }
//                #ifdef GUNTHER
                if((g_av_in.status ==0) && (enc_ctrl.enc1_status != ENC_APP_RUNNING))
//                if((enc_ctrl.enc1_status != ENC_APP_RUNNING))
                {
                    puts("\n gggggggggggggggg 5\n");
                    enc_ctrl.lock_current_file = 0;
//                    user_define.protect_next_file = 0;
                    if((sys_status.sys_gsensor_wakeup == 1)
//                       || (get_usb_charg_status() == TRUE)
//                       || (get_no_enought_space_status() == TRUE)
                       )
                    {
                        if ((usb_is_chargeing() == FALSE)
//                            || (get_no_enought_space_status() == TRUE)
                            )
                        {

                            sys_status.sys_gsensor_wakeup = 0;
                            put_msg_fifo(MSG_KEY_CLOSE ,MSG_TO_PROCESS ,MODE_SW_N);
                        }
//                        clear_usb_charg_status();
                    }
                }
//                #endif
////                如果拔出后在10s内手动停止录像，则不去关机
//                if(usb_charg_status == 2)//先拔出usb，再手动停止录像，这时候不关机
//                {
//                    if(usb_is_chargeing())
//                        usb_charg_status = 1;
//                    else
//                        usb_charg_status = 0;
//                }
            break ;


            case MSG_ENC_SAVE_FILE:
                if(imc_isr_stop_flag){
                    break;
                }
//                #ifdef GUNTHER
//                if(enc_ctrl.is_cycle_rec1 || enc_ctrl.is_cycle_rec0 || usb_is_chargeing())
//                {
//                    if (get_usb_charg_status())
//                    {
//                        puts("\n ggggg 5\n");
//                        clear_usb_charg_status();
//                        enc_ctrl.is_cycle_rec0 = 1;
//                        enc_ctrl.is_cycle_rec1 = 1;
//                    }
//                }
//                #endif
//

                if(enc_ctrl.enc0_status == ENC_APP_RUNNING){
                    if(enc_ctrl.is_cycle_rec0)
                    {
                        printf("\nkeep 0\r\n") ;
                        ret = ves_ioctrl(ves_enc0,0,VES_CMD_ENC_SAVE_FILE ) ;
                        enc0_info.fp = 0 ;
                        put_msg_fifo(MSG_ENC_START,MSG_TO_SELF,0) ;
                        if(enc_ctrl.lock_current_file)
                        {
                            protect_current_file(0);
//                            user_define.protect_file_num ++;///Burgess_151210
                            user_define.total_protect_size += (mutex_f_size((u8 *)video_path)>>20);
                            printf("\n user_define.total_protect_size **************== %d\n",user_define.total_protect_size);
                        }
                        enc_ctrl.enc0_status = ENC_APP_SAVE;
                    }
                }

                if(enc_ctrl.enc1_en && (enc_ctrl.enc1_status == ENC_APP_RUNNING)){
                    if(enc_ctrl.is_cycle_rec1)
                    {
                        printf("\nkeep 1\r\n") ;
                        ret = ves_ioctrl(ves_enc1,0,VES_CMD_ENC_SAVE_FILE ) ;
                        enc1_info.fp = 0 ;
                        if(enc_ctrl.lock_current_file)
                        {
                            protect_current_file(1);
//                            user_define.protect_file_num1 ++;
                            user_define.total_protect_size += (mutex_f_size((u8 *)video_path)>>20);
                            printf("\n user_define.total_protect_size **************== %d\n",user_define.total_protect_size);
                        }
                        put_msg_fifo(MSG_ENC1_START,MSG_TO_SELF,0) ;

                        enc_ctrl.enc1_status = ENC_APP_SAVE;
                    }
                }

//                printf("dfdfffffffffff****************\n");
                if((enc_ctrl.lock_current_file))// &&(enc_ctrl.is_cycle_rec0))
                {
//                    printf("df5554444444444****************\n");
                     enc_ctrl.lock_current_file = 0;
                }

                printf("user_define.protect_next_file  22222222 = %d\n",user_define.protect_next_file);
                if(user_define.protect_next_file)//保护下一个文件
                {
                    user_define.protect_next_file = 0;
                    enc_ctrl.lock_current_file = 1;
                }
              break ;



            case MSG_ENC1_OPEN:

                if(ves_enc1 == NULL){
                    ves_enc1 = ves_open() ;
                    if(!ves_enc1){
                        puts("ves0 open err\n");
                        break;
                    }
                }

                ret = ves_ioctrl(ves_enc1, &enc1_info ,VES_CMD_ENC_OPEN) ;
                if(ret != VES_ERR_NONE)
                {
                    printf("init enc err %x \r\n",ret ) ;
                    if(ret == VES_ERR_REOPEN){
                        put_msg_fifo(MSG_ENC1_START,MSG_TO_SELF,0) ;
                    }
                }else{
                    put_msg_fifo(MSG_ENC1_START,MSG_TO_SELF,0) ;
                    enc_ctrl.enc1_status = ENC_APP_OPEN;
                }

            break ;

            case MSG_ENC1_CLOSE:

                ret = ves_ioctrl(ves_enc1, NULL ,VES_CMD_ENC_CLOSE) ;
                if(ret != VES_ERR_NONE){
                    puts("ves enc1 close err\n");
                    break;
                }
                enc_ctrl.enc1_status = ENC_APP_CLOSE;

                puts("ves enc1 close ok\n");

            break;



            case MSG_ENC1_START:

                if(enc_ctrl.enc1_status == ENC_APP_RUNNING){
                    printf("\nenc1 restart0 \r\n ") ;
                    break ;
                }


                if(enc1_info.fp == 0){
                    enc1_info.fp = file_io_init(1) ;
                    if(!enc1_info.fp){
                        display_note(M154);
                        put_msg_fifo(MSG_ENC1_STOP,MSG_TO_SELF,0) ;
                        set_disp_note_m154_status();
                        puts("\n\nfile1 io is null  \n\n") ;
                        break ;
                    }
                }else{
                     printf("\nenc1 restart1 \r\n ") ;
                     //break ;
                }

                enc_ctrl.is_cycle_rec1 = (enc_ctrl.crec1_time > 0) ? 1 : 0;

                ret = ves_ioctrl(ves_enc1,enc1_info.fp, VES_CMD_ENC_START) ;
                if(ret != VES_ERR_NONE){
                    printf("start  enc1 err %x \r\n" ,ret ) ;
                }else{
                     enc_ctrl.enc1_status = ENC_APP_RUNNING;
                     printf("\n-------------------------ves enc1 start ok \r\n") ;
                }

            break ;

            case MSG_ENC1_STOP:

                //防止在循环录像最后1秒按OK按键导致状态机混乱
                if( (enc_ctrl.enc1_status == ENC_APP_SAVE) && (enc1_info.fp) ) {
                    //put_msg_fifo(MSG_KEY_OK ,MSG_TO_PROCESS ,MODE_SW_N) ;
                    puts("\n\n\nenc1 lijie\n\n\n");
                    break;
                }

                tmp_fp = enc1_info.fp;

                printf("stop 1\r\n") ;
                ret = ves_ioctrl(ves_enc1,0, VES_CMD_ENC_STOP) ;
                if(ret == VES_ERR_NONE){
                    enc1_info.fp = 0 ;
                    enc_ctrl.is_cycle_rec1 = 0;
                    enc_ctrl.enc1_status = ENC_APP_STOP;
                    puts("stop 1 succ\n");
                }
                if(enc_ctrl.lock_current_file)
                {
                    if(tmp_fp){
                        protect_current_file(1);
//                        user_define.protect_file_num1 ++;///Burgess_151210
                        user_define.total_protect_size += (mutex_f_size((u8 *)video_path)>>20);
                        printf("\n user_define.total_protect_size **************== %d\n",user_define.total_protect_size);
                    }
                     enc_ctrl.lock_current_file = 0;
                }
//                #ifdef GUNTHER
                if((sys_status.sys_gsensor_wakeup == 1)
//                   || (get_usb_charg_status() == TRUE)
//                   || (get_no_enought_space_status() == TRUE)
                   )
                {
                    puts("\n gggggggggggggggg 0\n");
                    if ((usb_is_chargeing() == FALSE)
//                        || get_no_enought_space_status()
                        )
                    {
                        clear_no_enought_space_status();
                    sys_status.sys_gsensor_wakeup = 0;
//                        if (get_disp_note_m154_status() == FALSE)
                        put_msg_fifo(MSG_KEY_CLOSE ,MSG_TO_PROCESS ,MODE_SW_N);
                    }
                }

            break ;

            case MSG_TACK_PHOTO_INIT:
                if(enc_ctrl.enc_type != PKG_TYPE_JPG){
                    break;
                }

                printf("msg photo open \r\n") ;

                if(ves_photo == NULL){
                    ves_photo = ves_open() ;
                    if(!ves_photo){
                        puts("vesphoto open err\n");
                        break;
                    }
                }


                ret = ves_ioctrl(ves_photo, &photo_info ,VES_CMD_ENC_OPEN) ;
                if(ret != VES_ERR_NONE){
                     printf("init enc err %x \r\n",ret ) ;
                     if(ret == VES_ERR_REOPEN){
                     }
                     break;
                }

                enc_ctrl.enc0_status = ENC_APP_OPEN;

                put_msg_fifo(MSG_TACK_PHOTO_BEGIN,MSG_TO_SELF,0) ;

            break ;

            case MSG_TACK_PHOTO_BEGIN:
                if(enc_ctrl.enc_type != PKG_TYPE_JPG){
                    break;
                }

                enc_ctrl.pic_inout = 1;

                if(g_double_dsc_status == DOUB_DSC_FRONT_WIN)
                    photo_info.fp = file_io_init(2);
                else
                    photo_info.fp = file_io_init(3);
                if(photo_info.fp == 0 ){
                    printf("open write file err \r\n");
                    display_note(M154);
                    enc_ctrl.pic_inout = 0;
                    break ;
                }

                ves_tack_photo_parm_t dd ;

                dac_api_play_ptone();
                photomode_icon_switch();
                num_of_photo_cutdown();
                dd.fp = photo_info.fp ;
                dd.ptr = 0 ;
                if(ves_ioctrl(ves_photo,&dd ,VES_CMD_PHOTO_TACK) != VES_ERR_NONE){
                    puts("take photo err\n");
                }
                enc_ctrl.enc0_status = ENC_APP_RUNNING;

                enc_ctrl.pic_inout = 0;

            break ;

            default :
                puts("\nenc unknown cmd\n");
            break;

        }
    }
}



void enc_skip_frame(void *ptr)
{
    ENC_CTRL_T * ctrl = (ENC_CTRL_T *)ptr;
    u32 fcnt;

    if(ctrl->enc_type != PKG_TYPE_AVI){
        return ;
    }

    if((ctrl->enc0_status == ENC_APP_RUNNING) || (ctrl->enc0_status == ENC_APP_SAVE)){
        if(ctrl->ves_enc && ctrl->skip_flag){
            ctrl->skip_cnt++;
            if(ctrl->skip_cnt >= 10){
                ctrl->skip_cnt = 0;
                fcnt = ctrl->skip_fps;
                ves_skip_fr((ves_t *)ctrl->ves_enc, &fcnt , NULL);
            }
        }
    }

}


void timetosavefile(void *ptr)
{
    ENC_CTRL_T * ctrl = (ENC_CTRL_T *)ptr;

    if( (ctrl->is_cycle_rec0) || (ctrl->is_cycle_rec1)){
//        printf("\nsave file \r\n") ;
//        put_msg_fifo(MSG_ENC_SAVE_FILE ,MSG_TO_PROCESS ,ENC_TASK_N) ;
    }

}

void enc_usb_charge(void *ptr)
{
    ENC_CTRL_T * ctrl = (ENC_CTRL_T *)ptr;

    if(ctrl->usb_charge_flag && g_startup_finish)//上电完成初始化之后，才开始录像
    {
//        ctrl->usb_charge_cnt++;
//        if(ctrl->usb_charge_cnt > 3)
//        {
            ctrl->usb_charge_flag = 0;

            if((mode_ctrl.cur_mode & ENC_MODE) && (ctrl->enc_type == PKG_TYPE_AVI) &&
               (sys_status.sys_gsensor_wakeup != 1) )
            {
                if((ctrl->enc0_status == ENC_APP_STOP) || (ctrl->enc0_status == ENC_APP_IDLE)
                    || (ctrl->enc0_status == ENC_APP_CLOSE))
                {
                    put_msg_fifo(MSG_KEY_OK ,MSG_TO_PROCESS ,MODE_SW_N) ;
                }
            }

//        }
    }

}


void motion_det_func(void *ptr)
{
    ENC_CTRL_T * ctrl = (ENC_CTRL_T *)ptr;
    static u32 cnt = 0;
    static u32 move_cnt = 0;
    wdt_clear_ctrl(1);
    if((is_sd_online() == FALSE))
    {
        cnt = 0;
        move_cnt = 0;
        return ;
    }

    if(get_menu_statu())
        return;
    if(ctrl->motion_detect && (enc_ctrl.enc_type == PKG_TYPE_AVI) && (mode_ctrl.cur_mode & ENC_MODE)){

        if((ctrl->enc0_status == ENC_APP_STOP) || (ctrl->enc0_status == ENC_APP_IDLE)
           || (ctrl->enc0_status == ENC_APP_CLOSE))
        {
            if(ctrl->enc1_en)
            {
                if((ctrl->enc1_status == ENC_APP_STOP) || (ctrl->enc1_status == ENC_APP_IDLE)
                    || (ctrl->enc1_status == ENC_APP_CLOSE))
                {

                    if(ispt_customize_mdStatus() && (!ctrl->motion_start_rec)){
                        move_cnt++;
                        if(move_cnt > 2)
                        {
                            //puts("start mrec\n");
                            if(sys_status.screen_off_cnt < 1)
                            {
                                sys_status.screen_off_cnt = 1;
                                ALCD_BL_ON();  //移动侦测屏保唤醒
                            }
                            put_msg_fifo(MSG_KEY_OK ,MSG_TO_PROCESS ,MODE_SW_N) ;
                            ctrl->motion_start_rec = 1;
                            cnt = 0;
                            move_cnt = 0;
                        }
                    }else if(move_cnt){
                        move_cnt--;
                    }
                }
            }
            else
            {
                if(ispt_customize_mdStatus() && (!ctrl->motion_start_rec)){
                    move_cnt++;
                    if(move_cnt > 2){
                        if(sys_status.screen_off_cnt < 1)
                        {
                            sys_status.screen_off_cnt = 1;
                            ALCD_BL_ON();   //移动侦测屏保唤醒
                        }
                        put_msg_fifo(MSG_KEY_OK ,MSG_TO_PROCESS ,MODE_SW_N) ;
                        ctrl->motion_start_rec = 1;
                        cnt = 0;
                        move_cnt = 0;
                    }
                }else if(move_cnt){
                    move_cnt--;
                }
            }
        }

        if( ctrl->enc0_status == ENC_APP_RUNNING )
        {
            if(ctrl->enc1_en)
            {
                if(ctrl->enc1_status == ENC_APP_RUNNING)
                {
                    if((ispt_customize_mdStatus() == 0) && ctrl->motion_start_rec){
                        cnt++;
                        if(cnt > MOVE_STOP_SEC){

                            //puts("stop mrec\n");
                            put_msg_fifo(MSG_KEY_OK ,MSG_TO_PROCESS ,MODE_SW_N) ;
                            ctrl->motion_start_rec = 0;
                            cnt = 0;
                            move_cnt = 0;
                        }
                    }else{
                        cnt = 0;
                    }
                }
            }
            else
            {
                if((ispt_customize_mdStatus() == 0) && ctrl->motion_start_rec){
                    cnt++;
                    if(cnt > MOVE_STOP_SEC){
                        put_msg_fifo(MSG_KEY_OK ,MSG_TO_PROCESS ,MODE_SW_N) ;
                        ctrl->motion_start_rec = 0;
                        cnt = 0;
                        move_cnt = 0;
                    }
                }else{
                    cnt = 0;
                }
            }
        }
        else
        {
            cnt = 0;
        }
    }
    else{
        cnt = 0;
        move_cnt = 0;
    }
}


void mic_mute_fun(u8 x)
{
//  enc_ctrl.dis_voice =  (x)?1:0;
  put_msg_fifo(MSG_KEY_NEXT ,MSG_TO_PROCESS ,ENC_TASK_N) ;
}

void disable_voice(u8 x)
{
//    if(sys_info.mic_mute != x)
//        sys_info.mic_mute = (x)?0:1;

    save_sysinfo(&sys_info);
    updata_mic_pic(); //更新图标
}

void motion_det_if(u8 x)
{
    enc_ctrl.motion_detect = (x)?1:0;
}


u8 get_parking_status(void)
{
    return g_av_in.parking_status;
}


void av_in_insert(void * priv)
{
    if(get_menu_statu())//菜单状态
        return;
    if(mode_ctrl.cur_mode & USB_MODE)//USB模式
        return;
    if(g_startup_finish == 0) //启动完成
        return;
    if(g_av_in.status == 1)
    {
        if( !AV_IN_INSERT() ){
            g_av_in.cnt++;
        }else{
            g_av_in.cnt = 0;
        }

        if(g_av_in.cnt > 5){
            g_av_in.cnt = 0;
            g_av_in.status = 0;
//            g_av_in.parking_status = 0;
            //puts("av in offline\n");
            put_msg_fifo(MSG_AV_IN_OFFLINE ,MSG_TO_PROCESS ,MODE_SW_N) ;
        }

        if(PARKING_DET_STATU())
        {
            if(!g_av_in.parking_status)
                g_av_in.p_cnt++;
        }
        else
        {
            if(g_av_in.p_cnt)
                g_av_in.p_cnt--;
        }
        if((g_av_in.p_cnt > 5) && (g_av_in.parking_status ==0))
        {
            g_av_in.parking_status = 1;
            puts("\n PARKING_DET_STATU on \n");
            put_msg_fifo(MSG_PARKING_SWITCH ,MSG_TO_PROCESS ,MODE_SW_N) ;
        }
        if((!g_av_in.p_cnt) && (g_av_in.parking_status ==1))
        {
            g_av_in.parking_status = 0;
            puts("\n PARKING_DET_STATU off \n");
            put_msg_fifo(MSG_PARKING_SWITCH ,MSG_TO_PROCESS ,MODE_SW_N) ;
        }
    }
    else
    {
        if(AV_IN_INSERT()){
            g_av_in.cnt++;
        }else{
            g_av_in.cnt = 0;
        }

        if(g_av_in.cnt > 5){
            //if(av_in_read())
            {
                g_av_in.cnt = 0;
                g_av_in.status = 1;

                //puts("av in online\n");

                put_msg_fifo(MSG_AV_IN_ONLINE ,MSG_TO_PROCESS ,MODE_SW_N) ;
            }
        }
    }

}
void av_in_check_init()
{
#ifdef DOUBLE_VEDIO
    AV_IN_DET_INIT();
    PARKING_DET_INIT();
    memset(&g_av_in, 0, sizeof(AV_IN_CTRL_T));
    //if(AV_IN_INSERT() && av_in_read()){
    if(AV_IN_INSERT()){
        enc_ctrl.enc1_en = 1;
        g_av_in.status = 1;
    }else{
        enc_ctrl.enc1_en = 0;
    }
    timer_reg(30 , av_in_insert , NULL);
#endif
}

void set_video_osd_en(u8 mark)
{
    enc_ctrl.osd_en = (mark)? 1:0;
    put_msg_fifo(MSG_SW_ENC_OSD ,MSG_TO_PROCESS ,ENC_TASK_N) ;
}
void set_pic_osd_en(u8 mark)
{
    enc_ctrl.pic_osd_en = (mark)? 1:0;
     put_msg_fifo(MSG_SW_ENC_OSD ,MSG_TO_PROCESS ,ENC_TASK_N) ;
}

