/*******************************************************************************************
 File Name: HWI.h

 Version: 1.00

 Discription:


 Author:yulin deng

 Email :flowingfeeze@163.com

 Date:2013-09-14 16:47:39

 Copyright:(c)JIELI  2011  @ , All Rights Reserved.
*******************************************************************************************/
#ifndef HWI_h
#define HWI_h
#include "typedef.h"
#include "config.h"

#define TIME0_INIT          2
#define TIME1_INIT          3
#define TIME2_INIT          4
#define TIME3_INIT          5
#define FUSB_SOF            6
#define FUSB_CTRL           7

#define HUSB_SOF            6
#define HUSB_CTRL           7

#define RTC_INIT            8
#define ALINK_INIT          9
#define AUDIO_INIT          10
#define IOPROT_INIT	        11
#define SPI0_INIT           12
#define SPI1_INIT	        13
#define SD0_INIT            14
#define SD1_INIT            15
#define UART0_INIT          16
#define UART1_INIT          17
#define UART2_INIT          18
#define PAP_INIT            19
#define IIC_INIT            20
#define ADC_INIT            21
#define ISP0_INT            25
#define LVD_INIT            22
#define WTD_INIT            22
#define OSA_INIT            22
#define JPG0_INIT            23
#define JPG1_INIT            24
#define ISP0_INIT                25
#define ISP1_INIT                26
#define IMC_INIT            27
#define IMB_INIT         28
#define PWM0_1_INIT            29


#define SOFT_INIT           31


#define REG_INIT_HANDLE(a)\
extern void a();\
void handle__##a()\
{\
 asm("[--sp] = RETS ; call _ISR_PUSH_REG ; ") ;asm("link 12;call %0 ;unlink   \t\t ; call _ISR_POP_REG ; RETS = [sp++] ;rti; ": : "i" (a )); ; \
}


#define INTALL_HWI(a,b,c)\
extern void handle__##b();\
HWI_Install(a,handle__##b,c)

#define ENABLE_INT()   asm volatile ("[--sp] =r0 ; r0 = syscfg ;bitset(r0, 8);/*bitclr(r0, 9) ; */syscfg = r0 ;r0 =[sp++];")
#define DISABLE_INT()  asm("[--sp] =r0 ; r0 = syscfg ;bitclr(r0, 8) ; syscfg = r0 ;r0 =[sp++];")
#endif





