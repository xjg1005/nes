
#ifndef _GRAVITY_SENSOR_MANAGE_H
#define _GRAVITY_SENSOR_MANAGE_H

#include "da380.h"
#include "GMA301.h"
#include "stk8312.h"
#include "i2c.h"
#include "gravity_sensor_api.h"
#include "msg_pool.h"
#include "msg.h"
#include "os_api.h"
#include "usb_slave_api.h"
#include "my_ui.h"
#include "timer.h"

extern OS_TCB TaskStartTCB;

typedef struct
{
    u8   logo[20];
    s8   (*gravity_sensor_check)(void);
    void (*gravity_sensor_init)(void);
    void (*gravity_sensor_sensity)(u8 gsid,u8 sensity);
    void (*gravity_sensor_interrupt)(void *pr);
}G_SENSOR_INTERFACE;



//#define GMAX 2

extern const G_SENSOR_INTERFACE g_sensor_dev[];
extern u8 gravity_sen_dev_cur;

extern G_SENSOR_INTERFACE gravity_dev_con;

void g_sensor_int_scan(void);

extern s8 gravity_dev_check(void);//识别挂载相印的g_sensor

void g_sensor_timer_unreg(void);
extern u8 gravity_api(void);//直接在主任务初始化过程调用，就完成相应g_sensor的挂载和配置
extern void g_sensor_int_flag_check(void *pram);//g_sensor INT脚中断
extern void g_sensor_low_power_mode(u8 flag,u8 sensity);
extern u8 *get_cur_gsensor_version();

#endif
