
#ifndef _DA380_H
#define _DA380_H

#include "i2c.h"
#include "iic.h"
#include "typedef.h"



//#if 0
//da380
#define WRITE_COMMAND_FOR_DA380       0x4e
#define READ_COMMAND_FOR_DA380        0x4f


//Read Only
#define RESET_DA380         0x00
#define CHIPID_DA380        0x01
#define ACC_X_LSB           0x02
#define ACC_X_MSB           0x03
#define ACC_Y_LSB           0x04
#define ACC_Y_MSB           0x05
#define ACC_Z_LSB           0x06
#define ACC_Z_MSB           0x07
#define MOTION_FLAG         0x09
#define NEWDATA_FLAG        0x0A
#define TAP_ACTIVE_STATUS   0x0B//Read Only
#define ORIENT_STATUS       0x0C

//R/W
#define RESOLUTION_RANGE    0x0F//
#define ODR_AXIS            0x10
#define MODE_BW             0x11
#define SWAP_POLARITY       0x12
#define INT_SET1            0x16//R/W
#define INT_SET2            0x17
#define INT_MAP1            0x19
#define INT_MAP2            0x1A
#define INT_CONFIG          0x20
#define INT_LTACH           0x21
#define FREEFALL_DUR        0x22
#define FREEFALL_THS        0x23
#define FREEFALL_HYST       0x24
#define ACTIVE_DUR          0x27
#define ACTIVE_THS          0x28
#define TAP_DUR             0x2A//
#define TAP_THS             0x2B//
#define ORIENT_HYST         0x2C
#define Z_BLOCK             0x2D

#define SELF_TEST           0x32
#define CUSTOM_OFF_X        0x38
#define CUSTOM_OFF_Y        0x39
#define CUSTOM_OFF_Z        0x3A
#define CUSTOM_FLAG         0x4E
#define CUSTOM_CODE         0x4F
#define Z_ROT_HODE_TM       0x51
#define Z_ROT_DUR           0x52
#define ROT_TH_H            0x53
#define ROT_TH_L            0x54

enum
{//分辨率选择 高 中 低
    G_SENSITY_HIGH = 1,
    G_SENSITY_MEDIUM,
    G_SENSITY_LOW,

};


enum
{//工作模式选择 正常  低电  暂停
    G_NORMAL_MODE,
    G_LOW_POWER_MODE,
    G_SUSPEND_MODE,
};


enum
{//gsensor菜单回调函数使用
    G_SENSOR_CLOSE = 0,
    G_SENSOR_HIGH ,
    G_SENSOR_MEDIUM,
    G_SENSOR_LOW,


    G_SENSOR_LOW_POWER_MODE,
};


extern u8 da380_sensor_command(u8 register_address, u8 function_command);//向register_address 写入命令function_command
extern u8 da380_sensor_get_data(u8 register_address);//读取register_address地址的数据

extern void da380_work_mode(u8 work_mode);// normal mode , low-power mode and suspend mode,three different power modes
extern void da380_motion_mode_interrupt(void);
extern void da380_resolution_range(u8 range);




extern s8 da380_check(void);
extern void da380_init(void);
extern void da380_gravity_sensity(u8 gsid,u8 sensity);
extern void da380_pin_int_interrupt(void *pr);


#endif


