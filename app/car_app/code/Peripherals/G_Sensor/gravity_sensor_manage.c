#include "gravity_sensor_manage.h"
#include "mode_switch.h"
#include "enc_mode.h"
#include "timer.h"

void iic_app_task(void *p_arg);

static timer_handle g_sensor_timer_hd;

void g_sensor_timer_unreg(void)
{
    if(g_sensor_timer_hd)
    {
        timer_unreg(g_sensor_timer_hd);
        g_sensor_timer_hd = NULL;
    }

}

static u8 no_g_sensor;//g sensor状态
u8 gravity_api(void)
{
    u32 err;
    no_g_sensor = TRUE;
    if (gravity_dev_check() != 0)//g_sensor_check
    {
        puts("\n no g_sensor\n");
        no_g_sensor = FALSE;
        return no_g_sensor;
    }

    sys_status.gsensor_online = TRUE;
    gravity_dev_con.gravity_sensor_init();

    gravity_dev_con.gravity_sensor_sensity(get_gravity_sensor(),0xFF);

    err = os_task_create(iic_app_task, 0, IIC_TASK_PRIO, 0x200, 40, IIC_TASK_N) ;
    if(err){
        puts("\ncreat iic task err\n");
        return -1;
    }

    g_sensor_timer_hd = timer_reg(30, g_sensor_int_flag_check, NULL);

    return no_g_sensor;
}

void iic_app_task(void *p_arg)
{
    u32 msg;
    msg_t *msg_d;
    while(1)
    {
       msg = get_msg();
        if(msg  > (u32)0x2000000)
        {
            msg_d = msg ;
            msg = msg_d->type ;
            printf("---------msg aas = %d ----------\n",msg);
        }

        switch(msg)
        {
            case MSG_DEVICECHANGE:
                free(msg_d);
            break;

            case MSG_CHECK_GSENSOR:
                if(sys_status.powerdown_flag == 0 )
                    g_sensor_int_scan();
            break;

            default :

            break;

        }
    }
}


extern OS_TCB TaskStartTCB;
///g_sensor_manage
u8 gravity_sen_dev_cur;		/*当前挂载的Gravity sensor*/
const G_SENSOR_INTERFACE g_sensor_dev [] =
{
    {//传感器一
        "-da380-\0",                  //gsensor name
        da380_check,                //检测该传感器的id是否正确
        da380_init,                 //如果id正确，则对该传感器进行初始化动作
        da380_gravity_sensity,      //该接口设置该传感器的灵敏程度
        da380_pin_int_interrupt,    //该接口为该传感器int脚，
                                    //有上升沿或者下降沿电平变化。作为外部中断源
    },

    {//传感器二
        "-301-\0",//gsensor
        gma301_check,
        gma301_init,
        gma301_gravity_sensity,
        gma301_pin_int_interrupt,
    },

    {//传感器三
        "-stk8312-\0",//gsensor
        stk8312_check,
        stk8312_init,
        stk8312_gravity_sensity,
        stk8312_pin_int_interrupt,
    },
};

/**************************************************************************************/
/**返回0 则初始化成功*/
s8 gravity0_sensor_check(void)
{
//    #if UART_DEBUG_EN == 0  //5203 JOHNSON 20160509
//     IOMC1 |= BIT(3);
//     #endif

    u8 i;
    u8 sensor_num = sizeof(g_sensor_dev) / sizeof(G_SENSOR_INTERFACE);

    if (gravity_sen_dev_cur >= sensor_num)
    {
        gravity_sen_dev_cur = 0;
    }

    for (i = sensor_num; i > 0; i--)
    {

        if (0 == g_sensor_dev[gravity_sen_dev_cur].gravity_sensor_check())
        {
            puts(g_sensor_dev[gravity_sen_dev_cur].logo);
            return 0;
        }

        ++gravity_sen_dev_cur;

        if (gravity_sen_dev_cur >= sensor_num)
        {
            gravity_sen_dev_cur = 0;
        }
    }

    return -1;
}


G_SENSOR_INTERFACE gravity_dev_con;

s8 gravity0_dev_init(void)
{
    s8 ret;

    ret = gravity0_sensor_check();
    if (0 == ret)
    {
        memcpy(&gravity_dev_con, &(g_sensor_dev[gravity_sen_dev_cur]), sizeof(G_SENSOR_INTERFACE));
        return 0;
    }
    return -1;
}


s8 gravity_dev_check(void)
{
    return (gravity0_dev_init());
}


void g_sensor_int_scan(void)
{
//    if (sys_status.gsensor_online == FALSE)
//    {
//        return ;
//    }

    gravity_dev_con.gravity_sensor_interrupt(NULL);
}

extern u8 g_h264_enc_status;
void g_sensor_int_flag_check(void *pram)
{
    if (g_startup_finish == FALSE)
    {
        return;
    }

    if((mode_ctrl.cur_mode & ENC_MODE))
        put_msg_fifo(MSG_CHECK_GSENSOR ,MSG_TO_PROCESS ,IIC_TASK_N) ;
}

void g_sensor_low_power_mode(u8 flag,u8 sensity)
{
//    if (sys_status.gsensor_online == FALSE)
//    {
//        return ;
//    }
//    extern timer_handle g_sensor_timer_reg;
//    if (g_sensor_timer_reg)
//    {
//        timer_unreg(g_sensor_timer_reg);
//        g_sensor_timer_reg = NULL;
//    }

    if (flag == TRUE)
    {
        gravity_dev_con.gravity_sensor_sensity(G_SENSOR_LOW_POWER_MODE,sensity);
//        gravity_dev_con.gravity_sensor_sensity(G_SENSOR_MEDIUM,sensity);
//        gravity_dev_con.gravity_sensor_sensity(G_SENSOR_MEDIUM);
        if (strcmp(gravity_dev_con.logo, "-stk8312-\0") == 0)
        {
            os_time_dly(200);
            stk8312_sensor_get_data(0x00);//clear int
            stk8312_sensor_get_data(0x01);//clear int
            stk8312_sensor_get_data(0x02);//clear int
            stk8312_sensor_get_data(0x03);//clear int
        }
        else if (strcmp(gravity_dev_con.logo, "-301-\0") == 0)
        {
            os_time_dly(200);
            gma301_sensor_get_data(0x1c);
            gma301_sensor_get_data(0x1D);
        }
    }
    else
    {
        gravity_dev_con.gravity_sensor_sensity(G_SENSOR_CLOSE,0XFF);
    }



}


u8 *get_cur_gsensor_version()
{
    if (sys_status.gsensor_online)
    {
        return (g_sensor_dev[gravity_sen_dev_cur].logo);
    }
    else
    {
        return ("no gSensor");
    }

}

