
/*******************************************************************************************
 File Name: OV9712.c

 Version: 1.00

 Discription: OV9712 摄像头初始化。

 Author:xuhuajun

 Email :

 Date:2014.8.19

 Copyright:(c)JIELI  2011  @ , All Rights Reserved.
*******************************************************************************************/
#ifndef OV9712_c
#define  OV9712_c
#include "iic.h"
#include "isp_dev.h"
#include "isp_com.h"

typedef struct
{
    u8 addr;
    u8 value;
}Sensor_reg_ini;

S_IIC_FUN OV9712_iic_fun;

const Sensor_reg_ini OV9712_INI_REG[]=
{
		{0xfe, 0x80},
		{0xfe, 0x80},
		{0xfe, 0x80},
		{0xf2, 0x0f},//sync_pad_io_ebi
		{0xf6, 0x00},//up down
		{0xfc, 0xc6},
		{0xf7, 0xb9},//pll enable
		{0xf8, 0x03},//Pll mode 2
		{0xf9, 0x2e},//[0] pll enable
		{0xfa, 0x00},//div
		{0xfe, 0x00},
	////////////////   ANALOG & CISCTL	 ////////////////
		{0x03, 0x02},
		{0x04, 0xb5},
 		{0x05, 0x01},
 		{0x06, 0x77},
 		{0x07, 0x00},
 		{0x08, 0x1a},
		{0x0d, 0x02},
		{0x0e, 0xe8},
		{0x0f, 0x05},
		{0x10, 0x10},
		{0x11, 0x00},
		{0x12, 0x0c},
		{0x17, 0x14},
		{0x18, 0x0a},
		{0x19, 0x06},
		{0x1a, 0x09},
		{0x1b, 0x4f},
		{0x1c, 0x21},
		{0x1d, 0xe0},//f0
		{0x1e, 0x7c},
		{0x1f, 0x08},//08//comv_r
		{0x20, 0xa5},//左边竖线
		{0x21, 0x6f},//2f//20//reg
		{0x22, 0xb0},
		{0x23, 0x32},//38
		{0x24, 0x2f},//PAD drive
		{0x25, 0xc0},
		{0x2a, 0x00},
		{0x2c, 0xb0},
		{0x2d, 0x0f},
		{0x2e, 0xf0},
		{0x2f, 0x1f},//exp not work
	//////////////////////	 ISP   //////////////////////
		{0xfe, 0x00},
		{0x8a, 0x00},
		{0x8c, 0x02},
		{0x8e, 0x02},//luma value
		{0x90, 0x01},
		{0x94, 0x02},
		{0x95, 0x02},
		{0x96, 0xd0},
		{0x97, 0x05},
		{0x98, 0x00},

	//////////////////////	 MIPI	/////////////////////

		{0xfe, 0x03},
		{0x01, 0x00},
		{0x02, 0x00},
		{0x03, 0x00},
		{0x06, 0x00},
		{0x10, 0x00},
		{0x15, 0x00},
	//////////////////////	 BLK	/////////////////////
		{0xfe, 0x03},
		{0x18, 0x02},
		{0x1a, 0x01},
		{0x40, 0x23},
		{0x5e, 0x00},
		{0x66, 0x1f},
	//////////////////////	 Dark SUN	/////////////////////
		{0xfe, 0x00},//dark sun
		{0xfe, 0x02},
		{0x49, 0x23},
		{0x04, 0x00},
		{0xfe, 0x00},
	//////////////////////	 Gain	/////////////////////
		{0xb0, 0x50},
		{0xb3, 0x40},
		{0xb4, 0x40},
		{0xb5, 0x40},
		{0xfe, 0x00}

};


unsigned char wrOV9712Reg(unsigned char regID, unsigned char regDat)
{
	OV9712_iic_fun.startSCCB();
	if(0 == OV9712_iic_fun.SCCBwriteByte(0x60))
	{
//		puts("a");
		OV9712_iic_fun.stopSCCB();
		return(0);
	}
	delay(100);
  	if(0 == OV9712_iic_fun.SCCBwriteByte(regID))
	{
//		puts("b");
		OV9712_iic_fun.stopSCCB();
		return(0);
	}
	delay(100);
  	if(0 == OV9712_iic_fun.SCCBwriteByte(regDat))
	{
//		puts("c");
		OV9712_iic_fun.stopSCCB();
		return(0);
	}
  	OV9712_iic_fun.stopSCCB();

  	return(1);
}

unsigned char rdOV9712Reg(unsigned char regID, unsigned char *regDat)
{
	//通过写操作设置寄存器地址
	OV9712_iic_fun.startSCCB();
	if(0 == OV9712_iic_fun.SCCBwriteByte(0x60))
	{
//		puts("1");
		OV9712_iic_fun.stopSCCB();
		return(0);
	}
	delay(100);
  	if(0 == OV9712_iic_fun.SCCBwriteByte(regID))
	{
//		puts("2");
		OV9712_iic_fun.stopSCCB();
		return(0);
	}
	OV9712_iic_fun.stopSCCB();
	delay(100);

	//设置寄存器地址后，才是读
	OV9712_iic_fun.startSCCB();
	if(0 == OV9712_iic_fun.SCCBwriteByte(0x61))
	{
//		puts("3");
		OV9712_iic_fun.stopSCCB();
		return(0);
	}
	delay(100);
  	*regDat = OV9712_iic_fun.SCCBreadByte();
  	OV9712_iic_fun.noAck();
  	OV9712_iic_fun.stopSCCB();

  	return(1);
}



/*************************************************************************************************
    sensor api
*************************************************************************************************/

void OV9712_config_SENSOR(u16 *width, u16 *height, u8 *format, u8 *frame_freq)
{
    u32 i;


    OV9712_set_output_size(width, height, frame_freq);

    for(i=0;i<sizeof(OV9712_INI_REG)/sizeof(Sensor_reg_ini);i++)
    {
        wrOV9712Reg(OV9712_INI_REG[i].addr,OV9712_INI_REG[i].value);
    }
    if(*frame_freq == 15)
    {

    }


    if(*frame_freq == 30)
    {

    }


  	*format = SEN_IN_FORMAT_RGRG;

//    wrOV9712Reg(0x8d, 0x12); //test mode
    return;
}


s32 OV9712_set_output_size(u16 *width, u16 *height, u8 *frame_freq)
{
	*width = 1280;//640;
	*height = 720;//480;

	*frame_freq = 30;//30;

	return 0;
}


s32 OV9712_power_ctl(u8 isp_dev, u8 is_work)
{
	if (is_work)
	{
		if (isp_dev==ISP_DEV_0)
		{
			ISP0_XCLK_IO_ABLE();
		}
		else
		{
			ISP1_XCLK_IO_ABLE();
		}
	}
	else
	{
		if (isp_dev==ISP_DEV_0)
		{
			ISP0_XCLK_IO_DISABLE();
		}
		else
		{
			ISP1_XCLK_IO_DISABLE();
		}
	}

	return 0;
}


void OV9712_xclk_set(u8 isp_dev)
{
	IOMC1 |= 0X0A<<16;//output PWM 24MHz

	if (isp_dev==ISP_DEV_0)
	{
		ISP0_XCLK_IO_ABLE();
	}
	else
	{
		ISP1_XCLK_IO_ABLE();
	}
}



s32 OV9712_ID_check(void)
{
    u8 pid = 0x00;
    u8 ver = 0x00;
    u8 i ;

    for(i = 0;i < 3;i++)//这里读取多次ID值，防止摄像头上电后不稳定
    {
        rdOV9712Reg(0x0a,&pid);
        rdOV9712Reg(0x0b,&ver);
    }

    puts("Sensor PID \n");
    put_u8hex(pid);
    put_u8hex(ver);
    puts("\n");

	if (pid != 0x97 || ver != 0x11)
	{
	    puts("\n----not OV9712-----\n");
		return -1;
	}
    puts("\n----hello OV9712-----\n");
    return 0;
}

void OV9712_reset(u8 isp_dev)
{
    printf("reset:%d\n",isp_dev);

    if (isp_dev==ISP_DEV_0)
    {

        SET_RESET_OUT_A();
        SET_RESET_L_A();
        delay(40000);
        SET_RESET_H_A();
        delay(40000);
    }
    else
    {
        SET_RESET_OUT_B();
        SET_RESET_L_B();
        delay(40000);
        SET_RESET_H_B();
        delay(40000);
    }
}

void OV9712_iic_set(u8 isp_dev)
{
	iic_select(&OV9712_iic_fun, isp_dev);
}

s32 OV9712_check(u8 isp_dev)
{
	OV9712_xclk_set(isp_dev);
	OV9712_iic_set(isp_dev);
	OV9712_reset(isp_dev);

	if (0 != OV9712_ID_check())
	{
		return -1;
	}

	return 0;
}


s32 OV9712_init(u8 isp_dev, u16 *width, u16 *height, u8 *format, u8 *frame_freq)
{
	puts("\n\n OV9712_init \n\n");

	if (0 != OV9712_check(isp_dev))
	{
		return -1;
	}

	OV9712_config_SENSOR(width, height, format, frame_freq);

	OV9712_set_output_size(width, height, frame_freq);

	return 0;
}



#endif


