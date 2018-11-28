#include "powercheck.h"
#include "adc_scan.h"

/*
阈值计算：
((value/3.0)*1024)/3.3 = PWR_VALUEXXX  1/3分压
*/
#if ENABLE_SAMPLE_VAL
#define PWR_VALUE_3_50		0x163//0x16A  // 3.5V
#define PWR_VALUE_3_55		0x168//0x16F  // 3.55V
#define PWR_VALUE_3_60		0x16D//0x174  // 3.6V
#define PWR_VALUE_3_65		0x172//0x179  // 3.65V
#define PWR_VALUE_3_70		0x177//0x17E  // 3.7V
#define PWR_VALUE_3_75		0x17D//0x183  // 3.75V
#define PWR_VALUE_3_80		0x184//0x189  // 3.8V

#define PWR_VALUE_3_85		0x189//0x18E  // 3.85V
#define PWR_VALUE_4_00		0x198//0x19D  // 4.00V
#define PWR_VALUE_4_05		0x19C//0x1A2  // 4.05V
#define PWR_VALUE_4_10		0x1A1//0x1A8  // 4.1V
#define PWR_VALUE_4_20		0x1AD//0x1B2  // 4.2V
#else
#define LDO_REFERENCE_VOL	    124		//1.24基准电压
#define VOL_3_70				370		//3.7V
#define VOL_3_75				375		//3.7V
#define VOL_3_85				385		//3.85V
#define VOL_4_00				400		//4.00V
#define VOL_4_05				405		//4.05V
#endif

#if ENABLE_SAMPLE_VAL
//u16 power_stag[] = {PWR_VALUE_3_60, PWR_VALUE_3_70, PWR_VALUE_3_85, PWR_VALUE_4_00, PWR_VALUE_4_05, PWR_VALUE_4_10};
u16 power_stag[] = {PWR_VALUE_3_50,PWR_VALUE_3_75, PWR_VALUE_3_85, PWR_VALUE_4_00, PWR_VALUE_4_05};
#else
u16 power_stag[] = {VOL_3_70, VOL_3_85, VOL_4_00, VOL_4_05};
#endif
#if USE_POWER_CHECK

u16 get_power_value(void)
{
	u32 ldo_in, ldo_refer;
	u16 val;
//    puts("get_power_value:");

    ldo_in = adc_channel_value[AD_CH_POWER];//*0x365;//*386;//*0x181;
    ldo_refer = adc_channel_value[AD_CH_LDO];

//    put_u16hex(ldo_in);
//    put_u16hex(ldo_refer);
#if ENABLE_SAMPLE_VAL
    val = ldo_in * 0x181 / ldo_refer;
#else
    val = (ldo_in * 3 * LDO_REFERENCE_VOL + 0x181 * 2) / ldo_refer;
#endif

//    put_u16hex(val);
    return val;
}

static u8 pwr_return = NO_PWR;

u8 power_scan()
{
    static u8 pwr_scan_cnt;
    static u8 pwr_saved_level = NO_PWR;

    u8 pwr_level;

    u16 pwr_vol = get_power_value();

    //获取电压值对应的消息
    for(pwr_level = 0; pwr_level<sizeof(power_stag)/sizeof(u16); pwr_level++)
    {
        if (pwr_vol <= power_stag[pwr_level])
            break;
    }
    if (pwr_level > 2)
        pwr_level--;

    //去抖
    if (pwr_level == NO_PWR || pwr_level != pwr_saved_level)
    {
        pwr_saved_level = pwr_level;
        pwr_scan_cnt = 0;
    }
    else
    {
        pwr_scan_cnt++;
        if (pwr_scan_cnt == PWR_SCAN_TIMES)			//去抖
        {
            pwr_return = pwr_saved_level;
            return pwr_return;
        }
        else if(pwr_scan_cnt > PWR_SCAN_TIMES)
        {
            pwr_scan_cnt = PWR_SCAN_TIMES - 1;
        }
    }

    //put_u32hex(pwr_return);

    return pwr_return;
}

void pwr_check_init()
{
    timer_reg(100, power_scan, NULL);
}

u8 get_power_level(void)
{
//    if(pwr_return == NO_PWR)
//        return 0;
    return pwr_return;
}

#else
u8 power_scan()
{
    return -1;
}

void pwr_check_init()
{

}

#endif
