#ifndef SAA7113_c
#define  SAA7113_c
#include "iic.h"
#include "isp_dev.h"
#include "isp_com.h"


S_IIC_FUN saa7113_iic_fun;


unsigned char wrSAA7113Reg(unsigned char regID, unsigned char regDat)
{
	saa7113_iic_fun.startSCCB();
	if(0 == saa7113_iic_fun.SCCBwriteByte(0x4A))
	{
//		puts("a");
		saa7113_iic_fun.stopSCCB();
		return(0);
	}
	delay(1000);
  	if(0 == saa7113_iic_fun.SCCBwriteByte(regID))
	{
//		puts("b");
		saa7113_iic_fun.stopSCCB();
		return(0);
	}
	delay(1000);
  	if(0 == saa7113_iic_fun.SCCBwriteByte(regDat))
	{
//		puts("c");
		saa7113_iic_fun.stopSCCB();
		return(0);
	}
  	saa7113_iic_fun.stopSCCB();


  	return(1);
}

unsigned char rdSAA7113Reg(unsigned char regID, unsigned char *regDat)
{
	//通过写操作设置寄存器地址
	saa7113_iic_fun.startSCCB();
	if(0 == saa7113_iic_fun.SCCBwriteByte(0x4A))
	{
//		puts("1");
		saa7113_iic_fun.stopSCCB();
		return(0);
	}
	delay(1000);
  	if(0 == saa7113_iic_fun.SCCBwriteByte(regID))
	{
//		puts("2");
		saa7113_iic_fun.stopSCCB();
		return(0);
	}
	saa7113_iic_fun.stopSCCB();
	delay(1000);

	//设置寄存器地址后，才是读
	saa7113_iic_fun.startSCCB();
	if(0 == saa7113_iic_fun.SCCBwriteByte(0x4B))
	{
//		puts("3");
		saa7113_iic_fun.stopSCCB();
		return(0);
	}
	delay(1000);
  	*regDat = saa7113_iic_fun.SCCBreadByte();
  	saa7113_iic_fun.noAck();
  	saa7113_iic_fun.stopSCCB();

  	return(1);
}

void SAA7113_config_SENSOR(u16 *width, u16 *height, u8 *format, u8 *frame_freq)
{
////CONTENT BEGIN
////	wrSAA7113Reg(0x00,0x00);//chip version,read_only
//	wrSAA7113Reg(0x01,0x08);
//	wrSAA7113Reg(0x02,0xC0);
//	wrSAA7113Reg(0x03,0x33);
//	wrSAA7113Reg(0x04,0x00);
//	wrSAA7113Reg(0x05,0x00);
//	wrSAA7113Reg(0x06,0xE9);
//	wrSAA7113Reg(0x07,0x0D);
//	wrSAA7113Reg(0x08,0xB8);
//	wrSAA7113Reg(0x09,0x01);
//	wrSAA7113Reg(0x0A,0x80);
//	wrSAA7113Reg(0x0B,0x47);
//	wrSAA7113Reg(0x0C,0x40);
//	wrSAA7113Reg(0x0D,0x00);
//	wrSAA7113Reg(0x0E,0x01);
//	wrSAA7113Reg(0x0F,0x2A);
//	wrSAA7113Reg(0x10,0x38);
//	wrSAA7113Reg(0x11,0x0C);
////--RTS0,RTS1 FORMAT
//	wrSAA7113Reg(0x12,0xE8);
//	wrSAA7113Reg(0x13,0x00);
//	wrSAA7113Reg(0x14,0x00);
//	wrSAA7113Reg(0x15,0x00);
//	wrSAA7113Reg(0x16,0x00);
//	wrSAA7113Reg(0x17,0x00);
//	wrSAA7113Reg(0x18,0x00);
//	wrSAA7113Reg(0x19,0x00);
//	wrSAA7113Reg(0x1A,0x00);
//	wrSAA7113Reg(0x1B,0x00);
//	wrSAA7113Reg(0x1C,0x00);
//	wrSAA7113Reg(0x1D,0x00);
//	wrSAA7113Reg(0x1E,0x00);
//	//wrSAA7113Reg(0x1F,0x00);//decoder status byte,read_only
//	wrSAA7113Reg(0x20,0x00);
//	wrSAA7113Reg(0x21,0x00);
//	wrSAA7113Reg(0x22,0x00);
//	wrSAA7113Reg(0x23,0x00);
//	wrSAA7113Reg(0x24,0x00);
//	wrSAA7113Reg(0x25,0x00);
//	wrSAA7113Reg(0x26,0x00);
//	wrSAA7113Reg(0x27,0x00);
//	wrSAA7113Reg(0x28,0x00);
//	wrSAA7113Reg(0x29,0x00);
//	wrSAA7113Reg(0x2A,0x00);
//	wrSAA7113Reg(0x2B,0x00);
//	wrSAA7113Reg(0x2C,0x00);
//	wrSAA7113Reg(0x2D,0x00);
//	wrSAA7113Reg(0x2E,0x00);
//	wrSAA7113Reg(0x2F,0x00);
//	wrSAA7113Reg(0x30,0x00);
//	wrSAA7113Reg(0x31,0x00);
//	wrSAA7113Reg(0x32,0x00);
//	wrSAA7113Reg(0x33,0x00);
//	wrSAA7113Reg(0x34,0x00);
//	wrSAA7113Reg(0x35,0x00);
//	wrSAA7113Reg(0x36,0x00);
//	wrSAA7113Reg(0x37,0x00);
//	wrSAA7113Reg(0x38,0x00);
//	wrSAA7113Reg(0x39,0x00);
//	wrSAA7113Reg(0x3A,0x00);
//	wrSAA7113Reg(0x3B,0x00);
//	wrSAA7113Reg(0x3C,0x00);
//	wrSAA7113Reg(0x3D,0x00);
//	wrSAA7113Reg(0x3E,0x00);
//	wrSAA7113Reg(0x3F,0x00);
//
//	wrSAA7113Reg(0x40,0x02);
//	wrSAA7113Reg(0x41,0xFF);
//	wrSAA7113Reg(0x42,0xFF);
//	wrSAA7113Reg(0x43,0xFF);
//	wrSAA7113Reg(0x44,0xFF);
//	wrSAA7113Reg(0x45,0xFF);
//	wrSAA7113Reg(0x46,0xFF);
//	wrSAA7113Reg(0x47,0xFF);
//	wrSAA7113Reg(0x48,0xFF);
//	wrSAA7113Reg(0x49,0xFF);
//	wrSAA7113Reg(0x4A,0xFF);
//	wrSAA7113Reg(0x4B,0xFF);
//	wrSAA7113Reg(0x4C,0xFF);
//	wrSAA7113Reg(0x4D,0xFF);
//	wrSAA7113Reg(0x4E,0xFF);
//	wrSAA7113Reg(0x4F,0xFF);
//	wrSAA7113Reg(0x50,0xFF);
//	wrSAA7113Reg(0x51,0xFF);
//	wrSAA7113Reg(0x52,0xFF);
//	wrSAA7113Reg(0x53,0xFF);
//	wrSAA7113Reg(0x54,0xFF);
//	wrSAA7113Reg(0x55,0xFF);
//	wrSAA7113Reg(0x56,0xFF);
//	wrSAA7113Reg(0x57,0xFF);
//	wrSAA7113Reg(0x58,0x00);
//	wrSAA7113Reg(0x59,0x00);
//	wrSAA7113Reg(0x5A,0x07);
//	wrSAA7113Reg(0x5B,0x80);
//	wrSAA7113Reg(0x5C,0x00);
//	wrSAA7113Reg(0x5D,0x00);
//	wrSAA7113Reg(0x5E,0x00);
//	wrSAA7113Reg(0x5F,0x00);


    //ITU 656
    //CONTENT BEGIN
	wrSAA7113Reg(0x01,0x08);
	wrSAA7113Reg(0x02,0xC0);
	wrSAA7113Reg(0x03,0x33);
	wrSAA7113Reg(0x04,0x00);
	wrSAA7113Reg(0x05,0x00);
	wrSAA7113Reg(0x06,0xE9);
	wrSAA7113Reg(0x07,0x0D);
	wrSAA7113Reg(0x08,0xB8);//wrSAA7113Reg(0x08,0x98);//wrSAA7113Reg(0x08,0xB8);
	wrSAA7113Reg(0x09,0x01);
	wrSAA7113Reg(0x0A,0x80);
	wrSAA7113Reg(0x0B,0x47);
	wrSAA7113Reg(0x0C,0x40);
	wrSAA7113Reg(0x0D,0x00);
	wrSAA7113Reg(0x0E,0x01);
	wrSAA7113Reg(0x0F,0x2A);
	wrSAA7113Reg(0x10,0x38);//wrSAA7113Reg(0x10,0x00);//wrSAA7113Reg(0x10,0x38);
	wrSAA7113Reg(0x11,0x0C);
//--RTS0,RTS1 FORMAT
	wrSAA7113Reg(0x12,0xE8);//wrSAA7113Reg(0x12,0x01);//wrSAA7113Reg(0x12,0xE8);
	wrSAA7113Reg(0x13,0x00);
	wrSAA7113Reg(0x14,0x00);
	wrSAA7113Reg(0x15,0x00);
	wrSAA7113Reg(0x16,0x00);
	wrSAA7113Reg(0x17,0x00);
	wrSAA7113Reg(0x18,0x00);
	wrSAA7113Reg(0x19,0x00);
	wrSAA7113Reg(0x1A,0x00);
	wrSAA7113Reg(0x1B,0x00);
	wrSAA7113Reg(0x1C,0x00);
	wrSAA7113Reg(0x1D,0x00);
	wrSAA7113Reg(0x1E,0x00);
	//wrSAA7113Reg(0x1F,0x00);//decoder status byte,read_only
	wrSAA7113Reg(0x20,0x00);
	wrSAA7113Reg(0x21,0x00);
	wrSAA7113Reg(0x22,0x00);
	wrSAA7113Reg(0x23,0x00);
	wrSAA7113Reg(0x24,0x00);
	wrSAA7113Reg(0x25,0x00);
	wrSAA7113Reg(0x26,0x00);
	wrSAA7113Reg(0x27,0x00);
	wrSAA7113Reg(0x28,0x00);
	wrSAA7113Reg(0x29,0x00);
	wrSAA7113Reg(0x2A,0x00);
	wrSAA7113Reg(0x2B,0x00);
	wrSAA7113Reg(0x2C,0x00);
	wrSAA7113Reg(0x2D,0x00);
	wrSAA7113Reg(0x2E,0x00);
	wrSAA7113Reg(0x2F,0x00);
	wrSAA7113Reg(0x30,0x00);
	wrSAA7113Reg(0x31,0x00);
	wrSAA7113Reg(0x32,0x00);
	wrSAA7113Reg(0x33,0x00);
	wrSAA7113Reg(0x34,0x00);
	wrSAA7113Reg(0x35,0x00);
	wrSAA7113Reg(0x36,0x00);
	wrSAA7113Reg(0x37,0x00);
	wrSAA7113Reg(0x38,0x00);
	wrSAA7113Reg(0x39,0x00);
	wrSAA7113Reg(0x3A,0x00);
	wrSAA7113Reg(0x3B,0x00);
	wrSAA7113Reg(0x3C,0x00);
	wrSAA7113Reg(0x3D,0x00);
	wrSAA7113Reg(0x3E,0x00);
	wrSAA7113Reg(0x3F,0x00);

	wrSAA7113Reg(0x40,0x02);
	wrSAA7113Reg(0x41,0xFF);
	wrSAA7113Reg(0x42,0xFF);
	wrSAA7113Reg(0x43,0xFF);
	wrSAA7113Reg(0x44,0xFF);
	wrSAA7113Reg(0x45,0xFF);
	wrSAA7113Reg(0x46,0xFF);
	wrSAA7113Reg(0x47,0xFF);
	wrSAA7113Reg(0x48,0xFF);
	wrSAA7113Reg(0x49,0xFF);
	wrSAA7113Reg(0x4A,0xFF);
	wrSAA7113Reg(0x4B,0xFF);
	wrSAA7113Reg(0x4C,0xFF);
	wrSAA7113Reg(0x4D,0xFF);
	wrSAA7113Reg(0x4E,0xFF);
	wrSAA7113Reg(0x4F,0xFF);
	wrSAA7113Reg(0x50,0xFF);
	wrSAA7113Reg(0x51,0xFF);
	wrSAA7113Reg(0x52,0xFF);
	wrSAA7113Reg(0x53,0xFF);
	wrSAA7113Reg(0x54,0xFF);
	wrSAA7113Reg(0x55,0xFF);
	wrSAA7113Reg(0x56,0xFF);
	wrSAA7113Reg(0x57,0xFF);
	wrSAA7113Reg(0x58,0x00);
	wrSAA7113Reg(0x59,0x00);//wrSAA7113Reg(0x59,0x54);//wrSAA7113Reg(0x59,0x00);  //Horizon offset for slice
	wrSAA7113Reg(0x5A,0x07);//Vertical offset for slice
	wrSAA7113Reg(0x5B,0x80);//wrSAA7113Reg(0x5B,0x83);//wrSAA7113Reg(0x5B,0x80);  //水平和垂直偏移的高位
	wrSAA7113Reg(0x5C,0x00);
	wrSAA7113Reg(0x5D,0x00);
	wrSAA7113Reg(0x5E,0x00);
	wrSAA7113Reg(0x5F,0x00);


	*format = SEN_IN_FORMAT_UYVY;

	SAA7113_set_output_size(width, height, frame_freq);

}


void SAA7113_dec_status(void)
{
    u8 dec_status = 0x00;

    rdSAA7113Reg(0x1f,&dec_status);

    puts("dec status : ");
    put_u8hex(dec_status);
    puts("\n");
}


s32 SAA7113_set_output_size(u16 *width, u16 *height, u8 *freq)
{
	*width = 720;
	*height = 480;

	*freq = 50/2;

	return 0;
}


s32 SAA7113_power_ctl(u8 isp_dev, u8 is_work)
{
	return 0;
}



s32 SAA7113_ID_check(void)
{
    u8 ver = 0x00;

    rdSAA7113Reg(0x00,&ver);

    puts("chip ver : ");
    put_u8hex(ver);
    puts("\n");

	if (ver != 0x11)
	{
		return -1;
	}

    return 0;
}


void SAA7113_iic_set(u8 isp_dev)
{
	iic_select(&saa7113_iic_fun, isp_dev);
}

s32 SAA7113_check(u8 isp_dev)
{
	SAA7113_iic_set(isp_dev);

	if (0 != SAA7113_ID_check())
	{
		return -1;
	}

	return 0;
}


s32 SAA7113_init(u8 isp_dev, u16 *width, u16 *height, u8 *format, u8 *frame_freq)
{
	puts("\n\n SAA7113_init \n\n");

	if (0 != SAA7113_check(isp_dev))
	{
		return -1;
	}

	SAA7113_config_SENSOR(width, height, format, frame_freq);

	SAA7113_dec_status();

	return 0;
}



#endif



