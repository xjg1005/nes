#include "da380.h"

#include "gravity_sensor_manage.h"
#include "mode_switch.h"
#include "enc_mode.h"

#if 1

S_IIC_FUN da380_iic_fun;

//gsensor_dev 1
void da380_iic_set(u8 gsensor_dev)
{
	iic_select(&da380_iic_fun, gsensor_dev);
}

u8 da380_sensor_command(u8 register_address, u8 function_command)
{
	da380_iic_fun.startSCCB();
	if(0 == da380_iic_fun.SCCBwriteByte(WRITE_COMMAND_FOR_DA380))
	{
		puts("a");
		da380_iic_fun.stopSCCB();
		return(0);
	}

	if(0 == da380_iic_fun.SCCBwriteByte((u8)(register_address)))
	{
		puts("b");
		da380_iic_fun.stopSCCB();
		return(0);
	}

  	if(0 == da380_iic_fun.SCCBwriteByte(function_command))
	{
		puts("c");
		da380_iic_fun.stopSCCB();
		return(0);
	}
  	da380_iic_fun.stopSCCB();

  	return(1);
}

u8 da380_sensor_get_data(u8 register_address)
{

    u8 regDat;
	//通过写操作设置寄存器地址
	da380_iic_fun.startSCCB();
	if(0 == da380_iic_fun.SCCBwriteByte(WRITE_COMMAND_FOR_DA380))
	{
		puts("1");
		da380_iic_fun.stopSCCB();
		return(0);
	}

    if(0 == da380_iic_fun.SCCBwriteByte((u8)(register_address)))
	{
		puts("2");
		da380_iic_fun.stopSCCB();
		return(0);
	}
	da380_iic_fun.stopSCCB();

	//设置寄存器地址后，才是读
	da380_iic_fun.startSCCB();
	if(0 == da380_iic_fun.SCCBwriteByte(READ_COMMAND_FOR_DA380))
	{
		puts("3");
		da380_iic_fun.stopSCCB();
		return(0);
	}

  	regDat = da380_iic_fun.SCCBreadByte();

  	da380_iic_fun.stopSCCB();

  	return (regDat);
}
#else
void da380_sensor_command(u8 register_address, u8 function_command)//往da380某寄存器写命令
{
    gravity_sensor_command(WRITE_COMMAND_FOR_DA380, register_address, function_command);
}

u8 da380_sensor_get_data(u8 register_address)//获取da380 sensor  数据
{
    return gravity_sensor_get_data(WRITE_COMMAND_FOR_DA380, READ_COMMAND_FOR_DA380, register_address);
}
#endif


//映射active interrupt to INT pin
void da380_int_map1(u8 map_int)
{
    switch(map_int)
    {
    case 0x04://active interrupt to INT
        da380_sensor_command(INT_MAP1, 0x04);
        break;
    }
}


#if 0
///获取当前三轴加速度值
void da380_get_acceleration(void)
{
///三轴加速度

    u16 acc_x, acc_y, acc_z;
    u8 acc_x_lsb, acc_x_msb;
    u8 acc_y_lsb, acc_y_msb;
    u8 acc_z_lsb, acc_z_msb;

    acc_x_lsb = da380_sensor_get_data(ACC_X_LSB);
    puts("\nACC_X_LSB : ");
    put_u16hex(acc_x_lsb);
    acc_x_msb = da380_sensor_get_data( ACC_X_MSB);
    puts("\nACC_X_MSB : ");
    put_u16hex(acc_x_msb);

    acc_x = (acc_x_lsb | (acc_x_msb << 8));

    acc_y_lsb = da380_sensor_get_data(ACC_Y_LSB);
    puts("\nACC_Y_LSB : ");
    put_u16hex(acc_y_lsb);
    acc_y_msb = da380_sensor_get_data(ACC_Y_MSB);
    puts("\nACC_Y_MSB : ");
    put_u16hex(acc_y_msb);

    acc_y = (acc_y_lsb | (acc_y_msb << 8));

    acc_z_lsb = da380_sensor_get_data(ACC_Y_LSB);
    puts("\nACC_Z_LSB : ");
    put_u16hex(acc_z_lsb);
    acc_z_msb = da380_sensor_get_data(ACC_Y_MSB);
    puts("\nACC_Z_MSB : ");
    put_u16hex(acc_z_msb);

    acc_z = (acc_z_lsb | (acc_z_msb << 8));
}
#endif

void da380_init(void)//config RESOLUTION_RANGE ,MODE_BW,INT_MAP1, ACTIVE_DUR, ACTIVE_THS
{
    u8 chipid;

    init_i2c_io();

    da380_sensor_command(RESET_DA380, 0x20);
    os_time_dly(20);

    da380_resolution_range(G_SENSITY_LOW);
    da380_sensor_command(ODR_AXIS,     0x06);//enable X/Y/Z axis,1000Hz
    da380_work_mode(G_LOW_POWER_MODE);//normal mode, 500Hz
    da380_sensor_command(SWAP_POLARITY,0x00);//remain the polarity of X/Y/Z-axis

/*INT中断配置， 分别有三种使用方式使能z轴，使能z和y轴，或者使能z,y和x，这三种方式*/

//    da380_sensor_command(INT_SET1,     0x04);//disable orient interrupt.enable the active interrupt for the  z,axis
//    da380_sensor_command(INT_SET1,     0x07);//disable orient interrupt.enable the active interrupt for the  z, y and x,axis
    ///Burgess_151210
	da380_sensor_command(INT_SET1,     0x27);//disable orient interrupt.enable the active interrupt for the  z, y and x,axis

    da380_sensor_command(INT_SET2,     0x00);//disable the new data interrupt and the freefall interupt
    da380_int_map1(0x04);             //mapping active interrupt to INT
    da380_sensor_command(INT_MAP2,     0x00);//doesn't mappint new data interrupt to INT
    da380_sensor_command(INT_CONFIG,   0x01);//push-pull output for INT ,selects active level high for pin INT
//    da380_sensor_command(INT_LTACH,    0x00);//reset all latched int, non-latched
    da380_sensor_command(INT_LTACH,    0x0E);///Burgess_151210
    da380_sensor_command(FREEFALL_DUR, 0x09);//freefall duration time = (freefall_dur + 1)*2ms
    da380_sensor_command(FREEFALL_THS, 0x30);//default is 375mg
    da380_sensor_command(FREEFALL_HYST,0x01);
    da380_sensor_command(ACTIVE_DUR,   0x11);//Active duration time = (active_dur + 1) ms
//    da380_sensor_command(ACTIVE_THS,   0xFF);//Threshold of active interrupt=Active_th*mg/LSB
    da380_sensor_command(ACTIVE_THS,   0x8F);
    da380_sensor_command(TAP_DUR,      0x04);//
    da380_sensor_command(TAP_THS,      0x0a);
    da380_sensor_command(ORIENT_HYST,  0x18);
    da380_sensor_command(Z_BLOCK,      0x08);
    da380_sensor_command(SELF_TEST,    0x00);//close self_test
    da380_sensor_command(CUSTOM_OFF_X, 0x00);
    da380_sensor_command(CUSTOM_OFF_Y, 0x00);
    da380_sensor_command(CUSTOM_OFF_Z, 0x00);
    da380_sensor_command(CUSTOM_FLAG,  0x00);
    da380_sensor_command(CUSTOM_CODE,  0X00);
    da380_sensor_command(Z_ROT_HODE_TM,0x09);
    da380_sensor_command(Z_ROT_DUR,    0xFF);
    da380_sensor_command(ROT_TH_H,     0x45);
    da380_sensor_command(ROT_TH_L,     0x35);

}


//工作分辨率选着
void da380_resolution_range(u8 range)
{
    switch(range)
    {
    case G_SENSITY_HIGH:
//        da380_sensor_command(RESOLUTION_RANGE, 0x00);//14bit +/-2g     对应分辨率4096 LSB/g   高
        ///Burgess_151210
        da380_sensor_command(RESOLUTION_RANGE, 0x0C);//14bit +/-2g     对应分辨率4096 LSB/g   高
        break;
    case G_SENSITY_MEDIUM:
//        da380_sensor_command(RESOLUTION_RANGE, 0x01);//14bit +/-4g     对应分辨率2048 LSB/g   中
        ///Burgess_151210
        da380_sensor_command(RESOLUTION_RANGE, 0x0D);//14bit +/-4g     对应分辨率2048 LSB/g   中
        break;
    case G_SENSITY_LOW:
//        da380_sensor_command(RESOLUTION_RANGE, 0x11);//14bit +/-16g    对应分辨率512 LSB/g
        ///Burgess_151210
        da380_sensor_command(RESOLUTION_RANGE, 0x0F);//14bit +/-16g    对应分辨率512 LSB/g
        break;
    }
}


//工作模式选着
void da380_work_mode(u8 work_mode)
{
    switch(work_mode)
    {
    case G_NORMAL_MODE:
        da380_sensor_command(MODE_BW, 0X1E);//normal mode
        break;
    case G_LOW_POWER_MODE:
        da380_sensor_command(MODE_BW, 0X5E);//low power mode
        break;
    case G_SUSPEND_MODE:
        da380_sensor_command(MODE_BW, 0X9E);//suspend mode
        break;
    }
}


/**提供给二级子菜单回调函数使用*/
void da380_gravity_sensity(u8 gsid,u8 sensity)
{

    if (gsid == G_SENSOR_HIGH)
    {
        da380_work_mode(G_NORMAL_MODE);//normal mode
        da380_resolution_range(G_SENSITY_HIGH);
        da380_sensor_command(ACTIVE_THS, 0x3F);//0x4F);
    }

    if (gsid == G_SENSOR_MEDIUM)
    {
        da380_work_mode(G_NORMAL_MODE);//normal mode
        da380_resolution_range(G_SENSITY_MEDIUM);
        da380_sensor_command(ACTIVE_THS, 0x5F);//0x8F);
    }

    if (gsid == G_SENSOR_LOW)
    {
        da380_work_mode(G_NORMAL_MODE);//normal mode
        da380_resolution_range(G_SENSITY_LOW);
        da380_sensor_command(ACTIVE_THS, 0x6F);//0xFF);
    }

    if (gsid == G_SENSOR_CLOSE)
    {
        da380_work_mode(G_SUSPEND_MODE);//暂停
    }

    if (gsid == G_SENSOR_LOW_POWER_MODE)//低功耗
    {
//        da380_work_mode(G_LOW_POWER_MODE);
        da380_work_mode(G_NORMAL_MODE);
         da380_sensor_command(INT_LTACH, 0x0F);//latched
        da380_resolution_range(G_SENSITY_MEDIUM);
//        da380_resolution_range(system_setting_function.set_gravity_reaction_id);
    }
}

/**
返回值 TRUE  重力传感器触发
       FALSE  重力传感器未触发
*/
u8 get_da380_int_state()
{
    u8 date_tmp;
//    date_tmp = da380_sensor_get_data(0x0b);

    //if (date_tmp)
        //put_u32hex(date_tmp);

//    if (date_tmp & 0x07)//Burgess_151210
//    {
//        return TRUE;
//    }
    date_tmp = da380_sensor_get_data(MOTION_FLAG);
    if (date_tmp & 0x24)//Burgess_151210
    {
        return TRUE;
    }
    return FALSE;
}
/**加速度超过与设定的阀值，产生中断。为当前视频文件上锁*/
void da380_pin_int_interrupt(void *pr)
{
    if (get_da380_int_state())
    {
//        gsensor_pr2Pnd_api(FALSE);
        if( (get_gravity_sensor() != 0)
             &&  (enc_ctrl.enc0_status == ENC_APP_RUNNING)
             &&  (mode_ctrl.cur_mode & ENC_MODE)
             &&  (enc_ctrl.lock_current_file  == 0)
        )
        {
                //printf("\n gma308 lock the file#######\n");
                puts("da\n");
                enc_ctrl.lock_current_file = 1;//锁当前文件
        }
    }
}
/**返回0 id正确*/
s8 da380_id_check(void)
{
    u8 i;
    u8 chipid = 0x00;
    puts("\n CHIPID_da380 : ");

    for(i=0;i<3;i++)
    {
        chipid = da380_sensor_get_data(CHIPID_DA380);
        put_u16hex(chipid);
        if (chipid == 0x13)
            break;
    }
    if (chipid != 0x13)
    {
        puts("\n  not da380 \n");
        return -1;
    }

    return 0;
}


/**返回0 id正确*/
s8 da380_check(void)
{
    init_i2c_io();

    da380_iic_set(1);

    da380_sensor_command(RESET_DA380, 0x20);

    if (0 != da380_id_check())
    {
        return -1;
    }

    return 0;
}








