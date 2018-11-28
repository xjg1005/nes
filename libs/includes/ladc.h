
#ifndef __LADC_H__
#define __LADC_H__


#include "config.h"


//buf��־λ��0��buf1�ɶ�д   1��buf0�ɶ�д
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



/* ע�⣺����line inͨ��ʱ��������������ֻ��ѡ��һ��,����ѡ����line_in0��,�Ͳ���ѡ��line_in1��line_in2������������*/

#define LADC_CH_LIN0_L                      1//����ԴΪLINE IN0(PB12)�����������
#define LADC_CH_LIN0_R                      2//����ԴΪLINE IN0(PB14)�����������
#define LADC_CH_LIN1_L                      3//����ԴΪLINE IN1(PC0)�����������
#define LADC_CH_LIN1_R                      4//����ԴΪLINE IN1(PC1)�����������
#define LADC_CH_MIC                         5//����ԴΪMIC�ķŴ���� (PB15)
#define LADC_CH_PB13                        6//����ԴΪPB13
#define LADC_CH_PB14                        7//����ԴΪPB14
#define LADC_CH_PB15                        8//����ԴΪPB15
#define LADC_CH_PC0                         9//����ԴΪPC0
#define LADC_CH_PC1                         10//����ԴΪPC1






typedef int (*ladc_func)(void *priv,void *buf , u32 len);
typedef int (*ladc_func_end)(void *priv,void * ptr );

typedef struct _LADC_INF
{
    u16 samp_ch;                     //����ͨ������
    u32 samp_pack_size;              //��������С,ÿ���������ݵ���һ��ladc_out_end
    ladc_func ladc_out;              //һ�β���������ص�����
    ladc_func_end ladc_out_end;      //һ�β�������������ص�����
    void * priv;                     //�����ص������Ĳ���
    void * ptr;

}LADC_INF;

typedef struct _LADC_VAR
{
    u8 * ch;        //��Ų���������ʼ��ַ
    u8 ch_num;      //����ͨ����
    u16 ch_type;    //����ͨ������
    u32 pack_offset;//��ǰ����С
    u32 pack_size;//��������С

    ladc_func output;       //һ�β���������ص�����
    ladc_func_end output_end;       //һ�β�������������ص�����
    void *priv;             //�����ص������Ĳ���
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








