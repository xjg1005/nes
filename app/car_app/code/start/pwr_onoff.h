
#ifndef __PWR_ONOFF_H__
#define __PWR_ONOFF_H__


#include "config.h"
#include "app_cfg.h"


#ifdef DOUBLE_VEDIO
//    #define APP_PWR_ON()        PORTF_DIR &= ~BIT(3);PORTF_OUT |= BIT(3)
//    #define APP_PWR_OFF()       PORTF_DIR &= ~BIT(3);PORTF_OUT &= ~BIT(3)
//    #define PWR_IO_INIT()       PORTB_DIR |= BIT(13);PORTB_PU &= ~BIT(13);PORTB_PD |= BIT(13) //上拉
//    #define PWR_IO_READ()       ( PORTB_IN & BIT(13) )
    //5203 JOHNSON 20170114
    #define APP_PWR_ON()        //PORTF_DIR &= ~BIT(3);PORTF_OUT |= BIT(3)
    #define APP_PWR_OFF()       //PORTF_DIR &= ~BIT(3);PORTF_OUT &= ~BIT(3)
    #define PWR_IO_INIT()       PORTC_DIR |= BIT(3);PORTC_PU &= ~BIT(3);PORTC_PD |= BIT(3)
    #define PWR_IO_READ()       ( PORTC_IN & BIT(3) )
#else
    #define APP_PWR_ON()        PORTA_DIR &= ~BIT(2);PORTA_OUT |= BIT(2)
    #define APP_PWR_OFF()       PORTA_DIR &= ~BIT(2);PORTA_OUT &= ~BIT(2)
    #define PWR_IO_INIT()       PORTH_DIR |= BIT(10);PORTH_PU &= ~BIT(10);PORTH_PD |= BIT(10)
    #define PWR_IO_READ()       ( PORTH_IN & BIT(10) )
#endif


typedef struct _PWR_ONOFF_T
{
    u8 status;
    u32 cnt;
    u8 auto_shutdown;//自动关机标志
    u32 auto_cnt;//自动关机计时器

}PWR_ONOFF_T;

void power_onoff_init();
u8 power_get_status();
void power_off();
void auto_power_off_ctrl(void);
void cfg_pdflag(u8 x);
#endif

