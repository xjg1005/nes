
/*******************************************************************************************
 File Name: ov9650.c

 Version: 1.00

 Discription: ov9650 摄像头初始化。

 Author:yulin deng

 Email :flowingfeeze@163.com

 Date:星期五, 四月 19 2013

 Copyright:(c)JIELI  2011  @ , All Rights Reserved.
*******************************************************************************************/
#ifndef ov9650_c
#define  ov9650_c
#include "iic.h"
#include "isp_dev.h"
#include "isp_com.h"


S_IIC_FUN ov9650_iic_fun;



static u8 ov9650_iic_sel;

static u8 ov9650_i2c_read(u8 iic_address,u8 *data)
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

static void ov9650_i2c_write(u8 SubAddr, u8 data)
{
	iic_tx(0x60);
	delay(10);
	iic_tx(SubAddr);
	delay(10);
	iic_tx_we(data);
	delay(10);
}


unsigned char wrOV9650Reg(unsigned char regID, unsigned char regDat)
{

    if(ov9650_iic_sel)
    {
        ov9650_i2c_write(regID, regDat);
        return 1;
    }

	ov9650_iic_fun.startSCCB();
	if(0 == ov9650_iic_fun.SCCBwriteByte(0x60))
	{
//		puts("a");
		ov9650_iic_fun.stopSCCB();
		return(0);
	}
	delay(100);
  	if(0 == ov9650_iic_fun.SCCBwriteByte(regID))
	{
//		puts("b");
		ov9650_iic_fun.stopSCCB();
		return(0);
	}
	delay(100);
  	if(0 == ov9650_iic_fun.SCCBwriteByte(regDat))
	{
//		puts("c");
		ov9650_iic_fun.stopSCCB();
		return(0);
	}
  	ov9650_iic_fun.stopSCCB();

  	return(1);
}

unsigned char rdOV9650Reg(unsigned char regID, unsigned char *regDat)
{
    if(ov9650_iic_sel)
    {
        ov9650_i2c_read(regID,regDat);
        return 1;
    }

	//通过写操作设置寄存器地址
	ov9650_iic_fun.startSCCB();
	if(0 == ov9650_iic_fun.SCCBwriteByte(0x60))
	{
//		puts("1");
		ov9650_iic_fun.stopSCCB();
		return(0);
	}
	delay(100);
  	if(0 == ov9650_iic_fun.SCCBwriteByte(regID))
	{
//		puts("2");
		ov9650_iic_fun.stopSCCB();
		return(0);
	}
	ov9650_iic_fun.stopSCCB();
	delay(100);

	//设置寄存器地址后，才是读
	ov9650_iic_fun.startSCCB();
	if(0 == ov9650_iic_fun.SCCBwriteByte(0x61))
	{
//		puts("3");
		ov9650_iic_fun.stopSCCB();
		return(0);
	}
	delay(100);
  	*regDat = ov9650_iic_fun.SCCBreadByte();
  	ov9650_iic_fun.noAck();
  	ov9650_iic_fun.stopSCCB();

  	return(1);
}



/*************************************************************************************************
    sensor api
*************************************************************************************************/

void OV9650_config_SENSOR(u16 *width, u16 *height, u8 *format, u8 *frame_freq)
{
    u8 i = 0;

    wrOV9650Reg(0x12, 0x80);  // reset sensor
    i = 0xff;
    while(i--)//这里需要延时较长的时间，让摄像头内部寄存器复位
    {
        delay(10000);
    }

    OV9650_set_output_size(width, height, frame_freq);

	if(*frame_freq == 15)
	{
        /////********** VGA 15fps CFG **********/
        wrOV9650Reg(0x11, 0x81);//0x81
        wrOV9650Reg(0x6b, 0x0a);
        wrOV9650Reg(0x6a, 0x3e);
        wrOV9650Reg(0x3b, 0x09);
        wrOV9650Reg(0x13, 0xe0);
        wrOV9650Reg(0x01, 0x80);
        wrOV9650Reg(0x02, 0x80);
        wrOV9650Reg(0x00, 0x00);
        wrOV9650Reg(0x10, 0x00);
        wrOV9650Reg(0x13, 0xe5);

        wrOV9650Reg(0x39, 0x43);//0x43,0x50 for 30fps
        wrOV9650Reg(0x38, 0x12);//0x12,0x92 for 30fps
        wrOV9650Reg(0x37, 0x00);
        wrOV9650Reg(0x35, 0x91);//0x91,0x81 for 30fps
        wrOV9650Reg(0x0e, 0x20);//ray fix:0x20
        wrOV9650Reg(0x1e, 0x04);

        wrOV9650Reg(0xa8, 0x80);
        wrOV9650Reg(0x12, 0x40);//Output Format:VGA
        wrOV9650Reg(0x04, 0x00);//wrOV9650Reg(0x04, 0x00 | BIT(6));656 mode
        wrOV9650Reg(0x0c, 0x04);//VarioPixel for VGA
        wrOV9650Reg(0x0d, 0x80);//ray fix:0x80

        //1589-309 = 1280
        wrOV9650Reg(0x18, 0xc6);//HSTOP
        wrOV9650Reg(0x17, 0x26);//HSTART
        wrOV9650Reg(0x32, 0xad);//HREF  //bit0-2 HSTART  //bit3-5 HSTOP

        //488-8 = 480
        wrOV9650Reg(0x03, 0x00);//VREF  //bit0-2 VSTART  //bit3-5 VSTOP
        wrOV9650Reg(0x1a, 0x3d);//VSTOP
        wrOV9650Reg(0x19, 0x01);//VSTART

        wrOV9650Reg(0x3f, 0xa6);
        wrOV9650Reg(0x14, 0x2e);
        wrOV9650Reg(0x15, 0x02);//0x02
        wrOV9650Reg(0x41, 0x02);
        wrOV9650Reg(0x42, 0x08);

        wrOV9650Reg(0x1b, 0x00);
        wrOV9650Reg(0x16, 0x06);
        wrOV9650Reg(0x33, 0xe2);//c0 for internal regulator
        wrOV9650Reg(0x34, 0xbf);
        wrOV9650Reg(0x96, 0x04);
        wrOV9650Reg(0x3a, 0x00);
        wrOV9650Reg(0x8e, 0x00);

        wrOV9650Reg(0x3c, 0x77);
        wrOV9650Reg(0x8b, 0x06);
        wrOV9650Reg(0x94, 0x88);
        wrOV9650Reg(0x95, 0x88);
        wrOV9650Reg(0x40, 0xc1);//wrOV9650Reg(0x40, 0x41);656 mode
        wrOV9650Reg(0x29, 0x3f);//2f for internal regulator
        wrOV9650Reg(0x0f, 0x42);

        wrOV9650Reg(0x3d, 0x92);
        wrOV9650Reg(0x69, 0x40);
        wrOV9650Reg(0x5c, 0xb9);
        wrOV9650Reg(0x5d, 0x96);
        wrOV9650Reg(0x5e, 0x10);
        wrOV9650Reg(0x59, 0xc0);
        wrOV9650Reg(0x5a, 0xaf);
        wrOV9650Reg(0x5b, 0x55);
        wrOV9650Reg(0x43, 0xf0);
        wrOV9650Reg(0x44, 0x10);
        wrOV9650Reg(0x45, 0x68);
        wrOV9650Reg(0x46, 0x96);
        wrOV9650Reg(0x47, 0x60);
        wrOV9650Reg(0x48, 0x80);
        wrOV9650Reg(0x5f, 0xe0);
        wrOV9650Reg(0x60, 0x8c);//0c for advanced AWB (related to lens)
        wrOV9650Reg(0x61, 0x20);
        wrOV9650Reg(0xa5, 0xd9);
        wrOV9650Reg(0xa4, 0x74);
        wrOV9650Reg(0x8d, 0x02);
        wrOV9650Reg(0x13, 0xe7);

        wrOV9650Reg(0x4f, 0x3a);
        wrOV9650Reg(0x50, 0x3d);
        wrOV9650Reg(0x51, 0x03);
        wrOV9650Reg(0x52, 0x12);
        wrOV9650Reg(0x53, 0x26);
        wrOV9650Reg(0x54, 0x38);
        wrOV9650Reg(0x55, 0x40);
        wrOV9650Reg(0x56, 0x40);
        wrOV9650Reg(0x57, 0x40);
        wrOV9650Reg(0x58, 0x0d);

        wrOV9650Reg(0x8c, 0x23);
        wrOV9650Reg(0x3e, 0x02);
        wrOV9650Reg(0xa9, 0xb8);
        wrOV9650Reg(0xaa, 0x92);
        wrOV9650Reg(0xab, 0x0a);

        wrOV9650Reg(0x8f, 0xdf);
        wrOV9650Reg(0x90, 0x00);
        wrOV9650Reg(0x91, 0x00);
        wrOV9650Reg(0x9f, 0x00);
        wrOV9650Reg(0xa0, 0x00);
        wrOV9650Reg(0x3a, 0x01);

        wrOV9650Reg(0x24, 0x70);
        wrOV9650Reg(0x25, 0x64);
        wrOV9650Reg(0x26, 0xc3);

        wrOV9650Reg(0x2a, 0x00);//10 for 50Hz//Dummy Pixel Insert MSB
        wrOV9650Reg(0x2b, 0x00);//40 for 50Hz//Dummy Pixel Insert LSB

    //    wrOV9650Reg(0x2a, 0x10);//10 for 50Hz//Dummy Pixel Insert MSB
    //    wrOV9650Reg(0x2b, 0x40);//40 for 50Hz//Dummy Pixel Insert LSB

        //gamma
        wrOV9650Reg(0x6c, 0x40);
        wrOV9650Reg(0x6d, 0x30);
        wrOV9650Reg(0x6e, 0x4b);
        wrOV9650Reg(0x6f, 0x60);
        wrOV9650Reg(0x70, 0x70);
        wrOV9650Reg(0x71, 0x70);
        wrOV9650Reg(0x72, 0x70);
        wrOV9650Reg(0x73, 0x70);
        wrOV9650Reg(0x74, 0x60);
        wrOV9650Reg(0x75, 0x60);
        wrOV9650Reg(0x76, 0x50);
        wrOV9650Reg(0x77, 0x48);
        wrOV9650Reg(0x78, 0x3a);
        wrOV9650Reg(0x79, 0x2e);
        wrOV9650Reg(0x7a, 0x28);
        wrOV9650Reg(0x7b, 0x22);
        wrOV9650Reg(0x7c, 0x04);
        wrOV9650Reg(0x7d, 0x07);
        wrOV9650Reg(0x7e, 0x10);
        wrOV9650Reg(0x7f, 0x28);
        wrOV9650Reg(0x80, 0x36);
        wrOV9650Reg(0x81, 0x44);
        wrOV9650Reg(0x82, 0x52);
        wrOV9650Reg(0x83, 0x60);
        wrOV9650Reg(0x84, 0x6c);
        wrOV9650Reg(0x85, 0x78);
        wrOV9650Reg(0x86, 0x8c);
        wrOV9650Reg(0x87, 0x9e);
        wrOV9650Reg(0x88, 0xbb);
        wrOV9650Reg(0x89, 0xd2);
        wrOV9650Reg(0x8a, 0xe6);
///////////////////////////////////////////////////////////
	}


    if(*frame_freq == 30)
	{
/////********** VGA 30fps CFG **********/
        wrOV9650Reg(0x12, 0x80);  // reset sensor
        i = 0xff;
        while(i--)//这里需要延时较长的时间，让摄像头内部寄存器复位
        {
            delay(10000);
        }

        wrOV9650Reg(0x11, 0x80);//0x81,0x80 for 30fps
        wrOV9650Reg(0x6b, 0x0a);
        wrOV9650Reg(0x6a, 0x7d);//0x3e,0x7d for 30fps
        wrOV9650Reg(0x3b, 0x09);
        wrOV9650Reg(0x13, 0xe0);
        wrOV9650Reg(0x01, 0x80);
        wrOV9650Reg(0x02, 0x80);
        wrOV9650Reg(0x00, 0x00);
        wrOV9650Reg(0x10, 0x00);
        wrOV9650Reg(0x13, 0xe5);

        wrOV9650Reg(0x39, 0x50);//0x43,0x50 for 30fps
        wrOV9650Reg(0x38, 0x92);//0x12,0x92 for 30fps
        wrOV9650Reg(0x37, 0x00);
        wrOV9650Reg(0x35, 0x81);//0x91,0x81 for 30fps
        wrOV9650Reg(0x0e, 0x20);//ray fix:0x20
        wrOV9650Reg(0x1e, 0x04);

        wrOV9650Reg(0xa8, 0x80);
        wrOV9650Reg(0x12, 0x40);//Output Format:VGA
        wrOV9650Reg(0x04, 0x00);//wrOV9650Reg(0x04, 0x00 | BIT(6));656 mode
        wrOV9650Reg(0x0c, 0x04);//VarioPixel for VGA
        wrOV9650Reg(0x0d, 0x80);//ray fix:0x80
        wrOV9650Reg(0x18, 0xc6);//HSTOP
        wrOV9650Reg(0x17, 0x26);//HSTART
        wrOV9650Reg(0x32, 0xad);//HREF
        wrOV9650Reg(0x03, 0x00);//VREF
        wrOV9650Reg(0x1a, 0x3d);//VSTOP
        wrOV9650Reg(0x19, 0x01);//VSTRT
        wrOV9650Reg(0x3f, 0xa6);
        wrOV9650Reg(0x14, 0x2e);
        wrOV9650Reg(0x15, 0x02);//0x02
        wrOV9650Reg(0x41, 0x02);
        wrOV9650Reg(0x42, 0x08);

        wrOV9650Reg(0x1b, 0x00);
        wrOV9650Reg(0x16, 0x06);
        wrOV9650Reg(0x33, 0xe2);//c0 for internal regulator
        wrOV9650Reg(0x34, 0xbf);
        wrOV9650Reg(0x96, 0x04);
        wrOV9650Reg(0x3a, 0x00);
        wrOV9650Reg(0x8e, 0x00);

        wrOV9650Reg(0x3c, 0x77);
        wrOV9650Reg(0x8b, 0x06);
        wrOV9650Reg(0x94, 0x88);
        wrOV9650Reg(0x95, 0x88);
        wrOV9650Reg(0x40, 0xc1);//wrOV9650Reg(0x40, 0x41);656 mode
        wrOV9650Reg(0x29, 0x3f);//2f for internal regulator
        wrOV9650Reg(0x0f, 0x42);

        wrOV9650Reg(0x3d, 0x92);
        wrOV9650Reg(0x69, 0x40);
        wrOV9650Reg(0x5c, 0xb9);
        wrOV9650Reg(0x5d, 0x96);
        wrOV9650Reg(0x5e, 0x10);
        wrOV9650Reg(0x59, 0xc0);
        wrOV9650Reg(0x5a, 0xaf);
        wrOV9650Reg(0x5b, 0x55);
        wrOV9650Reg(0x43, 0xf0);
        wrOV9650Reg(0x44, 0x10);
        wrOV9650Reg(0x45, 0x68);
        wrOV9650Reg(0x46, 0x96);
        wrOV9650Reg(0x47, 0x60);
        wrOV9650Reg(0x48, 0x80);
        wrOV9650Reg(0x5f, 0xe0);
        wrOV9650Reg(0x60, 0x8c);//0c for advanced AWB (related to lens)
        wrOV9650Reg(0x61, 0x20);
        wrOV9650Reg(0xa5, 0xd9);
        wrOV9650Reg(0xa4, 0x74);
        wrOV9650Reg(0x8d, 0x02);
        wrOV9650Reg(0x13, 0xe7);

        wrOV9650Reg(0x4f, 0x3a);
        wrOV9650Reg(0x50, 0x3d);
        wrOV9650Reg(0x51, 0x03);
        wrOV9650Reg(0x52, 0x12);
        wrOV9650Reg(0x53, 0x26);
        wrOV9650Reg(0x54, 0x38);
        wrOV9650Reg(0x55, 0x40);
        wrOV9650Reg(0x56, 0x40);
        wrOV9650Reg(0x57, 0x40);
        wrOV9650Reg(0x58, 0x0d);

        wrOV9650Reg(0x8c, 0x23);
        wrOV9650Reg(0x3e, 0x02);
        wrOV9650Reg(0xa9, 0xb8);
        wrOV9650Reg(0xaa, 0x92);
        wrOV9650Reg(0xab, 0x0a);

        wrOV9650Reg(0x8f, 0xdf);
        wrOV9650Reg(0x90, 0x00);
        wrOV9650Reg(0x91, 0x00);
        wrOV9650Reg(0x9f, 0x00);
        wrOV9650Reg(0xa0, 0x00);
        wrOV9650Reg(0x3a, 0x01);

        wrOV9650Reg(0x24, 0x70);
        wrOV9650Reg(0x25, 0x64);
        wrOV9650Reg(0x26, 0xc3);

        wrOV9650Reg(0x2a, 0x00);//10 for 50Hz//Dummy Pixel Insert MSB
        wrOV9650Reg(0x2b, 0x00);//40 for 50Hz//Dummy Pixel Insert LSB

    //    wrOV9650Reg(0x2a, 0x10);//10 for 50Hz//Dummy Pixel Insert MSB
    //    wrOV9650Reg(0x2b, 0x40);//40 for 50Hz//Dummy Pixel Insert LSB

        //gamma
        wrOV9650Reg(0x6c, 0x40);
        wrOV9650Reg(0x6d, 0x30);
        wrOV9650Reg(0x6e, 0x4b);
        wrOV9650Reg(0x6f, 0x60);
        wrOV9650Reg(0x70, 0x70);
        wrOV9650Reg(0x71, 0x70);
        wrOV9650Reg(0x72, 0x70);
        wrOV9650Reg(0x73, 0x70);
        wrOV9650Reg(0x74, 0x60);
        wrOV9650Reg(0x75, 0x60);
        wrOV9650Reg(0x76, 0x50);
        wrOV9650Reg(0x77, 0x48);
        wrOV9650Reg(0x78, 0x3a);
        wrOV9650Reg(0x79, 0x2e);
        wrOV9650Reg(0x7a, 0x28);
        wrOV9650Reg(0x7b, 0x22);
        wrOV9650Reg(0x7c, 0x04);
        wrOV9650Reg(0x7d, 0x07);
        wrOV9650Reg(0x7e, 0x10);
        wrOV9650Reg(0x7f, 0x28);
        wrOV9650Reg(0x80, 0x36);
        wrOV9650Reg(0x81, 0x44);
        wrOV9650Reg(0x82, 0x52);
        wrOV9650Reg(0x83, 0x60);
        wrOV9650Reg(0x84, 0x6c);
        wrOV9650Reg(0x85, 0x78);
        wrOV9650Reg(0x86, 0x8c);
        wrOV9650Reg(0x87, 0x9e);
        wrOV9650Reg(0x88, 0xbb);
        wrOV9650Reg(0x89, 0xd2);
        wrOV9650Reg(0x8a, 0xe6);
/////////////////////////////////////////////////////////////
	}

    if(*frame_freq == 7)
	{
///********** SXGA CFG **********/
        wrOV9650Reg(0x12, 0x80);  // reset sensor
        i = 0xff;
        while(i--)//这里需要延时较长的时间，让摄像头内部寄存器复位
        {
            delay(10000);
        }

        wrOV9650Reg(0x11, 0x80);//0x81
        wrOV9650Reg(0x6b, 0x0a);
        wrOV9650Reg(0x6a, 0x41);
        wrOV9650Reg(0x3b, 0x09);
        wrOV9650Reg(0x13, 0xe0);
        wrOV9650Reg(0x01, 0x80);
        wrOV9650Reg(0x02, 0x80);
        wrOV9650Reg(0x00, 0x00);
        wrOV9650Reg(0x10, 0x00);
        wrOV9650Reg(0x13, 0xe5);

        wrOV9650Reg(0x39, 0x50);//0x43,0x50 for 15fps
        wrOV9650Reg(0x38, 0x93);//0x12,0x93 for 15fps
        wrOV9650Reg(0x37, 0x81);//0x00,0x81 for 15fps
        wrOV9650Reg(0x35, 0x81);//0x91,0x81 for 15fps
        wrOV9650Reg(0x0e, 0x20);//ray fix:0x20
        wrOV9650Reg(0x1e, 0x04);

        wrOV9650Reg(0xa8, 0x80);
        wrOV9650Reg(0x12, 0x00);//Output Format:SXGA
        wrOV9650Reg(0x04, 0x00);//wrOV9650Reg(0x04, 0x00 | BIT(6));656 mode
        wrOV9650Reg(0x0c, 0x00);//VarioPixel for SXGA
        wrOV9650Reg(0x0d, 0x00);//ray fix:0x00
        wrOV9650Reg(0x18, 0xbd);//HSTOP
        wrOV9650Reg(0x17, 0x1d);//HSTART
        wrOV9650Reg(0x32, 0xad);//HREF
        wrOV9650Reg(0x03, 0x12);//VREF
        wrOV9650Reg(0x1a, 0x81);//VSTOP
        wrOV9650Reg(0x19, 0x01);//VSTRT
        wrOV9650Reg(0x14, 0x2e);
        wrOV9650Reg(0x15, 0x00);//0x02
        wrOV9650Reg(0x3f, 0xa6);
        wrOV9650Reg(0x41, 0x02);
        wrOV9650Reg(0x42, 0x08);

        wrOV9650Reg(0x1b, 0x00);
        wrOV9650Reg(0x16, 0x06);
        wrOV9650Reg(0x33, 0xe2);//c0 for internal regulator
        wrOV9650Reg(0x34, 0xbf);
        wrOV9650Reg(0x96, 0x04);
        wrOV9650Reg(0x3a, 0x00);
        wrOV9650Reg(0x8e, 0x00);

        wrOV9650Reg(0x3c, 0x77);
        wrOV9650Reg(0x8b, 0x06);
        wrOV9650Reg(0x94, 0x88);
        wrOV9650Reg(0x95, 0x88);
        wrOV9650Reg(0x40, 0xc1);//wrOV9650Reg(0x40, 0x41);656 mode
        wrOV9650Reg(0x29, 0x3f);//2f for internal regulator
        wrOV9650Reg(0x0f, 0x42);

        wrOV9650Reg(0x3d, 0x92);
        wrOV9650Reg(0x69, 0x40);
        wrOV9650Reg(0x5c, 0xb9);
        wrOV9650Reg(0x5d, 0x96);
        wrOV9650Reg(0x5e, 0x10);
        wrOV9650Reg(0x59, 0xc0);
        wrOV9650Reg(0x5a, 0xaf);
        wrOV9650Reg(0x5b, 0x55);
        wrOV9650Reg(0x43, 0xf0);
        wrOV9650Reg(0x44, 0x10);
        wrOV9650Reg(0x45, 0x68);
        wrOV9650Reg(0x46, 0x96);
        wrOV9650Reg(0x47, 0x60);
        wrOV9650Reg(0x48, 0x80);
        wrOV9650Reg(0x5f, 0xe0);
        wrOV9650Reg(0x60, 0x8c);//0c for advanced AWB (related to lens)
        wrOV9650Reg(0x61, 0x20);
        wrOV9650Reg(0xa5, 0xd9);
        wrOV9650Reg(0xa4, 0x74);
        wrOV9650Reg(0x8d, 0x02);
        wrOV9650Reg(0x13, 0xe7);

        wrOV9650Reg(0x4f, 0x3a);
        wrOV9650Reg(0x50, 0x3d);
        wrOV9650Reg(0x51, 0x03);
        wrOV9650Reg(0x52, 0x12);
        wrOV9650Reg(0x53, 0x26);
        wrOV9650Reg(0x54, 0x38);
        wrOV9650Reg(0x55, 0x40);
        wrOV9650Reg(0x56, 0x40);
        wrOV9650Reg(0x57, 0x40);
        wrOV9650Reg(0x58, 0x0d);

        wrOV9650Reg(0x8c, 0x23);
        wrOV9650Reg(0x3e, 0x02);
        wrOV9650Reg(0xa9, 0xb8);
        wrOV9650Reg(0xaa, 0x92);
        wrOV9650Reg(0xab, 0x0a);

        wrOV9650Reg(0x8f, 0xdf);
        wrOV9650Reg(0x90, 0x00);
        wrOV9650Reg(0x91, 0x00);
        wrOV9650Reg(0x9f, 0x00);
        wrOV9650Reg(0xa0, 0x00);
        wrOV9650Reg(0x3a, 0x01);

        wrOV9650Reg(0x24, 0x70);
        wrOV9650Reg(0x25, 0x64);
        wrOV9650Reg(0x26, 0xc3);

        wrOV9650Reg(0x2a, 0x00);//10 for 50Hz//Dummy Pixel Insert MSB
        wrOV9650Reg(0x2b, 0x00);//34 for 50Hz//Dummy Pixel Insert LSB

    //    wrOV9650Reg(0x2a, 0x10);//10 for 50Hz//Dummy Pixel Insert MSB
    //    wrOV9650Reg(0x2b, 0x34);//34 for 50Hz//Dummy Pixel Insert LSB

        //gamma
        wrOV9650Reg(0x6c, 0x40);
        wrOV9650Reg(0x6d, 0x30);
        wrOV9650Reg(0x6e, 0x4b);
        wrOV9650Reg(0x6f, 0x60);
        wrOV9650Reg(0x70, 0x70);
        wrOV9650Reg(0x71, 0x70);
        wrOV9650Reg(0x72, 0x70);
        wrOV9650Reg(0x73, 0x70);
        wrOV9650Reg(0x74, 0x60);
        wrOV9650Reg(0x75, 0x60);
        wrOV9650Reg(0x76, 0x50);
        wrOV9650Reg(0x77, 0x48);
        wrOV9650Reg(0x78, 0x3a);
        wrOV9650Reg(0x79, 0x2e);
        wrOV9650Reg(0x7a, 0x28);
        wrOV9650Reg(0x7b, 0x22);
        wrOV9650Reg(0x7c, 0x04);
        wrOV9650Reg(0x7d, 0x07);
        wrOV9650Reg(0x7e, 0x10);
        wrOV9650Reg(0x7f, 0x28);
        wrOV9650Reg(0x80, 0x36);
        wrOV9650Reg(0x81, 0x44);
        wrOV9650Reg(0x82, 0x52);
        wrOV9650Reg(0x83, 0x60);
        wrOV9650Reg(0x84, 0x6c);
        wrOV9650Reg(0x85, 0x78);
        wrOV9650Reg(0x86, 0x8c);
        wrOV9650Reg(0x87, 0x9e);
        wrOV9650Reg(0x88, 0xbb);
        wrOV9650Reg(0x89, 0xd2);
        wrOV9650Reg(0x8a, 0xe6);
///////////////////////////////////////////////////////////
	}


    wrOV9650Reg(0x3a, 0x0d);//UYVY
	*format = SEN_IN_FORMAT_UYVY;
//    wrOV9650Reg(0x8d, 0x12); //test mode



//    wrOV9650Reg(0x15, 0x10);
//    wrOV9650Reg(0x3a, 0x0d);//0x08

//    wrOV9650Reg(0x8d, 0x12); //test mode
    return;
}


s32 OV9650_set_output_size(u16 *width, u16 *height, u8 *frame_freq)
{
	*width = 640;//640;//1280;
	*height = 480;//480;//1024;

	*frame_freq = 30;//30;//7;

	return 0;
}


s32 OV9650_power_ctl(u8 isp_dev, u8 is_work)
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


void OV9650_xclk_set(u8 isp_dev)
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



s32 OV9650_ID_check(void)
{
    u8 pid = 0x00;
    u8 ver = 0x00;
    u8 i ;

    for(i = 0;i < 3;i++)//这里读取多次ID值，防止摄像头上电后不稳定
    {
        rdOV9650Reg(0x0a,&pid);
        rdOV9650Reg(0x0b,&ver);
    }

    puts("Sensor PID \n");
    put_u8hex(pid);
    put_u8hex(ver);
    puts("\n");

	if (pid != 0x96 || ver&0xf0 != 0x50)
	{
		return -1;
	}

    return 0;
}

void OV9650_reset(u8 isp_dev)
{
    puts("reset\n");

    if (isp_dev==ISP_DEV_0)
    {
        SET_RESET_OUT_A();
        SET_RESET_H_A();
        delay(40000);
        SET_RESET_L_A();
        delay(40000);
    }
    else
    {
        SET_RESET_OUT_B();
        SET_RESET_H_B();
        delay(40000);
        SET_RESET_L_B();
        delay(40000);
    }
}

void OV9650_iic_set(u8 isp_dev)
{
	iic_select(&ov9650_iic_fun, isp_dev);
}

s32 OV9650_check(u8 isp_dev)
{
    if(isp_dev == ISP_DEV_0){
        ov9650_iic_sel = 1;
    }else{
        ov9650_iic_sel = 0;
    }

    if(ov9650_iic_sel)
    {
        iic_ini();
    }


	OV9650_xclk_set(isp_dev);
	OV9650_iic_set(isp_dev);
	OV9650_reset(isp_dev);

	if (0 != OV9650_ID_check())
	{
		return -1;
	}

	return 0;
}


s32 OV9650_init(u8 isp_dev, u16 *width, u16 *height, u8 *format, u8 *frame_freq)
{
	puts("\n\n OV9650_init \n\n");

	if (0 != OV9650_check(isp_dev))
	{
		return -1;
	}

	OV9650_config_SENSOR(width, height, format, frame_freq);

	OV9650_set_output_size(width, height, frame_freq);

	return 0;
}



#endif


