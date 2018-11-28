
#ifndef GMA301_H
#define GMA301_H

#include "i2c.h"
#include "typedef.h"
#include "gravity_sensor_manage.h"

extern u8 gma301_sensor_command(u8 register_address, u8 function_command);//��register_address д������function_command
extern u8 gma301_sensor_get_data(u8 register_address);//��ȡregister_address��ַ������


extern void gma301_init(void);
extern void gma301_test(void);

extern s8 gma301_check(void);
extern void gma301_gravity_sensity(u8 gsid,u8 sensity);
extern void gma301_pin_int_interrupt(void *pr);
extern void delay_2ms(u32 cnt);

#define PULLING_GMA301  //��ѯ�����ݷ�ʽ�����жϷ�ʽ
#define WRITE_COMMAND_FOR_GMA301       0x30
#define READ_COMMAND_FOR_GMA301        0x31

#endif


