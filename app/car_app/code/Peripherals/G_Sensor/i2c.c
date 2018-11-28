
#include "i2c.h"

///IIC��������
void init_i2c_io(void)
{
    A_SDA_PU();//����sda��scl��������
    A_SCL_PU();
}

void start_I2c(void)
{
    A_SDA_H();  //�����߸ߵ�ƽ
    I2C_DELAY();

    A_SCL_H();  //��ʱ���߸ߵ�ʱ���������ɸ�����
    I2C_DELAY();

    A_SDA_L();
    I2C_DELAY();

    A_SCL_L();  //�����߻ָ��͵�ƽ��������������Ҫ
    I2C_DELAY();
}

void stop_I2c(void)
{
    A_SCL_L();
    I2C_DELAY();

//    A_SDA_OUT();
//    I2C_DELAY();

    A_SDA_L();
    I2C_DELAY();

    A_SCL_H();
    I2C_DELAY();

    A_SDA_H();
    I2C_DELAY();
}

u8 wait_Ack(void)
{
    u16 ErrTime = 0;

    A_SDA_IN();//SDA����Ϊ����
    I2C_DELAY();

    SET_SDA_H();
    I2C_DELAY();

    A_SCL_H();
    I2C_DELAY();
    I2C_DELAY();
    I2C_DELAY();
    I2C_DELAY();
    I2C_DELAY();
    I2C_DELAY();

    while(A_SDA_STATE())//�ȴ�
    {
        ErrTime++;
        if (ErrTime > 250)
        {
            stop_I2c();
//            puts("\nErrTime out \n");
            return 1;
        }
    }

    A_SCL_L();//ʱ�����0
    I2C_DELAY();
    return 0;
}

void Ack(void)
{
    A_SCL_L();
    I2C_DELAY();

    A_SDA_L();
    I2C_DELAY();

    A_SCL_H();
    I2C_DELAY();

    A_SCL_L();
    I2C_DELAY();
}

void no_Ack(void)
{
    A_SCL_L();
    I2C_DELAY();

    A_SDA_H();
    I2C_DELAY();

    A_SCL_H();
    I2C_DELAY();

    A_SCL_L();
    I2C_DELAY();

}

u8 write_byte_to_slave(u8 m_data)
{
    u8 j, tem;

    A_SCL_L();
    I2C_DELAY();


    for (j = 0; j < 8; j++)
    {
        if ((m_data << j) & 0x80)
        {
            A_SDA_H();
        }
        else
        {
            A_SDA_L();
        }
        I2C_DELAY();
        A_SCL_H();
        I2C_DELAY();
        A_SCL_L();
        I2C_DELAY();
    }
    I2C_DELAY();

    tem = !wait_Ack();
    A_SDA_OUT();//����SDAΪ���״̬

    return (tem);//tem 1Ϊ��Ӧ���ź�
}

u8 read_byte_from_slave(void)
{
    u8 read, j;
    read = 0x00;

    A_SDA_IN();//����SDAΪ����
    I2C_DELAY();
    for (j = 8; j > 0; j--)
    {
        A_SCL_L();
        I2C_DELAY();
        A_SCL_H();
        I2C_DELAY();
        read = (read << 1);
        if (A_SDA_STATE())
        {
            read = read + 1;
        }
        A_SCL_L();
        I2C_DELAY();

        I2C_DELAY();
    }

    return (read);
}














