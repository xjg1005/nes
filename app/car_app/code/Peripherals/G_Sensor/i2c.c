
#include "i2c.h"

///IIC驱动程序
void init_i2c_io(void)
{
    A_SDA_PU();//配置sda和scl上拉电阻
    A_SCL_PU();
}

void start_I2c(void)
{
    A_SDA_H();  //数据线高电平
    I2C_DELAY();

    A_SCL_H();  //在时钟线高的时候数据线由高至低
    I2C_DELAY();

    A_SDA_L();
    I2C_DELAY();

    A_SCL_L();  //数据线恢复低电平，单操作函数必要
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

    A_SDA_IN();//SDA设置为输入
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

    while(A_SDA_STATE())//等待
    {
        ErrTime++;
        if (ErrTime > 250)
        {
            stop_I2c();
//            puts("\nErrTime out \n");
            return 1;
        }
    }

    A_SCL_L();//时钟输出0
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
    A_SDA_OUT();//设置SDA为输出状态

    return (tem);//tem 1为有应答信号
}

u8 read_byte_from_slave(void)
{
    u8 read, j;
    read = 0x00;

    A_SDA_IN();//设置SDA为输入
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














