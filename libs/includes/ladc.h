
#ifndef __LADC_H__
#define __LADC_H__


#include "config.h"


//buf标志位，0：buf1可读写   1：buf0可读写
#define LADC_BUF_FLAG()                 (LADC_CON & BIT(8))

#define LADC_PND()          (LADC_CON & BIT(7))
#define LADC_CPND()         LADC_CON |= BIT(6)
#define LADC_IE()           (LADC_CON & BIT(5))



#define LADC_SAMP_RATE_44_1KHZ           0
#define LADC_SAMP_RATE_48KHZ             1
#define LADC_SAMP_RATE_32KHZ             2
#define LADC_SAMP_RATE_22_05KHZ          3
#define LADC_SAMP_RATE_24KHZ             4
#define LADC_SAMP_RATE_16KHZ             5
#define LADC_SAMP_RATE_11_025KHZ         6
#define LADC_SAMP_RATE_12KHZ             7
#define LADC_SAMP_RATE_8KHZ              8



/* 注意：采样line in通道时，左右声道各自只能选择一个,例如选择了line_in0左,就不能选择line_in1和line_in2的左声道采样*/

#define LADC_CH_LIN0_L                      1//采样源为LINE IN0(PB12)的左声道输出
#define LADC_CH_LIN0_R                      2//采样源为LINE IN0(PB14)的右声道输出
#define LADC_CH_LIN1_L                      3//采样源为LINE IN1(PC0)的左声道输出
#define LADC_CH_LIN1_R                      4//采样源为LINE IN1(PC1)的右声道输出
#define LADC_CH_MIC                         5//采样源为MIC的放大输出 (PB15)
#define LADC_CH_PB13                        6//采样源为PB13
#define LADC_CH_PB14                        7//采样源为PB14
#define LADC_CH_PB15                        8//采样源为PB15
#define LADC_CH_PC0                         9//采样源为PC0
#define LADC_CH_PC1                         10//采样源为PC1






typedef int (*ladc_func)(void *priv,void *buf , u32 len);
typedef int (*ladc_func_end)(void *priv,void * ptr );

typedef struct _LADC_INF
{
    u16 samp_ch;                     //采样通道类型
    u32 samp_pack_size;              //采样包大小,每隔多少数据调用一下ladc_out_end
    ladc_func ladc_out;              //一次采样的输出回调函数
    ladc_func_end ladc_out_end;      //一次采样的输出结束回调函数
    void * priv;                     //传进回调函数的参数
    void * ptr;

}LADC_INF;

typedef struct _LADC_VAR
{
    u8 * ch;        //存放采样数据起始地址
    u8 ch_num;      //采样通道号
    u16 ch_type;    //采样通道类型
    u32 pack_offset;//当前包大小
    u32 pack_size;//采样包大小

    ladc_func output;       //一次采样的输出回调函数
    ladc_func_end output_end;       //一次采样的输出结束回调函数
    void *priv;             //传进回调函数的参数
    void * ptr;

}LADC_VAR;


u32 ladc_open(u8 samp_rate);
void ladc_analog_init();
void ladc_analog_io_init(u16 samp_ch);
void ladc_io_init(u16 samp_ch);
u32 ladc_start(LADC_INF * ladc_info);
u32 ladc_stop(LADC_VAR * ladc_var);
u32 ladc_close();
void ladc_isr(void);



void ladc_set_mic_vol(u8 mic_vol);
void ladc_set_linein_vol(u8 linein_vol);

#endif








