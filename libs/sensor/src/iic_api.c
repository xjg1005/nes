/*--------------------------------------------------------------------------*/
/**@file     iic_api.c
   @brief    IIC_APIģ��
   @details
   @author
   @date   2011-3-7
   @note
*/
/*----------------------------------------------------------------------------*/

#include "iic_io.h"

u32 iic_busy = 0; ///<iic��æ���
/*----------------------------------------------------------------------------*/
/**@brief   IICд����
   @param   chip_id ��Ŀ��IC��ID��
   @param   iic_addr: Ŀ��IC��Ŀ��Ĵ����ĵ�ַ
   @param   *iic_dat: д��Ŀ��IC�����ݵ�ָ��
   @param   n:��Ҫд�����ݵ���Ŀ
   @return  ��
   @note    void  iic_write(u8 chip_id,u8 iic_addr,u8 *iic_dat,u8 n)
*/
/*----------------------------------------------------------------------------*/
void  iic_writen(u8 chip_id,u8 iic_addr,u8 *iic_dat,u8 n)
{
    iic_busy  = 1;
    iic_start();                //I2C����
    iic_sendbyte(chip_id);         //д����
    if (0xff != iic_addr)
    {
        iic_sendbyte(iic_addr);   //д��ַ
    }
    for (;n>0;n--)
    {
        iic_sendbyte(*iic_dat++);      //д����
    }
    iic_stop();                 //I2Cֹͣʱ��
    iic_busy = 0;
}


void  iic_write(u8 chip_id,u8 iic_addr,u8 dat)
{
    iic_busy  = 1;
    iic_start();                //I2C����
    iic_sendbyte(chip_id);         //д����
    //if (0xff != iic_addr)
    {
        iic_sendbyte(iic_addr);   //д��ַ
    }
    //for (;n>0;n--)
    {
        iic_sendbyte(dat);      //д����
    }
    delay(1000);//76us ������ʱ64us
    iic_stop();                 //I2Cֹͣʱ��
    iic_busy = 0;
}
/*----------------------------------------------------------------------------*/
/**@brief   IIC������һ��Ŀ��ID��ȡ��������
   @param   address : Ŀ��ID
   @param   *p     :  �浵��ȡ�������ݵ�bufferָ��
   @param   number :  ��Ҫ��ȡ�����ݵĸ���
   @return  ��
   @note    void i2c_read_nbyte(u8 address,u8 *p,u8 number)
*/
/*----------------------------------------------------------------------------*/
void iic_readn(u8 chip_id,u8 iic_addr,u8 *iic_dat,u16 n)
{
    iic_busy = 1;
    iic_start();                //I2C����
    iic_sendbyte(chip_id);         //д����
    //if (0xff != iic_addr)
    {
        iic_sendbyte(iic_addr);   //д��ַ
    }
    iic_start();
    iic_sendbyte(chip_id|BIT(0));
    for (;n>1;n--)
    {
        *iic_dat++ = iic_revbyte(0);//������
        delay(1000);//76us ������ʱ64us
    }
    *iic_dat++ = iic_revbyte(1);
    delay(1000);//76us ������ʱ64us
    iic_stop();                 //I2Cֹͣʱ��
    iic_busy = 0;
}

u8 iic_read(u8 chip_id,u8 iic_addr)
{
    u8  byte;

    iic_busy = 1;
    iic_start();                    //I2C����
    iic_sendbyte(chip_id);             //д����
    iic_sendbyte(iic_addr);       //д��ַ
    iic_start();                    //дתΪ�������Ҫ�ٴ�����I2C
    iic_sendbyte(chip_id|BIT(0));             //������
    byte = iic_revbyte(1);
    delay(1000);//76us ������ʱ64us
    iic_stop();                     //I2Cֹͣ
    iic_busy = 0;
    return  byte;
}

