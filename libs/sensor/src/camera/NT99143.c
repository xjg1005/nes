/*******************************************************************************************
 Reis 201506
*******************************************************************************************/
#ifndef NT99143_c
#define  NT99143_c
#include "iic.h"
#include "isp_dev.h"
#include "isp_com.h"
#include "NT99143.h"

S_IIC_FUN NT99143_iic_fun;
static u8 NT99143_iic_sel;

typedef struct
{
    u16 addr;
    u8 value;
}Sensor_reg_ini;

const Sensor_reg_ini NT99143_INI_REG[]=
{
//initial setting
{0x3069, 0x01},
{0x306A, 0x02},
{0x3100, 0x03},
{0x3101, 0x00},
{0x3102, 0x0A},
{0x3103, 0x00},
{0x3105, 0x03},
{0x3106, 0x04},
{0x3107, 0x10},
{0x3108, 0x00},
{0x3109, 0x00},
{0x307D, 0x00},
{0x310A, 0x05},
{0x310C, 0x00},
{0x310D, 0x80},
{0x3110, 0x33},
{0x3111, 0x59},
{0x3112, 0x44},
{0x3113, 0x66},
{0x3114, 0x66},
{0x311D, 0x40},
{0x3127, 0x01},
{0x3129, 0x44},
{0x3136, 0x59},
{0x313F, 0x02},
//{0x3012, 0x02},
//{0x3013, 0x00},
{0x3012, 0x01},
{0x3013, 0xc6},
{0x3262, 0x10},
{0x32B3, 0x80},
{0x32B4, 0x20},
{0x32CB, 0x30},
{0x32CC, 0x70},
{0x32CD, 0xA0},
{0x3297, 0x03},
{0x30A1, 0x23},
{0x30A2, 0x70},
{0x30A3, 0x01},
{0x30A0, 0x03},
{0x303E, 0x04},
{0x303F, 0x32},
{0x3055, 0x00},
{0x3056, 0x18},
{0x305F, 0x00},
{0x3268, 0x01},
{0x3051, 0x3A},
{0x308E, 0x3A},
{0x3360, 0x14},
{0x3361, 0x24},
{0x3362, 0x28},
{0x3363, 0x01},
{0x324F, 0x00},
{0x3210, 0x0C},
{0x3211, 0x0C},
{0x3212, 0x0C},
{0x3213, 0x0C},
{0x3214, 0x08},
{0x3215, 0x08},
{0x3216, 0x08},
{0x3217, 0x08},
{0x3218, 0x08},
{0x3219, 0x08},
{0x321A, 0x08},
{0x321B, 0x08},
{0x321C, 0x0C},
{0x321D, 0x0C},
{0x321E, 0x0C},
{0x321F, 0x0C},
{0x3230, 0x00},
{0x3231, 0x17},
{0x3232, 0x09},
{0x3233, 0x09},
{0x3234, 0x00},
{0x3235, 0x00},
{0x3236, 0x00},
{0x3237, 0x00},
{0x3238, 0x18},
{0x3239, 0x18},
{0x323A, 0x18},
{0x3243, 0xC3},
{0x3244, 0x00},
{0x3245, 0x00},
{0x3270, 0x00},
{0x3271, 0x0B},
{0x3272, 0x16},
{0x3273, 0x2B},
{0x3274, 0x3F},
{0x3275, 0x51},
{0x3276, 0x72},
{0x3277, 0x8F},
{0x3278, 0xA7},
{0x3279, 0xBC},
{0x327A, 0xDC},
{0x327B, 0xF0},
{0x327C, 0xFA},
{0x327D, 0xFE},
{0x327E, 0xFF},
{0x3371, 0x00},
{0x3372, 0x00},
{0x3374, 0x00},
{0x3379, 0x00},
{0x337A, 0x00},
{0x337C, 0x00},
#if NT99143_NEW_A_GAIN==1
// 使用0x32CF来调试A Gain, 那么以下三个寄存器分别是不同Gain值下面的滤波强度阀值
{0x33A9, 0x01},
{0x33AA, 0x03},
{0x33AC, 0x05},
#else
// 固定降噪滤波强度，一般设置在0x5-0xA,最大不超过0x20
{0x334E, 0x02},
{0x33A9, 0x02},
{0x334A, 0x02},
{0x33AC, 0x02},
#endif

{0x33AD, 0x00},
{0x33AE, 0x00},
{0x33B0, 0x00},
{0x3710, 0x07},
{0x371E, 0x02},
{0x371F, 0x02},
{0x3364, 0x0B},
{0x33BD, 0x00},
{0x33BE, 0x08},
{0x33BF, 0x10},
{0x3700, 0x00},
{0x3701, 0x0C},
{0x3702, 0x18},
{0x3703, 0x32},
{0x3704, 0x44},
{0x3705, 0x54},
{0x3706, 0x70},
{0x3707, 0x88},
{0x3708, 0x9D},
{0x3709, 0xB0},
{0x370A, 0xCF},
{0x370B, 0xE2},
{0x370C, 0xEF},
{0x370D, 0xF7},
{0x370E, 0xFF},
{0x3250, 0x35},
{0x3251, 0x21},
{0x3252, 0x30},
{0x3253, 0x10},
{0x3254, 0x3E},
{0x3255, 0x2A},
{0x3256, 0x2A},
{0x3257, 0x1B},
{0x3258, 0x4D},
{0x3259, 0x3C},
{0x325A, 0x28},
{0x325B, 0x16},
{0x325C, 0x80},
{0x325D, 0x08},
{0x3813, 0x07},
{0x32BB, 0xE7},
{0x32B8, 0x08},
{0x32B9, 0x04},
{0x32BC, 0x40},
{0x32BD, 0x04},
{0x32BE, 0x02},
{0x32BF, 0x60},
{0x32C0, 0x6A},
{0x32C1, 0x6A},
{0x32C2, 0x6A},
{0x32C3, 0x00},
{0x32C4, 0x30},
{0x32C5, 0x3F},
{0x32C6, 0x3F},
{0x32D3, 0x00},
{0x32D4, 0xE3},
{0x32D5, 0x7C},
{0x32D6, 0x00},
{0x32D7, 0xBD},
{0x32D8, 0x77},
{0x32F0, 0x20},
{0x3200, 0x00},
{0x3201, 0x08},
{0x302A, 0x80 | (N143_PCLK_DIV&0x3) | (((N143_PCLK_MUL)&0x3)<<2)},
{0x302C, 0x17},
{0x302D, 0x11},
{0x3022, 0x00},
{0x300A, N143_CLK_PER_ROW>>8},
{0x300B, N143_CLK_PER_ROW&0xff},
{0x300C, 0x02},
{0x300D, 0xF4},
#if NT99143_NEW_A_GAIN==1
{0x301D, 0x00}, //a-gain
#else
{0x301D, 0x30}, //a-gain
#endif
{0x301E, 0x08},
{0x3024, 0x00},
{0x334B, 0x00},
{0x3810, 0x00},
{0x320A, 0x00},
{0x3021, 0x02},
//{0x3021, 0x82}, // swap MSB&LSB dataline
//{0x3060, 0x02},


};

#define WRCMD 0x54
#define RDCMD 0x55

unsigned char wrNT99143Reg(u16 regID, unsigned char regDat)
{
    if(NT99143_iic_sel)
    {
        iic_tx(WRCMD);
        delay(10);
        iic_tx((u8)(regID>>8));
        delay(10);
        iic_tx((u8)(regID&0xff));
        delay(10);
        iic_tx_we(regDat);
        delay(10);
        return 1 ;
    }
    else
    {
        return 0;
    }

}

unsigned char rdNT99143Reg(u16 regID, unsigned char *regDat)
{
    if(NT99143_iic_sel)
    {
        iic_tx(WRCMD);
        delay(10);
        iic_tx_we((u8)(regID>>8));
        delay(10);
        iic_tx_we((u8)(regID&0xff));
        delay(10);

        iic_tx(RDCMD);
        delay(10);
        *regDat = iic_rx_we();
        return 1 ;
    }
    else
    {
        return 0;
    }

}

/*************************************************************************************************
    sensor api
*************************************************************************************************/
void NT99143_config_SENSOR(u16 *width, u16 *height, u8 *format, u8 *frame_freq)
{
    u32 i;
    u8 pid = 0x00;
    u32 frame_exp_rows;

    NT99143_set_output_size(width, height, frame_freq);


    for(i=0;i<sizeof(NT99143_INI_REG)/sizeof(Sensor_reg_ini);i++)
    {
        wrNT99143Reg(NT99143_INI_REG[i].addr,NT99143_INI_REG[i].value);
    }

    frame_exp_rows = 1000000/(N143_ROW_TIME_US*(*frame_freq));
    wrNT99143Reg(0x300C,(frame_exp_rows>>8)&0xff);
    wrNT99143Reg(0x300D,(frame_exp_rows>>0)&0xff);

    //printf("NT99143 Frame Rate Preset : (%dfps, 0x%x)\n",*frame_freq, frame_exp_rows);
    *format = SEN_IN_FORMAT_RGRG;
    wrNT99143Reg(0x3060,0x02);

    return;
}



s32 NT99143_set_output_size(u16 *width, u16 *height, u8 *freq)
{
	*width = 1280;
	*height = 720;
	*freq = 30;

	return 0;
}



s32 NT99143_power_ctl(u8 isp_dev, u8 is_work)
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


void NT99143_xclk_set(u8 isp_dev)
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



s32 NT99143_ID_check(void)
{
    u8 pid = 0x00;
    u8 ver = 0x00;
    u8 i ;
    u8 tmp = 0x00;

    rdNT99143Reg(0x3000, &pid);
    puts("NT99143 Sensor PID\n");
    put_u8hex(pid);
      puts("\n");
    rdNT99143Reg(0x3001, &ver);
    puts("NT99143 Sensor VER\n");
    put_u8hex(ver);

    //wrNT99143Reg(0x3060, 0x11);
   // rdNT99143Reg(0x3060, &tmp);

    printf("%x\n",tmp);

	if (pid != 0x14 || ver != 0x14)
	{
	    puts("---!NT99143---\n");
		return -1;
	}

    return 0;
}

void NT99143_reset(u8 isp_dev)
{
    puts("NT99143_reset\n");

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

void NT99143_iic_set(u8 isp_dev)
{
	iic_select(&NT99143_iic_fun, isp_dev);
}

s32 NT99143_check(u8 isp_dev)
{

    if(isp_dev == ISP_DEV_0){
        NT99143_iic_sel = 1;
    }else{
        NT99143_iic_sel = 0;
    }

    if(NT99143_iic_sel)
    {
        extern void  iic_ini(void);
        iic_ini();
    }

	NT99143_xclk_set(isp_dev);
	NT99143_iic_set(isp_dev);
	NT99143_reset(isp_dev);

	if (0 != NT99143_ID_check())
	{
		return -1;
	}

	return 0;
}


s32 NT99143_init(u8 isp_dev, u16 *width, u16 *height, u8 *format, u8 *frame_freq)
{
	puts("\n\n NT99143_init \n\n");

	if (0 != NT99143_check(isp_dev))
	{
		return -1;
	}

	NT99143_config_SENSOR(width, height, format, frame_freq);

	return 0;
}


static s32 cur_ev_level =80;
static s32 aGain_bk = -1;
static s32 dGain_bk = -1;
static s32 exp_time_bk = -1;
static NT99143_AE_lut_t g_ae_level;
static s32 pre_ev_level;
extern NT99143_AE_lut_t NT99143_AE_lut_50Hz[];

static void set_aGain(u32 aGain)
{
    if(aGain==aGain_bk)
        return;
    else
        aGain_bk = aGain;

#if NT99143_NEW_A_GAIN==1
    // 301D改为32CF的计算方法；
    aGain = ((((aGain>>4)&0x1) + ((aGain>>5)&0x1) + ((aGain>>6)&0x1)) << 4) | (aGain & 0xF);
    wrNT99143Reg(0x32CF, aGain);
#else
    wrNT99143Reg(0x301D, aGain);
#endif

}

static void set_shutter(u32 exp_time)
{
    int s = N143_GET_SHUTTER(exp_time);
//printf("shutter:0x%x",s);
    if(exp_time==exp_time_bk)
        return;
    else
        exp_time_bk = exp_time;


    wrNT99143Reg(0x3012, s>>8);
    wrNT99143Reg(0x3013, s&0xff);
}

static void set_dgain(int temp)
{
    if(temp==dGain_bk)
        return;
    else
        dGain_bk = temp;

    wrNT99143Reg(0x301C, temp&0x3);
}

static u32 nt143_isAE_stable = 0;
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

s32 NT99143_update_cfg(u32 cur_mean)
{
    int pre_level = cur_ev_level;

    int target = NT99143_TARGET_LUMA;

    cur_mean = ispt_smt_luma(cur_mean);

    if(abs(cur_mean-target)<=6 && nt143_isAE_stable == 1){
         //printf("%3d ",m_mean);
         goto AE_IS_OK;
    }

    if (cur_mean > target && abs(cur_mean-target)>=4) {
        if(0)  cur_ev_level --; else {
            if(cur_mean - target > 80)
                cur_ev_level -=8;
            else if(cur_mean - target > 40)
                cur_ev_level -=6;
            else if(cur_mean - target > 20)
                cur_ev_level -=4;
            else if(cur_mean - target > 10)
                cur_ev_level -=2;
            else
                cur_ev_level --;

            nt143_isAE_stable = 0;
        }
    }else if (cur_mean < target && abs(cur_mean-target)>=4){
         if(0)  cur_ev_level ++; else {
        if(target - cur_mean > 80)
            cur_ev_level +=8;
        else if(target - cur_mean > 40)
            cur_ev_level +=6;
        else if(target - cur_mean > 20)
            cur_ev_level +=4;
        else if(target - cur_mean > 10)
            cur_ev_level +=2;
        else
            cur_ev_level ++;
         }
         nt143_isAE_stable = 0;
    }else{
        nt143_isAE_stable = 1;
    }

//cur_ev_level = 110;
    if (cur_ev_level < 0)
        cur_ev_level = 0;
    else if (cur_ev_level > NT99143_MAX_EV_LEVEL)
        cur_ev_level = NT99143_MAX_EV_LEVEL;



#if 0
    {

        static int upward=1;
        static int cur_ev_level2 = 0;
        if(upward==1){
            if(cur_ev_level2 < NT99143_MAX_EV_LEVEL-1){
                cur_ev_level2 +=1;
            }else{
                cur_ev_level2 +=1;
                upward=0;
            }
        }
        if(upward==0){
            if(cur_ev_level2 >1){
                cur_ev_level2 -=1;
            }else{
                cur_ev_level2 -=1;
                upward=1;
            }
        }
        cur_ev_level = cur_ev_level2;
    }
#endif

AE_IS_OK:
    g_ae_level = NT99143_AE_lut_50Hz[cur_ev_level];
    //printf("(%3d,%2d,0x%2x,%5d,%2d,%2d)\n",cur_mean, cur_ev_level, g_ae_level.a_gain, g_ae_level.exp_time,g_ae_level.d_gain, NT99143_AE_lut_50Hz[cur_ev_level].ev_level);


    return NT99143_AE_lut_50Hz[cur_ev_level].ev_level;

}

s32 NT99143_flush_cfg()
{
    set_aGain(g_ae_level.a_gain);
    set_dgain(g_ae_level.d_gain);
    set_shutter(g_ae_level.exp_time);
    wrNT99143Reg(0x3060, 0x02);
    return 0;
}

void NT99143_sleep(){;}
void NT99143_wakeup(){;}

void NT99143_W_Reg(u16 addr, u16 val)
{
    printf_without_mutex("update reg%x with %x\n",addr,val);
    wrNT99143Reg(addr,(u8)val);
}
u16 NT99143_R_Reg(u16 addr)
{
    u8 val;
    rdNT99143Reg(addr,&val);
    return val;
}


void reset_99143()
{

nt143_isAE_stable = 0;
pre_smt_luma = 60;
cur_ev_level =80;
aGain_bk = -1;
dGain_bk = -1;
exp_time_bk = -1;
memset(&g_ae_level, 0 ,sizeof(NT99143_AE_lut_t));
pre_ev_level = 0;

}


#endif

