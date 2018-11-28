/*******************************************************************************************
 File Name: dac.h

 Version: 1.00

 Discription:


 Author:yulin deng

 Email :flowingfeeze@163.com

 Date:2014-01-13 17:09:41

 Copyright:(c)JIELI  2011  @ , All Rights Reserved.
*******************************************************************************************/
#ifndef dac_h
#define dac_h

#include "config.h"



typedef struct  __dac_wparm_t
{
u8 * buf;
u32 len;
} dac_wparm_t ;


typedef int (*dac_func)(void *priv,void *buf , u32 len);


typedef struct _DAC_INF
{
    dac_func dac_input;
    void *priv;

}DAC_INF;

typedef struct _DAC_VAR
{
    dac_func dac_input;
    void *priv;

}DAC_VAR;




#define DAC_44_1KHZ             44100
#define DAC_48KHZ               48000
#define DAC_32KHZ               32000
#define DAC_22_05KHZ            22050
#define DAC_24KHZ               24000
#define DAC_16KHZ               16000
#define DAC_11_025KHZ           11025
#define DAC_12KHZ               12000
#define DAC_8KHZ                8000


#define DAC_SR(x)           DAC_CON = ((DAC_CON & ~(BIT(0)|BIT(1)|BIT(2)|BIT(3))) | (x & 0xf))



/*------------------------------------------------------------------
   ------------DAC 配置选项 - 1----------------

    在音乐模式下，音量调节的是数字音量，
    如果要减少音量级数，需要修改数字音量表，但必须保留最大值：16384
    如果需要减小音乐模式下的DAC输出音量，可以对输出音量作以下的限制
-------------------------------------------------------------------*/
#define DECODE_VOL_MAX_L  31          //在音乐模式下，DAC输出音量最大值(0~31)
#define DECODE_VOL_MAX_R  31
//
////系统主音量级数
#define MAX_SYS_VOL_L    31
#define MAX_SYS_VOL_R    31


/*------------------------------------------------------------------

    ------------DAC 配置选项 - 2----------------

    DACVDD_VCOMO选择，DAC_VDD和VCOMO是同一PIN，
    同时只能选择其中一种功能，也可以两种都不选(DAC_VDD悬空)
-------------------------------------------------------------------*/
//#define DAC_VDD_EN                      //DAC_VDD外接电容，SNR可以提高1.5dB
//
////是否选择VCMO直推耳机
//#ifndef DAC_VDD_EN
//#define VCOMO_EN
//#endif

/*------------------------------------------------------------------

    ------------DAC 配置选项 - 3----------------

    按键音使能, 如果使用按键音，则不可以使用数据检测自动mute/unmute功能
-------------------------------------------------------------------*/
//#define KEY_TONE_EN

/*------------------------------------------------------------------

    ------------DAC 配置选项 - 4----------------

    数字通道发声时调节音量控制
        ——DECODER_ANALOG_MAX ： 此时调节数字音量，模拟音量为最大
        ——DECODER_DIGITAL_MAX ： 此时调节模拟音量，数字音量为最大
-------------------------------------------------------------------*/
//#define DECODER_ANALOG_MAX  0
//#define DECODER_DIGITAL_MAX BIT(7)

/*---DAC_CON sfr Setting----*/
//#define DAC_SR(x)           DAC_CON = ((DAC_CON & ~(BIT(0)|BIT(1)|BIT(2)|BIT(3))) | (x & 0xf))
//#define DAC_DIGITAL_EN(x)   DAC_CON = ((DAC_CON & ~BIT(4)) | ((x & 0x1)<<4))
//#define DAC_IE(x)           DAC_CON = ((DAC_CON & ~BIT(5)) | ((x & 0x1)<<5))
//#define DAC_CPND(x)         DAC_CON |= BIT(6)
//#define DAC_DCCS(x)         DAC_CON = ((DAC_CON & ~(BIT(12)|BIT(13)|BIT(14)|BIT(15))) | ((x & 0xf)<<12))

/*---DAA_CON0 sfr Setting----*/
//#define DAC_ANALOG_EN(x)    DAA_CON0 = ((DAA_CON0 & ~BIT(0)) | (x & 0x1))
//#define LDO1_EN(x)          DAA_CON0 = ((DAA_CON0 & ~BIT(2)) | ((x & 0x1)<<2))
//#define LDO2_EN(x)          DAA_CON0 = ((DAA_CON0 & ~BIT(3)) | ((x & 0x1)<<3))
//#define HP_R_EN(x)          DAA_CON0 = ((DAA_CON0 & ~BIT(4)) | ((x & 0x1)<<4))
//#define HP_L_EN(x)          DAA_CON0 = ((DAA_CON0 & ~BIT(5)) | ((x & 0x1)<<5))
//#define PNS_EN(x)           DAA_CON0 = ((DAA_CON0 & ~BIT(6)) | ((x & 0x1)<<6))
//#define MUTE_EN(x)          DAA_CON0 = ((DAA_CON0 & ~BIT(7)) | ((x & 0x1)<<7))
//
//#define ADC_BUF_GAIN(x)     DAA_CON0 = ((DAA_CON0 & ~(BIT(8)|BIT(9)|BIT(10))) | ((x & 0x7)<<8))
//#define ADC_BUF_BP(x)       DAA_CON0 = ((DAA_CON0 & ~BIT(11)) | ((x & 0x1)<<11))
//#define ADC_BUF_EN(x)       DAA_CON0 = ((DAA_CON0 & ~BIT(12)) | ((x & 0x1)<<12))
//#define TRIM_EN(x)          DAA_CON0 = ((DAA_CON0 & ~BIT(13)) | ((x & 0x1)<<13))
//#define TRIM_SEL(x)         DAA_CON0 = ((DAA_CON0 & ~BIT(14)) | ((x & 0x1)<<14))
//#define TRIM_SW(x)          DAA_CON0 = ((DAA_CON0 & ~BIT(15)) | ((x & 0x1)<<15))

/*---DAA_CON1 sfr Setting----*/
#define CHANNEL_L_GAIN(x)   DAA_CON1 = ((DAA_CON1 & ~(BIT(0)|BIT(1)|BIT(2)|BIT(3)|BIT(4))) | (x & 0x1f))
#define VREF_SEL(x)         DAA_CON1 = ((DAA_CON1 & ~BIT(5)) | ((x & 0x1)<<5))
#define LR_2_L(x)           DAA_CON1 = ((DAA_CON1 & ~BIT(6)) | ((x & 0x1)<<6))
#define LR_2_R(x)           DAA_CON1 = ((DAA_CON1 & ~BIT(7)) | ((x & 0x1)<<7))

#define CHANNEL_R_GAIN(x)   DAA_CON1 = ((DAA_CON1 & ~(BIT(8)|BIT(9)|BIT(10)|BIT(11)|BIT(12))) | ((x & 0x1f)<<8))
#define MIC_2_L(x)          DAA_CON1 = ((DAA_CON1 & ~BIT(14)) | ((x & 0x1)<<14))
#define MIC_2_R(x)          DAA_CON1 = ((DAA_CON1 & ~BIT(15)) | ((x & 0x1)<<15))


/*---DAA_CON2 sfr Setting----*/
//#define LIN0_CHANNL_L_EN(x) DAA_CON2 = ((DAA_CON2 & ~BIT(0)) | (x & 0x1))
//#define LIN0_CHANNL_R_EN(x) DAA_CON2 = ((DAA_CON2 & ~BIT(1)) | ((x & 0x1)<<1))
//#define LIN1_CHANNL_L_EN(x) DAA_CON2 = ((DAA_CON2 & ~BIT(2)) | ((x & 0x1)<<2))
//#define LIN1_CHANNL_R_EN(x) DAA_CON2 = ((DAA_CON2 & ~BIT(3)) | ((x & 0x1)<<3))
//#define LIN2_CHANNL_L_EN(x) DAA_CON2 = ((DAA_CON2 & ~BIT(4)) | ((x & 0x1)<<4))
//#define LIN2_CHANNL_R_EN(x) DAA_CON2 = ((DAA_CON2 & ~BIT(5)) | ((x & 0x1)<<5))
//#define AMUX_G(x)           DAA_CON2 = ((DAA_CON2 & ~BIT(6)) | ((x & 0x1)<<6))
//#define AMUX_EN(x)          DAA_CON2 = ((DAA_CON2 & ~BIT(7)) | ((x & 0x1)<<7))
//
//#define VCM_DET_EN(x)       DAA_CON2 = ((DAA_CON2 & ~BIT(8)) | ((x & 0x1)<<8))
//#define VCM_EN(x)           DAA_CON2 = ((DAA_CON2 & ~BIT(9)) | ((x & 0x1)<<9))
//#define VCM_OUT_EN(x)       DAA_CON2 = ((DAA_CON2 & ~BIT(10)) | ((x & 0x1)<<10))
//#define VCM_OUT_PD(x)       DAA_CON2 = ((DAA_CON2 & ~BIT(11)) | ((x & 0x1)<<11))
//#define DAC_OE(x)           DAA_CON2 = ((DAA_CON2 & ~BIT(12)) | ((x & 0x1)<<12))
//#define DCK_OE(x)           DAA_CON2 = ((DAA_CON2 & ~BIT(13)) | ((x & 0x1)<<13))
//#define DAC_EXT(x)          DAA_CON2 = ((DAA_CON2 & ~BIT(14)) | ((x & 0x1)<<14))


//#define DAC_SAMPLE_POINT    (32*2*2)
//#define DAC_BUF_LEN         DAC_SAMPLE_POINT
//#define DAC_DUAL_BUF_LEN    (DAC_SAMPLE_POINT * 2)


void dac_open(DAC_INF * info);
void *dac_get_cb();
void dac_isr();
void dac_set_samplerate(u16 rate);
//void dac_write(u8 *buf ,u32 len);
u32 dac_close();
void set_dac_vol(u16* vol);


#endif
