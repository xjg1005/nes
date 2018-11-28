
#include "gravity_sensor_api.h"


u8 write_command_to_gravity_sensor(u8 write_command, u8 register_address, u8 function_command)
{
    u8 tem;

    start_I2c();
    tem = write_byte_to_slave(write_command);//写命令
    if (!tem)
    {
        puts("\n\n$$$$1\n\n");
        return 0;
    }
    tem = write_byte_to_slave(register_address);//写寄存器地址
    if (!tem)
    {
        puts("\n\n$$$$2\n\n");
        return 0;
    }

    tem = write_byte_to_slave(function_command);//某功能对应的命令
    if (!tem)
    {
        puts("\n\n$$$$3\n\n");
        return 0;
    }

    stop_I2c();

    return 1;
}


u8 read_data_from_gravity_sensor(u8 write_command, u8 read_command, u8 register_address)
{
    u8 tem;
    u8 read_data;

    start_I2c();
    tem = write_byte_to_slave(write_command);//写命令
    if (!tem)
    {
        puts("\n\n$$$$fff1\n\n");
        return 0;
    }

    tem = write_byte_to_slave(register_address);//写寄存器地址
    if (!tem)
    {
        puts("\n\n$$$$fff2\n\n");
        return 0;
    }


    start_I2c();//从写数据状态转成读取数据状态
    tem = write_byte_to_slave(read_command);//读命令
    if (!tem)
    {
        puts("\n\n$$$$fff3\n\n");
        return 0;
    }

    read_data = read_byte_from_slave();//读取数据

    no_Ack();

    stop_I2c();

    return (read_data);
}


u8 gravity_sensor_command(u8 write_command, u8 register_address, u8 function_command)
{
    u8 tem;
    tem = write_command_to_gravity_sensor(write_command, register_address, function_command);
    return tem;
}

u8 gravity_sensor_get_data(u8 write_command, u8 read_command, u8 register_address)
{
    u8 gsensor_data;
    gsensor_data = read_data_from_gravity_sensor(write_command, read_command, register_address);
    return (gsensor_data);
}
