#ifndef _KEY_DRV_MANAGER_
#define _KEY_DRV_MANAGER_

#include "config.h"
#include "dv12.h"
#include "msg.h"
#include "pwr_onoff.h"


#define KEY_EN	1



#if KEY_EN
#define USE_AD_KEY0     1
#define USE_AD_KEY1		0
#define USE_IO_KEY		0
#define USE_IR_KEY		0
#define USE_TCH_KEY		0
#else
#define USE_AD_KEY0		0
#define USE_AD_KEY1		0
#define USE_IO_KEY		0
#define USE_IR_KEY		0
#define USE_TCH_KEY		0
#endif


#if ( (USE_AD_KEY0==1) || (USE_AD_KEY1==1) )
#define USE_AD_KEY	1
#else
#define USE_AD_KEY	0
#endif


#define NO_KEY          0xff
//#define NO_MSG          MSG_NONE	//0xff


/*按键门槛值*/
#define KEY_BASE_CNT  6
#define KEY_LONG_CNT  75
#define KEY_HOLD_CNT  15
#define KEY_SHORT_CNT 7

/*按键类型*/
#define KEY_SHORT_UP    0x0
#define KEY_LONG        0x1
#define KEY_HOLD        0x2
#define KEY_LONG_UP     0x3

extern void key_scan_process(void*);

extern void key_drv_init(void);

extern void key_disable(u8 mark);
extern u8 get_keystatus(void);
#endif
