/*--------------------------------------------------------------------------*/
/**@file     iic_io.c
   @brief    IOģ���IIC������
   @details
   @author
   @date   2011-3-7
   @note
*/
/*----------------------------------------------------------------------------*/
#include "iic_io.h"

/*----------------------------------------------------------------------------*/
/**@brief   IIC��IO�ڳ�ʼ��
   @param   ��
   @return  ��
   @note    void iic_init_io(void)
*/
/*----------------------------------------------------------------------------*/
void iic_init_io(void)
{
    iic_data_out();         //SDA���ó����
    iic_clk_out();         	//SCL���ó����
	iic_clk_h();
	iic_delay();
    iic_data_h();
    iic_delay();
}

u16 iic_data_r()
{
    iic_data_in();
    iic_delay();
    return iic_data_read();//PORTA_IN&BIT(15);
}


/*----------------------------------------------------------------------------*/
/**@brief   ��ȡACK
   @param   ��
   @return  �����ACK/NACK�ĵ�ƽ
   @note    bool r_ack(void)
*/
/*----------------------------------------------------------------------------*/
void iic_start(void)
{
    //iic_init_io();
    iic_data_h();
    iic_delay();
    iic_clk_h();
    iic_delay();
    iic_data_l();
    iic_delay();
    iic_clk_l();
    iic_delay();
    //iic_data_h();
}

/*----------------------------------------------------------------------------*/
/**@brief   STOP IIC
   @param   ��
   @return  ��
   @note    void iic_stop(void)
*/
/*----------------------------------------------------------------------------*/
void iic_stop(void)
{
    iic_data_out();
    iic_data_l();
    iic_delay();
    iic_clk_h();
    iic_delay();
    iic_data_h();
    iic_delay();
	//iic_clk_l();  //IIC��ֹ�źţ�ʱ����Ӧ���õͣ���ʾ��ʱ���źŽ���
}

/*----------------------------------------------------------------------------*/
/**@brief   ��ȡACK
   @param   ��
   @return  �����ACK/NACK�ĵ�ƽ
   @note    bool r_ack(void)
*/
/*----------------------------------------------------------------------------*/
u8 r_ack(void)
{
    u8 nack;
    iic_data_in();
    iic_delay();
    iic_clk_h();
    iic_delay();
    nack = iic_data_r();
    iic_clk_l();
    iic_delay();
    return nack;
}

/*----------------------------------------------------------------------------*/
/**@brief   ����һ��ACK�źŵ�����,
   @param   flag �����͵�ACK/nack������
   @return  ��
   @note    void s_ack(u8 flag)
*/
/*----------------------------------------------------------------------------*/
void s_ack(u8 flag)
{
    iic_data_out();
    iic_clk_l();
    if (flag)
    {
        iic_data_h();
    }
    else
    {
        iic_data_l();
    }
    iic_delay();
    iic_clk_h();
    iic_delay();
    iic_clk_l();
}
/*----------------------------------------------------------------------------*/
/**@brief   ��IIC���߽���һ��BYTE������,
   @param   ��
   @return  ��ȡ�ص�����
   @note    u8 iic_revbyte_io( void )
*/
/*----------------------------------------------------------------------------*/
u8 iic_revbyte_io( void )
{
    u8 byteI2C = 0;
    u8 i;
    iic_data_in();
    iic_delay();
    for (i = 0;i < 8;i++)
    {
        iic_clk_h();
        byteI2C <<= 1;
        iic_delay();
        if (iic_data_r())
        {
            byteI2C++;
        }
        iic_clk_l();
        iic_delay();
    }
    return byteI2C;
}
/*----------------------------------------------------------------------------*/
/**@brief   ��IIC���߽���һ��BYTE������,������һ��ָ����ACK
   @param   para ������ACK ���� NACK
   @return  ��ȡ�ص�����
   @note    u8 iic_revbyte( u8 para )
*/
/*----------------------------------------------------------------------------*/
u8 iic_revbyte( u8 para )
{
    u8 byte;
    byte = iic_revbyte_io();
    s_ack(para);
    return byte;
}
/*----------------------------------------------------------------------------*/
/**@brief   ��IIC���߷���һ��BYTE������
   @param   byte ��Ҫд��EEROM�ĵ�ַ
   @return  ��
   @note    void iic_sendbyte_io(u8 byte)
*/
/*----------------------------------------------------------------------------*/
void iic_sendbyte_io(u8 byte)
{
    u8 i;
    iic_data_out();
    iic_delay();
    for (i = 0;i < 8;i++)
    {
        if (byte & BIT(7))
            iic_data_h();                //���λ�Ƿ�Ϊ1,Ϊ1��SDA= 1,���� SDA=0
        else
            iic_data_l();
        //iic_delay();
        iic_clk_h();
        iic_delay();
        byte <<= 1;                   //��������һλ,������һ������
        iic_clk_l();
        iic_delay();
    }
}
/*----------------------------------------------------------------------------*/
/**@brief   ��IIC���߷���һ��BYTE������,����ȡACK
   @param   byte ��Ҫд��EEROM�ĵ�ַ
   @return  ��
   @note    void iic_sendbyte(u8 byte)
*/
/*----------------------------------------------------------------------------*/
void iic_sendbyte(u8 byte)
{
    iic_sendbyte_io(byte);
    if(r_ack())
        ;//putchar('n');
    else
        ;//putchar('r');
}



