#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__


#include "dsc.h"
#include "lane_dect.h"
//#define  OSC_1PIN_EN  1
#define WDT_EN

#define FLASH_LIGHT//补光灯开关

#ifdef FLASH_LIGHT
    //#define HL_LED_OPEN()      PORTF_DIR &= ~BIT(13);PORTF_OUT |= BIT(13)
    //#define HL_LED_CLOSE()     PORTF_DIR &= ~BIT(13);PORTF_OUT &= ~BIT(13)
    #define HL_LED_CLOSE()      PORTH_DIR |= BIT(4);/*PORTH_OUT |= BIT(4)*/
    #define HL_LED_OPEN()     PORTH_DIR &= ~BIT(4);/*PORTH_OUT &= ~ BIT(4)*/
//    #define HL_LED_OPEN()      PORTF_DIR &= ~BIT(0);PORTF_OUT |= BIT(0)
//    #define HL_LED_CLOSE()     PORTF_DIR &= ~BIT(0);PORTF_OUT &= ~BIT(0)

#endif

#define LANE_DECT_EN    0
#define LSC_EN  0  //////如果用旧版工具生成的ISP_cfg文件时，需在app_cfg.h中把宏LSC_EN改成0；
#define   BORAD_V11
#define   UART_DEBUG_EN   0//0

#define  DOUBLE_VEDIO
#define  DEBUG_LCD  1
#define  DEBUG_ISP  0 //摄像头效果调节时候，需要打开此宏。反之，关闭清0

#define  SENSOR_VDD32 1  // 1:提供GC1014模组3.2V电压 0:提供H42模组 2.8V电压
//#define N_BOOT_SCR //开机动画使能

#ifdef DOUBLE_VEDIO
    #define AV_OUT      1
    #define LCD_C6		1//0
    #define LCD_43      0//1
    #define LCD_30		0
    #define DOUBLE_UI_A  1
//    #define DOUBLE_UI_B  0
#else
    #define AV_OUT      0
    #define LCD_C6		1
    #define SINGLE_UI_A  1
#endif

#ifdef DOUBLE_VEDIO             //5203 JOHNSON 20160509
#define SENSOR1_PWR_CTRL()      //PORTC_DIR &= ~BIT(1);PORTC_OUT |= BIT(1)
#else
    #define SENSOR1_PWR_CTRL()
#endif

#define APP_FUSB_IO()       FUSB_CON0 = 3
//输出
#define APP_FUSB_DP_H()     FUSB_IO_CON0 &= ~BIT(2);FUSB_IO_CON0 |= BIT(0)
#define APP_FUSB_DP_L()     FUSB_IO_CON0 &= ~BIT(2);FUSB_IO_CON0 &= ~BIT(0)

#define APP_FUSB_DM_H()     FUSB_IO_CON0 &= ~BIT(3);FUSB_IO_CON0 |= BIT(1)
#define APP_FUSB_DM_L()     FUSB_IO_CON0 &= ~BIT(3);FUSB_IO_CON0 &= ~BIT(1)

//输入
#define APP_FUSB_DP_IN()    FUSB_IO_CON0 |= BIT(2)
#define APP_FUSB_DM_IN()    FUSB_IO_CON0 |= BIT(3)
#define APP_FUSB_DP()       (FUSB_IO_CON0 & BIT(10))
#define APP_FUSB_DM()       (FUSB_IO_CON0 & BIT(11))


                                //5203 JOHNSON 20160509
#define AV_IN_DET_INIT()        //PORTF_DIR |= BIT(0);PORTF_PU &= ~BIT(0);PORTF_PD &= ~BIT(0)
#define AV_IN_INSERT()          0//( !(PORTF_IN & BIT(0)) )

#define PARKING_DET_INIT()      //PORTC_DIR |= BIT(0);PORTC_PU &= ~BIT(0);PORTC_PD &= ~BIT(0)
#define PARKING_DET_STATU()     0//     ( !(PORTC_IN & BIT(0)) )

#define   SENNOR_VERSION      get_cur_sensor_logo()
//#define   SENNOR_VERSION      "SENSOR_GC1043"
#define   SOFTWARE_VERSION    "Y303-9325-20160618"//"RT200B-160312"  //版本日期
#define   G_SENSOR_VERSION    get_cur_gsensor_version()
#define   UI_PARA_REFRESH     0x31

//#define   LINGDUPRJ //凌度版本
#ifdef LINGDUPRJ
#define   LD_PJ_VERSION      "6900L"   //凌度版本项目名称
#define   LD_SW_VERSION    "160309LJB"   //凌度版本日期
#endif

enum{
    DOUB_DSC_IDLE,
    DOUB_DSC_BIG_SMALL,     //´óÐ¡´°
    DOUB_DSC_HALF_WIN,      //¸÷°ëÆÁ
    DOUB_DSC_FRONT_WIN,     //Ö»Ç°´°
    DOUB_DSC_BACK_WIN,     //Ö»ºó´°
};

#if LANE_DECT_EN

#define SMALL_WIN_WIDTH     0//160//120
#define SMALL_WIN_HEIGH     0//120//160
#define SMALL_WIN_START_X     0//320//160
#define SMALL_WIN_START_Y     0//24//160

#else

#define SMALL_WIN_WIDTH     160//120
#define SMALL_WIN_HEIGH     120//160
#define SMALL_WIN_START_X     320//160
#define SMALL_WIN_START_Y     24//160


#endif


//任务名
#define START_TASK_N            "start_task"
#define MODE_SW_N               "mode_switch"
#define DEC_TASK_N              "dec_app_task"
#define ENC_TASK_N              "enc_app_task"
#define USB_TASK_N              "usb_app_task"
#define UI_PROCESS_N            "ui_process_task"
#define UI_DISP_N               "ui_disp_task"
#define DAC_TASK_N              "dac_app_task"
#define IIC_TASK_N              "iic_app_task"
#define VIDEO_TASK_N        "video_task"
#define INFONES_TASK_N            "infones_task"
//任务优先级

/*库里面已经使用的优先级, 系统所有任务的优先级不要重复*/
#define LIB_PRIO_ISP0   3
#define LIB_PRIO_DRV   12//31
#define LIB_PRIO_JPG_DEC   22
#define LIB_PRIO_UNPKG   20
#define LIB_PRIO_PKG0   18
#define LIB_PRIO_PKG1   19
#define LIB_PRIO_DYNHUFF  9
/*------------------------*/

#define START_TASK_PRIO         10
#define MODE_SW_PRIO            15
#define WFILE_TASE_PRIO         29//23
#define DEC_TASE_PRIO           27
#define ENC_TASE_PRIO           28
#define USB_TASE_PRIO           16
#define MODE_DISP_PRIO          11
#define DAC_TASK_PRIO           17
#define LDET_TASK_PRIO          8
#define IIC_TASK_PRIO            14
#define VIDEO_TASK_PRIO     13
#define INFONES_PRIO          30

/***********************************************/
/***********************************************/


#define PKG_TYPE_AVI        0x01
#define PKG_TYPE_JPG        0x02
//
#if 1 //JOHNSON 20160622

#define VID0_FBUF_SIZE       0//(2*1024*1024 )//3

#if LANE_DECT_EN
#define VID1_FBUF_SIZE       0//(1*1024*1024 - 512*1024)
#else
#define VID1_FBUF_SIZE       0//(1*1024*1024 + 512*1024)//1
#endif
#define AUD_FBUF_SIZE        0//(512*1024)

#else //old

                             //5203 JOHNSON 20160509 //(3*1024*1024)
#define VID0_FBUF_SIZE       0//(3*1024*1024 )//+ 512*1024 + 512*1024 + 250*1024)

#if LANE_DECT_EN
#define VID1_FBUF_SIZE       0//(1*1024*1024)
#else                        //5203 JOHNSON 20160509 //(1*1024*1024 + 256*1024)
#define VID1_FBUF_SIZE       0//(1*1024*1024 )//+ 512*1024  + 256*1024)
#endif

#define AUD_FBUF_SIZE        0//(512*1024)

#endif



extern u8 * g_vid0_buf;
extern u8 * g_vid1_buf;
extern u8 * g_aud_buf ;

extern disp_iparm_t disp0_info ,disp1_info;
extern dsc_hdl_t * g_dsc0_hdl;
extern dsc_hdl_t * g_dsc1_hdl;
extern u8 g_double_dsc_status;
extern u8 * dsc1_small_ptr;
extern u8 * dsc1_half_ptr;
extern u8 * dsc1_all_ptr;

extern u8 g_double_dsc_status;
extern u8 g_startup_finish ;
extern u8 g_ss_flag;
extern u8 g_startup_usbcharge ;

extern const u8 isp0_name[10] ;
extern const u8 isp1_name[10];
extern const u8 rep_name[10];
extern const u8 lcd_name[10] ;
extern const u8 avout_name[10] ;
extern u32 g_chip_id ;
extern u32 g_spec_val ;

#if LANE_DECT_EN

#define LDET_WIDTH  480
#define LDET_HEIGH  270

#endif

extern ldet_iparm_t g_ldet_info;
extern ldet_hdl_t * g_ldet_ptr;
extern u32 g_vanish_y ;
extern u32 g_carHead ;

void com_ASCII_ToUpper(void *buf, u32 len);
u32 com_ASCII_StrCmp(const u8 *src, const u8 *dst, u32 len);
u32 com_ASCII_StrLen(void *str , u32 len);
void app_sd_reg();

void av_in_check_init();


#endif

