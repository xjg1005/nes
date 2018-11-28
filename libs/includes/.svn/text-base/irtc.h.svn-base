#ifndef _IRTC_H
#define _IRTC_H


#include "typedef.h"
#include "config.h"
#include "clock.h"
#include "HWI.h"


#define IRTC_X2IE(x)		IRTC_CON = ((IRTC_CON & ~(BIT(2))) | (x & 0x01)<<2)
#define IRTC_X512IE(x)		IRTC_CON = ((IRTC_CON & ~(BIT(4))) | (x & 0x01)<<4)
#define IRTC_WKIE(x)	    IRTC_CON = ((IRTC_CON & ~(BIT(6))) | (x & 0x01)<<6)


#define IRTC_WKCLRPND       IRTC_CON |= BIT(15)

#define IRTC_CS_EN			IRTC_CON |= BIT(0)
#define IRTC_CS_DIS			IRTC_CON &= ~BIT(0)

#define WRITE_RTC 			0x40
#define READ_RTC  			0xC0

#define WRITE_ALARM			0x10
#define READ_ALARM			0x90

#define WRITE_REG           0x20
#define READ_REG            0xA0

#define YEAR 		1927U//1925U
#define MONTH 		1
#define DAY 		1
#define HOUR 		0
#define MINUTES 	0
#define SECOND 		0

#define DATE 		4


typedef struct _RTC_TIME
{
    u16 dYear;		///<年份
    u8 	bMonth;		///<月份
    u8 	bDay;		///<天数
    u8 	bHour;		///<时
    u8 	bMin;		///<分
    u8 	bSec;		///<秒
//    u8 	bWeekday;	///<星期几
}RTC_TIME;


//RTC_TIME current_time;

//extern void  write_IRTC(u8 cmd, RTC_TIME *w_time);
//extern void  read_IRTC(u8 cmd, RTC_TIME *r_time);
extern void  write_sys_time(RTC_TIME *curr_time);
extern void  read_sys_time(RTC_TIME *curr_time);
extern void set_alarm(RTC_TIME *time);
extern void rtc_init(void);
extern void reset_rtc(void);

#endif




