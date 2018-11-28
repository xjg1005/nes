#include "usb_mode.h"
#include "msg.h"
#include "ui_msg.h"
#include "dev_ctrl.h"
#include "drv_manage.h"
#include "app_cfg.h"
#include "car_fs_api.h"
#include "sdfile.h"
#include "vm_api.h"
#include "timer.h"
#include "ves.h"
#include "mode_switch.h"
#include "usb_slave_api.h"

#include "enc_mode.h"
#include "fileSelecter.h"
#include "irtc.h"
#include "my_ui.h"



USB_CTRL_T usb_ctrl;
u8 g_usb_startup = 0;


#define USB_ONLINE_CNT     30
#define USB_OFFLINE_CNT    1//15

#define USER_DEV_STA        0xff
#define USB_IS_CHARGING     (USER_DEV_STA+1)
#define USB_NO_CHARGING     (USER_DEV_STA+2)
#define USB_NO_STABLE       (USER_DEV_STA+3)

extern u32 ves_usb_ioctrl(ves_t * handle,void *parm ,u32 cmd);
static DEV_HANDLE susb;


static ves_t * usb_ves;
static u8 is_usb_camera_open = 0;
static u32 usb_camera_fr_size = 0;
const CAMERA_INFO camera_info =
{
    0x500,0x2D0,        //1280 720
    0x0320,0x0258, 		//800  600

    0x0280,0x01E0,      //640  480
    0x0160,0x0120,      //352  288
    0x0140,0x00F0,      //320  240
};

void set_usb_camera_open(u8 is_open)
{
	is_usb_camera_open = is_open;
}

u8 usb_camera_is_open(void)
{
	return is_usb_camera_open;
}

static u32 usb_video_indata(void *ptr,u32 len,BOOL *is_frame_end)
{
	u32 ret ;
    ves_jpg_bits_t jpg_bits;

    len = (len > 512) ? 512 : len;
    jpg_bits.buf = ptr;
    jpg_bits.len = len;
    jpg_bits.is_frame_end = is_frame_end;
    jpg_bits.is_drop_frame = 0;

    ret = 0;
    if(usb_ves){
        ret = ves_usb_ioctrl(usb_ves,(void *)(&jpg_bits) ,VES_CMD_JPEG_OUT);
    }

	return ret;
}

static void usb_open_camera(u32 frame_index)
{
	set_usb_camera_open(1);
	put_msg_fifo(MSG_ENC_OPEN, MSG_TO_PROCESS, USB_TASK_N);
	put_msg_fifo(MSG_ENC_START, MSG_TO_PROCESS, USB_TASK_N);
}

static void usb_close_camera()
{
	set_usb_camera_open(0);
	put_msg_fifo(MSG_ENC_STOP, MSG_TO_PROCESS, USB_TASK_N);
	put_msg_fifo(MSG_ENC_CLOSE, MSG_TO_PROCESS, USB_TASK_N);
}

UVC_CTRL_ARG uvc_ctrl_arg;
void *uvc_arg_init()
{
    uvc_ctrl_arg.open_camera = usb_open_camera;
    uvc_ctrl_arg.close_camera = usb_close_camera;
    uvc_ctrl_arg.video_indata = usb_video_indata;
    uvc_ctrl_arg.info = (CAMERA_INFO *)&camera_info;
    return &uvc_ctrl_arg;
}


#if USB_MIC_EN || USB_SPEAKER_EN
u32 usb_audio_volume_del(USB_AUDIO_CTL_TYPE type,u32 volume) //test
{
    u8 tmp_vol = 0;
    switch(type)
    {
#if USB_MIC_EN
    case USB_MIC_VOLUME:
    	printf("mic volume : %x", volume);
        break;

    case USB_MIC_LEFT_VOLUME:
    	printf("mic left volume : %x", volume);
        break;

    case USB_MIC_RIGHT_VOLUME:
    	printf("mic right volume : %x", volume);
        break;

    case USB_MIC_MUTE:
    	printf("mic mute volume : %x", volume);
        break;
#endif

#if USB_SPEAKER_EN
    case USB_SPK_VOLUME:
    case USB_SPK_LEFT_VOLUME:
    case USB_SPK_RIGHT_VOLUME:
        if(volume == -1)
        {
            return 100;
        }

        printf("PC_vol = %x   ",volume);
        tmp_vol = (u8)(volume/3);
        tmp_vol = (tmp_vol == 0) ? 0 : tmp_vol;
        tmp_vol = (tmp_vol >= 31) ? 31 : tmp_vol;
        printf("tmp_vol2 = %x\n",tmp_vol);

        break;

    case USB_SPK_MUTE:
        if(volume == -1)
        {
            return 100;
        }

        if(volume)          //1: UNMUTE    0:MUTE
        {
            puts("UNMUTE\n");
        }
        else
        {
            puts("MUTE\n");
        }

        break;
#endif
    default:
        break;
    }
    return 0;
}

//unsigned char data[64] =
//{
//    0x00, 0x00, 0x9C, 0x13, 0x77, 0x26, 0xD7, 0x37, 0x12, 0x47, 0x92, 0x53, 0xDC, 0x5C, 0x94, 0x62,
//    0x83, 0x64, 0x94, 0x62, 0xDC, 0x5C, 0x92, 0x53, 0x12, 0x47, 0xD7, 0x37, 0x77, 0x26, 0x9C, 0x13,
//    0x00, 0x00, 0x64, 0xEC, 0x89, 0xD9, 0x29, 0xC8, 0xEE, 0xB8, 0x6E, 0xAC, 0x24, 0xA3, 0x6C, 0x9D,
//    0x7D, 0x9B, 0x6C, 0x9D, 0x24, 0xA3, 0x6E, 0xAC, 0xEE, 0xB8, 0x29, 0xC8, 0x89, 0xD9, 0x64, 0xEC
//};
static u16 get_mic_data(void *buf , u32 len)
{
    //memcpy(buf, (u8*)data, len); //test
    return len;
}

//u8 speak_buf[1024]; //test
static void speak_write_data(void *ptr, u16 len)
{
//	memcpy(speak_buf, ptr, len); //todo
//	printf_buf(speak_buf, 0x10);//test

}

USB_AUDIO_ARG uac_ctl_arg;
void *uac_arg_init()
{
	//test
	uac_ctl_arg.usb_spk_write_data = speak_write_data;
	uac_ctl_arg.usb_mic_get_data = get_mic_data;
	uac_ctl_arg.usb_audio_volume_ctl = (void *)usb_audio_volume_del;

	return &uac_ctl_arg;
}

#endif


static u32 usb_mode_select(u32 usb_class)
{
	u32 class = 0;

    class = MASSSTORAGE_CLASS;
	if(usb_class == APP_MASSSTORAGE_CLASS){
        class = MASSSTORAGE_CLASS;
	}
	else if(usb_class == APP_VIDEO_CLASS){

	    #if ((DEBUG_LCD == 1) || (DEBUG_ISP == 1))
//	    #if (DEBUG_LCD == 1)
        class = VIDEO_CLASS | MASSSTORAGE_CLASS ;
        #else
        class = VIDEO_CLASS;//modify by lt
        #endif
	}
	else if(usb_class == APP_HID_CLASS){
        class = HID_CLASS;
	}

	return class;
}


u8 usb_fun_ctrl(u32 class)
{
	u8 ret = TRUE;
	u32 parm;

	parm = class;
	susb = dev_open("usb0slv", (void *)&parm);

	if (class & MASSSTORAGE_CLASS == MASSSTORAGE_CLASS)
	{
#if USB_VIDEO_EN
		if ((class & VIDEO_CLASS) != VIDEO_CLASS)
#endif
		{
			printf("%s() %d --------- add msd dev.\n", __FUNCTION__, __LINE__);
			dev_ioctl(susb,(void*)dev_get_sd1_hdl(), USB_SLAVE_MSD_ADD_DEV);
			dev_ioctl(susb, (void*)dev_get_flash_hdl(), USB_SLAVE_INIT_UPDATE);
		}
		else
		{
			dev_ioctl(susb, (void*)&usb_camera_fr_size, USB_SLAVE_SET_HOOK_INFO);
		}
		put_msg_fifo(MSG_MSD_RUN, MSG_TO_SELF, NULL);
	}

#if USB_VIDEO_EN
	if ((class & VIDEO_CLASS) == VIDEO_CLASS)
	{
		dev_ioctl(susb, uvc_arg_init(), USB_SLAVE_SET_UVC_INFO);
	}
#endif

#if USB_MIC_EN || USB_SPEAKER_EN
	if (((class & MIC_CLASS) == MIC_CLASS) || ((class & SPEAKER_CLASS) == SPEAKER_CLASS))
	{
		dev_ioctl(susb, uac_arg_init(), USB_SLAVE_SET_UAC_INFO);
	}
#endif

#if USB_HID_EN
	if ((class & HID_CLASS) == HID_CLASS)
	{

	}
#endif
}

/***************************************************************
 *brief : 从机DEVICECHANGE消息处理
 *
 *param ： parm -- 消息类型
 *
 *return：无
 ***************************************************************/
u32 usb_slave_msg_ctrl(void *parm)
{
	u32 class;
	msg_devicechange_t *devsta;

//    os_time_dly(10);

    devsta = (msg_devicechange_t *)parm;

//    printf("%s() %d, sta : 0x%x\n", __FUNCTION__, __LINE__, devsta->sta);
    if (DEV_ONLINE == devsta->sta && devsta->hdev->dev_io->device_type == DEV_USB_SLAVE )
    {
    	printf("%s() %d, usb slave online.\n", __FUNCTION__, __LINE__);
//    	class = usb_mode_select(devsta);
//    	usb_fun_ctrl(class);
        extern u8 not_open_dsc_flag;
        if (not_open_dsc_flag)
        {
            //printf("\n ======= 0 get_os_time() %d\n",get_os_time());
            not_open_dsc_flag = FALSE;
            extern IMGLAYER *image0;
            extern IMGLAYER *image1;
            g_dsc0_hdl = dsc_open(&disp0_info, image0);
            g_dsc1_hdl = dsc_open(&disp1_info, image1);
            ui_open_imb();

            if(g_dsc0_hdl){//显示完后必须关闭图层
                if(dsc_close(g_dsc0_hdl) == FALSE){
                    puts("\n\n\nclose dsc err\n\n\n");
                }else{
                    g_dsc0_hdl = NULL;
                }
            }
            if(g_dsc1_hdl){
                if(dsc_close(g_dsc1_hdl) == FALSE){
                    puts("\n\n\nclose dsc1 err\n\n\n");
                }else{
                    g_dsc1_hdl = NULL;
                }
            }
            //printf("\n ======= 1 get_os_time() %d\n",get_os_time());
        }

        usb_ctrl.enter_usb = 1;
        g_usb_startup = 1;

    	return 1;
    }
    else if (DEV_OFFLINE == devsta->sta && devsta->hdev->dev_io->device_type == DEV_USB_SLAVE )
    {
    	printf("%s() %d, usb slave offline.\n", __FUNCTION__, __LINE__);
    	if (usb_camera_is_open())
    	{
    		usb_close_camera();
    	}//todo

    	g_usb_startup = 0;

    	return 2;
    }
    else if (USB_IS_CHARGING == devsta->sta)
    {
    	printf("%s() %d, usb is charging.\n", __FUNCTION__, __LINE__);
    	//todo
        g_startup_usbcharge = 1;
    	return 3;
    }
    else if (USB_NO_CHARGING == devsta->sta)
    {
    	printf("%s() %d, usb no charing.\n", __FUNCTION__, __LINE__);
    	//todo
//    	if((mode_ctrl.cur_mode & ENC_MODE) && (enc_ctrl.enc_type == PKG_TYPE_AVI)
//        && (mode_ctrl.cur_mode & USB_MODE) == 0 && (sys_status.sys_gsensor_wakeup != 1)
//        )
//    	{
//            if((enc_ctrl.enc0_status == ENC_APP_RUNNING) )
//            {
//                put_msg_fifo(MSG_KEY_OK ,MSG_TO_PROCESS ,MODE_SW_N) ;
//            }
//    	}
    	return 4;
    }

    return 0;
}

#if 0
void test_usb_vedio(void * priv)
{
    BOOL fend_flag;
    static u8 flag = 0;
    if(usb_bflag)
    {
        fend_flag = 0;
        usb_video_indata(usb_vid_buf, 3 * 1024 - 16, &fend_flag);

        if(flag){
            printf_buf(usb_vid_buf, 0x10);
            flag = 0;
        }


        if(fend_flag){
            puts("usb fend\n");
            flag = 1;
        }
    }
}
#endif





void USB_CHARGE_INIT()
{
#ifdef DOUBLE_VEDIO
    u8 pr_io_dir_out;
    u8 pr_io_hd_in;
    u8 pr_io_pd_pu;

    pr_io_dir_out = read_RTC_reg(READ_REG + 2);
    pr_io_pd_pu = read_RTC_reg(READ_REG + 3);

    pr_io_dir_out |= BIT(5);//PR1
    pr_io_pd_pu &= ~( BIT(1) | BIT(5) );

    write_RTC_reg(WRITE_REG + 2, pr_io_dir_out);
    write_RTC_reg(WRITE_REG + 3, pr_io_pd_pu);
#else
    PORTH_DIR |= BIT(14);
    PORTH_PU &= ~BIT(14);
    PORTH_PD &= ~BIT(14);
    delay(3);
#endif
}

u8 USB_IS_INSERT()
{
#ifdef DOUBLE_VEDIO
    u8 pr_io_hd_in;

    pr_io_hd_in = read_RTC_reg(READ_REG + 1);

    if(pr_io_hd_in & BIT(1)){
        return 1;
    }
    return 0;
#else
    if (PORTH_IN & BIT(14))
    {
        return 1;
    }
    return 0;
#endif
}


void usb_charge_detect_init(void)
{
//    PORTH_DIR |= BIT(7);
//    PORTH_PU &= ~BIT(7);
//    PORTH_PD &= ~BIT(7);

    USB_CHARGE_INIT();
}

BOOL usb_is_chargeing(void)
{
	BOOL ret;

	//usb_charge_detect_init();


//	ret = (PORTH_IN & BIT(7) ? TRUE : FALSE);
	ret = (USB_IS_INSERT() ? TRUE : FALSE);

//	printf("%s() %d ,ret : %d\n", __FUNCTION__, __LINE__, ret);
	return ret;
}

static u16 offline_cnt = 0;
static u16 charging_cnt = 0;
static u16 no_charging_cnt = 0;
/***************************************************************
 *brief : 检测USB从机插入充电、未充电、下线
 *
 *param ： dev -- 设备句柄，暂时还用不到
 *		  cur_sta -- usb检测到设备状态
 *
 *return：有状态 返回DEV_OFFLINE、USB_IS_CHARGING、USB_NO_CHARGING
 *		     无状态返回USER_DEV_STA
 ***************************************************************/
u32 usb_charge_detect(void *dev, u32 volatile cur_sta)
{
    u32 ret_sta = USER_DEV_STA;
    static u32 user_sta = 0;
    static u8 charging_sta = 0;

	if (cur_sta == DEV_ONLINE || cur_sta == DEV_POWER_ON)
	{
	    no_charging_cnt = 0;

        //部分充电器拔出时usb接触不稳，导致DP拉高出现USB_IS_CHARGING和DEV_ONLINE
        if (user_sta == USB_IS_CHARGING && cur_sta == DEV_ONLINE)
        {
            if (!charging_sta)
            {
                if (charging_cnt >= USB_ONLINE_CNT)
                {
                    charging_cnt = 0;
                }
            }
        }

		if (usb_is_chargeing() == FALSE)
		{
		    charging_cnt = 0;
            if (offline_cnt < USB_OFFLINE_CNT)
			{
				++offline_cnt;
			}

            if (offline_cnt == USB_OFFLINE_CNT)
			{
				++offline_cnt;
				printf("%s() %d, dev offline.\n", __FUNCTION__, __LINE__);
				ret_sta = DEV_OFFLINE;
			}
		}
		else
		{
			offline_cnt = 0;
			if (charging_cnt == 0)//第一次置OFFLINE信号，用于稳定USB
			{
			    ret_sta = DEV_OFFLINE;
			}
			if (charging_cnt > 0 && charging_cnt < USB_ONLINE_CNT) //USB上线稳定
			{
                ++charging_cnt;
                ret_sta = USB_NO_STABLE;
			}
            if (charging_cnt == USB_ONLINE_CNT)
			{
				++charging_cnt;
				charging_sta = 1;
			}
		}
	}
	else
	{
		offline_cnt = 0;

		if (usb_is_chargeing())
		{
			no_charging_cnt = 0;
			if (charging_cnt < USB_ONLINE_CNT)
			{
				++charging_cnt;
			}
			if (charging_cnt == USB_ONLINE_CNT)
			{
				++charging_cnt;
//				printf("%s() %d, cur status : 0x%x.\n", __FUNCTION__, __LINE__, cur_sta);
				ret_sta = USB_IS_CHARGING;
			}
		}
		else
		{
		    //printf("%s() %d, no charging cnt : %d.\n", __FUNCTION__, __LINE__, no_charging_cnt);
			charging_cnt = 0;
			if (no_charging_cnt < USB_OFFLINE_CNT)
			{
				++no_charging_cnt;
			}

			if (no_charging_cnt == USB_OFFLINE_CNT)
			{
				++no_charging_cnt;

				charging_sta = 0;
//				printf("%s() %d, no charging cnt : %d.\n", __FUNCTION__, __LINE__, no_charging_cnt);
				ret_sta = USB_NO_CHARGING;
			}
		}
	}

    if (charging_sta && user_sta == USB_NO_CHARGING)//设备上线并且充电时重置状态
    {
        user_sta = USB_IS_CHARGING;
    }

    if(ret_sta > USER_DEV_STA)
    {
        if (user_sta == ret_sta)
        {
            return DEV_HOLD;
        }
        user_sta = ret_sta;
    }

	return ret_sta;
}


void app_usb_reg()
{
//    u32 usb_class;

    struct usb_slv_arg m_usb;

    m_usb.isr_priority = 2;
    m_usb.pstr_desc = NULL;
    m_usb.parm = usb_charge_detect;

    DEVICE_REG(usb0slv, &m_usb);
    susb = dev_open("usb0slv", NULL);

//    usb_class = MASSSTORAGE_CLASS;
//    susb = dev_open("usb0slv", &usb_class);
}

#ifdef TEST_HID
u8 usb_hid_key_ok[] = {0xCD};
u8 usb_hid_key_up[] = {0x50};
u8 usb_hid_key_down[] = {0x51};
#endif
void usb_app_task(void *p_arg)
{
    u32 err;
    u32 ret;
    u32 msg ;
    msg_t *msg_d;
    msg_devicechange_t *devsta;
    ves_encode_data_t usb_enc_info;
    timer_handle usb_timer_hdl;
    HID_CMD hid_cmd;
    u32 usb_class;

    err = dev_reg_msg(NULL);

    //enc0
    usb_enc_info.vsize = VID0_FBUF_SIZE ;
    usb_enc_info.vedio = isp0_name ;
    usb_enc_info.format = VES_FORAMT_JPG_BITS;

    usb_enc_info.drop_frame_en = 0;
    usb_enc_info.org_fps = 0;
    usb_enc_info.fr = 30;
    usb_enc_info.h = 720;
    usb_enc_info.w = 1280;
    usb_enc_info.q = 5;
    usb_enc_info.abr_en = 0;
    usb_enc_info.abr_kbps = 16000;
    usb_enc_info.samplerate = 8000 ;
	usb_enc_info.osd_en = 0;
    usb_enc_info.vid_buf = g_vid0_buf;
    usb_enc_info.dynamic_huffman_en = 0;

    usb_enc_info.time_lapse_photography_ms = 0;
    usb_enc_info.discard_aud = 0;

    usb_ves = NULL;
    memset(&usb_ctrl, 0x00, sizeof(USB_CTRL_T));

    usb_ctrl.usb_class = APP_VIDEO_CLASS;
    usb_camera_fr_size = usb_enc_info.h | (usb_enc_info.w << 16);
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

            case MSG_MODE_SW:
                if((mode_ctrl.cur_mode & USB_MODE) == 0){
                    break;
                }

                //进入USB
                if(usb_ctrl.enter_usb){

        #if LANE_DECT_EN
                    if(g_ldet_ptr){//切换到USB模式时，停止车道检测
                        lane_dect_stop(g_ldet_ptr);
                    }
        #endif

                    usb_ctrl.usb_sddev_flag = 1;

                    usb_class = usb_mode_select(usb_ctrl.usb_class);
                    usb_fun_ctrl(usb_class);
#if DEBUG_LCD
                    if(usb_class & VIDEO_CLASS)
                        put_msg_fifo(MSG_DEBUG_LCD ,MSG_TO_PROCESS ,MODE_SW_N) ;
#endif
                    usb_ctrl.usb_vid_status = ENC_APP_IDLE;
                    usb_ctrl.enter_usb = 0;

                    put_msg_fifo(MSG_USB_MODE ,MSG_TO_PROCESS ,UI_DISP_N);
                }

                //退出USB
                if(usb_ctrl.leave_usb)
                {
                    if(usb_ves){
                        if(ves_close(usb_ves) == FALSE){
                            puts("\nusb ves close err\n");
                            break;
                        }
                    }

#if DEBUG_LCD
                    put_msg_fifo(MSG_DEBUG_LCD ,MSG_TO_PROCESS ,MODE_SW_N) ;
#endif

                    usb_ctrl.usb_sddev_flag = 0;

                    usb_ves = NULL;
                    usb_ctrl.leave_usb = 0;
//                    dev_open("usb0slv", NULL);
                    mode_ctrl.mode_index = 0;
                    mode_ctrl.cur_mode &= ~USB_MODE;

                    FILE_ENTER_MUTEX();
					remount('B');
                    if (!init_record_dir())
                    {
                        puts("\n\ninit record fail\n\n");
                    }
                    FILE_EXIT_MUTEX();

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

                    put_msg_fifo(MSG_MODE_SW ,MSG_TO_PROCESS ,MODE_SW_N) ;

                }
                break;

            case MSG_DEVICECHANGE:

                if((mode_ctrl.cur_mode & USB_MODE) == 0){
                    free(msg_d);
                    break;
                }

                puts("usb dev change\n");
                devsta = msg_d->ptr;
                printf("\n, find dev  %s--%d--%d \n", devsta->hdev->dev_io->name, devsta->sta);
                ret = usb_slave_msg_ctrl(devsta);
                if(ret){
                    if(ret == 2){
                        put_msg_fifo(MSG_USB_FLAG ,MSG_TO_SELF ,0) ;
                        free(msg_d);
                        break;
                    }
                    break;//属于USB的消息
                }

                if(usb_ctrl.usb_sddev_flag == 1){
                    file_sel_msg_ctrl(msg,devsta);
                }

                free(msg_d);

                break;


            case MSG_USB_FLAG:
                usb_ctrl.leave_usb = 1;
                usb_ctrl.enter_usb = 0;
                if((usb_ctrl.usb_vid_status == ENC_APP_IDLE) || (usb_ctrl.usb_vid_status == ENC_APP_CLOSE)){

                    put_msg_fifo(MSG_MODE_SW ,MSG_TO_SELF ,0) ;
                }

            break;

            case MSG_MSD_RUN:

                //puts("\nmsg msd run\n");
                if(husb_msd_run() != DEV_ERR_OFFLINE)
                {
                	os_time_dly(1);
                    put_msg_fifo(MSG_MSD_RUN, MSG_TO_SELF, NULL);
                }
                break;
            case MSG_ENC_OPEN:

                if(usb_ves == NULL){
                    usb_ves = ves_open() ;
                    if(usb_ves == NULL){
                        puts("\nmalloc usb_ves err\n");
                        break;
                    }
                }

                ret = ves_ioctrl(usb_ves, &usb_enc_info ,VES_CMD_ENC_OPEN) ;
                if(ret != VES_ERR_NONE){
                    puts("usb enc open err\n");
                    break;
                }
                updata_menu_cfg();
                usb_ctrl.usb_vid_status = ENC_APP_OPEN;

                break;

            case MSG_ENC_CLOSE:
                if(usb_ves == NULL) break;

                ret = ves_ioctrl(usb_ves, NULL ,VES_CMD_ENC_CLOSE) ;
                if(ret != VES_ERR_NONE){
                    puts("ves enc close err\n");
                    break;
                }

                usb_ctrl.usb_vid_status = ENC_APP_CLOSE;

                if(usb_ctrl.leave_usb){
                    put_msg_fifo(MSG_MODE_SW ,MSG_TO_SELF ,0) ;
                }

                break;

            case MSG_ENC_START:
                if(usb_ves == NULL) break;

            	ves_usb_ioctrl(usb_ves, (void *)NULL ,VES_CMD_JPEG_OUT_START) ;

            	usb_ctrl.usb_vid_status = ENC_APP_RUNNING;

                break;

            case MSG_ENC_STOP:
                if(usb_ves == NULL) break;

            	ves_usb_ioctrl(usb_ves, (void *)NULL ,VES_CMD_JPEG_OUT_STOP) ;

            	usb_ctrl.usb_vid_status = ENC_APP_STOP;

                break;

#ifdef TEST_HID
            case MSG_KEY_NEXT:
            	hid_cmd.addr = usb_hid_key_up;
            	hid_cmd.len = sizeof(usb_hid_key_up);
            	dev_ioctl(susb, (void *)&hid_cmd, USB_SLAVE_HID_TX);
            	break;
            case MSG_KEY_PREV:
            	hid_cmd.addr = usb_hid_key_down;
            	hid_cmd.len = sizeof(usb_hid_key_down);
            	dev_ioctl(susb, (void *)&hid_cmd, USB_SLAVE_HID_TX);
            	break;
            case MSG_KEY_OK:
            	hid_cmd.addr = usb_hid_key_ok;
            	hid_cmd.len = sizeof(usb_hid_key_ok);
            	dev_ioctl(susb, (void *)&hid_cmd, USB_SLAVE_HID_TX);
            	break;
#endif
        }
    }
}















