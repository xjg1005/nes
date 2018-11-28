
/*******************************************************************************************
 File Name: GC1004.c

 Version: 1.00

 Discription: GC1004 ¨¦???¨ª¡¤3?¨º??¡¥?¡ê

 Author:xuhuajun

 Email :

 Date:2014.8.19

 Copyright:(c)JIELI  2011  @ , All Rights Reserved.
*******************************************************************************************/
#ifndef gc1004_c
#define  gc1004_c
#include "iic.h"
#include "isp_dev.h"
#include "isp_com.h"
#include "gc1004.h"

extern Sensor_AE_lut_t GC1004_AE_lut_50Hz[];
extern Sensor_AE_lut_t GC1004_AE_lut_50Hz2[];



typedef struct
{
    u8 addr;
    u8 value;
} Sensor_reg_ini;

Sensor_reg_ini GC1004_INI_REG[]=
{
#if (IS_GC1014==0)
		{0xfe, 0x80},
		{0xfe, 0x80},
		{0xfe, 0x80},
		{0xf2, 0x0f},//sync_pad_io_ebi
		{0xf6, 0x00},//up down
		{0xfc, 0xc6},

		//{0xf7, 0xb9},//pll enable
		//{0xf8, 0x03},//Pll mode 2
		{0xf7, (0xb9|(GC_PLL_DIV2<<1))},//pll enable
		{0xf8, (GC_PLL_DIV5B&0x1f)},//Pll mode 2

		//{0xf8, 0x08},//Pll mode 2
		{0xf9, 0x2e},//[0] pll enable
		{0xfa, 0x20},//div
		{0xfe, 0x00},
	////////////////   ANALOG & CISCTL	 ////////////////
		{0x03, 0x02},
		{0x04, 0xb5},
 		{0x05, 0x02},
 		{0x06, 0x77},
 		{0x07, 0x00},
 		{0x08, 0x0c},
 		//{0x08, 0x5a},

		//{0x0d, 0x02},
		//{0x0e, 0xd8},
		//{0x0f, 0x05},
		//{0x10, 0x10},
{0x11,(REG_SH_DELAY)>>8},
{0x12,(REG_SH_DELAY)&0xFF},
		{0x17, 0x14},
		{0x18, 0x0a},
		{0x19, 0x06},
		{0x1a, 0x09},
		{0x1b, 0x4f},
		{0x1c, 0x21},
		{0x1d, 0xe0},//f0
		{0x1e, 0x7c},
		{0x1f, 0x08},//08//comv_r
		{0x20, 0xa9},//×ó±ßÊúÏß
		{0x21, 0x6f},//2f//20//reg
		{0x22, 0xb0},
		{0x23, 0x32},//38
		{0x24, 0x55},//PAD drive {0x24, 0x2f}, {0x24, 0xaa} 20151230 san
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

#if (GC1004_OUTPUT_W==1120 && GC1004_OUTPUT_H==640)
        {0x90, 0x01}, //crop window mode
        {0x91, 0x00},
        {0x92, 0x28},
        {0x93, 0x00},
		{0x94, 0x38},
		{0x95, 0x02},
		{0x96, 0x80},
		{0x97, 0x04},
		{0x98, 0x60},
#elif (GC1004_OUTPUT_W==1280 && GC1004_OUTPUT_H==720)

		{0x90, 0x01}, //crop window mode
		{0x94, 0x02},
		{0x95, 0x02},
		{0x96, 0xd0},
		{0x97, 0x05},
		{0x98, 0x00},
#elif (GC1004_OUTPUT_W==800 && GC1004_OUTPUT_H==448)

		{0x90, 0x01}, //crop window mode
        {0x91, 0x00},
        {0x92, 0x88},
        {0x93, 0x00},
		{0x94, 0xd8},
		{0x95, 0x01},
		{0x96, 0xc0},
		{0x97, 0x03},
		{0x98, 0x20},
#else
#error "GC1004.c not support current output size"
#endif
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
		{0x66, 0x20},
	//////////////////////	 Dark SUN	/////////////////////
		{0xfe, 0x00},//dark sun
		{0xfe, 0x02},
		{0x49, 0x23},
		{0x04, 0x00},
		{0xfe, 0x00},
	//////////////////////	 Gain	/////////////////////
		//{0xb0, 0x40}, //reis 201503
		{0xb0, 0x40},
		{0xb1, 250>>8},
		{0xb2, 250&0xff},
		{0xb6, 0x02},
		{0xb3, 0x40},
		{0xb4, 0x40},
		{0xb5, 0x40},
		{0xfe, 0x00},

    //////////////////////8bit
        {0x89, 0x80},
    //////////////////////exposure
        {0x03, 0x02},
		{0x04, 0xb5},
    //////////////////////div & dummy
#if 0 //reis
        {0xf7, 0x33},
        {0xf8, 0x08},
        {0xfa, 0x00},//div//
        //{0xfa, 0x11},//div
#else
        {0xfa, GC_DCLK_DIV},
#endif
        {0xfd, 0x11},
 		//{0x05, 0x01},
 		//{0x06, 0x77},
        {0x05 , (REG_HBLANK>>8)},
        {0x06 , (REG_HBLANK&0xFF)},
 		{0x07, REG_VBLANK>>8},
 		{0x08, (REG_VBLANK&0xFF)},

 		{0x8c, 0x02},
        {0xd2, 0x90},
        {0xd0, 0xcb},
        {0xd3, 0x48},
// 		//not crop windows
 		//{0x90, 0x01}, //!crop window mode
//

 		{0x0d, REG_WIN_HEIGHT>>8},
		{0x0e, REG_WIN_HEIGHT&0xff},
		{0x0f, REG_WIN_WIDTH>>8},
		{0x10, REG_WIN_WIDTH&0xff},
//        {0x8b, 0xb2},
//        {0x8d, 0xf3},
#else
/////////////////////////////////////////////////////
//////////////////////,0x SYS,0x //////////////////////
/////////////////////////////////////////////////////
{0xfe,0x80},
{0xfe,0x80},
{0xfe,0x80},
{0xf2,0x02},
{0xf6,0x00},
{0xfc,0xc6},
{0xf7, (0xb9|(GC_PLL_DIV2<<1))},//pll enable
{0xf8, (GC_PLL_DIV5B&0x1f)},//Pll mode 2
{0xf9,0x0e},//{0xf9,0x2e},//20140901
{0xfa, GC_DCLK_DIV},
{0xfe,0x00},
/////////////////////////////////////////////////////
////////////////,0x ANALOG & CISCTL,0x ////////////////
/////////////////////////////////////////////////////
{0x03,0x02},
{0x04,0xb5},
{0x05, (REG_HBLANK>>8)},
{0x06, (REG_HBLANK&0xFF)},
{0x07, REG_VBLANK>>8},
{0x08, (REG_VBLANK&0xFF)},
{0x0d, REG_WIN_HEIGHT>>8},
{0x0e, REG_WIN_HEIGHT&0xff},
{0x0f, REG_WIN_WIDTH>>8},
{0x10, REG_WIN_WIDTH&0xff},
{0x11,(REG_SH_DELAY)>>8},
{0x12,(REG_SH_DELAY)&0xFF},
{0x17,0x14},//reverse
{0x19,0x06},
{0x1b,0x4f},
{0x1c,0x41},
{0x1d,0xe0},
{0x1e,0xfc},
{0x1f,0x38},
{0x20,0x81},
{0x21,0x2f},  //6f//2f
{0x22,0xc0},
{0x23,0xf2},
{0x24,0x55},  //{0x24,0x2f}, 20151230 san
{0x25,0xd4},
{0x26,0xa8},
{0x29,0x3f},//54//3f
{0x2a,0x00},
{0x2c,0xd0},//DC--30fps,0x d0---50fps
{0x2d,0x0f},
{0x2e,0x00},
{0x2f,0x1f},
{0xcc,0x25},
{0xce,0xf3},
{0x3f,0x08},
{0x30,0x00},
{0x31,0x01},
{0x32,0x02},
{0x33,0x03},
{0x34,0x04},
{0x35,0x05},
{0x36,0x06},
{0x37,0x07},
{0x38,0x0f},
{0x39,0x17},
{0x3a,0x1f},
/////////////////////////////////////////////////////
//////////////////////,0x ISP,0x //////////////////////
/////////////////////////////////////////////////////
{0xfe,0x00},
{0x8a,0x00},
{0x8c,0x02}, //{0x8c,0x06} 下降沿 {0x8c,0x02} 上升沿
{0x8e,0x02},
{0x8f,0x15}, //added on 20150901 for 50fps
{0x90, 0x01}, //crop window mode
{0x94, 0x02},
{0x95, GC1004_OUTPUT_H>>8},
{0x96, GC1004_OUTPUT_H&0xff},
{0x97, GC1004_OUTPUT_W>>8},
{0x98, GC1004_OUTPUT_W&0xff},
/////////////////////////////////////////////////////
//////////////////////	 MIPI	/////////////////////
/////////////////////////////////////////////////////
{0xfe,0x03},
{0x01,0x00},
{0x02,0x00},
{0x03,0x00},
{0x06,0x00},
{0x10,0x00},
{0x15,0x00},
/////////////////////////////////////////////////////
//////////////////////	 BLK	/////////////////////
/////////////////////////////////////////////////////
{0xfe,0x00},
{0x18,0x0a},
{0x1a,0x11},
{0x40,0x2b},//2b
{0x5e,0x00},
{0x66,0x80},
/////////////////////////////////////////////////////
////////////////////// Dark SUN /////////////////////
/////////////////////////////////////////////////////
#if 1
{0xfe,0x00},
{0xcc,0x25},
{0xce,0xf3},
{0x3f,0x08},
#else
{0xfe,0x02},
{0x49,0x23},
{0xa4,0x00},
#endif
/////////////////////////////////////////////////////
//////////////////////	 Gain	/////////////////////
/////////////////////////////////////////////////////
{0xfe,0x00},
{0xb0,0x40},
{0xb3,0x40},
{0xb4,0x40},
{0xb5,0x40},
{0xb6,0x00},

{0x89, 0x80}, //comment on 20150901
/////////////////////////////////////////////////////
//////////////////////,0x pad enable,0x ///////////////
/////////////////////////////////////////////////////
{0xf2,0x0f},
{0xfe,0x00},




#endif
};

S_IIC_FUN gc1004_iic_fun;
static u8 gc1004_iic_sel;

static void my_delay(u32 clocks)
{
    return;
    delay(clocks);
}

unsigned char wrGC1004Reg(unsigned char regID, unsigned char regDat)
{
    if(gc1004_iic_sel)
    {
        iic_tx(0x78);
        delay(10);
        iic_tx(regID);
        delay(10);
        iic_tx_we(regDat);
        delay(10);
        return 1 ;
    }

	gc1004_iic_fun.startSCCB();
	if(0 == gc1004_iic_fun.SCCBwriteByte(0x78))
	{
//		puts("a");
		gc1004_iic_fun.stopSCCB();
		return(0);
	}
	my_delay(100);
  	if(0 == gc1004_iic_fun.SCCBwriteByte(regID))
	{
//		puts("b");
		gc1004_iic_fun.stopSCCB();
		return(0);
	}
	my_delay(100);
  	if(0 == gc1004_iic_fun.SCCBwriteByte(regDat))
	{
//		puts("c");
		gc1004_iic_fun.stopSCCB();
		return(0);
	}
  	gc1004_iic_fun.stopSCCB();

  	return(1);
}

unsigned char rdGC1004Reg(unsigned char regID, unsigned char *regDat)
{
    if(gc1004_iic_sel)
    {
        iic_tx(0x78);
        delay(10);
        iic_tx_we(regID);
        delay(10);

        iic_tx(0x79);
        delay(10);
        *regDat = iic_rx_we();

        return 1 ;
    }

	//¨ª¡§1yD¡ä2¨´¡Á¡Â¨¦¨¨????¡ä??¡Â¦Ì??¡¤
	gc1004_iic_fun.startSCCB();
	if(0 == gc1004_iic_fun.SCCBwriteByte(0x78))
	{
//		puts("1");
		gc1004_iic_fun.stopSCCB();
		return(0);
	}
	my_delay(100);
  	if(0 == gc1004_iic_fun.SCCBwriteByte(regID))
	{
//		puts("2");
		gc1004_iic_fun.stopSCCB();
		return(0);
	}
	gc1004_iic_fun.stopSCCB();
	my_delay(100);

	//¨¦¨¨????¡ä??¡Â¦Ì??¡¤o¨®¡ê?2?¨º??¨¢
	gc1004_iic_fun.startSCCB();
	if(0 == gc1004_iic_fun.SCCBwriteByte(0x79))
	{
//		puts("3");
		gc1004_iic_fun.stopSCCB();
		return(0);
	}
	my_delay(100);
  	*regDat = gc1004_iic_fun.SCCBreadByte();
  	gc1004_iic_fun.noAck();
  	gc1004_iic_fun.stopSCCB();

  	return(1);
}


/*************************************************************************************************
    sensor api
*************************************************************************************************/
static u32 gc1004_pclk;
static u32 gc1004_hblank;
static u32 gc1004_vblank;
static u32 gc_pll_div5b;

static u32 gc1004_get_shutter(u32 exp_time_us)
{

    u32 s = (((gc1004_pclk/2) * 4ul * (exp_time_us)) / (gc1004_hblank+REG_SH_DELAY+(REG_WIN_WIDTH/2)+4));
    //u32 s = (((REG_CLK/2) * 4ul * (exp_time_us)) / (REG_HBLANK+REG_SH_DELAY+(REG_WIN_WIDTH/2)+4));
    return s;

}

static Sensor_AE_lut_t *ae_lut = GC1004_AE_lut_50Hz_min_33fps;

void GC1004_set_ae_bf_type(Sensor_AE_BF_TYPE ae_t)
{
    if(ae_t == AE_BF_50HZ) {
           //ae_lut = GC1004_AE_lut_50Hz_min_33fps;
        //ae_lut = GC1004_AE_lut_50Hz_min_25fps;
        ae_lut = GC1004_AE_lut_50Hz_min_33fps;
        puts("------- Anti 50hz banding filter\n");
    }else if(ae_t == AE_BF_60HZ) {
        ae_lut = GC1004_AE_lut_60Hz_min_30fps;

    }else{
        ae_lut = GC1004_AE_lut_min_30fps;
    }
}

void GC1004_config_SENSOR(u16 *width, u16 *height, u8 *format, u8 *frame_freq)
{
    u32 i;


    GC1004_set_output_size(width, height, frame_freq);

    for(i=0;i<sizeof(GC1004_INI_REG)/sizeof(Sensor_reg_ini);i++)
    {
        wrGC1004Reg(GC1004_INI_REG[i].addr,GC1004_INI_REG[i].value);
    }
    #if 1

    if(*frame_freq == 30) {
        #if 1 //for larger hblank;
        gc1004_hblank = 0x27c;
        gc1004_vblank = 0x10;
        gc_pll_div5b = 0x4;
        #else
        gc1004_hblank = 0x177;
        gc1004_vblank = 0x10;
        gc_pll_div5b = 0x3;
        #endif
    }else if(*frame_freq == 50){
        gc1004_hblank = 0x1c2;
        gc1004_vblank = 0x4;
        gc_pll_div5b = 0x6;
    }else if(*frame_freq == 60){
        gc1004_hblank = 0x18d;
        gc1004_vblank = 0x4;
        gc_pll_div5b = 0x7;
    }else{
        #if 1 //for larger hblank;
        gc1004_hblank = 0x27c;
        gc1004_vblank = 0x10;
        gc_pll_div5b = 0x4;
        #else
        gc1004_hblank = 0x177;
        gc1004_vblank = 0x10;
        gc_pll_div5b = 0x3;
        #endif
    }
    gc1004_pclk = ((HOST_MCLK*(gc_pll_div5b+1)/(1+GC_PLL_DIV2))/(2*(GC_DCLK_DIV+1)));
    wrGC1004Reg(0xf8, (gc_pll_div5b&0x1f));
    wrGC1004Reg(0x05, gc1004_hblank>>8);
    wrGC1004Reg(0x06, gc1004_hblank&0xff);
    wrGC1004Reg(0x07, gc1004_vblank>>8);
    wrGC1004Reg(0x08, gc1004_vblank&0xff);
    #endif

  	*format = SEN_IN_FORMAT_RGRG;

    wrGC1004Reg(0x8d, 0x12);

	//set_ae_lut();
    return;
}


s32 GC1004_set_output_size(u16 *width, u16 *height, u8 *frame_freq)
{
	*width = GC1004_OUTPUT_W;//640;
	*height = GC1004_OUTPUT_H;//480;

	return 0;
}


s32 GC1004_power_ctl(u8 isp_dev, u8 is_work)
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


void GC1004_xclk_set(u8 isp_dev)
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



s32 GC1004_ID_check(void)
{
    u8 pid = 0x00;
    u8 ver = 0x00;
    u8 i ;

    for(i = 0;i < 3;i++)//?a¨¤??¨¢¨¨??¨¤¡ä?ID?¦Ì¡ê?¡¤¨¤?1¨¦???¨ª¡¤¨¦?¦Ì?o¨®2??¨¨?¡§
    {
        rdGC1004Reg(0xf0,&pid);
        rdGC1004Reg(0xf1,&ver);
    }

    puts("Sensor PID \n");
    put_u8hex(pid);
    put_u8hex(ver);
    puts("\n");

	if (pid != 0x10 || ver != 0x04)
	{
	    puts("\n----not GC1004-----\n");
		return -1;
	}
    puts("\n----hello GC1004-----\n");
    return 0;
}

void GC1004_reset(u8 isp_dev)
{
    sensor_puts("reset \n");
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

void GC1004_iic_set(u8 isp_dev)
{
	iic_select(&gc1004_iic_fun, isp_dev);
}

s32 GC1004_check(u8 isp_dev)
{
    if(isp_dev == ISP_DEV_0){
        gc1004_iic_sel = 1;
    }else{
        gc1004_iic_sel = 0;
    }

    if(gc1004_iic_sel)
    {
        extern void  iic_ini(void);
        iic_ini();
    }

	GC1004_xclk_set(isp_dev);
	GC1004_iic_set(isp_dev);
	GC1004_reset(isp_dev);

	if (0 != GC1004_ID_check())
	{
		return -1;
	}

	return 0;
}

void resetStatic();
s32 GC1004_init(u8 isp_dev, u16 *width, u16 *height, u8 *format, u8 *frame_freq)
{
	puts("\n\n gc1004_init22 \n\n");

	if (0 != GC1004_check(isp_dev))
	{
		return -1;
	}



	GC1004_config_SENSOR(width, height, format, frame_freq);
    resetStatic();



	return 0;
}

void update_GC1004_shutter(u16 iShutter)
{
    unsigned char reg1,reg2;



    //Update Shutter
    wrGC1004Reg(0x04, (iShutter) & 0xFF);
    wrGC1004Reg(0x03, (iShutter >> 8) & 0x1F);

/*
    //analog gain
    rdGC1004Reg(0x04, &reg1);//
    rdGC1004Reg(0x03, &reg2);
    if((reg1 != (iShutter) & 0xFF)  || (reg2 !=(iShutter >> 8) & 0x1F)){
       // ISP_SHOWLINE;
    }
*/

    //delay_2ms(1);
}



static u8 global_gain = 0x00;
BOOL update_global_gain(int iGainStatus)
{
    u16 newgain;
    u16 iReg,temp;

    newgain = global_gain;
    if (iGainStatus>0)
    {
        if (newgain)
            newgain--;
    }
    else
        newgain ++;
    if (newgain >GAIN_MAX)
        newgain = GAIN_MAX;
    if (newgain <GAIN_MIN)
        newgain = GAIN_MIN;
    if (newgain != global_gain)
    {
        global_gain = newgain;
        wrGC1004Reg(0xb6, global_gain);
        return TRUE;
    }
    return FALSE;
 }


int MAX3(int a, int b, int c)
{
   if(a<b)
      a=b;

   if(a>c)
       return a;
   else
       return c;
}



static int cur_ev_level = 85;
static s32 aGain_bk = -1;
static s32 dGain_bk = -1;
static s32 gGain_bk = -1;
static s32 exp_time_bk = -1;


static void set_aGain(u32 aGain)
{

    unsigned char reg;
#if 1
    if(aGain==aGain_bk)
        return;
    else
        aGain_bk = aGain;
#endif

    //analog gain
    wrGC1004Reg(0xb6, aGain);//
//    rdGC1004Reg(0xb6, &reg);
//    if(reg != aGain){

//    }
}

static void set_sensor_dGain(u32 dGain, int *awb_gain)
{

    wrGC1004Reg(0xb1, dGain>>6);
    wrGC1004Reg(0xb2, (dGain<<2)&0xfc);
}


static int set_shutter(u32 exp_time)
{
    int s;

    s = gc1004_get_shutter(exp_time);

#if 1
    if(exp_time==exp_time_bk)
        return s;
    else
        exp_time_bk = exp_time;
#endif
    update_GC1004_shutter((u16)s);

    return s;
}
/*
#define GLOBAL_GAIN_PRECISE 0x40
void decompose_gain(int total_gain, int gG, int *aG, int *dG)
{
    u32 temp;
#if TRAINING_ANTIFLICKER
    int iReg = total_gain;
#else
    int iReg = total_gain*GLOBAL_GAIN_PRECISE/gG;
#endif



    if(iReg < ANALOG_GAIN_1) {
        iReg = ANALOG_GAIN_1;
    }

    if((ANALOG_GAIN_1<= iReg)&&(iReg < ANALOG_GAIN_2)){
        *aG = 0;
        *dG = DGAIN_PRECISE*iReg/ANALOG_GAIN_1;

    }else if((ANALOG_GAIN_2<= iReg)&&(iReg < ANALOG_GAIN_3)){
        *aG = 1;
        *dG = DGAIN_PRECISE*iReg/ANALOG_GAIN_2;

    }else if((ANALOG_GAIN_3<= iReg)&&(iReg < ANALOG_GAIN_4)){
        *aG = 2;
        *dG = DGAIN_PRECISE*iReg/ANALOG_GAIN_3;

    }else if((ANALOG_GAIN_4<= iReg)&&(iReg < ANALOG_GAIN_5)){
        *aG = 3;
        *dG = DGAIN_PRECISE*iReg/ANALOG_GAIN_4;

    }else if((ANALOG_GAIN_5<= iReg)&&(iReg < ANALOG_GAIN_6)){
        *aG = 4;
        *dG = DGAIN_PRECISE*iReg/ANALOG_GAIN_5;

    }else if((ANALOG_GAIN_6<= iReg)&&(iReg < ANALOG_GAIN_7)){
        *aG = 5;
        *dG = DGAIN_PRECISE*iReg/ANALOG_GAIN_6;

    }else if((ANALOG_GAIN_7<= iReg)&&(iReg < ANALOG_GAIN_8)){
        *aG = 6;
        *dG = DGAIN_PRECISE*iReg/ANALOG_GAIN_7;

    }else if((ANALOG_GAIN_8<= iReg)&&(iReg < ANALOG_GAIN_9)){
        *aG = 7;
        *dG = DGAIN_PRECISE*iReg/ANALOG_GAIN_8;

    }else{
        *aG = 8;
        *dG = DGAIN_PRECISE*iReg/ANALOG_GAIN_9;
    }
}
*/

static void set_dgain(int temp)
{
#if 1
    if(temp==dGain_bk)
        return;
    else
        dGain_bk = temp;

#endif

    wrGC1004Reg(0xb1, temp>>6);
    wrGC1004Reg(0xb2, (temp<<2)&0xfc);
}

static void set_ggain(int temp)
{
    if(temp==gGain_bk)
        return;
    else
        gGain_bk = temp;
    wrGC1004Reg(0xb0, temp);
}

static Sensor_AE_lut_t g_ae_level;


static u8 gc1004_cmd[6][3]=
{
0x78 ,0xb6 ,0x00 , //again
0x78 ,0xb1 ,0x00 , //dgainL
0x78 ,0xb2 ,0x00 , //dgainH
0x78 ,0xb0 ,0x00,  //ggaim
0x78 ,0x04 ,0x00,  //exptime L
0x78 ,0x03 ,0x00   //exptime H
} ;

//#define IIC_SET_ACK()               (IIC_CON &= ~BIT(10))
//void   iic_write_dma(u8 *buf, u16 len)
//{
//    IIC_SET_ACK();
//    IIC_DMA_ADR = (u32)buf;
//    IIC_DMA_CNT = len;
//}

s32 GC1004_flush_cfg()
{
#if 1
    static u32 pragain ,predgain,preexptime,preggain ;

    //g_ae_level.exp_time = 1000;
    if( (pragain!=g_ae_level.a_gain) || (predgain != g_ae_level.d_gain ) ||(preexptime!=g_ae_level.exp_time ) || (g_ae_level.g_gain!=preggain))
    {
        u16 tmp ;
        tmp = g_ae_level.exp_time;
        pragain=g_ae_level.a_gain;
        predgain=g_ae_level.d_gain;
        preexptime=g_ae_level.exp_time;
        //preggain = g_ae_level.g_gain;

    //    set_aGain(g_ae_level.a_gain);
    //    set_dgain(g_ae_level.d_gain);
        set_ggain(g_ae_level.g_gain);
    //    set_shutter(preexptime);


        gc1004_cmd[0][2] = g_ae_level.a_gain ;
        gc1004_cmd[1][2] = g_ae_level.d_gain>>6 ;

        gc1004_cmd[2][2] = (g_ae_level.d_gain<<2)&0xfc;

        gc1004_cmd[3][2] = g_ae_level.g_gain ;


        //wrGC1004Reg(0x04, (iShutter) & 0xFF);
        //wrGC1004Reg(0x03, (iShutter >> 8)  & 0x1F);
        tmp = gc1004_get_shutter(preexptime);

        gc1004_cmd[4][2] = tmp& 0xFF ;
        gc1004_cmd[5][2] = ( tmp>>8)&0x1F;

        IIC_DMA_NRATE = 2 ;
        iic_write_dma((u8*)gc1004_cmd ,18) ;
    }



#else
    set_aGain(g_ae_level.a_gain);
    set_dgain(g_ae_level.d_gain);
    set_ggain(g_ae_level.g_gain);
    set_shutter(g_ae_level.exp_time);
#endif

    return 0;
}


void resetStatic()
{
   // extern s32 test_huff();
   // test_huff();

    g_ae_level = GC1004_AE_lut_50Hz_min_33fps[cur_ev_level];
    GC1004_flush_cfg();
}

//平滑统计亮度，在快速光照变化的场景中，缓解AE震荡

static u32 ispt_smt_luma(u32 cur_luma)
{
    //return cur_luma;
    static u32 pre_smt_luma = 60;
    if((cur_luma > pre_smt_luma && cur_luma - pre_smt_luma > 20) || \
       (cur_luma < pre_smt_luma && pre_smt_luma - cur_luma > 20)){
        pre_smt_luma = cur_luma;
        return cur_luma;
    }

    cur_luma = (cur_luma*3+pre_smt_luma*1+2) >> 2;
    if(cur_luma < 0) cur_luma = 0;
    if(cur_luma > 255) cur_luma = 255;
    pre_smt_luma = cur_luma;
    return cur_luma;
}

static u32 gc1004_isAE_stable=0;
s32 GC1004_update_cfg(u32 m_mean)
{
    Sensor_AE_lut_t ae_level;
    int diff_bk,diff;
    int pre_level = cur_ev_level;
    int cur_mean;
    int target_mean = GC1004_TARGET_LUMA;

    cur_mean = m_mean;

    cur_mean = ispt_smt_luma(cur_mean);

    if(ae_lut == 0){
        return 0;
    }

    if(abs(cur_mean-target_mean)<= 8 && gc1004_isAE_stable == 1) {
        goto GC1004_AE_OK;
    }


    if (cur_mean > target_mean) {

        diff_bk = diff = cur_mean - target_mean;

        if ((diff /=5) != 0) {
            if ((diff >>= 1) == 0)
                cur_ev_level--;               // step up exposure table
            else {
                cur_ev_level -= diff;         // step up exposure table
                //cur_ev_level --;
            }

            gc1004_isAE_stable = 0;
        }else{
            gc1004_isAE_stable = 1;
        }

    }else if (cur_mean < target_mean){
        diff_bk = diff = target_mean - cur_mean;

        if ((diff /=5) != 0){
            if ((diff >>= 1) == 0)
                cur_ev_level++;               // step up exposure table
            else {
                cur_ev_level += diff;         // step up exposure table
                //cur_ev_level++;
            }
            gc1004_isAE_stable = 0;
        }else{
            gc1004_isAE_stable = 1;
        }


    }

    if (cur_ev_level < 0)
        cur_ev_level = 0;
    else if (cur_ev_level > MAX_EV_LEVEL)
        cur_ev_level = MAX_EV_LEVEL;

    //if(cur_ev_level < MAX_EV_LEVEL && (GC1004_AE_lut_50Hz[cur_ev_level+1]).a_gain>7)
    //    cur_ev_level = pre_level;

GC1004_AE_OK:
    ae_level = ae_lut[cur_ev_level];

    if(cur_ev_level!=pre_level)
    {
        //printf("(%3d,%2d,%d,%4d,%3d,%3d,%d)\n",m_mean, cur_ev_level,ae_level.a_gain,ae_level.exp_time,ae_level.d_gain,ae_level.g_gain, ae_lut[cur_ev_level].ev_level);
        g_ae_level = ae_level;
    }

    return ae_lut[cur_ev_level].ev_level;
}


void GC1004_sleep()
{

    wrGC1004Reg(0xf6,0x01);
}

void GC1004_wakeup()
{

    wrGC1004Reg(0xf6,0x00);
}

void GC1004_W_Reg(u16 addr, u16 val)
{
    printf_without_mutex("update reg%x with %x\n",addr,val);
    wrGC1004Reg((u8)addr,(u8)val);
}
u16 GC1004_R_Reg(u16 addr)
{
    u8 val;
    rdGC1004Reg((u8)addr,&val);
    return val;
}


void reset_gc10X4()
{
gc1004_isAE_stable=0;

    memset(&g_ae_level, 0, sizeof(Sensor_AE_lut_t));

cur_ev_level = 85;
aGain_bk = -1;
dGain_bk = -1;
gGain_bk = -1;
exp_time_bk = -1;

global_gain = 0x00;


gc1004_pclk = 0;
gc1004_hblank = 0;
gc1004_vblank = 0;
gc_pll_div5b = 0;
ae_lut = GC1004_AE_lut_50Hz_min_33fps;

}

#endif


