/*******************************************************************************************
 File Name: ov9710.c

 Version: 1.00

 Discription: ov9710 摄像头初始化。

 Author:yulin deng

 Email :flowingfeeze@163.com

 Date:星期五, 四月 19 2013

 Copyright:(c)JIELI  2011  @ , All Rights Reserved.
*******************************************************************************************/
#ifndef OV9710_c
#define  OV9710_c
#include "iic.h"
#include "isp_dev.h"
#include "isp_com.h"


S_IIC_FUN ov9710_iic_fun;

typedef struct
{
    u8 addr;
    u8 value;
}Sensor_reg_ini;
const Sensor_reg_ini OV971x_INI_REG[]=
{
//    u32 i;

#if 1
    {0x12, 0x80},//reset
    {0x1e, 0x07},
    {0x5f, 0x18},
    {0x69, 0x04},
    {0x65, 0x2a},
    {0x68, 0x0a},
    {0x39, 0x28},
    {0x4d, 0x90},
    {0xc1, 0x80},

   // {0x0c, 0x30},
   // {0x6d, 0x02},

    //DSP
    {0x96, 0xf1},
    {0xbc, 0x68},
    //resolution and format
    {0x12, 0x00}, //with pixel overlay
    {0x3b, 0x00},
    {0x97, 0x80},//0x88,0x89,0x8A, three type color bar (without pixel overlay)
    {0x17, 0x25},// dsp input size
    {0x18, 0xa2},// dsp input size
    {0x19, 0x01},// dsp input size
    {0x1a, 0xca},// dsp input size
    {0x03, 0x0a},// 0x0a
    {0x32, 0x07},// dsp input size
    {0x98, 0x00},
    {0x99, 0x00},
    {0x9a, 0x00},
    {0x57, 0x00},// dsp output size (Width{2:0} << 2)|(Height{1:0})
    {0x58, 0xc8},// dsp output size Height{9:2}, 0xc8:800,0xb4:720
    {0x59, 0xa0},// dsp output size Width{10:3}
    {0x4c, 0x13},
    {0x4b, 0x36},
    {0x3d, 0x3c},
    {0x3e, 0x03},
    {0xbd, 0xa0},
    {0xbe, 0xc8},
    //YAVG
    {0x4e, 0x55},
    {0x4f, 0x55},
    {0x50, 0x55}, //for CarDV
    {0x51, 0x55},
    {0x24, 0x55}, //AE high Threshold
    {0x25, 0x40}, //AE Low Threshold
    {0x26, 0xa1}, // {7:4}x16,Higher Th, {3:0}x16, Lower Th, adjust with 2xStep
    //clock
    {0x5c, 0x59},
    {0x5d, 0x00},
    {0x11, 0x00},
    //{0x2a, 0x9b},
    //{0x2b, 0x06},
    {0x2a, 0xdf},    //0x630,0x7df
    {0x2b, 0x07},
    {0x2d, 0x00},
    {0x2e, 0x00},
     //banding step
  //  {0x49, 0xd8}, // 10ms * 30fps * 720p = 216 = 0xd8
   // {0x4A, 0x00},
    //general
    {0x13, 0xad}, //0xad,0x85?
    {0x14, 0x48},//0x48,0x40? gain ceiling 8X

    {0x0e, 0x48}

#else
    {0x09, 0x10},           //sleep mode
    //Core settings
    {0x1e, 0x07},
    {0x5f, 0x18},
    {0x69, 0x04},
    {0x65, 0x2a},
    {0x68, 0x0a},
    {0x39, 0x28},
    {0x4d, 0x90},
    {0xc1, 0x80},

    {0x0c, 0x30},
    {0x6d, 0x02},

    {0x96, 0xfd},
    {0xbc, 0x68},

    	//Resolution and Format
    {0x12, 0x00},//color bar {0x12, 0x02},
    {0x3b, 0x00},
    {0x97, 0x80},//color bar {0x97, 0x88},
    {0x17, 0x25},//{0x17, 0x25},
    {0x18, 0xa2},
    {0x19, 0x01},
    {0x1a, 0xca},
    {0x03, 0x0a},
    {0x32, 0x07},
    {0x98, 0x00},//(0x98, 0x40);//;	/*{0x98, 0x00},*/
    {0x99, 0x00},//(0x99, 0xA0);//;	/*{0x99, 0x00},*/
    {0x9a, 0x00},//(0x9a, 0x01);//;	/*{0x9a, 0x00},*/
    {0x57, 0x00},
    {0x58, 0xc8},//(0x58, 0x78);//	/*{0x58, 0xc8},*/
    {0x59, 0xa0},//(0x59, 0x50);//	/*{0x59, 0xa0},*/
    {0x4c, 0x13},
    {0x4b, 0x36},
    {0x3d, 0x3c},
    {0x3e, 0x03},
    {0xbd, 0xa0},//(0xbd, 0x50);//	/*{0xbd, 0xa0},*/
    {0xbe, 0xc8},//(0xbe, 0x78);//	/*{0xbe, 0xc8},*/


    {0x4e, 0x55},           //AVERAGE
    {0x4f, 0x55},
    {0x50, 0x55},
    {0x51, 0x55},
    {0x24, 0x55},          //Exposure windows
    {0x25, 0x40},
    {0x26, 0xa1},

    //Clock
    {0x5c, 0x59},
    {0x5d, 0x20},
    {0x11, 0x00},
    {0x2a, 0xdf},    //0x630,0x7df
    {0x2b, 0x07},
    {0x2d, 0x00},
    {0x2e, 0x00},

    //General & AEC
    {0x13, 0x6d},
    {0x14, 0x18},        //Gain Ceiling 8X
    {0x16, 0x10},
    {0x10, 0x80},
    {0x16, 0x01},
    {0x10, 0x9a},

    {0x4a, 0x00},
    {0x49, 0xce},
    {0x22, 0x03},        //Maxium smooth banding steps
    {0x09, 0x00},

    {0x1a, 0xc8},
    {0x03, 0x0a},
    {0x18, 0xa1},
    //clip mirror
    {0x04, 0xc0},

//    //awb
//    {0x38,0x00},
//    {0xb6,0x03},
//    {0x00,0x80},
//    {0x01,0x40},
//    {0x02,0x40},
    //test
    //{0xca,0x03}

#endif

};

static u8 ov9710_iic_sel;

static u8 ov9710_i2c_read(u8 iic_address,u8 *data)
{
    iic_tx(0x60);
    delay(10);
    iic_tx_we(iic_address);
    delay(10);

    iic_tx(0x61);
    delay(10);
    *data = iic_rx_we();
    return *data;
}

static void ov9710_i2c_write(u8 SubAddr, u8 data)
{
	iic_tx(0x60);
	delay(10);
	iic_tx(SubAddr);
	delay(10);
	iic_tx_we(data);
	delay(10);
}



unsigned char wrOV9710Reg(unsigned char regID, unsigned char regDat)
{
    if(ov9710_iic_sel)
    {
        ov9710_i2c_write(regID, regDat);
        return 1;
    }


	ov9710_iic_fun.startSCCB();
	if(0 == ov9710_iic_fun.SCCBwriteByte(0x60))
	{
		puts("a");
		ov9710_iic_fun.stopSCCB();
		return(0);
	}
	delay(1000);
  	if(0 == ov9710_iic_fun.SCCBwriteByte(regID))
	{
		puts("b");
		ov9710_iic_fun.stopSCCB();
		return(0);
	}
	delay(1000);
  	if(0 == ov9710_iic_fun.SCCBwriteByte(regDat))
	{
		puts("c");
		ov9710_iic_fun.stopSCCB();
		return(0);
	}
  	ov9710_iic_fun.stopSCCB();

  	return(1);
}

unsigned char rdOV9710Reg(unsigned char regID, unsigned char *regDat)
{

    if(ov9710_iic_sel)
    {
        ov9710_i2c_read(regID,regDat);
        return 1;
    }



	//通过写操作设置寄存器地址
	ov9710_iic_fun.startSCCB();
	if(0 == ov9710_iic_fun.SCCBwriteByte(0x60))
	{
		puts("1");
		ov9710_iic_fun.stopSCCB();
		return(0);
	}
	delay(1000);
  	if(0 == ov9710_iic_fun.SCCBwriteByte(regID))
	{
		puts("2");
		ov9710_iic_fun.stopSCCB();
		return(0);
	}
	ov9710_iic_fun.stopSCCB();
	delay(1000);

	//设置寄存器地址后，才是读
	ov9710_iic_fun.startSCCB();
	if(0 == ov9710_iic_fun.SCCBwriteByte(0x61))
	{
		puts("3");
		ov9710_iic_fun.stopSCCB();
		return(0);
	}
	delay(1000);
  	*regDat = ov9710_iic_fun.SCCBreadByte();
  	ov9710_iic_fun.noAck();
  	ov9710_iic_fun.stopSCCB();

//  	puts("finish");
  	return(1);
}



/*************************************************************************************************
    sensor api
*************************************************************************************************/
void OV9710_config_SENSOR(u16 *width, u16 *height, u8 *format, u8 *frame_freq)
{
    u32 i;


    OV9710_set_output_size(width, height, frame_freq);

    for(i=0;i<sizeof(OV971x_INI_REG)/sizeof(Sensor_reg_ini);i++)
    {
        wrOV9710Reg(OV971x_INI_REG[i].addr,OV971x_INI_REG[i].value);
    }
    if(*frame_freq == 15)
    {

    }


    if(*frame_freq == 30)
    {

    }


  	*format = SEN_IN_FORMAT_BGBG;

//    wrOV9712Reg(0x8d, 0x12); //test mode
    return;
}




s32 OV9710_set_output_size(u16 *width, u16 *height, u8 *freq)
{
	*width = 1280;
	*height = 720;

	*freq = 30;

	return 0;
}



s32 OV9710_power_ctl(u8 isp_dev, u8 is_work)
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


void OV9710_xclk_set(u8 isp_dev)
{
//	IOMC1 |= 0X0A<<16;//output PWM 24MHz
    ISP_XCLK_MAPOUT(); //output 24MHz

	if (isp_dev==ISP_DEV_0)
	{
		ISP0_XCLK_IO_ABLE();
	}
	else
	{
		ISP1_XCLK_IO_ABLE();
	}
}


static s32 OV9710_ID_check2(void)
{
    u8 pid = 0x00;
    u8 ver = 0x00;
    u8 i ;

    for(i = 0;i < 3;i++)//这里读取多次ID值，防止摄像头上电后不稳定
    {
        rdOV9710Reg(0x0A,&pid);
        rdOV9710Reg(0x0B,&ver);
    }

    puts("1:Sensor PID \n");
    put_u8hex(pid);
    put_u8hex(ver);
    puts("\n");

	if (pid != 0x97 || ver != 0x11)
	{
	    rdOV9710Reg(0x1c, &pid);
        rdOV9710Reg(0x1d, &ver);
        puts("2:Sensor PID \n");
        put_u8hex(pid);
        put_u8hex(ver);
        puts("\n");

        if (pid != 0x7f || ver != 0xa2){
            puts("\n 3 ----not OV9712-----\n");
            return -1;
        }
	}
    puts("\n 1 ----hello OV9712-----\n");
    return 0;
}
s32 OV9710_ID_check(void)
{
    u8 pid = 0x00;
    u8 ver = 0x00;
    u8 i ;

    for(i = 0;i < 3;i++)//这里读取多次ID值，防止摄像头上电后不稳定
    {
        rdOV9710Reg(0x0a, &pid);
        rdOV9710Reg(0x0b, &ver);

//        i2c_read(0x0a,&pid);
//        i2c_read(0x0b,&ver);
    }

        rdOV9710Reg(0x1c, &pid);
        rdOV9710Reg(0x1d, &ver);
    puts("Sensor PID \n");
    put_u8hex(pid);
    put_u8hex(ver);
    puts("\n");

	if (pid != 0x7f || ver != 0xa2)
	{
	    puts("---!ov971X---\n");
		return -1;
	}

    return 0;
}

void OV9710_reset(u8 isp_dev)
{
    puts("reset\n");

    if (isp_dev==ISP_DEV_0)
    {
        SET_RESET_OUT_A();
        SET_RESET_H_A();
        delay(40000);
        SET_RESET_L_A();
        delay(40000);
        SET_RESET_H_A();
        delay(40000);
    }
    else
    {
        SET_RESET_OUT_B();
        SET_RESET_H_B();
        delay(40000);
        SET_RESET_L_B();
        delay(40000);
        SET_RESET_H_B();
        delay(40000);
    }
}

void OV9710_iic_set(u8 isp_dev)
{
	iic_select(&ov9710_iic_fun, isp_dev);
}

s32 OV9710_check(u8 isp_dev)
{
    if(isp_dev == ISP_DEV_0){
        ov9710_iic_sel = 1;
    }else{
        ov9710_iic_sel = 0;
    }

    if(ov9710_iic_sel)
    {
        iic_ini();
    }

	OV9710_xclk_set(isp_dev);
	OV9710_iic_set(isp_dev);
	OV9710_reset(isp_dev);

	if (0 != OV9710_ID_check2())
	{
		return -1;
	}

	return 0;
}


s32 OV9710_init(u8 isp_dev, u16 *width, u16 *height, u8 *format, u8 *frame_freq)
{
	puts("\n\n OV971x_init \n\n");

	if (0 != OV9710_check(isp_dev))
	{
		return -1;
	}

	OV9710_config_SENSOR(width, height, format, frame_freq);

	return 0;
}

void OV9710_sleep()
{

    wrOV9710Reg(0x10,0x10);
    printf("9710 into sleep\n");
}

void OV9710_wakeup()
{

    wrOV9710Reg(0x10,0x00);
    printf("9710 into wakeup\n");
}

void OV9710_W_Reg(u16 addr, u16 val)
{
    wrOV9710Reg((u8)addr,(u8)val);
}
u16 OV9710_R_Reg(u16 addr)
{
    u8 val;
    rdOV9710Reg((u8)addr,&val);
    return val;
}
#endif

