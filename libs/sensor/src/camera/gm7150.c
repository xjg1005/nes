#include "gm7150.h"
#include "iic.h"


//0xB8
//0xB9


S_IIC_FUN gm7150_iic_fun;


#define WRCMD 0xB8
#define RDCMD 0xB9

unsigned char wrGM7150Reg(u16 regID, unsigned char regDat)
{

//    iic_write(WRCMD, (u8)regID,regDat);
//    return 1;

	gm7150_iic_fun.startSCCB();
	if(0 == gm7150_iic_fun.SCCBwriteByte(WRCMD))
	{
		puts("a");
		gm7150_iic_fun.stopSCCB();
		return(0);
	}
	delay(50);

	if(0 == gm7150_iic_fun.SCCBwriteByte((u8)(regID)))
	{
		puts("b");
		gm7150_iic_fun.stopSCCB();
		return(0);
	}
	delay(50);
  	if(0 == gm7150_iic_fun.SCCBwriteByte(regDat))
	{
		puts("c");
		gm7150_iic_fun.stopSCCB();
		return(0);
	}
  	gm7150_iic_fun.stopSCCB();

  	return(1);
}

unsigned char rdGM7150Reg(u16 regID, unsigned char *regDat)
{
//    *regDat = iic_read(WRCMD,(u8)regID);
//    return 1;


	//通过写操作设置寄存器地址
	gm7150_iic_fun.startSCCB();
	if(0 == gm7150_iic_fun.SCCBwriteByte(WRCMD))
	{
		puts("1");
		gm7150_iic_fun.stopSCCB();
		return(0);
	}
	delay(50);
  if(0 == gm7150_iic_fun.SCCBwriteByte((u8)(regID)))
	{
		puts("2");
		gm7150_iic_fun.stopSCCB();
		return(0);
	}
	gm7150_iic_fun.stopSCCB();
	delay(50);

	//设置寄存器地址后，才是读
	gm7150_iic_fun.startSCCB();
	if(0 == gm7150_iic_fun.SCCBwriteByte(RDCMD))
	{
		puts("3");
		gm7150_iic_fun.stopSCCB();
		return(0);
	}
	delay(50);
  	*regDat = gm7150_iic_fun.SCCBreadByte();

  	gm7150_iic_fun.stopSCCB();

//  	puts("finish");
  	return(1);
}

void gm7150_reset()
{
    //puts("\nGM7150 Reset");
    GM7150_NORMAL();

	GM7150_RESET_H();
    //delay_2ms(100);
    delay(50000);
    GM7150_RESET_L();
    //delay_2ms(300);//500ms
    delay(50000);
    GM7150_RESET_H();
}

s32 gm7150_id_check()
{
    u16 id = 0;
    u16 id1 = 0;
    u16 version = 0;
    u16 version1 = 0;

    //delay_2ms(50);//需要加延时，否则前面几个byte读得不正确
    delay(50000);

    wrGM7150Reg(0x05, 0x01);//Soft Reset

    wrGM7150Reg(0x05, 0x00);

    rdGM7150Reg(0x80, (unsigned char *)&id);
    id <<= 8;
    rdGM7150Reg(0x81, (unsigned char *)&id1);
    id |= id1;
    puts("\nid:");
    put_u16hex(id);

    rdGM7150Reg(0x82, (unsigned char *)&version);
    version <<= 8;
    rdGM7150Reg(0x83, (unsigned char *)&version1);
    version |= version1;
    puts("\nver:");
    put_u16hex(version);

    if(id == 0x7150)
    {
        puts("\ngm7150 init!\n");
        return 1;
    }
    else if(id == 0x5150)
    {
        puts("\ntvp5150 init!\n");
        return 2;
    }

    return 0;
}


void GM7150_iic_set(u8 isp_dev)
{
	iic_select(&gm7150_iic_fun, isp_dev);
}




s32 gm7150_check(u8 isp_dev)
{
    gm7150_reset();
    GM7150_iic_set(isp_dev);

	if (0 == gm7150_id_check()){
		return -1;
	}

	return 0;
}


s32 gm7150_set_output_size(u16 *width, u16 *height, u8 *freq)
{
	*width = 720;
	*height = 480;

	*freq = 30;

	return 0;
}

extern void gm7150_init();
void GM7150_config_SENSOR(u16 *width, u16 *height, u8 *format, u8 *frame_freq)
{
    gm7150_init();//兼容tvp5150

	*format = SEN_IN_FORMAT_UYVY;
	gm7150_set_output_size(width, height, frame_freq);
}

s32 gm7150_initialize(u8 isp_dev, u16 *width, u16 *height, u8 *format, u8 *frame_freq)
{
	puts("\ngm7150_init \n");
	if (0 != gm7150_check(isp_dev)){
		return -1;
	}

    GM7150_config_SENSOR(width, height, format, frame_freq);

	return 0;
}


void gm7150_init()
{
    u8 status = 0;
    u32 cnt=0;
    u16 avid_start,avid_stop;

    wrGM7150Reg(0x00,0x00);//AIP1A
#ifdef GM7150_656
    wrGM7150Reg(0x03,0x09);
#else//601
    wrGM7150Reg(0x03,0x0d);//CVBS输入,输出行场同步信号
#endif
//    iic_write(0xB8,0x03,0x0F);//场消隐开 VBLK开启

#ifndef GM7150_656
    wrGM7150Reg(0x0D,0x40);//BT601
    wrGM7150Reg(0x0F,0x00);
#endif

    wrGM7150Reg(0x16,0x80);//hsync start position
    wrGM7150Reg(0x18,0x00);//VBLK Start
    wrGM7150Reg(0x19,0x00);//VBLK_Stop

    avid_start = 0;
    avid_stop = 0;
    wrGM7150Reg(0x11,avid_start>>2);                     //AVID Start
    wrGM7150Reg(0x12,BIT(2)|(avid_start&0x03));
    wrGM7150Reg(0x13,avid_stop>>2);                      //AVID Stop
    wrGM7150Reg(0x14,(avid_stop&0x03));

//    iic_write(0xB8,0x03,0xcf);//PIN27 输出VBLK信号
//    iic_write(0xB8,0x0f,0x0a);

    wrGM7150Reg(0x08,0x40);//亮度处理控制
    wrGM7150Reg(0x09,0x70);//明度
    wrGM7150Reg(0x0a,0x80);//色度饱和度
    wrGM7150Reg(0x0b,0x00);//色度色调
    wrGM7150Reg(0x0c,0x50);//亮度对比度

//    iic_write(0xB8,0x28,0x04);//指定输入格式 (B,D,G,H,I,N)PAL

#if 0
    while(1)
    {
        rdGM7150Reg(0x88, &status);
        put_u8hex(status);
        if((status & 0x06) == 0x06)
        {
            cnt++;
            if(cnt > 200)
            {
                puts("\n0x88:");
                put_u8hex(status);
                rdGM7150Reg(0x8C, &status);
                puts("\n0x8c:");
                put_u8hex(status);
                break;
            }
        }
        else
        {
            cnt = 0;
        }
    }
#endif

    //delay_2ms(50);
    delay(50000);
}

s32 gm7150_power_ctl(u8 isp_dev, u8 is_work)
{
	return 0;
}






