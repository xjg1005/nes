#include "adc_scan.h"
#include "app_cfg.h"
#include "timer.h"


#define AD_KEY_PA2		0x0	//已被使用
#define AD_KEY_PA5		0x1
#define AD_KEY_PG6 		0x2	//已被使用
#define AD_KEY_PG7 		0x3	//已被使用
#define AD_KEY_PH0		0x4
#define AD_KEY_PH1		0x5
#define AD_KEY_PH6		0x6	//已被使用
#define AD_KEY_PH7		0x7

#define AD_KEY_PH8 		0x8	//已被使用
#define AD_KEY_PH9 		0x9	//已被使用
#define AD_KEY_PH10 	0xa	//已被使用
#define AD_KEY_PH11 	0xb	//已被使用

#define RTC_P0_TG_OUT	0xc
#define RTC_P1_TG_OUT	0xd
#define ADC_V50_D		0xe

#define LDO_VBG	 		0xf

#define ADC_WTIME(x)    ADC_CON = (ADC_CON & ~(BIT(15)| BIT(14)| BIT(13)| BIT(12))) | (x<<12)
#define ADCSEL(x)       ADC_CON = (ADC_CON & ~(BIT(11)| BIT(10)| BIT(9)| BIT(8))) | (x<<8)
#define ADC_PND         ADC_CON & BIT(7)
#define KITSTART        ADC_CON |= BIT(6)
#define ADC_IE(x)       ADC_CON = (ADC_CON & ~BIT(5)) | (x<<5)
#define ADC_EN(x)       ADC_CON = (ADC_CON & ~BIT(4)) | (x<<4)
#define ADCEN(x)        ADC_CON = (ADC_CON & ~BIT(3)) | (x<<3)
#define ADC_BAUD(x)     ADC_CON = (ADC_CON & ~(BIT(2)| BIT(1)| BIT(0))) | (x)

u16 adc_channel_value[MAX_AD_CHANNEL];

static const u16 adc_scan_table[MAX_AD_CHANNEL] =
{
	LDO_VBG,
#if USE_AD_KEY0

#ifdef DOUBLE_VEDIO
    AD_KEY_PH1,//AD_KEY_PA2,//AD_KEY_PA13, //5203 JOHNSON 20160509
#else
    AD_KEY_PH8,//
#endif

#endif
#if USE_AD_KEY1
	AD_KEY_PH1,
#endif
#if USE_POWER_CHECK
	ADC_V50_D,//AD_KEY_PH1,//AD_KEY_PA12,
#endif
};

//------------------------------------ ADC_CON -------------------------------------//
// wait time | ADCSEL | adc_pnd | clr_wait_cnt | adc_ie | adc_en | ADCEN | adc_baud //
//  [15:12]  | [11:8] |    7    | clr_adc_pnd 6|    5   |    4   |   3   |  [2:0]   //
//----------------------------------------------------------------------------------//

void adc_scan_init(void)
{

    ADCSEL(adc_scan_table[AD_CH_LDO]);
    ADC_EN(1);
    ADCEN(1);
    ADC_BAUD(0x7);	//div: 1, 6, 12, 24, 48, 72, 96, 128
    ADC_WTIME(0x1);
    KITSTART;

    LDO_CON |= BIT(11);//VBG_EN
//    LDO_CON |= BIT(5);//使用adc 1.24基准电压时候需要配置
//    LDO_CON |= BIT(4);
//    LDO_CON &= (BIT(3) | BIT(2) | BIT(1) | BIT(0));


	timer_reg(1, adc_scan_process, NULL);

}

void adc_scan_process(void* parm)
{
    static u8 channel=0;

//    put_u16hex(ADC_RES);
    adc_channel_value[channel++] = ADC_RES;

    if (channel >= MAX_AD_CHANNEL)
    {
        channel = AD_CH_LDO;
    }

    ADCSEL(adc_scan_table[channel]);
    KITSTART;
}



