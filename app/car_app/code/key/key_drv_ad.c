
#include "key_drv_ad.h"
#include "app_cfg.h"
#include "adc_scan.h"

#if USE_AD_KEY


#if USE_AD_KEY0

/*-------------ADKEY GROUP 1----------------*/
#define ADC0_33   (0x3FF)
#define ADC0_30   (0x3ff*30/33) //0x3A2
#define ADC0_27   (0x3ff*27/33) //0x345
#define ADC0_23   (0x3ff*23/33) //0x2C9
#define ADC0_20   (0x3ff*20/33) //0x26C
#define ADC0_17   (0x3ff*17/33) //0x1F0
#define ADC0_13   (0x3ff*13/33) //0x193
#define ADC0_10   (0x3ff*10/33) //0x136
#define ADC0_07   (0x3ff*07/33) //0xD9
#define ADC0_04   (0x3ff*04/33) //0x7C
#define ADC0_00   (0)

#define AD0_NOKEY0      ((ADC0_33 + ADC0_30)/2)
#define ADKEY0_0		((ADC0_30 + ADC0_27)/2)
#define ADKEY0_1		((ADC0_27 + ADC0_23)/2)
#define ADKEY0_2		((ADC0_23 + ADC0_20)/2)
#define ADKEY0_3		((ADC0_20 + ADC0_17)/2)
#define ADKEY0_4		((ADC0_17 + ADC0_13)/2)
#define ADKEY0_5		((ADC0_13 + ADC0_10)/2)
#define ADKEY0_6		((ADC0_10 + ADC0_07)/2)
#define ADKEY0_7		((ADC0_07 + ADC0_04)/2)
#define ADKEY0_8		((ADC0_04 + ADC0_00)/2) //

static const u16 ad_key0_table[] = {ADKEY0_0,ADKEY0_1,ADKEY0_2,ADKEY0_3,ADKEY0_4,ADKEY0_5,ADKEY0_6,ADKEY0_7,ADKEY0_8};

static const u8 adkey0_msg_table[4][10];

//AD按键初始化函数
void ad_key0_init(void)
{
#ifdef DOUBLE_VEDIO
//    PORTA_PD &= ~BIT(2);
//    PORTA_PU &= ~BIT(2);
//    PORTA_DIR |= BIT(2);
//    PORTA_DIE &= ~BIT(2);
    PORTH_PD &= ~BIT(1); //5203 JOHNSON 20160509
    PORTH_PU &= ~BIT(1);
    PORTH_DIR |= BIT(1);
    PORTH_DIE &= ~BIT(1);
#else
    PORTH_PD &= ~BIT(8);
    PORTH_PU &= ~BIT(8);
    PORTH_DIR |= BIT(8);
    PORTH_DIE &= ~BIT(8);
#endif
}

/*----------------------------------------------------------------------------*/
/**@brief   A/D 按键序号获取函数
   @param   key_value：AD键值
   @return  按键序号
   @author  Change.tsai
   @note    u8 ad_key(tu16 key_value)
*/
/*----------------------------------------------------------------------------*/
u8 ad_key0_get_value(void)
{
    u8 key_num;
    u16 key_value;
    key_value = adc_channel_value[AD_CH_KEY0];

    if (key_value > AD0_NOKEY0)
    {
        return NO_KEY;
    }

//    put_u16hex(key_value);
    for(key_num=0; key_num < (sizeof(ad_key0_table)/sizeof(ad_key0_table[0])); key_num++)
    {
        if (key_value > ad_key0_table[key_num])
        {
            break;
        }
    }
    return key_num;
}

u8 ad_key0_pressed_to_msg(u8 key_style, u8 key_num)
{
    printf("key_style=%d  key_num=%d",key_style,key_num);
    return adkey0_msg_table[key_style][key_num];
}
#ifdef DOUBLE_VEDIO
#define ADKEY0_SHORT	\
                        /*00*/    NO_MSG,\
                        /*01*/    NO_MSG,\
                        /*02*/    NO_MSG,\
                        /*03*/    MSG_KEY_LOCK,\
                        /*04*/    MSG_KEY_MODE,\
                        /*05*/    MSG_KEY_PREV,\
                        /*06*/    MSG_KEY_NEXT,\
                        /*07*/    MSG_KEY_MENU,\
                        /*08*/    MSG_KEY_OK,\
                        /*09*/    MSG_KEY_START,



#define ADKEY0_LONG		\
                        /*00*/    NO_MSG,\
                        /*01*/    NO_MSG,\
                        /*02*/    NO_MSG,\
                        /*03*/    NO_MSG,\
                        /*04*/    NO_MSG,\
                        /*05*/    NO_MSG,\
                        /*06*/    NO_MSG,\
                        /*07*/    NO_MSG,\
                        /*08*/    NO_MSG,\
                        /*09*/    MSG_KEY_CLOSE,
#else
#define ADKEY0_SHORT	\
                        /*00*/    NO_MSG,\
                        /*01*/    NO_MSG,\
                        /*02*/    NO_MSG,\
                        /*03*/    NO_MSG,\
                        /*04*/    MSG_KEY_START,\
                        /*05*/    MSG_KEY_MODE,\
                        /*06*/    MSG_KEY_PREV,\
                        /*07*/    MSG_KEY_OK,\
                        /*08*/    MSG_KEY_NEXT,\
                        /*09*/    MSG_KEY_MENU,
#define ADKEY0_LONG		\
                        /*00*/    NO_MSG,\
                        /*01*/    NO_MSG,\
                        /*02*/    NO_MSG,\
                        /*03*/    NO_MSG,\
                        /*04*/    MSG_KEY_CLOSE,\
                        /*05*/    NO_MSG,\
                        /*06*/    NO_MSG,\
                        /*07*/    NO_MSG,\
                        /*08*/    NO_MSG,\
                        /*09*/    NO_MSG,
#endif


#define ADKEY0_HOLD		\
                        /*00*/    NO_MSG,\
                        /*01*/    NO_MSG,\
                        /*02*/    NO_MSG,\
                        /*03*/    NO_MSG,\
                        /*04*/    NO_MSG,\
                        /*05*/    NO_MSG,\
                        /*06*/    NO_MSG,\
                        /*07*/    MSG_KEY_OK_3S,\
                        /*08*/    NO_MSG,\
                        /*09*/    NO_MSG,

#define ADKEY0_LONG_UP	\
                        /*00*/    NO_MSG,\
                        /*01*/    NO_MSG,\
                        /*02*/    NO_MSG,\
                        /*03*/    NO_MSG,\
                        /*04*/    NO_MSG,\
                        /*05*/    NO_MSG,\
                        /*06*/    NO_MSG,\
                        /*07*/    MSG_KEY_OK_3S,\
                        /*08*/    NO_MSG,\
                        /*09*/    NO_MSG,

static const u8 adkey0_msg_table[4][10] =
{
    /*短按*/	    {ADKEY0_SHORT},
    /*长按*/		{ADKEY0_LONG},
    /*连按*/		{ADKEY0_HOLD},
    /*长按抬起*/	{ADKEY0_LONG_UP},
};

#endif


#if USE_AD_KEY1

/*-------------ADKEY GROUP 1----------------*/
#define ADC1_33   (0x3FF)
#define ADC1_30   (0x3ff*30/33)
#define ADC1_27   (0x3ff*27/33)
#define ADC1_23   (0x3ff*23/33)
#define ADC1_20   (0x3ff*20/33)
#define ADC1_17   (0x3ff*17/33)
#define ADC1_13   (0x3ff*13/33)
#define ADC1_10   (0x3ff*10/33)
#define ADC1_07   (0x3ff*07/33)
#define ADC1_04   (0x3ff*04/33)
#define ADC1_00   (0)

#define AD1_NOKEY0      ((ADC1_33 + ADC1_30)/2)
#define ADKEY1_0		((ADC1_30 + ADC1_27)/2)
#define ADKEY1_1		((ADC1_27 + ADC1_23)/2)
#define ADKEY1_2		((ADC1_23 + ADC1_20)/2)
#define ADKEY1_3		((ADC1_20 + ADC1_17)/2)
#define ADKEY1_4		((ADC1_17 + ADC1_13)/2)
#define ADKEY1_5		((ADC1_13 + ADC1_10)/2)
#define ADKEY1_6		((ADC1_10 + ADC1_07)/2)
#define ADKEY1_7		((ADC1_07 + ADC1_04)/2)
#define ADKEY1_8		((ADC1_04 + ADC1_00)/2)

static const u16 ad_key1_table[] = {ADKEY1_0,ADKEY1_1,ADKEY1_2,ADKEY1_3,ADKEY1_4,ADKEY1_5,ADKEY1_6,ADKEY1_7,ADKEY1_8};

static const u8 adkey1_msg_table[4][10];

//AD按键初始化函数
void ad_key1_init(void)
{
    PORTH_PD &= ~BIT(7);
    PORTH_PU &= ~BIT(7);
    PORTH_DIR |= BIT(7);
    PORTH_DIE &= ~BIT(7);
}

/*----------------------------------------------------------------------------*/
/**@brief   A/D 按键序号获取函数
   @param   key_value：AD键值
   @return  按键序号
   @author  Change.tsai
   @note    u8 ad_key(tu16 key_value)
*/
/*----------------------------------------------------------------------------*/
u8 ad_key1_get_value(void)
{
    u8 key_num;
    u16 key_value;
    key_value= adc_channel_value[AD_CH_KEY1];

    if (key_value > AD1_NOKEY0)
    {
        return NO_KEY;
    }

//    put_u16hex(key_value);
    for(key_num=0; key_num < (sizeof(ad_key1_table)/sizeof(ad_key1_table[0])); key_num++)
    {
        if (key_value > ad_key1_table[key_num])
        {
            break;
        }
    }
    return key_num;
}

u8 ad_key1_pressed_to_msg(u8 key_style, u8 key_num)
{
    return adkey1_msg_table[key_style][key_num];
}



#define ADKEY1_SHORT	\
                        /*00*/    NO_MSG,\
                        /*01*/    NO_MSG,\
                        /*02*/    NO_MSG,\
                        /*03*/    NO_MSG,\
                        /*04*/    NO_MSG,\
                        /*05*/    NO_MSG,\
                        /*06*/    NO_MSG,\
                        /*07*/    NO_MSG,\
                        /*08*/    NO_MSG,\
                        /*09*/    NO_MSG,

#define ADKEY1_LONG		\
                        /*00*/    NO_MSG,\
                        /*01*/    NO_MSG,\
                        /*02*/    NO_MSG,\
                        /*03*/    NO_MSG,\
                        /*04*/    NO_MSG,\
                        /*05*/    NO_MSG,\
                        /*06*/    NO_MSG,\
                        /*07*/    NO_MSG,\
                        /*08*/    NO_MSG,\
                        /*09*/    NO_MSG,


#define ADKEY1_HOLD		\
                        /*00*/    NO_MSG,\
                        /*01*/    NO_MSG,\
                        /*02*/    NO_MSG,\
                        /*03*/    NO_MSG,\
                        /*04*/    NO_MSG,\
                        /*05*/    NO_MSG,\
                        /*06*/    NO_MSG,\
                        /*07*/    NO_MSG,\
                        /*08*/    NO_MSG,\
                        /*09*/    NO_MSG,

#define ADKEY1_LONG_UP	\
                        /*00*/    NO_MSG,\
                        /*01*/    NO_MSG,\
                        /*02*/    NO_MSG,\
                        /*03*/    NO_MSG,\
                        /*04*/    NO_MSG,\
                        /*05*/    NO_MSG,\
                        /*06*/    NO_MSG,\
                        /*07*/    NO_MSG,\
                        /*08*/    NO_MSG,\
                        /*09*/    NO_MSG,

static const u8 adkey1_msg_table[4][10] =
{
    /*短按*/	    {ADKEY1_SHORT},
    /*长按*/		{ADKEY1_LONG},
    /*连按*/		{ADKEY1_HOLD},
    /*长按抬起*/	{ADKEY1_LONG_UP},
};

#endif

#endif	//#if USE_AD_KEY

