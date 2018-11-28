

#ifndef _G_SENSOR_API_H
#define _G_SENSOR_API_H


#include "da380.h"
#include "gravity_sensor_manage.h"
#include "usb_slave_api.h"


extern u8 write_command_to_gravity_sensor(u8 write_command, u8 register_address, u8 function_command);
extern u8 read_data_from_gravity_sensor(u8 write_command, u8 read_command, u8 register_address);

extern u8 gravity_sensor_command(u8 write_command, u8 register_address, u8 function_command);
extern u8 gravity_sensor_get_data(u8 write_command, u8 read_command, u8 register_address);




#endif
