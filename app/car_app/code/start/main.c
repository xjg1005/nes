
#include "typedef.h"
#include "hwi.h"
#include "os_api.h"
#include "mango_dev_spi.h"
#include "mango_dev_spi0flash.h"
#include "mango_dev_sd.h"
#include "mango_dev_uart.h"
#include "imb_module.h"
#include "imc_if.h"
#include "jpeg_enc_if.h"
#include "dsc_ram.h"
#include "vds.h"
#include "jpeg_dec_if.h"
#include "msg.h"
#include "debug.h"
#include "clock.h"
#include "ui.h"
#include "ui_con.h"
#include "menu.h"
#include "dev_ctrl.h"
#include "drv_manage.h"
#include "mango_dev_sd.h"
#include "app_cfg.h"
#include "car_fs_api.h"
#include "sdfile.h"
#include "vm_api.h"
#include "timer.h"
#include "lcd.h"
#include "enc_mode.h"
#include "dec_mode.h"
#include "usb_mode.h"
#include "mode_switch.h"
#include "wfile_serve.h"
#include "ui_task.h"
#include "audio_api.h"
#include "my_ui.h"
#include "pwr_onoff.h"
#include "upgrade.h"
#include "wdt.h"
#include "lane_dect.h"
#include "irtc.h"
#include "pwr_onoff.h"
#include "video_play.h"
#include "InfoNES.h"

static char debug_msg[][20]=
{
    /*0---7*/
    "reserved",
    "reserved",
    "prp_ex_limit_err",
    "sdr_wr_limit_err",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    /*8---15*/
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    /*16---23*/
    "dsp_pc_limit_err",
    "dsp_ex_limit_err",
    "dsp_illegal",
    "dsp_misaligne",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    /*24---31*/
    "dsp_if_bus_inv",
    "dsp_of_bus_inv",
    "dsp_ex_bus_inv",
    "prp_bus_inv",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
};
void exception_isr(void )
{
    u32 rets_addr ,reti_addr;
    u32 i;

     FPGA_TRI =  0 ;

    asm("%0 = RETS ;" : "=r"(rets_addr));
    asm("%0 = RETI ;" : "=r"(reti_addr));




    printf("\n\n\n!!!!! %s: rets_addr = 0x%x, reti_addr = 0x%x\n DEBUG_MSG = 0x%x, DEBUG_WRNUM = 0x%x DSPCON=%x\n",
           __FUNCTION__,rets_addr,reti_addr, DEBUG_MSG, DEBUG_WRNUM,DSPCON);

    for(i=0; i<32; i++)
    {
        if(BIT(i)&DEBUG_MSG)
        {
            puts(debug_msg[i]);
            putchar('\n');
        }
    }

    while(1);
}

//REG_INIT_HANDLE(exception_isr);





const u8 isp0_name[10] = "isp0";
const u8 isp1_name[10] = "isp1";
const u8 rep_name[10] = "rep";
const u8 lcd_name[10] = "lcd";
const u8 avout_name[10] = "avout";

u32 g_chip_id = 0;
u32 g_spec_val = 0;

void set_audio_clk()
{

#if 0

    CLK_CON1 &= ~(BIT(2) | BIT(3));//24m->dac_clkl
    CLK_CON1 &= ~(3<<6);//96m->ladc_clk


    CLK_CON2 &= ~(3<<6);//pll->apc_clk

    PLL_CON0 &= ~(BIT(18) | BIT(19));//pll0_480->apc_clk
    PLL_CON0 |= BIT(18);

    PLL_CON0 &= ~(BIT(12) | BIT(13) | BIT(14) | BIT(15));//apc480mdiv6
    PLL_CON0 |= (5<<12);

    PLL_CON0 &= ~BIT(21);//PLL0->96M

//    pll_apc_div(PLL_CLOCK_DIV6);

#else

    CLK_CON1 &= ~(BIT(2) | BIT(3));//24m->dac_clkl
    CLK_CON1 &= ~(3<<6);//96m->ladc_clk

    pll_apc_div(PLL_CLOCK_DIV6);

#endif


}

static u8 g_vbuf0[VID0_FBUF_SIZE] ALIGNE(32);
static u8 g_vbuf1[VID1_FBUF_SIZE] ALIGNE(32);
//static
//u8 g_dbuf[LCD_W * LCD_H * 3 / 2 * 3 + SMALL_WIN_WIDTH * SMALL_WIN_HEIGH * 3 / 2 * 3] ALIGNE(32);
//AVOUT JOHNSON 20160530
u8 g_dbuf[400 * 240 * 3 / 2 * 3 + SMALL_WIN_WIDTH * SMALL_WIN_HEIGH * 3 / 2 * 3] ALIGNE(32);
//static u8 g_dbuf0[LCD_W * LCD_H * 3 / 2 * 3] ALIGNE(32);
//static u8 g_dbuf1[LCD_W * LCD_H * 3 / 2 * 3] ALIGNE(32);

u8 * dsc1_small_ptr;
u8 * dsc1_half_ptr;
u8 * dsc1_all_ptr;


u8 * g_vid0_buf = NULL;
u8 * g_vid1_buf = NULL;
u8 * g_aud_buf = NULL;

disp_iparm_t disp0_info ,disp1_info;
dsc_hdl_t * g_dsc0_hdl;
dsc_hdl_t * g_dsc1_hdl;
u8 g_double_dsc_status;

u8 g_startup_finish = 0;//系统启动完成标志
u8 g_startup_usbcharge = 0;

u32 g_sdfile_head_offset;

u8 g_special_flag = 0;
void set_special_flag(u8 flag)
{
    g_special_flag = flag;
}


u8 g_ss_flag = 0;
u8 * start_stop_malloc(u32 * len, u8 * flag)
{
    u8 * buf;

    *len = ( VID1_FBUF_SIZE - AUD_FBUF_SIZE ) / 2;
    *flag = g_ss_flag;
    buf = g_vbuf1 + AUD_FBUF_SIZE;

    return buf;
}
u8 * pic_malloc(u32 * len, u8 * flag)
{
    u8 * buf;

    *len = ( VID1_FBUF_SIZE - AUD_FBUF_SIZE ) / 2;
    *flag = g_ss_flag;
    buf = g_vbuf1 + AUD_FBUF_SIZE + ( (VID1_FBUF_SIZE - AUD_FBUF_SIZE) / 2 );

    return buf;
}




ldet_iparm_t g_ldet_info;
ldet_hdl_t * g_ldet_ptr = NULL;
u32 g_vanish_y = 0;
u32 g_carHead = 0;

u32 g_ldet_status = 0;
u32 g_ldet_old_status = 0;
u32 g_ldet_cnt = 0;

#if LANE_DECT_EN

#define LDET_BUF_NUM    2

//static u8 g_ldet_buf[LDET_WIDTH * LDET_HEIGH * 3 / 2 * LDET_BUF_NUM] ALIGNE(32);
u8 * get_ldet_buf()
{
    u32 len = LDET_WIDTH * LDET_HEIGH * 3 / 2 * LDET_BUF_NUM;

    if(len > (VID1_FBUF_SIZE - AUD_FBUF_SIZE)){
        return NULL;
    }

    return (g_vbuf1 + AUD_FBUF_SIZE);
}

void ldet_putf(void * parm)
{
    //putchar('Y');
    if(lane_dect_warning(g_ldet_ptr)){
        puts("w\n");
        lane_dect_clr_warning(g_ldet_ptr);
        g_ldet_cnt++;
        if(g_ldet_cnt >= 1){
            g_ldet_status = 1;
            dac_api_put_wmsg();//报警
        }
    }else{
        g_ldet_status = 0;
        g_ldet_cnt = 0;
    }

    if(g_ldet_old_status != g_ldet_status){

        if(g_ldet_status){//报警
            //这里就发送命令去显示
          put_msg_fifo(MSG_LDETE_WARNING ,MSG_TO_PROCESS ,UI_DISP_N) ;
        }else{//去除警报
            //这里就发送命令去显示
         put_msg_fifo(MSG_LDETE_WARNING ,MSG_TO_PROCESS ,UI_DISP_N) ;
        }
    }
    g_ldet_old_status = g_ldet_status;
}

#endif

void lvd_cfg(u8 x)
{
 LVD_CON = 0;

 LVD_CON |= (0x3 & (x)); // 0： 2.2V; 1:2.4V; 2:2.6V; 3:2.8V;

    if ((CHIP_ID & 0xf) < 3)
        LVD_CON |=  BIT(2)|BIT(3); //使能 LVD 复位
    else
        LVD_CON &=  ~(BIT(2)|BIT(3)); //使能 LVD 复位
}

extern u8 not_open_dsc_flag;
extern IMGLAYER *image0;
extern IMGLAYER *image1;
#if 1 //def N_BOOT_SCR
const u8 boot_res_filename[] = "BOOT.RES\0";
const u8 close_res_filename[]="CLOSE.RES\0";
void disp_buf_init(u8 *buf,u8 num,u16 width,u16 height,u32 color)
{
    u32 offset = 0;
    u8 i;
    offset = 0;
    for(i=0;i<num;i++)
    {
        memset(&buf[offset],(color>>16)&0xff,width*height);
        offset += width*height;
        memset(&buf[offset],(color>>8)&0xff,width*height/4);
        offset += width*height/4;
        memset(&buf[offset],color,width*height/4);
        offset += width*height/4;
    }
}
#endif


static u8 power_crtl_flag = FALSE;
u8 get_power_ctrl_flag()
{
    return power_crtl_flag;
}
void first_power_crtl()
{
    usb_charge_detect_init();
    delay(1000);
    PWR_IO_INIT();
    if (usb_is_chargeing() || PWR_IO_READ())
        return;

    if (read_pdflag())
    {
        if (LVD_CON & BIT(15))
        {
            LVD_CON |= BIT(14);//CLEAR PND
            puts("\n clear lvd pnd 1\n");
        }
        puts("\n tttttttttttt 0\n");
        if (check_firsttime_power_tmp() == 1)
            power_crtl_flag = TRUE;
        else
            power_crtl_flag = FALSE;
    }
    else
    {
        puts("\n tttttttttttt 1\n");
        power_crtl_flag = FALSE;
    }

}

void lvd_reset_ctrl(u8 mode)
{
    PWR_IO_INIT();
    if (usb_is_chargeing() || PWR_IO_READ() || (get_power_ctrl_flag() == TRUE))
    {
        puts("\n usb is charging lvd return\n");
        if (LVD_CON & BIT(15))
        {
            LVD_CON |= BIT(14);//CLEAR PND
            puts("\n clear lvd pnd 1\n");
        }
        return;
    }


    if (mode & 0x03)
    {
        //lvd 复位，重新关机
        //PWR_CTL 置 0
        puts("\n xxx LVD APP_PWR_OFF \n");
        if (LVD_CON & BIT(15))
        {
            LVD_CON |= BIT(14);//CLEAR PND
            puts("\n clear lvd pnd\n");
        }
        if (check_firsttime_power_tmp() == 1)
            power_crtl_flag = TRUE;

    }
}



extern int lcd_fb_display_px(WORD color, int x, int y);

void lcd_test()
{
    //u16 scr_w, scr_h;
    //lcd_get_size(&scr_w, &scr_h);

    	u16 x,y,rgb,lcd_x = 0, lcd_y =32;

	for ( y = 0; y < 240; y++ ) {
        for ( x = 0; x < 400; x++ ) {
					//rgb = gvBitmapTable[ ( y  ) + x ];
                    //lcd_fb_display_px(rgb, x, y);
				}
			}

}


bool check_tvout_flag = FALSE;  //AVOUT JOHNSON 20160530 ADD
extern void infones_task(void *p_arg);
u32 reset_flag;
u32 flash_size;
void start_task(void *p_arg)
{
    u32 err;
    u32 expire_time;
    u32 msg ;
    msg_t *msg_d;
    msg_devicechange_t *devsta;
    u16 scr_w,scr_h;
    u32 cpu_sr;

#ifdef FLASH_LIGHT //红外灯开机置为关 JOHNSON 20160618 add
    HL_LED_CLOSE();
    puts("\n HL_LED_CLOSE mainnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn\n");
#endif

    g_startup_finish = 0;
    set_special_flag(1);

//    puts("\ninit_tick_timer\n");
    OSInitTick(OS_TICKS_PER_SEC);

    puts("timer1_init\n");
    timer1_init();
    wdt_ctf_ctrl(16);//配置看门狗
    SENSOR1_PWR_CTRL();


    rtc_init();
    reset_rtc();
    power_onoff_init();

    ALCD_BL_ON();

#ifdef BORAD_V11
    if(g_chip_id >1)
    {
        check_firsttime_power();
    }
#endif

    first_power_crtl();
    lvd_reset_ctrl(reset_flag);

     lvd_cfg(1);

    lcd_get_size(&scr_w, &scr_h);

//    dsc1_small_ptr = g_dbuf + scr_w * scr_h * 3 / 2 * 3; //old
//    dsc1_half_ptr = g_dbuf + (scr_w / 2) * scr_h * 3 / 2 * 3;
//    dsc1_small_ptr = g_dbuf + 720 * 576 * 3 / 2 * 3; //AVOUT JOHNSON 20160530
//    dsc1_half_ptr = g_dbuf + (720 / 2) * 576 * 3 / 2 * 3;
    dsc1_all_ptr = g_dbuf;


    //disp0
//    disp0_info.drc = isp0_name;
//    disp0_info.x = 0;
//    disp0_info.y = 0;
//    disp0_info.des_w = scr_w ;
//    disp0_info.des_h = scr_h ;
//    disp0_info.des = lcd_name;
//    disp0_info.frame_cb = NULL;
//
//    disp0_info.yuv_val = lcd_get_bottom_color();
//
    disp0_info.dsc_buf = g_dbuf;//malloc(disp0_info.des_w * disp0_info.des_h *3/2*3);
//    if(disp0_info.dsc_buf == NULL){
//        puts("\ndsc0 buf null\n");
//    }
//
//    disp1
//    disp1_info.drc = isp1_name;
//    disp1_info.x = SMALL_WIN_START_X;
//    disp1_info.y = SMALL_WIN_START_Y;
//    disp1_info.des_w = SMALL_WIN_WIDTH;
//    disp1_info.des_h = SMALL_WIN_HEIGH;
//    disp1_info.des = lcd_name;
//    disp1_info.frame_cb = NULL;
//
//    disp1_info.yuv_val = lcd_get_bottom_color();
//
//    disp1_info.dsc_buf = dsc1_small_ptr;//malloc(scr_w * scr_h *3/2*3);//先申请满屏的显存
//    if(disp1_info.dsc_buf == NULL){
//        puts("\ndsc1 buf null\n");
//    }
//
//    g_double_dsc_status = DOUB_DSC_IDLE;
//    g_double_dsc_status = DOUB_DSC_FRONT_WIN;
//
//    g_vid0_buf = g_vbuf0;//malloc(VID0_FBUF_SIZE);
//    if(g_vid0_buf == NULL){
//        puts("vid0 malloc err\n");
//    }
//
//    g_vid1_buf = g_vbuf1;//malloc(VID1_FBUF_SIZE);
//    if(g_vid1_buf == NULL){
//        puts("vid1 malloc err\n");
//    }
#ifndef  N_BOOT_SCR
    set_audio_clk();

    u8 samp_rate = LADC_SAMP_RATE_8KHZ;
    if(audio_open(&samp_rate, AUDIO_LADC_TYPE) == 0){
        puts("open ladc err\n");
        while(1);
    }

    dac_api_init();
    dac_api_open();
#endif
    video_ram_init();
    init_sys_status();
    fs_mutex_init();

    dev_init(LIB_PRIO_DRV,0) ;
    ves_init() ;

    app_flash_reg();
    Load_vfs(5);
    flash_size = sdfile_init((void *)dev_get_flash_hdl(), (void *)g_sdfile_head_offset);
    if( (flash_size != (1*1024*1024)) && (flash_size != (2*1024*1024))
        && (flash_size != (4*1024*1024)) && (flash_size != (8*1024*1024))
        && (flash_size != (16*1024*1024)) ){
        puts("\n\n\nflash_size err\n\n\n");
        put_u32hex(flash_size);
        while(1);
    }
    vm_api_init(dev_get_flash_hdl(), flash_size - 68 * 1024);
    disp_vm_init();


    //config_disp();//配置、启动显示相关的模块 lcd imb


#if 1 //AVOUT JOHNSON 20160530 ADD
    ///检测是否tvout输出
    avout_det_io_init();
    os_time_dly(10);
    LCD_MODE = LCD_TYPE;
    if(avout_det_io())
    {
        puts("AVOUT1111111111111111111111111111111111");
        check_tvout_flag = TRUE;
        LCD_MODE = LCD_AVOUT;
        if (sys_info.TV_mode ==  0)//pal
        {
            lcd_size_init(720, 576);
        }
        else if (sys_info.TV_mode == 1)//ntsc
        {
            lcd_size_init(720, 480);
        }
    }
    else
    {
        puts("ILI9325 22222222222222222222222222222222222222222222");
        lcd_size_init(400,240);//(320,240);
    }
    lcd_get_size(&scr_w, &scr_h);
    disp0_info.des_w = scr_w ;
    disp0_info.des_h = scr_h ;

//     while(1)
//    {
//      HL_LED_CLOSE();
//      os_time_dly(100);
//      os_time_dly(100);
//      os_time_dly(100);
//      os_time_dly(100);
//      os_time_dly(100);
//      os_time_dly(100);
//      os_time_dly(100);
//      os_time_dly(100);
//      os_time_dly(100);
//      os_time_dly(100);
//      HL_LED_OPEN();
//      os_time_dly(100);
//      os_time_dly(100);
//      os_time_dly(100);
//      os_time_dly(100);
//      os_time_dly(100);
//      os_time_dly(100);
//      os_time_dly(100);
//      os_time_dly(100);
//      os_time_dly(100);
//      os_time_dly(100);
//
//    }


    config_disp(LCD_MODE);//配置、启动显示相关的模块 lcd imb



#endif


#if 1
        //#ifndef  N_BOOT_SCR  //JOHNSON 20160607
    disp_startlogo();//开机图

    //not_open_dsc_flag = TRUE; //5203 JOHNSON 20160509
    //#endif  //JOHNSON 20160607

    DAA_CON2 |= (BIT(6)|BIT(4));//MIC 放大1000倍 //jm


    //#ifndef N_BOOT_SCR   //JOHNSON 20160607

//    audio_ctl(AUDIO_CMD_LADC_SET_MIC_VOL,(void *)((xxxxx));//xxxxx取值 0 到63
    audio_ctl(AUDIO_CMD_LADC_SET_RL_VOL,(void *)((31*256)+31));//
    audio_ctl(AUDIO_CMD_LADC_SET_MIC_VOL,(void *)(15));//x取值 0 到63 mic增益倍数
    ALCD_BL_ON();
    dac_api_play_stone();
    //#endif   //JOHNSON 20160607
#endif
//    lcd_test();
//***************************************
#if 0 //JOHNSON 20160530
   #ifdef N_BOOT_SCR

    disp_buf_init(g_dbuf,3,scr_w,scr_h,0x008080);

    if(video_init((u8*)boot_res_filename,NULL))
    {
        video_task_create(VIDEO_TASK_PRIO,VIDEO_TASK_N);
 //       ALCD_BL_ON();
    }
    set_audio_clk();
    u8 audio_samp_rate = video_get_audio_sample();
    u8 samp_rate;
    //if(audio_samp_rate == 16)
    //samp_rate = LADC_SAMP_RATE_16KHZ;//LADC_SAMP_RATE_8KHZ;
    //else if(audio_samp_rate == 8)
    samp_rate = LADC_SAMP_RATE_8KHZ;
    if(audio_open(&samp_rate, AUDIO_LADC_TYPE) == 0){
        puts("open ladc err\n");
        while(1);
    }
    dac_api_init();
    dac_api_open();
    u16 dac_sample;// = DAC_16KHZ;
    if(audio_samp_rate == 16)
        dac_sample = DAC_16KHZ;
    else if(audio_samp_rate == 8)
        dac_sample = DAC_8KHZ;
    //audio_ctl(AUDIO_CMD_DAC_SET_SRATE,(void*)&dac_sample);
    audio_ctl(AUDIO_CMD_LADC_SET_RL_VOL,(void*)(25*256+25));//(void*)(25*256+25)
    audio_ctl(AUDIO_CMD_LADC_SET_MIC_VOL,(void *)(15));//x取值 0 到63 mic增益倍数
    if(video_get_audio_number())
    {
    audio_ctl(AUDIO_CMD_DAC_SET_SRATE,(void*)&dac_sample);
    OSTimeDly(5);
    ALCD_BL_ON();
    dac_api_play_stone2();
    }
    else
    {
    dac_sample = DAC_8KHZ;
    audio_ctl(AUDIO_CMD_DAC_SET_SRATE,(void*)&dac_sample);
    OSTimeDly(5);
    ALCD_BL_ON();
    dac_api_play_stone();
    }
    video_wait_play_end();
    video_close();

    if (dac_sample != DAC_8KHZ)
    {
        dac_sample = DAC_8KHZ;
        audio_ctl(AUDIO_CMD_DAC_SET_SRATE,(void*)&dac_sample);
    }

    #endif
#endif
//***************************************

    adc_scan_init();
    ves_avin_init();
    clear_wakeu_pend();


    ves_dsensor_init();


#if LANE_DECT_EN

    g_ldet_info.src = IMC_SRC_ISP0;
    g_ldet_info.drc = isp0_name;
    g_ldet_info.dsc_buf = get_ldet_buf();
    g_ldet_info.buf_num = LDET_BUF_NUM;
    g_ldet_info.task_prio = LDET_TASK_PRIO;

    g_ldet_info.dsc_w = LDET_WIDTH;
    g_ldet_info.dsc_h = LDET_HEIGH;

    g_ldet_ptr = lane_dect_open(&g_ldet_info);
    if(g_ldet_ptr == NULL){
 //       puts("\n\n\n\n\n\nopen ldet fail\n\n\n\n\n\n");
    }else{
        timer_reg(500 , ldet_putf , NULL);
    }

#endif


    err = os_task_create(enc_app_task,0,ENC_TASE_PRIO, 0x2000,40,ENC_TASK_N) ;
    if(err){
        puts("\ncreat enc task err\n");
        while(1);
    }
//
    err = os_task_create(dec_app_task,0,DEC_TASE_PRIO, 0x2000,40,DEC_TASK_N) ;
    if(err){
        puts("\ncreat dec task err\n");
        while(1);
    }
//
    err = os_task_create(usb_app_task,0,USB_TASE_PRIO, 0x2000,40,USB_TASK_N) ;
    if(err){
        puts("\ncreat usb task err\n");
        while(1);
    }
//
    err = os_task_create(mode_sched,0,MODE_SW_PRIO, 0x2000,40,MODE_SW_N) ;
    if(err){
        puts("\ncreat      mode task err\n");
        while(1);
    }
//
	err = os_task_create(ui_disp_task,0,MODE_DISP_PRIO, 0x2000,40,UI_DISP_N) ;
    if(err){
        puts("\ncreat ui disp task err\n");
        while(1);
    }

    err = os_task_create(infones_task,0,INFONES_PRIO, 0x2000,40,INFONES_TASK_N) ;
    if(err){
        puts("\ncreat infones task err\n");
        while(1);
    }

    av_in_check_init();

    start_run();

    app_sd_reg();

    expire_time = 0;
    APP_SD_IO_HIGH();
//    delay(200);
    //while(APP_SD_IO_READ() && (expire_time < 20)) //20160115 san
    while((expire_time < 20)) //5203 JOHNSON 20160506
    {
        delay(100);
        expire_time++;
    }
    if(expire_time >= 20){ //< //解决不卡升级 5203 JOHNSON 20160513
        wait_sd_ok();
        upgrade_check();
    }

    usb_charge_detect_init();
    app_usb_reg();

    //这里必须要延时，如果是插着电脑USB开机，则g_usb_startup为1，这时候不可以打开双显示，否则可以打开
    //os_time_dly(100);
    expire_time = 0;
    while((g_startup_usbcharge == 0) && (expire_time < 10) )
    {
        expire_time++;
        os_time_dly(10);
    }

    key_disable(1);
    key_drv_init();//系统启动之后，按键才响应
    pwr_check_init();

    user_isp_cfg();
    //os_time_dly(200);//延长开机时间显示时间 JOHNSON 20160509
    updata_menu_cfg();
    os_time_dly(50);

	#ifdef N_BOOT_SCR
    ui_close_imb();//显示完开机图片或者开机动画后，要关闭IMB
	#endif
//    if(g_dsc0_hdl){//显示完后必须关闭图层
//        if(dsc_close(g_dsc0_hdl) == FALSE){
//            puts("\n\n\nclose dsc err\n\n\n");
//        }else{
//            g_dsc0_hdl = NULL;
//        }
//	}
//	if(g_dsc1_hdl){
//        if(dsc_close(g_dsc1_hdl) == FALSE){
//            puts("\n\n\nclose dsc1 err\n\n\n");
//        }else{
//            g_dsc1_hdl = NULL;
//        }
//	}


    if(g_usb_startup == 0)
    {
        not_open_dsc_flag = FALSE;
#ifdef N_BOOT_SCR
        image0 = dsc_imb_open_tmp(&disp0_info);
        image1 = dsc_imb_open_tmp(&disp1_info);
        extern void ui_close_imb1();
        ui_close_imb1();
#endif
        g_dsc0_hdl = dsc_open(&disp0_info,image0);
        if(g_dsc0_hdl){
            if(dsc_start(g_dsc0_hdl) == FALSE){
                puts("\n----dsc0 start err\n");
            }
        }else{
            puts("\n----dsc0 open err\n");
        }


        if(enc_ctrl.enc1_en)
        {
            g_dsc1_hdl = dsc_open(&disp1_info,image1);
            ui_open_imb();
            if(g_dsc1_hdl){
                if(dsc_start(g_dsc1_hdl) == FALSE){
                    puts("\n----dsc1 start err\n");
                }
            }else{
                puts("\n----dsc1 open err\n");
            }
            g_double_dsc_status = DOUB_DSC_BIG_SMALL;
        }
        else
        {
            extern IMGLAYER *image1;
            g_dsc1_hdl = dsc_open(&disp1_info, image1);
            if(g_dsc1_hdl){
                if(dsc_close(g_dsc1_hdl) == FALSE){
                    puts("\n\n\nclose dsc1 err\n\n\n");
                }else{
                    g_dsc1_hdl = NULL;
                }
            }
        }

    }
    g_startup_finish = 1;
    sys_status.gsensor_online =  gravity_api();

    put_u32hex(sys_status.sys_gsensor_wakeup);

    if(get_parking_guard() == 0 && sys_status.gsensor_online == TRUE)
    {
        if(sys_status.sys_gsensor_wakeup == 1 && is_sd_online() == TRUE )
        {
            enc_ctrl.lock_current_file = 1;
            puts("\n sys_gsensor_wakeup OK key \n");
            put_msg_fifo(MSG_KEY_OK ,MSG_TO_PROCESS ,MODE_SW_N) ;
        }
        else
        {
            sys_status.sys_gsensor_wakeup = 0;
        }
    }
    else
        sys_status.sys_gsensor_wakeup = 0;



//    g_startup_finish = 1;
    os_time_dly(40);
    key_disable(0);


    timer_reg(60, check_imc_isr ,NULL) ;

    extern void disp_note_m154_timer_reg();
    disp_note_m154_timer_reg();

    while(1)
    {
        msg = get_msg();
        if(msg  > (u32)0x2000000)
        {
            msg_d = msg ;
            msg = msg_d->type ;
//            printf("---------msg = %d ----------\n",msg);
        }

        switch(msg)
        {
            case MSG_DEVICECHANGE:

                free(msg_d);

            break;

            default :

            break;

        }
    }



}


//void main(u32 i, u32 j, u32 k)
//void main(void *pSDF,u32 spi_run_mode,u32 boot_pll_info)
void main(u32 sdfile_head_offset,u32 spi_run_mode,u32 boot_pll_info)
{
    volatile u32 err;

    g_sdfile_head_offset = sdfile_head_offset;

    FUSB_CON0 = 0;

    DISABLE_INT();

    APP_FUSB_IO();
    ALCD_BL_OFF();

//    LDO_CON &= ~BIT(1);
//    LDO_CON |= (BIT(2) | BIT(0));

//    sys_clk_init(SRC_HTC, 6200000, 160);
//    PH13_clk_out(PH13_CLOCK_OUT_SYS);

    init_clk_info(boot_pll_info);

    uart_init();
    puts_without_mutex("uart_init------------------------------------------main run-> \n");

//    put_u32hex(get_apb_clk());


    APP_PWR_ON();

    init_spi_info(spi_run_mode, boot_pll_info);

#if 1
//系统复位原因：0x00:上电复位  0x01:VCM复位 0x02:PB1或者PD1的“4”秒 “0”电平复位
//              0x03:LVD 低电压复位 0x04:WDT复位 0x05:软件复位
    //PWR_CON |= BIT(4);

    reset_flag = (PWR_CON & 0xe0) >> 5;
    puts("\nreset_flag:");
    put_u32hex(reset_flag);
#endif

#if SENSOR_VDD32
    /*------------SC1045&GC1014  power note -----------------*/
    LDO_CON &= ~((0x07)<<3); // DVDD 1.5V reset
    LDO_CON &= ~((0x07)<<6);  // AVDD&DOVDD 3.3V for SC1045&GC1014 reset
    LDO_CON |= (((0x01)<<3) | ((0x07)<<6));
    LDO_CON |= (BIT(10) | BIT(9) | BIT(8) | BIT(7) | BIT(6));//3.2V 输出使能
    /*------------SC1045&GC1014  power note -----------------*/
#else
    /*------------H42 power note -----------------*/
    LDO_CON &= ~((0x07)<<3); // DVDD 1.8V reset
    LDO_CON &= ~((0x07)<<6);  // AVDD&DOVDD 3.3V for H42 reset
    LDO_CON |= (((0x05)<<3) | ((0x02)<<6));
    LDO_CON |= (BIT(10) | BIT(9));//2.8V 1.8V输出使能
    /*------------H42 power note -----------------*/
#endif

    g_chip_id = (CHIP_ID & 0xf);
    g_spec_val = 0;

#ifdef DOUBLE_VEDIO
    if(g_chip_id == 0){
        g_spec_val = 32;
    }
#endif

    hwi_init();
    HWI_Install(1, exception_isr, 0);


    os_init();
    err = init_uart_mutex();


    err = os_task_create(start_task,0,START_TASK_PRIO, 0x2000,40,START_TASK_N) ;

    puts_without_mutex("osmain os_start.\n");
    err = os_start();
    ENABLE_INT();

      while(1)
      {
          asm("nop;");
            asm("nop;");
              asm("nop;");
                asm("nop;");
      }
}




















