#include "dec_mode.h"
#include "hwi.h"
#include "os_api.h"
#include "imb_module.h"
#include "imc_if.h"
#include "dsc_ram.h"
#include "vds.h"
#include "jpeg_dec_if.h"
#include "msg.h"
#include "dev_ctrl.h"
#include "drv_manage.h"
#include "app_cfg.h"
#include "car_fs_api.h"
#include "sdfile.h"
#include "vm_api.h"
#include "timer.h"
#include "mode_switch.h"
#include "audio_api.h"
#include "menu.h"
#include "UI.h"
#include "my_ui.h"

extern disp_iparm_t disp0_info ,disp1_info;
extern dsc_hdl_t * g_dsc0_hdl;
extern dsc_hdl_t * g_dsc1_hdl;








DEC_CTRL_T dec_ctrl;


//函数声明
void kick_startframe(void * parm);
void test_putf(void * parm);
void ff_fr_tctrl(void * parm);

static  pic_size_t tmp_pic_size;

void dec_app_task(void *p_arg)
{

    u32 err;
    void * pFile;
    vds_t * avi_vds;
    vds_t * jpg_vds;
    u32 msg ;
    u32 ret;
    u8 need_stop ;
    u8 tmp_status;
    dec_info_t dec_info ;
    msg_t *msg_d;
    msg_devicechange_t *devsta;

    timer_handle kick_shdl;

    pic_info_t pic_info;
    sdfile_t jpg_sdfile;
    u16 scr_w,scr_h;
    u8 search_mode;
    u8 tmp_dec_type;
    u32 ff_fr_type;
    u32 vframe;
    u32 dec_usb_sw;
    u32 cpu_sr;
    avi_unpkg_info_t  avi_info ;
    u8 dev_cnt;
    u8 dev_flag;
    AVI_UNPKG_FD_T * app_avi_fd;



    err = dev_reg_msg(NULL);

    memset(&dec_ctrl, 0x00, sizeof(DEC_CTRL_T));

    avi_vds = vds_open();
    if(avi_vds == NULL){
        puts("open vds err\n");
        while(1);
    }

    jpg_vds = vds_open();
    if(jpg_vds == NULL){
        puts("open vds err\n");
        while(1);
    }

    lcd_get_size(&scr_w, &scr_h);


    dec_ctrl.dec_status = DEC_APP_IDLE;
    dec_ctrl.vds = avi_vds;
    dec_ctrl.ksframe_en = 0;
    need_stop = 0;
    ff_fr_type = 0;

    kick_shdl = timer_reg(1 , kick_startframe , &dec_ctrl);

    timer_reg(1 , test_putf , &dec_ctrl);//打印

    timer_reg(1 , ff_fr_tctrl , &dec_ctrl);

    dec_usb_sw = 0;


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
                if( (mode_ctrl.cur_mode & DEC_MODE) == 0){
                    break;
                }

                if(dec_ctrl.dec_status)
                {
                    if(dec_ctrl.dec_type == PKG_TYPE_AVI)
                    {
                        if( (dec_ctrl.dec_status == DEC_APP_READY) || (dec_ctrl.dec_status == DEC_APP_ABORT) )
                        {
                            puts("avi close\n");
                            if(need_stop){
                                dac_api_set_play(0);
                                dac_api_clr_fb();

                                vds_ioctrl(avi_vds , NULL, VDS_DEC_STOP);
                                need_stop = 0;
                            }

                            OS_ENTER_CRITICAL();

                            dec_ctrl.ksframe_en = 0;
                            dec_ctrl.dec_fps_cnt = 0;
                            dec_ctrl.vds = NULL;

                            OS_EXIT_CRITICAL();

                            os_time_dly(10);

                            puts("vds begin close\n");
                            if(vds_ioctrl(avi_vds ,NULL ,VDS_DEC_CLOSE) != VDS_ERR_OK){
                                puts("close dec err\n");
                                break;
                            }

                            puts("vds close out\n");

                            dec_ctrl.dec_status = DEC_APP_CLOSE;

                            vds_close(avi_vds);

                            avi_vds = NULL;

                            puts("\navi close out\n");
                            clear_msg();
                        }
                        else
                        {
                            if(dec_usb_sw == 0){
                                put_msg_fifo(MSG_DECODE_STOP,MSG_TO_SELF,0) ;
                                break;
                            }
                        }
                    }
                    else
                    {
                        puts("jpeg close\n");
                        if(vds_ioctrl(jpg_vds ,NULL ,VDS_JPG_CLOSE) != VDS_ERR_OK){
                            puts("close jpg dec err\n");
                            break;
                        }
                        dec_ctrl.dec_status = DEC_APP_CLOSE;

                        vds_close(jpg_vds);
                        jpg_vds = NULL;

                        puts("jpeg close out\n");
                        os_time_dly(10);
                        clear_msg();
                    }

                    need_stop = 0;
                    ff_fr_type = 0;
                    dec_ctrl.ff_fr_status = 0;
                    dec_ctrl.is_del_all = 0;

                    if(dec_usb_sw){
                        mode_ctrl.cur_mode |= USB_MODE;
                        dec_usb_sw = 0;
                        put_msg_fifo(MSG_USB_MENU ,MSG_TO_PROCESS ,UI_DISP_N);
                    }
                    else
                        put_msg_fifo(MSG_MODE_SW ,MSG_TO_PROCESS ,MODE_SW_N) ;
                }

                if(dec_ctrl.dec_status == DEC_APP_IDLE){

                    if(dec_usb_sw){
                        mode_ctrl.cur_mode |= USB_MODE;
                        dec_usb_sw = 0;
                        put_msg_fifo(MSG_USB_MENU ,MSG_TO_PROCESS ,UI_DISP_N);
                        break;
                    }

                    get_folader_status();//更新文件夹状态

                    pFile = read_file(FS_FIRST_FILE, &dec_ctrl.dec_type);

                    if(pFile == NULL){
                        pb_fctrl.no_file = 1;
                        put_msg_fifo(MSG_KEY_MODE,  MSG_TO_PROCESS,UI_DISP_N);
                        puts("\ndec enter no file\n");
                        break;
                    }
                    need_stop = 0;
                    ff_fr_type = 0;
                    dec_ctrl.ff_fr_status = 0;
                    dec_ctrl.is_del_all = 0;
                    put_msg_fifo(MSG_KEY_MODE,  MSG_TO_PROCESS,UI_DISP_N);
                    put_msg_fifo(MSG_DEC_OPEN,MSG_TO_SELF,0) ;
                }

            break;

            case MSG_KEY_OK_3S:
                puts("\n fffffffffffffff 0\n");
                unlock_all_file();
                user_define.unlock_all_flag = 2;//提示的标志
                os_time_dly(10);
                if(user_define.is_recording)
                {
                    user_define.is_recording = 0;
                    put_msg_fifo(MSG_KEY_OK ,MSG_TO_PROCESS ,MODE_SW_N);//重新开始录像
                }
                break;

            case MSG_DEVICECHANGE:
                if((mode_ctrl.cur_mode & DEC_MODE) == 0){
                    free(msg_d);
                    break;
                }

                puts("\ndecode dev change\n");
                devsta = msg_d->ptr;

                ret = usb_slave_msg_ctrl(devsta);
                if(ret)
                {
                    if(ret == 1){
                        if(get_menu_statu()) //菜单状态 先退出菜单
                            put_msg_fifo(MSG_MENU_EXIT ,MSG_TO_PROCESS ,UI_DISP_N);
                        put_msg_fifo(MSG_USB_FLAG ,MSG_TO_SELF ,0) ;
                    }

                    free(msg_d);
                    break;
                }


                file_sel_msg_ctrl(msg,devsta);
                printf("\nfind dev  %s--%d \n",devsta->hdev->dev_io->name, devsta->sta);
                if(devsta->sta ==DEV_ONLINE && devsta->hdev->dev_io->device_type == DEV_LOGIC_DISK  )
                {
                    puts("begin dec \n") ;
                    if(dec_ctrl.dec_status == DEC_APP_IDLE){
                        put_msg_fifo(MSG_MODE_SW,MSG_TO_SELF,0) ;
                    }else{

//                        get_folader_status();//更新文件夹状态
//                        put_msg_fifo(MSG_NEXT_FILE,MSG_TO_SELF,0) ;
                        put_msg_fifo(MSG_DEC_DEV_CHANGE,MSG_TO_SELF,0) ;
                        dev_flag = 1;
                        dev_cnt = 0;
                    }
                }
                else
                {
                    ret = strcmp(devsta->hdev->dev_io->name, "sd1");
                    if( (ret == 0) && (devsta->sta == DEV_OFFLINE) ){
                        if( (dec_ctrl.dec_type == PKG_TYPE_AVI) && (dec_ctrl.dec_status == DEC_APP_READY) ){
                            //put_msg_fifo(MSG_DECODE_RUN,MSG_TO_SELF,0) ;
                            dec_ctrl.ksframe_en = 0;
                            dec_ctrl.dec_fps_cnt = 0;

                            put_msg_fifo(MSG_DECODE_ERR,MSG_TO_SELF,0) ;

                        }
                    }
                }
                free(msg_d);

            break;

            case MSG_DEC_DEV_CHANGE:

                if(dev_flag == 1){
                    dev_flag = 0;
                    dev_cnt = 1;
                }
                if(dev_cnt){
                    dev_cnt++;
                    os_time_dly(10);
                    put_msg_fifo(MSG_DEC_DEV_CHANGE,MSG_TO_SELF,0) ;
                    if(dev_cnt > 20){
                        dev_cnt = 0;
                        if(is_phy_sd_online()){
                            get_folader_status();//更新文件夹状态
                            put_msg_fifo(MSG_NEXT_FILE,MSG_TO_SELF,0) ;
                        }
                    }
                }
            break;


            case MSG_USB_FLAG:

                dec_usb_sw = 1;
                if(dec_ctrl.dec_type == PKG_TYPE_AVI)
                {
                    if(ff_fr_type){
                        dec_ctrl.ff_fr_status = 2;
                        ff_fr_type = 0;
                    }

                    if((dec_ctrl.dec_status == DEC_APP_RUNNING) || (dec_ctrl.dec_status == DEC_APP_PAUSE))
                    {
                        puts("avi close\n");
                        if(need_stop){
                            dac_api_set_play(0);
                            dac_api_clr_fb();

                            vds_ioctrl(avi_vds , NULL, VDS_DEC_STOP);
                            need_stop = 0;
                        }

                        OS_ENTER_CRITICAL();

                        dec_ctrl.ksframe_en = 0;
                        dec_ctrl.dec_fps_cnt = 0;
                        dec_ctrl.vds = NULL;

                        OS_EXIT_CRITICAL();

                        if(vds_ioctrl(avi_vds ,NULL ,VDS_DEC_CLOSE) != VDS_ERR_OK){
                            puts("close dec err\n");
                            break;
                        }
                        dec_ctrl.dec_status = DEC_APP_CLOSE;

                        vds_close(avi_vds);

                        avi_vds = NULL;

                        os_time_dly(10);
                        clear_msg();

                        puts("\navi close out\n");
                    }
                }
                put_msg_fifo(MSG_MODE_SW ,MSG_TO_SELF ,0) ;

            break;


            case MSG_DEC_DEL:
                if(dec_ctrl.dec_type == PKG_TYPE_AVI)
                {
                    if( (dec_ctrl.dec_status == DEC_APP_READY) || (dec_ctrl.dec_status == DEC_APP_ABORT) ){

                        //不是删除全部，并且当前文件是保护文件
                        if( (dec_ctrl.is_del_all == 0) && dec_file_pro() ){
                            break;
                        }

                        if(dec_ctrl.dec_status != DEC_APP_ABORT)
                        {
                            dec_ctrl.ksframe_en = 0;
                            dec_ctrl.dec_fps_cnt = 0;
                            dac_api_set_play(0);
                            dac_api_clr_fb();

                            vds_ioctrl(avi_vds , NULL, VDS_DEC_STOP);
                            need_stop = 0;
                            dec_ctrl.dec_status = DEC_APP_STOP;
                        }

                        ret = dec_del_file();
                        if(ret != 0){
                            if(ret == 1){//保护文件

                            }else{
                                puts("del file err\n");
                                break;
                            }
                        }

                        if(dec_only_profile() && (dec_ctrl.is_del_all == 1)){//只剩下保护文件
                            dec_ctrl.is_del_all = 0;
                            if(get_pro_fnum()== 0)
                                pb_fctrl.no_file = 1;
                        }

                        search_mode = FS_NEXT_FILE;
                        put_msg_fifo(MSG_SET_PLAY_FILE,MSG_TO_SELF,0) ;
                    }
                }
                else
                {
                    if(dec_ctrl.dec_status == DEC_APP_RUNNING){

                        //不是删除全部，并且当前文件是保护文件
                        if( (dec_ctrl.is_del_all == 0) && dec_file_pro() ){
                            break;
                        }

                        ret = dec_del_file();
                        if(ret != 0){
                            if(ret == 1){//保护文件

                            }else{
                                puts("del file err\n");
                                break;
                            }
                        }

                        if(dec_only_profile()){//只剩下保护文件
                            dec_ctrl.is_del_all = 0;
                            if(get_pro_fnum()== 0)
                                pb_fctrl.no_file = 1;
                        }

                        search_mode = FS_NEXT_FILE;
                        put_msg_fifo(MSG_SET_PLAY_FILE,MSG_TO_SELF,0) ;
                    }
                }

            break;


            case MSG_KEY_NEXT:
                if(dec_ctrl.dec_status == DEC_APP_RUNNING){
                    if(!dec_ctrl.ff_fr_disable){
                        ff_fr_type = MSG_DECODE_FF;
                    }
                }
	        case MSG_NEXT_FILE:
                if(dec_ctrl.dec_type == PKG_TYPE_AVI)
                {
                    if( dec_ctrl.dec_status == DEC_APP_READY){
                        search_mode = FS_NEXT_FILE;
                        put_msg_fifo(MSG_SET_PLAY_FILE,MSG_TO_SELF,0) ;
                    }else if(dec_ctrl.dec_status == DEC_APP_RUNNING){

                        if((dec_ctrl.ff_fr_status == 0) && (!dec_ctrl.ff_fr_disable)){
                            put_msg_fifo(ff_fr_type,MSG_TO_SELF,0) ;
                        }
                    }else if(dec_ctrl.dec_status == DEC_APP_ABORT){
                        search_mode = FS_NEXT_FILE;
                        put_msg_fifo(MSG_SET_PLAY_FILE,MSG_TO_SELF,0) ;
                    }
                }
                else if(dec_ctrl.dec_type == PKG_TYPE_JPG)
                {
                    search_mode = FS_NEXT_FILE;
                    put_msg_fifo(MSG_SET_PLAY_FILE,MSG_TO_SELF,0) ;
                }

                dec_ctrl.dec_inout = 0;

            break;

            case MSG_KEY_PREV:
                if(dec_ctrl.dec_status == DEC_APP_RUNNING){
                    if(!dec_ctrl.ff_fr_disable){
                        ff_fr_type = MSG_DECODE_FB;
                    }
                }
	        case MSG_PREV_FILE:
                if(dec_ctrl.dec_type == PKG_TYPE_AVI)
                {
                    if( dec_ctrl.dec_status == DEC_APP_READY){
                        search_mode = FS_PREV_FILE;
                        put_msg_fifo(MSG_SET_PLAY_FILE,MSG_TO_SELF,0) ;
                    }else if(dec_ctrl.dec_status == DEC_APP_RUNNING){

                        if((dec_ctrl.ff_fr_status == 0) && (!dec_ctrl.ff_fr_disable)){
                            put_msg_fifo(ff_fr_type,MSG_TO_SELF,0) ;
                        }
                    }else if(dec_ctrl.dec_status == DEC_APP_ABORT){
                        search_mode = FS_PREV_FILE;
                        put_msg_fifo(MSG_SET_PLAY_FILE,MSG_TO_SELF,0) ;
                    }
                }
                else if(dec_ctrl.dec_type == PKG_TYPE_JPG)
                {
                    search_mode = FS_PREV_FILE;
                    put_msg_fifo(MSG_SET_PLAY_FILE,MSG_TO_SELF,0) ;
                }

                dec_ctrl.dec_inout = 0;

	        break;

            case MSG_SET_PLAY_FILE:

                if(dec_ctrl.dec_type == PKG_TYPE_AVI)
                {
                    if(need_stop){
                        dac_api_set_play(0);
                        dac_api_clr_fb();

                        vds_ioctrl(avi_vds , NULL, VDS_DEC_STOP);
                        need_stop = 0;
                    }
                }

                pFile = read_file(search_mode, &tmp_dec_type);
                if(pFile == NULL){
                    puts("no file\n");
                    if(!dec_ctrl.is_del_all)
                        pb_fctrl.no_file = 1; //
                    dec_ctrl.is_del_all = 0;
                    dec_ctrl.dec_status = DEC_APP_ABORT;
                    if(get_menu_statu()&& pb_fctrl.no_file == 1) //菜单状态 退出菜单
                         put_msg_fifo(MSG_MENU_EXIT ,MSG_TO_PROCESS ,UI_DISP_N) ;
                    break;
                }

                if(tmp_dec_type != dec_ctrl.dec_type){
                    if(dec_ctrl.dec_type == PKG_TYPE_AVI)
                    {
                        puts("avi close\n");
                        if(vds_ioctrl(avi_vds ,NULL ,VDS_DEC_CLOSE) != VDS_ERR_OK){
                            puts("close dec err\n");
                            break;
                        }
                        dec_ctrl.dec_status = DEC_APP_CLOSE;
                        puts("avi close out\n");
                    }
                    else
                    {
                        puts("jpeg close\n");
                        if(vds_ioctrl(jpg_vds ,NULL ,VDS_JPG_CLOSE) != VDS_ERR_OK){
                            puts("close jpg dec err\n");
                            break;
                        }
                        dec_ctrl.dec_status = DEC_APP_CLOSE;
                        puts("jpeg close out\n");
                    }

                    dec_ctrl.dec_type = tmp_dec_type;
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
                    put_msg_fifo(MSG_DEC_OPEN,MSG_TO_SELF,0) ;
                    break;
                }

                dec_ctrl.dec_type = tmp_dec_type;

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

                if(dec_ctrl.dec_type == PKG_TYPE_AVI){
                    put_msg_fifo(MSG_DECODE_AVI,MSG_TO_SELF,0) ;
                }else{
                    put_msg_fifo(MSG_DEC_PIC,MSG_TO_SELF,0) ;
                }
            break;


            case MSG_DEC_PIC :

                if( (dec_ctrl.dec_status == DEC_APP_OPEN) ||
                   (dec_ctrl.dec_status == DEC_APP_RUNNING))
                {
                    ret = vds_ioctrl(jpg_vds ,pFile, VDS_JPG_DEC_PIC);
                    if(ret != VDS_ERR_OK){
                        printf("\n\njpg pic dec err %x\n\n",ret);
                    }

                    vds_ioctrl(jpg_vds, &tmp_pic_size, VDS_JPG_GET_SIZE);//获取图片尺寸

                    dec_ctrl.dec_status = DEC_APP_RUNNING;

                    if(dec_ctrl.is_del_all){
                        put_msg_fifo(MSG_DEC_DEL,MSG_TO_SELF,0) ;
                    }else{
                        put_msg_fifo(MSG_DEC_INFORMATION ,MSG_TO_PROCESS ,UI_DISP_N) ; //更新解码模式等信息
                    }

                }
            break;

	        case MSG_DECODE_AVI:

                puts("\ndec avi\n");
                if(dec_ctrl.dec_type == PKG_TYPE_AVI)
                {
                    if( (dec_ctrl.dec_status != DEC_APP_OPEN) && (dec_ctrl.dec_status != DEC_APP_ABORT)
                       && (dec_ctrl.dec_status != DEC_APP_STOP) && (dec_ctrl.dec_status != DEC_APP_READY)){
                            puts("\nerr dec_status\n");
                            put_u32hex(dec_ctrl.dec_status);
                            break;
                       }

                    if(vds_ioctrl(avi_vds ,pFile, VDS_DEC_SET_AVI) != VDS_ERR_OK){
                        puts("set avi file err\n");

                        dec_ctrl.dec_status = DEC_APP_ABORT;

                        if(dec_ctrl.is_del_all){
                            put_msg_fifo(MSG_DEC_DEL,MSG_TO_SELF,0) ;
                        }else{
                            display_note(M174);
                            put_msg_fifo(MSG_DEC_INFORMATION ,MSG_TO_PROCESS ,UI_DISP_N) ;//更新解码模式等信息
                        }

                        break;
                    }

                    vds_ioctrl(avi_vds , NULL, VDS_DEC_START);

                    if(vds_ioctrl(avi_vds , NULL, VDS_DEC_PREVIEW) != VDS_ERR_OK){
                        puts("\npreview err\n");
                        dec_ctrl.dec_status = DEC_APP_ABORT;
                        if(dec_ctrl.is_del_all){
                            put_msg_fifo(MSG_DEC_DEL,MSG_TO_SELF,0) ;
                        }else{
                            display_note(M174);
                            put_msg_fifo(MSG_DEC_INFORMATION ,MSG_TO_PROCESS ,UI_DISP_N) ;//更新解码模式等信息
                        }
                        break;
                    }

                    app_avi_fd = vds_ioctrl(avi_vds , NULL, VDS_DEC_GET_AVI_HDL);
                    if(app_avi_fd->destroy_flag){
                        dec_ctrl.ff_fr_disable = 1;//禁止快进快退
                    }else{
                        dec_ctrl.ff_fr_disable = 0;
                    }

                    need_stop = 1;
                    dec_ctrl.dec_status = DEC_APP_READY;
                    puts("\n\npreview\n\n");

                    if(dec_ctrl.is_del_all){
                        put_msg_fifo(MSG_DEC_DEL,MSG_TO_SELF,0) ;
                    }else{
                        put_msg_fifo(MSG_DEC_INFORMATION ,MSG_TO_PROCESS ,UI_DISP_N) ;//更新解码模式等信息
                    }


                }

	        break;


            case MSG_DECODE_RUN:

                if(dec_ctrl.is_del_all){
                    break;
                }

                if(dec_ctrl.dec_status == DEC_APP_READY){
                    put_msg_fifo(MSG_DECODE_PLAY ,MSG_TO_SELF ,0) ;
                }else if(dec_ctrl.dec_status == DEC_APP_RUNNING){
                    put_msg_fifo(MSG_DECODE_PAUSE ,MSG_TO_SELF ,0) ;
                }else if(dec_ctrl.dec_status == DEC_APP_PAUSE){
                    put_msg_fifo(MSG_DECODE_CONTINUE ,MSG_TO_SELF ,0) ;
                }
                put_msg_fifo(MSG_KEY_OK ,MSG_TO_PROCESS ,UI_DISP_N) ;
            break;


            case MSG_DECODE_PLAY :

                if(dec_ctrl.dec_type != PKG_TYPE_AVI){
                    break;
                }

                if(dec_ctrl.dec_status == DEC_APP_READY)
                {
                    vds_ioctrl(avi_vds, &avi_info, VDS_DEC_GET_AVI_INF);
                    dec_ctrl.dec_fps_cnt = 0;
                    dec_ctrl.dec_fps = avi_info.vid_fps;
                    dec_ctrl.dec_isr_val = ((1000 / dec_ctrl.dec_fps) + 1) / 2 ;// * 2;

                    need_stop = 1;
                    dec_ctrl.ksframe_en = 1;
                    dec_ctrl.dec_status = DEC_APP_RUNNING;

                    dac_api_reset_dec((void *)vds_ioctrl(avi_vds , NULL, VDS_DEC_GET_AVI_HDL));
                    dac_api_set_play(1);
                }

            break;


	        case MSG_DEC_OPEN:

                if((dec_ctrl.dec_status != DEC_APP_CLOSE) && (dec_ctrl.dec_status != DEC_APP_IDLE)){
                    break;
                }

                if(dec_ctrl.dec_type == PKG_TYPE_AVI)
                {
                    if(avi_vds == NULL){
                        avi_vds = vds_open();
                        if(avi_vds == NULL){
                            puts("open avi vds err\n");
                            break;
                        }
                        dec_ctrl.vds = avi_vds;
                    }

                    memset(&dec_info,0x00,sizeof(dec_info_t));
                    dec_info.bufsize = VID0_FBUF_SIZE ;
                    dec_info.abufsize = AUD_FBUF_SIZE;
                    dec_info.x = 0;
                    dec_info.y = 0;
                    dec_info.des_w = scr_w;
                    dec_info.des_h = scr_h;
                    dec_info.des = lcd_name;
                    dec_info.dsc_buf = disp0_info.dsc_buf;
                    dec_info.vid_buf = g_vid0_buf;
                    dec_info.aud_buf = g_vid1_buf;
                    dec_info.yuv_val = lcd_get_bottom_color();

                    if(vds_ioctrl(avi_vds ,&dec_info, VDS_DEC_OPEN) != VDS_ERR_OK){
                        puts("open dec err\n");
                        break;
                    }

                    dec_ctrl.dec_status = DEC_APP_OPEN;

                    put_msg_fifo(MSG_DECODE_AVI, MSG_TO_SELF, 0) ;

                }
                else
                {
                    if(jpg_vds == NULL){
                        jpg_vds = vds_open();
                        if(jpg_vds == NULL){
                            puts("open jpg vds err\n");
                            break;
                        }
                    }

                    memset(&dec_info,0x00,sizeof(dec_info_t));
                    dec_info.bufsize = VID0_FBUF_SIZE;
                    dec_info.x = 0;
                    dec_info.y = 0;
                    dec_info.des_w = scr_w;
                    dec_info.des_h = scr_h;
                    dec_info.des = lcd_name;

                    dec_info.dsc_buf = disp0_info.dsc_buf;
                    dec_info.vid_buf = g_vid0_buf;
                    dec_info.yuv_val = lcd_get_bottom_color();

                    if(vds_ioctrl(jpg_vds ,&dec_info, VDS_JPG_OPEN) != VDS_ERR_OK){
                        puts("open jpg dec err\n");
                        break;
                    }
                    dec_ctrl.dec_status = DEC_APP_OPEN;

                    put_msg_fifo(MSG_DEC_PIC, MSG_TO_SELF, 0) ;
                }
	        break;

	        case MSG_DEC_CLOSE:
                if(dec_ctrl.dec_type == PKG_TYPE_AVI)
                {
                    puts("avi close\n");

                    if(vds_ioctrl(avi_vds ,NULL ,VDS_DEC_CLOSE) != VDS_ERR_OK){
                        puts("close dec err\n");
                        break;
                    }

                    dec_ctrl.dec_status = DEC_APP_CLOSE;

                    puts("avi close out\n");
                }
                else
                {
                    puts("jpeg close\n");

                    if(vds_ioctrl(jpg_vds ,NULL ,VDS_JPG_CLOSE) != VDS_ERR_OK){
                        puts("close jpg dec err\n");
                        break;
                    }

                    dec_ctrl.dec_status = DEC_APP_CLOSE;

                    puts("jpeg close out\n");
                }
	        break;


            case MSG_DECODE_PAUSE :
                if(dec_ctrl.dec_type != PKG_TYPE_AVI){
                    break;
                }

                if(ff_fr_type){
                    dec_ctrl.ff_fr_status = 2;
                    ff_fr_type = 0;
                    os_time_dly(100);

                    break;
                }


                dac_api_set_play(0);

                dec_ctrl.ksframe_en = 0;

                vds_ioctrl(avi_vds, NULL, VDS_DEC_PAUSE) ;

                dec_ctrl.dec_status = DEC_APP_PAUSE;

            break ;

            case MSG_DECODE_CONTINUE :
                if(dec_ctrl.dec_type != PKG_TYPE_AVI){
                    break;
                }

                dac_api_set_play(1);

                dec_ctrl.ksframe_en = 1;
                vds_ioctrl(avi_vds, NULL, VDS_DEC_CONTINUE) ;

                dec_ctrl.dec_status = DEC_APP_RUNNING;

            break ;

            case MSG_DECODE_STOP:
                if(dec_ctrl.dec_type != PKG_TYPE_AVI){
                    break;
                }

                dec_ctrl.ff_fr_status = 2;

                dec_ctrl.ksframe_en = 0;
                dec_ctrl.dec_fps_cnt = 0;

                //audio_close(AUDIO_DAC_TYPE);
                dac_api_set_play(0);
                dac_api_clr_fb();

                vds_ioctrl(avi_vds , NULL, VDS_DEC_STOP);
                if(sys_status.powerdown_flag==0)
                {
                    search_mode = FS_NEXT_FILE;
                    put_msg_fifo(MSG_SET_PLAY_FILE,MSG_TO_SELF,0) ;
                }


                need_stop = 0;

                dec_ctrl.dec_status = DEC_APP_STOP;

            break;

            case MSG_DECODE_END:
                dec_ctrl.ff_fr_status = 2;

                dec_ctrl.ksframe_en = 0;
                dec_ctrl.dec_fps_cnt = 0;
                puts("\ndecode end\n");

                dac_api_set_play(0);
                dac_api_clr_fb();

                vds_ioctrl(avi_vds , NULL, VDS_DEC_STOP);

                search_mode = FS_NEXT_FILE;
                put_msg_fifo(MSG_SET_PLAY_FILE,MSG_TO_SELF,0) ;

                need_stop = 0;

                dec_ctrl.dec_status = DEC_APP_STOP;

            break;

            case MSG_DECODE_ERR:
                dec_ctrl.ksframe_en = 0;
                dec_ctrl.dec_fps_cnt = 0;
                need_stop = 0;

                if(dec_ctrl.dec_type == PKG_TYPE_AVI){
                    dac_api_set_play(0);
                    dac_api_clr_fb();
                }

                //预览的情况下拔开卡
                if(dec_ctrl.dec_status == DEC_APP_READY){
                    err = 0x55;
                    vds_ioctrl(avi_vds , &err, VDS_DEC_STOP);
                }

                dec_ctrl.dec_status = DEC_APP_ABORT;

                //clear_msg();

                puts("dec err deal\n");


            break;

            case MSG_DECODE_FF:
                if((dec_ctrl.dec_type != PKG_TYPE_AVI) || dec_ctrl.ff_fr_disable){
                    break;
                }

                if(dec_ctrl.dec_status == DEC_APP_RUNNING)
                {
                    dec_ctrl.ksframe_en = 0;
                    dec_ctrl.dec_fps_cnt = 0;

                    dac_api_set_play(0);
                    //dac_api_clr_fb();
                    dac_api_fffr_clr_fb();

                    tmp_status = dec_ctrl.dec_status;
                    dec_ctrl.dec_status = DEC_APP_FF;

                    vds_ioctrl(avi_vds, NULL, VDS_DEC_FF) ;

                    dec_ctrl.dec_status = tmp_status;

                    dac_api_set_play(1);
                    dec_ctrl.ksframe_en = 1;

                    //vds_ioctrl(avi_vds, &avi_info, VDS_DEC_GET_AVI_INF);
                    vds_ioctrl(avi_vds, &vframe, VDS_DEC_GET_AVI_FRAME);
                    if(vframe >= avi_info.total_frames){
                        put_msg_fifo(MSG_DECODE_STOP,MSG_TO_SELF,0) ;
                        break;
                    }

                    dec_ctrl.ff_fr_status = 1;
                }

            break ;
            case MSG_DECODE_FB:
                if((dec_ctrl.dec_type != PKG_TYPE_AVI) || dec_ctrl.ff_fr_disable){
                    break;
                }

                if(dec_ctrl.dec_status == DEC_APP_RUNNING)
                {
                    dec_ctrl.ksframe_en = 0;
                    dec_ctrl.dec_fps_cnt = 0;

                    dac_api_set_play(0);
                    //dac_api_clr_fb();
                    dac_api_fffr_clr_fb();

                    tmp_status = dec_ctrl.dec_status;
                    dec_ctrl.dec_status = DEC_APP_FB;

                    vds_ioctrl(avi_vds, NULL, VDS_DEC_FR) ;

                    dec_ctrl.dec_status = tmp_status;

                    dac_api_set_play(1);
                    dec_ctrl.ksframe_en = 1;

                    vds_ioctrl(avi_vds, &vframe, VDS_DEC_GET_AVI_FRAME);
                    if(vframe < 3){
                        put_msg_fifo(MSG_DECODE_STOP,MSG_TO_SELF,0) ;
                        break;
                    }

                    dec_ctrl.ff_fr_status = 1;
                }

            break ;
           case MSG_UNLOCK_ALLFILE:
//                if(pb_fctrl.ftype != PKG_TYPE_AVI)
//                    break;
//                if(pb_fctrl.file_protect == 0)
//                    break;
                if(pFile == NULL)
                    break;
                else
                    mutex_f_Close(pFile);
                puts("\n fffffffffffffff 1\n");
                unlock_all_file();

                video_path[9] = 'r';
                video_path[10] = 'e';
                video_path[11] = 'c';

                printf("\n video_path = %s\n",video_path);

                pFile =  mutex_f_Open((u8 *)video_path,openReadOnly);
                if(pFile)
                    pb_fctrl.file_protect = 0;

                user_define.total_protect_size =0;

                dec_ctrl.ff_fr_status = 2;
                dec_ctrl.ksframe_en = 0;
                dec_ctrl.dec_fps_cnt = 0;

                dac_api_set_play(0);
                dac_api_clr_fb();

                vds_ioctrl(avi_vds , NULL, VDS_DEC_STOP);

                put_msg_fifo(MSG_DECODE_AVI,MSG_TO_SELF,0) ;
                need_stop = 0;
                dec_ctrl.dec_status = DEC_APP_STOP;
                break;
            case MSG_UNLOCK_FILE:
                    if(pb_fctrl.ftype != PKG_TYPE_AVI)
                        break;
                    if(pb_fctrl.file_protect == 0)
                        break;
                    if(pFile == NULL)
                        break;
                    else
                        mutex_f_Close(pFile);

                    unlock_current_file();
                    puts("MSG_UNLOCK_FILE 3\n");
//                    pFile = last_file_hdl;
                    pFile =  mutex_f_Open((u8 *)video_path,openReadOnly);
                     if(pFile)
                        pb_fctrl.file_protect = 0;
//printf("video_path::::::::::::: %c \n",video_path[7]);
                    if(video_path[7] =='A')
                    {
                        puts("file A----------------------\n");
                        pb_fctrl.fldA_pro_fnum--;
//                        if(user_define.protect_file_num > 0)
//                            user_define.protect_file_num --;
//                        else
//                            user_define.protect_file_num = 0;
                        user_define.total_protect_size -= (mutex_f_size((u8 *)video_path)>>20);
                        printf("\n user_define.total_protect_size **************== %d\n",user_define.total_protect_size);
                    }
                    else if(video_path[7] =='B')
                    {
                        pb_fctrl.fldB_pro_fnum--;
//                        if(user_define.protect_file_num1 > 0)
//                            user_define.protect_file_num1 --;
//                        else
//                            user_define.protect_file_num1 = 0;
                        user_define.total_protect_size -= (mutex_f_size((u8 *)video_path)>>20);
                        printf("\n user_define.total_protect_size **************== %d\n",user_define.total_protect_size);
                        puts("file  B-------------------\n");
                    }
//                    if((user_define.protect_file_num*file_size(0, get_cycle_time())+user_define.protect_file_num1*file_size(1, get_cycle_time())) >
//                               ALLOCATE_40_PER_FOR_PROTECT)
//                        user_define.note_protect_full = 2;
//                    else if((user_define.protect_file_num*file_size(0, get_cycle_time())+user_define.protect_file_num1*file_size(1, get_cycle_time())) >
//                                               ALLOCATE_30_PER_FOR_PROTECT)
//                        user_define.note_protect_full = 1;
//                    else user_define.note_protect_full = 0;
                    if(user_define.total_protect_size >ALLOCATE_40_PER_FOR_PROTECT)
                        user_define.note_protect_full = 2;
                    else if(user_define.total_protect_size >ALLOCATE_30_PER_FOR_PROTECT)
                        user_define.note_protect_full = 1;
                    else user_define.note_protect_full = 0;


                    dec_ctrl.ff_fr_status = 2;
                    dec_ctrl.ksframe_en = 0;
                    dec_ctrl.dec_fps_cnt = 0;

                    dac_api_set_play(0);
                    dac_api_clr_fb();

                    vds_ioctrl(avi_vds , NULL, VDS_DEC_STOP);

                    put_msg_fifo(MSG_DECODE_AVI,MSG_TO_SELF,0) ;
                    need_stop = 0;
                    dec_ctrl.dec_status = DEC_APP_STOP;

            break;

            case MSG_LOCK_FILE:
                     if(pb_fctrl.ftype != PKG_TYPE_AVI)
                        break;
                    if(pb_fctrl.file_protect == 1)
                        break;
                    if(pFile == NULL)
                        break;
                    else
                        mutex_f_Close(pFile);
                     puts("MSG_LOCK_FILE 3\n");
                    lock_current_file();

//                    pFile = last_file_hdl;
                    pFile =  mutex_f_Open((u8 *)video_path,openReadOnly);

                     if(pFile)
                        pb_fctrl.file_protect = 1;
                    if(video_path[7] =='A')
                    {
                        pb_fctrl.fldA_pro_fnum++;
//                        user_define.protect_file_num ++;
                        user_define.total_protect_size += (mutex_f_size((u8 *)video_path)>>20);
                        printf("\n user_define.total_protect_size **************== %d\n",user_define.total_protect_size);
                        puts("file A----------------------\n");
                    }
                    else if(video_path[7] =='B')
                    {
                        pb_fctrl.fldB_pro_fnum++;

//                        user_define.protect_file_num1 ++;

                        user_define.total_protect_size += (mutex_f_size((u8 *)video_path)>>20);
                        printf("\n user_define.total_protect_size **************== %d\n",user_define.total_protect_size);
                        puts("file  B-------------------\n");
                    }
//                    if((user_define.protect_file_num*file_size(0, get_cycle_time())+user_define.protect_file_num1*file_size(1, get_cycle_time())) >
//                               ALLOCATE_40_PER_FOR_PROTECT)
//                        user_define.note_protect_full = 2;
//                    else if((user_define.protect_file_num*file_size(0, get_cycle_time())+user_define.protect_file_num1*file_size(1, get_cycle_time())) >
//                                               ALLOCATE_30_PER_FOR_PROTECT)
//                        user_define.note_protect_full = 1;
//                    else user_define.note_protect_full = 0;
                    if(user_define.total_protect_size > ALLOCATE_40_PER_FOR_PROTECT)
                        user_define.note_protect_full = 2;
                    else if(user_define.total_protect_size > ALLOCATE_30_PER_FOR_PROTECT)
                        user_define.note_protect_full = 1;
                    else user_define.note_protect_full = 0;

                    dec_ctrl.ff_fr_status = 2;
                    dec_ctrl.ksframe_en = 0;
                    dec_ctrl.dec_fps_cnt = 0;

                    dac_api_set_play(0);
                    dac_api_clr_fb();

                    vds_ioctrl(avi_vds , NULL, VDS_DEC_STOP);

                    put_msg_fifo(MSG_DECODE_AVI,MSG_TO_SELF,0) ;
                    need_stop = 0;
                    dec_ctrl.dec_status = DEC_APP_STOP;

            break;

            default :
                puts("\ndec unknown cmd\n");
            break;
        }
    }
}


void kick_startframe(void * parm)
{
    DEC_CTRL_T * tmp_ctrl = (DEC_CTRL_T *)parm;

    if(tmp_ctrl->ksframe_en){

        if((dec_ctrl.dec_fps_cnt) && ((dec_ctrl.dec_fps_cnt % dec_ctrl.dec_isr_val) == 0)){
            vds_ioctrl((vds_t * )tmp_ctrl->vds , NULL, VDS_DEC_KFBG);
        }
        dec_ctrl.dec_fps_cnt++;
    }
}

void ff_fr_tctrl(void * parm)
{
    static cnt = 0;
    DEC_CTRL_T * tmp_ctrl = (DEC_CTRL_T *)parm;

    if(tmp_ctrl->ff_fr_status == 2){
        cnt = 0;
        tmp_ctrl->ff_fr_status = 0;
    }

    if(cnt){
        cnt++;
        if(cnt >= 40){
            cnt = 0;
            if(tmp_ctrl->ff_fr_status == 0){
                put_msg_fifo(MSG_NEXT_FILE,MSG_TO_PROCESS, DEC_TASK_N) ;
            }
        }
    }

    if(tmp_ctrl->ff_fr_status == 1){
        cnt = 1;
        tmp_ctrl->ff_fr_status = 0;
    }
}

void test_putf(void * parm)
{
    DEC_CTRL_T * tmp_ctrl = (DEC_CTRL_T *)parm;
    static u32 cnt = 0;

    cnt++;
    if(((cnt % 500) == 0) && (tmp_ctrl->ksframe_en) )
    {
        extern volatile u32 timer_fcnt;
        put_u32hex(timer_fcnt);
        timer_fcnt = 0;
    }
}




u32 get_current_decPic_inf(void)
{
    if(pb_fctrl.no_file == 1)
        return 0;
    return tmp_pic_size.h;

}

u32 get_current_decfile_inf(u8 type)
{
    if(pb_fctrl.no_file == 1)
        return 0;
    avi_unpkg_info_t  file_in;

    if(dec_ctrl.vds)
    {
        vds_ioctrl(dec_ctrl.vds, &file_in, VDS_DEC_GET_AVI_INF);
        if( type == 0){
            if(dec_ctrl.ff_fr_disable){
                file_in.total_frames = 0;
            }
            return file_in.total_frames;
        }
        else if(type ==1 )
            return file_in.width;
        else
            return file_in.vid_fps;
    }
    else
    {
        return 0;
    }


}

u32 get_current_frame(void)
{
    u32 temp_frame_num = 0;
    if(pb_fctrl.no_file == 1)
        return 0;
    else{
        if(dec_ctrl.vds){
            temp_frame_num = vds_ioctrl(dec_ctrl.vds, NULL, VDS_DEC_CUR_FRAME);
        }
    }

    return temp_frame_num;
}

TIME get_fieltotal_time(void)
{
    TIME time;
    u32 total_seconds = 0;
    float frame_rate = 0;
    frame_rate = get_current_decfile_inf(3);
    total_seconds =  (u32) (get_current_decfile_inf(0)/frame_rate);
//    if(pb_fctrl.cur_dec_folder == 1)
//        total_seconds =  (u32) (get_current_decfile_inf(0)/25.0);
//    else
//        total_seconds =  (u32) (get_current_decfile_inf(0)/30.0);

    time.hour = (total_seconds/3600)%24;
    time.minute = (total_seconds/60)%60;
    time.second = total_seconds%60;
    return time;
}

TIME get_flie_current_palytime(void)
{
    TIME time;
    u32 total_seconds = 0;
    float frame_rate = 0;
    frame_rate = get_current_decfile_inf(3);
    total_seconds = (u32) (get_current_frame()/frame_rate);
//    if(pb_fctrl.cur_dec_folder == 1)
//        total_seconds = (u32) (get_current_frame()/25.0);
//    else
//        total_seconds = (u32) (get_current_frame()/30.0);
//    puts("\n --:-- \n");
//    put_u32hex0(total_seconds);
    time.hour = (total_seconds/3600)%24;
    time.minute = (total_seconds/60)%60;
    time.second = total_seconds%60;
    return time;
}


void set_del_allfile_mark(u8 x)
{
    dec_ctrl.is_del_all = (x)?1:0;
}
u8 get_del_allfile_mark(void)
{
    return dec_ctrl.is_del_all;
}
