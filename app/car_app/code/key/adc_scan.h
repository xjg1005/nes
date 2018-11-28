#ifndef __ADC_SCAN_H__
#define __ADC_SCAN_H__

#include "key_drv_manager.h"
#include "powercheck.h"

/*ADC通道定义*/
enum
{
    AD_CH_LDO = 0,
#if USE_AD_KEY0
    AD_CH_KEY0,
#endif
#if USE_AD_KEY1
    AD_CH_KEY1,
#endif
#if USE_POWER_CHECK
    AD_CH_POWER,
#endif
    MAX_AD_CHANNEL,
};


extern u16 adc_channel_value[MAX_AD_CHANNEL];


extern void adc_scan_init(void);

extern void adc_scan_process(void*);


#endif
