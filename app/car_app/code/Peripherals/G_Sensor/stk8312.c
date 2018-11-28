#include "gravity_sensor_manage.h"
#include "mode_switch.h"
#include "enc_mode.h"
#include "iic.h"


#define WRITE_COMMAND_FOR_STK8312   (0x3D << 1)
#define READ_COMMAND_FOR_STK8312    ((0x3D << 1) | BIT(0))

#if 1

S_IIC_FUN stk8312_iic_fun;

//gsensor_dev 1
void stk8312_iic_set(u8 gsensor_dev)
{
	iic_select(&stk8312_iic_fun, gsensor_dev);
}

u8 stk8312_sensor_command(u8 register_address, u8 function_command)
{

	stk8312_iic_fun.startSCCB();
	if(0 == stk8312_iic_fun.SCCBwriteByte(WRITE_COMMAND_FOR_STK8312))
	{
		puts("a");
		stk8312_iic_fun.stopSCCB();
		return(0);
	}

	if(0 == stk8312_iic_fun.SCCBwriteByte((u8)(register_address)))
	{
		puts("b");
		stk8312_iic_fun.stopSCCB();
		return(0);
	}

  	if(0 == stk8312_iic_fun.SCCBwriteByte(function_command))
	{
		puts("c");
		stk8312_iic_fun.stopSCCB();
		return(0);
	}
  	stk8312_iic_fun.stopSCCB();

  	return(1);
}

u8 stk8312_sensor_get_data(u8 register_address)
{
    u8 regDat;
	//ͨ��д�������üĴ�����ַ
	stk8312_iic_fun.startSCCB();
	if(0 == stk8312_iic_fun.SCCBwriteByte(WRITE_COMMAND_FOR_STK8312))
	{
		puts("1");
		stk8312_iic_fun.stopSCCB();
		return(0);
	}

  if(0 == stk8312_iic_fun.SCCBwriteByte((u8)(register_address)))
	{
		puts("2");
		stk8312_iic_fun.stopSCCB();
		return(0);
	}
	stk8312_iic_fun.stopSCCB();


	//���üĴ�����ַ�󣬲��Ƕ�
	stk8312_iic_fun.startSCCB();
	if(0 == stk8312_iic_fun.SCCBwriteByte(READ_COMMAND_FOR_STK8312))
	{
		puts("3");
		stk8312_iic_fun.stopSCCB();
		return(0);
	}

  	regDat = stk8312_iic_fun.SCCBreadByte();

  	stk8312_iic_fun.stopSCCB();

  	return (regDat);
}



#else
void stk8312_sensor_command(u8 register_address, u8 function_command)//��stk8312ĳ�Ĵ���д����
{
    gravity_sensor_command(WRITE_COMMAND_FOR_STK8312, register_address, function_command);
}

u8 stk8312_sensor_get_data(u8 register_address)//��ȡstk8312 sensor  ����
{
    return gravity_sensor_get_data(WRITE_COMMAND_FOR_STK8312, READ_COMMAND_FOR_STK8312, register_address);
}
#endif
void stk8312_set_enable(int en_able)
{
    u8 RegAddr, RegWriteValue;
    u8 readvalue = 0;
    u32 count = 10;   //time out setting

    if(en_able)
    {
        //---------------
        //Set to Active mode
        //---------------
        //0x07: Mode Register (Read/Write)
        //D7        	D6      	D5      		D4      		D3      		D2          	D1      		D0
        //IAH           IPP         Reserved        Reserved        Reserved        Reserved    	Reserved      	Mode
        //
        RegAddr       = 0x07;   //set Register Address
        RegWriteValue = 0xC1;   //INT# active-high, push-pull, Active-Mode
//        RegWriteValue = 0x81;	//Interrupt active high, open-drain, active-mode
        stk8312_sensor_command(RegAddr, RegWriteValue);
		os_time_dly(5);

		stk8312_sensor_command(0x24, 0xdc);
//#if 0
//        os_time_dly(5);
//        //---------------
//        //Engineering Setting
//        //---------------
//
//        stk8312_sensor_command(0x3D, 0x70);
//        stk8312_sensor_command(0x3D, 0x70);
//        stk8312_sensor_command(0x3F, 0x02);
//
//        os_time_dly(5);
//
//        while((readvalue>>7)!=1 && count!=0)
//        {
//            os_time_dly(5);
//            readvalue = stk8312_sensor_get_data(0x3F);// Read Engineering Register 0x3F
//            --count;
//        }
//
//        readvalue = stk8312_sensor_get_data(0x3E);
//
//        if(readvalue != 0x00)
//            stk8312_sensor_command(0x24, readvalue);
//
//        os_time_dly(100);	//Waiting for data ready
//#endif
    }
    else
    {
        //---------------------
        //Set to standby mode
        //---------------------
        //0x07��MODE/Features: 11000000
        //D7            D6          D5          	D4          	D3         		D2          	D1          	D0
        //IAH           IPP         Reserved        Reserved        Reserved        Reserved    	Reserved      	Mode
        //
        RegAddr       = 0x07;
        RegWriteValue = 0xC0;	//Interrupt active high, push-pull, standby-mode
        stk8312_sensor_command(RegAddr, RegWriteValue);
    }

}


void stk8312_init(void)//config RESOLUTION_RANGE ,MODE_BW,INT_MAP1, ACTIVE_DUR, ACTIVE_THS
{
    u8 chipid;
    u8 RegAddr, RegWriteValue;

    init_i2c_io();
    //---------------------
    //Software Reset
    //---------------------
    RegAddr       = 0x20;
    RegWriteValue = 0x00;
    stk8312_sensor_command(RegAddr, RegWriteValue);
    os_time_dly(20);

    //---------------------
    //Set to standby mode
    //---------------------
    //0x07��MODE/Features: 11000000
    //D7            D6          D5          	D4          	D3         		D2          	D1          	D0
    //IAH           IPP         Reserved        Reserved        Reserved        Reserved    	Reserved      	Mode
    //
    RegAddr       = 0x07;
    RegWriteValue = 0xC0;	//Interrupt active high, push-pull, standby-mode
//    RegWriteValue = 0xC1;	//Interrupt active high, push-pull, active-mode
//    RegWriteValue = 0x81;	//Interrupt active high, open-drain, active-mode
    stk8312_sensor_command(RegAddr, RegWriteValue);

	//RegAddr       = 0x03;
  //  RegWriteValue = 0x80;
//	stk8312_sensor_command(RegAddr, RegWriteValue);
    //---------------
    //Output Data Rate Settings :
    //---------------
    //0x08: Auto-Wake and Active Mode Portrait/Landscape Samples per Seconds Register (Read/Write)
    //D7        D6      D5      D4      D3      D2      D1      D0
    //FILT[2]   FILT[1] FILT[0] AWSR[1] AWSR[0] AMSR[2] AMSR[1] AMSR[0]
    RegAddr       = 0x08;
    RegWriteValue = 0xe3;	//ODR=50Hz
//    RegWriteValue = 0x03;	//ODR=50Hz
//    RegWriteValue = 0xe1;	//ODR=200Hz
//    RegWriteValue = 0x02;	//ODR=100Hz
//    RegWriteValue = 0xe4;	//ODR=25Hz
//    RegWriteValue = 0xe5;	//ODR=12.5Hz
    stk8312_sensor_command(RegAddr, RegWriteValue);

    //---------------
    //Set Mode as interrupt and measurement operation mode
    //---------------

    //0x06: Interrupt Setup Register (Read/Write)
    //D7        D6      D5      D4      D3      D2      D1      D0
    //SHINTX    SHINTY  SHINTZ  GINT    ASINT   TINT    PLINT   FBINT
    RegAddr       = 0x06;   //set Register Address
//    RegWriteValue = 0x10;   //New Data Interrupt enabled
    RegWriteValue = 0xe0;   //New Data Interrupt enabled add by lt
//    RegWriteValue = 0xF0;   //New Data Interrupt enabled add by lt
    stk8312_sensor_command(RegAddr, RegWriteValue);

///////////////////////////////////////////
#if 0
    RegAddr       = 0x09;//tap/pulse detection register
//    RegWriteValue = 0x00;
    RegWriteValue = 0xe0;//62.5mg   tap detection threshold
//    RegWriteValue = 0xe1;//125mg
//    RegWriteValue = 0xe2;//187.5mg
//    RegWriteValue = 0xe3;//250mg
//    RegWriteValue = 0xe4;//312.5mg
//    RegWriteValue = 0xe5;//375mg
//    RegWriteValue = 0xe6;//437.5mg
//    RegWriteValue = 0xe7;//500mg
    stk8312_sensor_command(RegAddr, RegWriteValue);



    RegAddr       = 0x0f;//tap latency regitser
//    RegWriteValue = 0x3c;//300ms
    RegWriteValue = 0x00;//
    stk8312_sensor_command(RegAddr, RegWriteValue);


    RegAddr       = 0x10;//tap window regitser
//    RegWriteValue = 0x3c;//300ms
    RegWriteValue = 0x00;////disable the double-tap
    stk8312_sensor_command(RegAddr, RegWriteValue);
#endif

 /*   RegAddr       = 0x11;//free fall threshold register
    RegWriteValue = 0x06;//
    stk8312_sensor_command(RegAddr, RegWriteValue);

    RegAddr       = 0x12;//free fall time register
    RegWriteValue = 0x28;//
    stk8312_sensor_command(RegAddr, RegWriteValue);
*/
#if 1
    //---------------
    //Set Measure Range, Resolution
    //---------------
    //0x13: Dynamic Range Setup and Shake Threshold Register (Read/Write)
    //D7            D6          D5          D4      D3          D2          D1          D0
    //RNG[1]        RNG[0]      -           -       -           STH[2]      STH[1]      STH[0]
    // *** RNG[1:0]     MEASUREMENT RANGE   Resolution ***
    //     00           ��1.5 g                6
    //     01           ��6   g                8
    //     10           ��16  g                8
    //     11           RESERVED
    RegAddr       = 0x13;
    RegWriteValue = 0x42;	//6g, 8bit      shake threshold 1.375g
//    RegWriteValue = 0x46;	//6g, 8bit     shake threshold 1.875g
//    RegWriteValue = 0x02;	//1.5g, 6bit    shake threshold 1.375g
//    RegWriteValue = 0x86;	//16g, 8bit     shake threshold 1.875g
    stk8312_sensor_command(RegAddr, RegWriteValue);
#endif

  //  RegAddr       = 0x14;
 //   RegWriteValue = 0x02;
//    RegWriteValue = 0x0b;
//    RegWriteValue = 0x0a;
//    RegWriteValue = 0x08;
    stk8312_sensor_command(RegAddr, RegWriteValue);

    stk8312_set_enable(1);

puts("\n stk8312 init success\n");

}



void Polling_xyz_data(void *parm)	//X = data_out[0], Y = data_out[1], Z = data_out[2]
{
    puts("\n XXXXXX : ");
//    if (stk8312_sensor_get_data(0x00) & BIT(6) == 0)
    {
        put_u16hex(stk8312_sensor_get_data(0x00));
    }
    puts("\n YYYYYY : ");
    put_u16hex(stk8312_sensor_get_data( 0X01));

    puts("\n ZZZZZZ : ");
    put_u16hex(stk8312_sensor_get_data( 0X02));

    puts("\n CCCCCC : ");
    put_u16hex(stk8312_sensor_get_data( 0X03));
}


//�����ֱ���ѡ��
void stk8312_resolution_range(u8 range)
{
    u8 RegAddr, RegWriteValue;
    switch(range)
    {
    case G_SENSITY_HIGH:
        RegAddr       = 0x13;
//        RegWriteValue = 0x00;	//1.5g, 6bit  shake threshold 1.125g
        RegWriteValue = 0x02;	//1.5g, 6bit  shake threshold 1.375g
        stk8312_sensor_command(RegAddr, RegWriteValue);


        break;
    case G_SENSITY_MEDIUM:
        RegAddr       = 0x13;
//        RegWriteValue = 0x40;	//6g, 8bit  shake threshold 1.125g
//        RegWriteValue = 0x42;	//6g, 8bit  shake threshold 1.375g
//        RegWriteValue = 0x44;	//6g, 8bit  shake threshold 1.625g
        RegWriteValue = 0x46;	//6g, 8bit  shake threshold 1.875g
//        RegWriteValue = 0x47;	//6g, 8bit  shake threshold 2g
        stk8312_sensor_command(RegAddr, RegWriteValue);

        break;
    case G_SENSITY_LOW:
        RegAddr       = 0x13;
//        RegWriteValue = 0x80;	//16g, 8bit  shake threshold 1.12g
//        RegWriteValue = 0x82;	//16g, 8bit  shake threshold 1.375g
//        RegWriteValue = 0x84;	//16g, 8bit  shake threshold 1.625g
//        RegWriteValue = 0x86;	//16g, 8bit  shake threshold 1.875g
        RegWriteValue = 0x87;	//16g, 8bit  shake threshold 2g
        stk8312_sensor_command(RegAddr, RegWriteValue);


        break;
    }
}


//����ģʽѡ��
void stk8312_work_mode(u8 work_mode)
{
    switch(work_mode)
    {
    case G_NORMAL_MODE:
        stk8312_set_enable(1);
        break;
    case G_LOW_POWER_MODE:
        stk8312_set_enable(1);
        break;
    case G_SUSPEND_MODE:
        stk8312_set_enable(0);
        break;
    }
}


/**�ṩ�������Ӳ˵��ص�����ʹ��*/
void stk8312_gravity_sensity(u8 gsid)
{

    stk8312_set_enable(0);

    if (gsid == G_SENSOR_HIGH)
    {
        stk8312_resolution_range(G_SENSITY_HIGH);
        stk8312_work_mode(G_NORMAL_MODE);//normal mode
    }

    if (gsid == G_SENSOR_MEDIUM)
    {
        stk8312_resolution_range(G_SENSITY_MEDIUM);
        stk8312_work_mode(G_NORMAL_MODE);//normal mode
    }

    if (gsid == G_SENSOR_LOW)
    {
        stk8312_resolution_range(G_SENSITY_LOW);
        stk8312_work_mode(G_NORMAL_MODE);//normal mode
    }

//    if ((gsid != G_SENSOR_LOW_POWER_MODE)
//        && (gsid != G_SENSOR_CLOSE))
    {
        stk8312_set_enable(1) ;
    }

    if (gsid == G_SENSOR_LOW_POWER_MODE)//�͹���
    {
//        stk8312_resolution_range(get_gravity_sensor());
        stk8312_resolution_range(G_SENSITY_MEDIUM);
        stk8312_work_mode(G_LOW_POWER_MODE);
    }


    if (gsid == G_SENSOR_CLOSE)
    {
        stk8312_work_mode(G_SUSPEND_MODE);//��ͣ
    }



}

/**
����ֵ TRUE  ��������������
       FALSE  ����������δ����
*/
u8 get_stk8312_int_state()
{
    u8 date_tmp;

    if (get_menu_statu() == 1)
    {
        return FALSE;
    }

    date_tmp = stk8312_sensor_get_data(0x07);
    if (date_tmp == 0xC0)
    {//standby mode �������
        return FALSE;
    }

    date_tmp = stk8312_sensor_get_data(0x03);
    if ((date_tmp & BIT(6)) == 0)
    {
        if (((date_tmp & BIT(7)) >> 7)//shake
//            || ((date_tmp & BIT(5)) >> 5)//tap
            )
        {
//            puts("\n jjjj 0\n");
//            puts("\n get_stk8312_int_state : ");
//            put_u8hex(date_tmp);
            return TRUE;
        }

    }

    return FALSE;
}

/**���ٶȳ������趨�ķ�ֵ�������жϡ�Ϊ��ǰ��Ƶ�ļ�����*/
void stk8312_pin_int_interrupt(void *pr)
{
    if (get_stk8312_int_state())
    {

//        puts("\n ssssssssssssssssssssssssssssss\n");

        if( (get_gravity_sensor() != 0)
                &&  (enc_ctrl.enc0_status == ENC_APP_RUNNING)
                &&  (mode_ctrl.cur_mode & ENC_MODE)
                &&  (enc_ctrl.lock_current_file  == 0)
          )
        {
            //printf("\n stk8312 lock the file#######\n");
            puts("da\n");
            enc_ctrl.lock_current_file = 1;//����ǰ�ļ�
//            clear_wakeu_pend();
            gsensor_pr2Pnd_api(FALSE);
            delay(100);
            stk8312_sensor_get_data(0x03);//clear int
        }
//        clear_wakeu_pend();
        gsensor_pr2Pnd_api(FALSE);
//		delay(100);
		stk8312_sensor_get_data(0x03);//clear int

    }
}

/**����0 id��ȷ*/
s8 stk8312_id_check(void)
{
    u8 chipid = 0x00;
    puts("\n stk8312 : ");
    chipid = stk8312_sensor_get_data(0x12);
    put_u16hex(chipid);

    if (chipid != 0x28)
    {
        puts("\n not stk8312 \n");
        return -1;
    }

    return 0;
}


/**����0 id��ȷ*/
s8 stk8312_check(void)
{
    u8 RegAddr, RegWriteValue;
    init_i2c_io();
    stk8312_iic_set(1);
    //---------------------
    //Software Reset
    //---------------------
    RegAddr       = 0x20;
    RegWriteValue = 0x00;
    stk8312_sensor_command(RegAddr, RegWriteValue);
    os_time_dly(20);

    if (0 != stk8312_id_check())
    {
        return -1;
    }

    return 0;
}








