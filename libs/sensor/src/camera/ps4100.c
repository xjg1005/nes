
/*******************************************************************************************
 File Name: PS4100.c

 Version: 1.00

 Discription: GC1004 ¨¦???¨ª¡¤3?¨º??¡¥?¡ê

 Author:xuhuajun

 Email :

 Date:2014.8.19

 Copyright:(c)JIELI  2011  @ , All Rights Reserved.
*******************************************************************************************/
#ifndef PS4100_c
#define  PS4100_c
#include "iic.h"
#include "isp_dev.h"
#include "isp_com.h"
#include "PS4100.h"

typedef struct
{
    u8 addr;
    u8 value;
} Sensor_reg_ini;

#define PLL_INPUT_MCLK      24

#define PLL_M_CNT           0x21
#define PLL_R_CNT           0x4
#define PLL_VCO             (PLL_INPUT_MCLK*PLL_M_CNT)/PLL_R_CNT
#define PLL_MAIN_CLK_DIV    0x2
#define PLL_MAIN_CLK        (PLL_VCO)/(PLL_MAIN_CLK_DIV+1)
//-------------
// update them at same time
#define PLL_PPCLK_DIV      0x1
#define PLL_PPCLK          (PLL_MAIN_CLK)*2/3
//------------------
#define PLL_PCLK_DIV        0x1
#define PLL_PCLK            (PLL_PPCLK)/(2-PLL_PCLK_DIV)

#define PS4100_CLK_PER_ROW  0x6df
#define PS4100_CLK_ROWS  0x340
//#define PS4100_CLK_PER_ROW  0x671
//#define PS4100_CLK_ROWS     0x383
#define PS4100_ROW_TIME_US  (PS4100_CLK_PER_ROW/PLL_PCLK)
#define PS4100_GET_SHUTTER(exp_time)      (exp_time*(PLL_PCLK)/PS4100_CLK_PER_ROW)


Sensor_reg_ini PS4100_INI_REG[]=
{
		{0x29,0x98},
		{0x03,0x00},
		{0x05,0x03},
		{0x06,PS4100_CLK_PER_ROW>>8},//{0x06,0x06},
		{0x07,PS4100_CLK_PER_ROW&0xff},//{0x07,0xdf},
		{0x08,PS4100_CLK_ROWS>>8},//{0x08,0x03},
		{0x09,PS4100_CLK_ROWS&0xff},//{0x09,0x40},
		{0x14,0x00},
		{0x15,0x0b},
		{0x25,((PLL_MAIN_CLK_DIV&0x3)<<3)|(PLL_PPCLK_DIV&0x7)},//{0x25,0x11},
		{0x26,0xCB},
		{0x28,0x30},
		{0x33,0x03},
		{0x34,0x03},
		{0x38,0x5f},
		{0x41,PLL_M_CNT},
		{0x42,PLL_R_CNT},
		{0x40,0x10},
		{0x05,0x00},
		{0x40,0x00},
		{0x03,0x01},
		{0xcf,0x02}, //
		{0xd0,0xe8},
		{0xd1,0x00},
		{0xd2,0x00},
		{0xd3,0x40},
		{0x03,0x01},
		{0x16,0x45},
		{0x03,0x02},
		{0x04,0x00},
		{0x05,0x00}
};

S_IIC_FUN ps4100_iic_fun;
static u8 ps4100_iic_sel;

static void my_delay(u32 clocks)
{
    return;
    delay(clocks);
}

unsigned char wrPS4100Reg(unsigned char regID, unsigned char regDat)
{
    if(ps4100_iic_sel)
    {
        iic_tx(0x64);
        delay(10);
        iic_tx(regID);
        delay(10);
        iic_tx_we(regDat);
        delay(10);
        return 1 ;
    }

	ps4100_iic_fun.startSCCB();
	if(0 == ps4100_iic_fun.SCCBwriteByte(0x64))
	{
//		puts("a");
		ps4100_iic_fun.stopSCCB();
		return(0);
	}
	my_delay(100);
  	if(0 == ps4100_iic_fun.SCCBwriteByte(regID))
	{
//		puts("b");
		ps4100_iic_fun.stopSCCB();
		return(0);
	}
	my_delay(100);
  	if(0 == ps4100_iic_fun.SCCBwriteByte(regDat))
	{
//		puts("c");
		ps4100_iic_fun.stopSCCB();
		return(0);
	}
  	ps4100_iic_fun.stopSCCB();

  	return(1);
}

unsigned char rdPS4100Reg(unsigned char regID, unsigned char *regDat)
{
    if(ps4100_iic_sel)
    {
        iic_tx(0x64);
        delay(10);
        iic_tx_we(regID);
        delay(10);

        iic_tx(0x65);
        delay(10);
        *regDat = iic_rx_we();

        return 1 ;
    }

	//¨ª¡§1yD¡ä2¨´¡Á¡Â¨¦¨¨????¡ä??¡Â¦Ì??¡¤
	ps4100_iic_fun.startSCCB();
	if(0 == ps4100_iic_fun.SCCBwriteByte(0x64))
	{
//		puts("1");
		ps4100_iic_fun.stopSCCB();
		return(0);
	}
	my_delay(100);
  	if(0 == ps4100_iic_fun.SCCBwriteByte(regID))
	{
//		puts("2");
		ps4100_iic_fun.stopSCCB();
		return(0);
	}
	ps4100_iic_fun.stopSCCB();
	my_delay(100);

	//¨¦¨¨????¡ä??¡Â¦Ì??¡¤o¨®¡ê?2?¨º??¨¢
	ps4100_iic_fun.startSCCB();
	if(0 == ps4100_iic_fun.SCCBwriteByte(0x65))
	{
//		puts("3");
		ps4100_iic_fun.stopSCCB();
		return(0);
	}
	my_delay(100);
  	*regDat = ps4100_iic_fun.SCCBreadByte();
  	ps4100_iic_fun.noAck();
  	ps4100_iic_fun.stopSCCB();

  	return(1);
}


static PS4100_AE_lut_t *ae_lut = PS4100_AE_lut_50Hz_min_25fps;

static u32 ps4100_frame_rate=30;

static u32 PS4100_get_shutter(u32 exp_time_us)
{
    return 0;
}

void PS4100_set_ae_bf_type(Sensor_AE_BF_TYPE ae_t)
{
    ae_lut = PS4100_AE_lut_50Hz_min_25fps;
}

void PS4100_config_SENSOR(u16 *width, u16 *height, u8 *format, u8 *frame_freq)
{
    u32 i;


    PS4100_set_output_size(width, height, frame_freq);

    for(i=0;i<sizeof(PS4100_INI_REG)/sizeof(Sensor_reg_ini);i++)
    {
        wrPS4100Reg(PS4100_INI_REG[i].addr,PS4100_INI_REG[i].value);
    }
    wrPS4100Reg(0x03,0x01);
    wrPS4100Reg(0xcf, (PS4100_GET_SHUTTER(10000)>>8));
    wrPS4100Reg(0xd0, (PS4100_GET_SHUTTER(10000)&0xff));

  	*format = SEN_IN_FORMAT_RGRG;

    //wrGC1004Reg(0x8d, 0x12);

	//set_ae_lut();
    return;
}


s32 PS4100_set_output_size(u16 *width, u16 *height, u8 *frame_freq)
{
	*width = PS4100_OUTPUT_W;//640;
	*height = PS4100_OUTPUT_H;//480;

	return 0;
}


s32 PS4100_power_ctl(u8 isp_dev, u8 is_work)
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

void PS4100_xclk_set(u8 isp_dev)
{
    //CLK_CON0 |= 0x0d<<10;
	//IOMC1 |= 0X0c<<12;//output PWM 24MHz
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



s32 PS4100_ID_check(void)
{
    u8 pid = 0x00;
    u8 ver = 0x00;
    u8 i ;

    for(i = 0;i < 3;i++)//?a¨¤??¨¢¨¨??¨¤¡ä?ID?¦Ì¡ê?¡¤¨¤?1¨¦???¨ª¡¤¨¦?¦Ì?o¨®2??¨¨?¡§
    {
        rdPS4100Reg(0x00,&pid);
        rdPS4100Reg(0x01,&ver);
    }

    puts("Sensor ID \n");
    put_u8hex(pid);
    put_u8hex(ver);
    puts("\n");

	if (pid != 0x41 || ver != 0x00)
	{
	    puts("\n----not PS4100-----\n");
		return -1;
	}
    puts("\n----hello PS4100-----\n");
    printf("PLL_PCLK=%d, 10ms=0x%x,20ms=0x%x,30ms=0x%x, 0.04ms = 0x%x\n",\
           PLL_PCLK, PS4100_GET_SHUTTER(10000), PS4100_GET_SHUTTER(20000), PS4100_GET_SHUTTER(30000)\
           ,PS4100_GET_SHUTTER(40));
    return 0;
}

void PS4100_reset(u8 isp_dev)
{
    sensor_puts("ps4100reset \n");
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

void PS4100_iic_set(u8 isp_dev)
{
	iic_select(&ps4100_iic_fun, isp_dev);
}

s32 PS4100_check(u8 isp_dev)
{
    if(isp_dev == ISP_DEV_0){
        ps4100_iic_sel = 1;
    }else{
        ps4100_iic_sel = 0;
    }

    if(ps4100_iic_sel)
    {
        extern void  iic_ini(void);
        iic_ini();
    }

	PS4100_xclk_set(isp_dev);
	PS4100_iic_set(isp_dev);
	PS4100_reset(isp_dev);

	if (0 != PS4100_ID_check())
	{
		return -1;
	}

	return 0;
}

void resetStatic();
s32 PS4100_init(u8 isp_dev, u16 *width, u16 *height, u8 *format, u8 *frame_freq)
{
	puts("\n\n PS4100_init \n\n");

	if (0 != PS4100_check(isp_dev))
	{
		return -1;
	}



	PS4100_config_SENSOR(width, height, format, frame_freq);
	return 0;
}



static s32 cur_ev_level =80;
static s32 aGain_bk = -1;
static s32 dGain_bk = -1;
static s32 exp_time_bk = -1;
static PS4100_AE_lut_t g_ae_level;
static s32 pre_ev_level;


static void set_aGain(u32 aGain)
{

    if(aGain==aGain_bk)
        return;
    else
        aGain_bk = aGain;

    wrPS4100Reg(0xd2, aGain); // ppk add Again on 20151015

}

static void set_shutter(u32 exp_time)
{
    int s = PS4100_GET_SHUTTER(exp_time);

    int s2 = (exp_time - s*PS4100_CLK_PER_ROW/PLL_PCLK)*255*PLL_PCLK/PS4100_CLK_PER_ROW; // get fraction exptime

    if(exp_time==exp_time_bk)
        return;
    else
        exp_time_bk = exp_time;

//printf("===== shutter = %d =======\n", s);
   wrPS4100Reg(0xcf, s>>8);
   wrPS4100Reg(0xd0, s&0xff); //
   wrPS4100Reg(0xd1, s2); //
}

static void set_dgain(int temp)
{
   // return;
    //temp = 0x40;
    if(temp==dGain_bk)
        return;
    else
        dGain_bk = temp;

    wrPS4100Reg(0xd3, temp); // ppk add dgain on 20151015
}


static u32 ps4100_isAE_stable = 0;
static u32 pre_smt_luma = 60;

static u32 ispt_smt_luma(u32 cur_luma)
{
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


s32 PS4100_update_cfg(u32 cur_mean)
{
    int pre_level = cur_ev_level;

    int target = PS4100_TARGET_LUMA;

    cur_mean = ispt_smt_luma(cur_mean);

    if(abs(cur_mean-target)<=6 && ps4100_isAE_stable == 1){
         goto AE_IS_OK;
    }

    if (cur_mean > target && abs(cur_mean-target)>=4) {
        if(0)  cur_ev_level --; else {
            if(cur_mean - target > 40)
                cur_ev_level -=5;
            else if(cur_mean - target > 30)
                cur_ev_level -=4;
            else if(cur_mean - target > 20)
                cur_ev_level -=3;
            else if(cur_mean - target > 10)
                cur_ev_level -=2;
            else
                cur_ev_level --;

            ps4100_isAE_stable = 0;
        }
    }else if (cur_mean < target && abs(cur_mean-target)>=4){
         if(0)  cur_ev_level ++; else {
        if(target - cur_mean > 40)
            cur_ev_level +=5;
        else if(target - cur_mean > 30)
            cur_ev_level +=4;
        else if(target - cur_mean > 20)
            cur_ev_level +=3;
        else if(target - cur_mean > 10)
            cur_ev_level +=2;
        else
            cur_ev_level ++;
         }
         ps4100_isAE_stable = 0;
    }else{
        ps4100_isAE_stable = 1;
    }

    if (cur_ev_level < 0)
        cur_ev_level = 0;
    else if (cur_ev_level > PS4100_MAX_EV_LEVEL)
        cur_ev_level = PS4100_MAX_EV_LEVEL;

AE_IS_OK:
    g_ae_level = ae_lut[cur_ev_level];
   // printf("(%3d,%2d,0x%2x,%5d,%2d,%2d)\n",cur_mean, cur_ev_level, g_ae_level.a_gain, g_ae_level.exp_time,g_ae_level.d_gain, ae_lut[cur_ev_level].ev_level);

    return ae_lut[cur_ev_level].ev_level;  // ppk on 20151015using NT99143 tbl for AE&banding
}


s32 PS4100_flush_cfg()
{

    wrPS4100Reg(0x03,0x01);
    set_aGain(g_ae_level.a_gain);
    set_dgain(g_ae_level.d_gain);
    set_shutter(g_ae_level.exp_time);

    wrPS4100Reg(0x03,0x00);
    return 0;
}

void PS4100_sleep()
{

}

void PS4100_wakeup()
{

}

void PS4100_W_Reg(u16 addr, u16 val)
{
    printf_without_mutex("update reg%x with %x\n",addr,val);
    wrPS4100Reg((u8)addr,(u8)val);
}
u16 PS4100_R_Reg(u16 addr)
{
    u8 val;
    rdPS4100Reg((u8)addr,&val);
    return val;
}

void reset_ps4100()
{
    ps4100_isAE_stable = 0;
    pre_smt_luma = 60;

    cur_ev_level =80;
    aGain_bk = -1;
    dGain_bk = -1;
    exp_time_bk = -1;
    memset(&g_ae_level, 0, sizeof(PS4100_AE_lut_t));
    pre_ev_level = 0;

    ae_lut = PS4100_AE_lut_50Hz_min_25fps;
}

#endif


