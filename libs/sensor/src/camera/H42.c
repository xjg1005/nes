
/*******************************************************************************************
   reis 201508
*******************************************************************************************/
#ifndef H42_c
#define  H42_c
#include "iic.h"
#include "isp_dev.h"
#include "isp_com.h"
#include "H42.h"

#define H42_IIC_R_PORT 0x61
#define H42_IIC_W_PORT 0x60

#include "h42_tbl.c"

// ===============================
// default config
// ===============================
#define H42_MCLK                       24
#define H42_PCLK_DIV2_MINUS1           0x9  //  PLL2, PLL Clock divider
#define H42_PCLK_DIV1_MINUS1           0x1  // PLL_pre_Ratio
#define H42_PCLK_MUL                   0x1E // PLL3, VCO mulitplier

#define H42_PCLK                       ((H42_MCLK*H42_PCLK_MUL/(1+H42_PCLK_DIV1_MINUS1))/(H42_PCLK_DIV2_MINUS1+1))
#define H42_CLK_PER_ROW                0x640//0x640
#define H42_ROW_TIME_US                (H42_CLK_PER_ROW / H42_PCLK)   //us
#define H42_GET_SHUTTER(exp_time)      (exp_time*(H42_PCLK))/(H42_CLK_PER_ROW)

#define H42_DEFAULT_FPS_FACTOR          0x2EE
// ===============================
// ===============================

typedef struct
{
    u8 addr;
    u8 value;
} Sensor_reg_ini;

Sensor_reg_ini H42_INI_REG[]=
{
//[JXH42(CM)_1280x720x30_Fx3_DVP_10b.reg]
//;;INI Start
{0x12,0x40},
//;;DVP Setting
//{0x0C,0x01}, // test pattern
{0x0D,0x40},
{0x1F,0x04},//{0x1F,0x06},//{0x1F,0x04},
//;;PLL Setting
{0x0E,0x1c|H42_PCLK_DIV1_MINUS1},
{0x0F,H42_PCLK_DIV2_MINUS1},
{0x10,H42_PCLK_MUL},
{0x11,0x80},
//;;Frame/Window
{0x20,H42_CLK_PER_ROW&0xff},
{0x21,H42_CLK_PER_ROW>>8},
{0x22,H42_DEFAULT_FPS_FACTOR&0xff}, //降帧率，则提高FrameHeightTime
{0x23,H42_DEFAULT_FPS_FACTOR>>8},
{0x24,H42_OUTPUT_W&0xff},
{0x25,H42_OUTPUT_H&0xff},
{0x26,((H42_OUTPUT_H&0xf00)>>4) | ((H42_OUTPUT_W>>8)&0x0f)},
//{0x24,0x00},
//{0x25,0xD0},
//{0x26,0x25},
{0x27,0x45},//{0x27,0x34},//{0x27,0x45},
{0x28,0x0D},
{0x29,0x01},
{0x2A,0x24},
{0x2B,0x29},
{0x2C,0x00},
{0x2D,0x00},
{0x2E,0xB9},
{0x2F,0x00},
//;;Sensor Timing
{0x30,0x92},
{0x31,0x0A},
{0x32,0xAA},
{0x33,0x14},
{0x34,0x38},
{0x35,0x54},
{0x42,0x41},
{0x43,0x50},
//;;Interface
{0x1D,0xFF},
{0x1E,0x1F},
{0x6C,0x90},
{0x73,0xB3},
{0x70,0x68},
{0x76,0x40},
{0x77,0x06},
//;;Array/AnADC/PWC
{0x48,0x40},
{0x60,0xA4},
{0x61,0xFF},
{0x62,0x40},
{0x65,0x00},
{0x66,0x20},
{0x67,0x30},
{0x68,0x04},
{0x69,0x74},
{0x6F,0x04},
//;0x;Black Sun
{0x63,0x19},
{0x6A,0x09},
//;;AE/AG/ABLC
{0x13,0x87}, // 0:
{0x14,0x80},
{0x16,0xC0},
{0x17,0x40},
{0x18,0xC2},
{0x19,0x20}, //added to avoid gain & shutter aysnc
{0x38,0x35},
{0x39,0x98},
{0x4a,0x03},
{0x49,0x00},//{0x49,0x10}, //OB
//;;INI End
{0x12,0x00},
//;;PWDN Setting
};

S_IIC_FUN H42_iic_fun;
static u8 H42_iic_sel;

static void my_delay(u32 clocks)
{
    return;
    delay(clocks);
}

unsigned char wrH42Reg(unsigned char regID, unsigned char regDat)
{
    if(H42_iic_sel)
    {
        iic_tx(H42_IIC_W_PORT);
        delay(10);
        iic_tx(regID);
        delay(10);
        iic_tx_we(regDat);
        delay(10);
        return 1 ;
    }

	H42_iic_fun.startSCCB();
	if(0 == H42_iic_fun.SCCBwriteByte(H42_IIC_W_PORT))
	{
//		puts("a");
		H42_iic_fun.stopSCCB();
		return(0);
	}
	my_delay(100);
  	if(0 == H42_iic_fun.SCCBwriteByte(regID))
	{
//		puts("b");
		H42_iic_fun.stopSCCB();
		return(0);
	}
	my_delay(100);
  	if(0 == H42_iic_fun.SCCBwriteByte(regDat))
	{
//		puts("c");
		H42_iic_fun.stopSCCB();
		return(0);
	}
  	H42_iic_fun.stopSCCB();

  	return(1);
}

unsigned char rdH42Reg(unsigned char regID, unsigned char *regDat)
{
    if(H42_iic_sel)
    {
        iic_tx(H42_IIC_W_PORT);
        delay(10);
        iic_tx_we(regID);
        delay(10);

        iic_tx(H42_IIC_R_PORT);
        delay(10);
        *regDat = iic_rx_we();

        return *regDat ;
    }

	//¨a?§1yD??2¨′?á??¨|¨¨??????????|ì???¤
	H42_iic_fun.startSCCB();
	if(0 == H42_iic_fun.SCCBwriteByte(H42_IIC_W_PORT))
	{
//		puts("1");
		H42_iic_fun.stopSCCB();
		return(0);
	}
	my_delay(100);
  	if(0 == H42_iic_fun.SCCBwriteByte(regID))
	{
//		puts("2");
		H42_iic_fun.stopSCCB();
		return(0);
	}
	H42_iic_fun.stopSCCB();
	my_delay(100);

	//¨|¨¨??????????|ì???¤o¨??ê?2?¨o??¨￠
	H42_iic_fun.startSCCB();
	if(0 == H42_iic_fun.SCCBwriteByte(H42_IIC_R_PORT))
	{
//		puts("3");
		H42_iic_fun.stopSCCB();
		return(0);
	}
	my_delay(100);
  	*regDat = H42_iic_fun.SCCBreadByte();
  	H42_iic_fun.noAck();
  	H42_iic_fun.stopSCCB();

  	return(1);
}


/*************************************************************************************************
    sensor api
*************************************************************************************************/
static u32 h42_pclk;
static u32 h42_row_time_us;
static u32 h42_pre_pll_ratio;

static H42_AE_lut_t *ae_lut = H42_AE_lut_50Hz_min_33fps;

static u32 h42_fps = 30;
static Sensor_AE_BF_TYPE h42_BF_type = AE_BF_50HZ;
static h42_ae_lut_size = 0;

void H42_set_ae_bf_type(Sensor_AE_BF_TYPE ae_t)
{
    if(ae_t == AE_BF_50HZ || ae_t == AE_BF_60HZ) {
        h42_BF_type = ae_t;
    }
    /*
    if(ae_t == AE_BF_50HZ) {
       // ae_lut = H42_AE_lut_min_30fps;
         //ae_lut = H42_AE_lut_50Hz_min_33fps;
    ae_lut = H42_AE_lut_50Hz_min_25fps;
    //ae_lut = H42_AE_lut_50Hz_min_20fps;
    //ae_lut = H42_AE_lut_50Hz_min_16fps;
        puts("\n------- Anti 50hz banding filter\n");
    }else if(ae_t == AE_BF_60HZ) {
        ae_lut = H42_AE_lut_60Hz_min_30fps;

    }else{
        ae_lut = H42_AE_lut_min_30fps;
    }
    */
}


static u32 h42_get_shutter(u32 exp_time_us)
{
    //return H42_GET_SHUTTER(exp_time_us);
    return exp_time_us*4*h42_pclk/H42_CLK_PER_ROW;
}

void H42_config_SENSOR(u16 *width, u16 *height, u8 *format, u8 *frame_freq)
{
    u32 i;
    u32 frame_exp_rows;
    u8 tmp;


    H42_set_output_size(width, height, frame_freq);

    for(i=0;i<sizeof(H42_INI_REG)/sizeof(Sensor_reg_ini);i++)
    {
        wrH42Reg(H42_INI_REG[i].addr,H42_INI_REG[i].value);
    }

    if((*frame_freq)==60){
        h42_pre_pll_ratio = 0;
        h42_fps = 60;
        ae_lut = H42_AE_lut_min_30fps;
        h42_ae_lut_size = sizeof(H42_AE_lut_min_30fps)/sizeof(H42_AE_lut_t);
    }else if((*frame_freq)==30){
        h42_pre_pll_ratio = 1;
        h42_fps = 30;
        ae_lut = H42_AE_lut_min_30fps;
        h42_ae_lut_size = sizeof(H42_AE_lut_min_30fps)/sizeof(H42_AE_lut_t);
    }else if((*frame_freq)==50){
        h42_pre_pll_ratio = 0;
        h42_fps = 50;
        ae_lut = H42_AE_lut_50Hz_min_25fps;
        h42_ae_lut_size = sizeof(H42_AE_lut_50Hz_min_25fps)/sizeof(H42_AE_lut_t);
    }else {
        h42_pre_pll_ratio = 1;
        h42_fps = 25;
        ae_lut = H42_AE_lut_50Hz_min_25fps;
        h42_ae_lut_size = sizeof(H42_AE_lut_50Hz_min_25fps)/sizeof(H42_AE_lut_t);
    }


    h42_pclk = ((H42_MCLK*H42_PCLK_MUL/(1+h42_pre_pll_ratio))/(H42_PCLK_DIV2_MINUS1+1));
    h42_row_time_us = (H42_CLK_PER_ROW / h42_pclk);
    frame_exp_rows = (1000000*h42_pclk)/(H42_CLK_PER_ROW*(*frame_freq));
    wrH42Reg(0x0e,0x1c|h42_pre_pll_ratio);
    wrH42Reg(0x22,(frame_exp_rows>>0)&0xff);
    wrH42Reg(0x23,(frame_exp_rows>>8)&0xff);

    printf("H42 Frame Rate Preset : (%dfps, 0x%x)\n",*frame_freq, frame_exp_rows);

    // h42 bug;
    rdH42Reg(0x09,&tmp);
    if(tmp==0x81){
        wrH42Reg(0x27,0x3b);
        wrH42Reg(0x0c,0x04);
        wrH42Reg(0x48,0x60);
        wrH42Reg(0x63,0x51);
    }else if(tmp==0x00 || tmp==0x80 ){
        wrH42Reg(0x27,0x45);
        wrH42Reg(0x0c,0x00);
        wrH42Reg(0x48,0x40);
        wrH42Reg(0x63,0x19);
    }

    wrH42Reg(0x12, 0x00);



  	*format = SEN_IN_FORMAT_BGBG;
    puts("\n----hello H42 222 -----\n");
	//H42_set_ae_bf_type();
    return;
}


s32 H42_set_output_size(u16 *width, u16 *height, u8 *frame_freq)
{
	*width = H42_OUTPUT_W;//640;
	*height = H42_OUTPUT_H;//480;

	//*frame_freq = 30;//30;

	return 0;
}


s32 H42_power_ctl(u8 isp_dev, u8 is_work)
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


void H42_xclk_set(u8 isp_dev)
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



s32 H42_ID_check(void)
{
    u8 pid = 0x00;
    u8 ver = 0x00;
    u8 i ;

    for(i = 0;i < 3;i++)//
    {
        rdH42Reg(0x0A,&pid);
        rdH42Reg(0x0B,&ver);
    }

    puts("Sensor PID \n");
    put_u8hex(pid);
    put_u8hex(ver);
    puts("\n");

	if (pid != 0xA0 || ver != 0x42)
	{
	    puts("\n----not H42-----\n");
		return -1;
	}
    puts("\n----hello H42-----\n");
    return 0;
}

void H42_reset(u8 isp_dev)
{
    //printf("reset:%d\n",isp_dev);

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

    wrH42Reg(0x12, 0x80);
}

void H42_iic_set(u8 isp_dev)
{
	iic_select(&H42_iic_fun, isp_dev);
}

s32 H42_check(u8 isp_dev)
{
    if(isp_dev == ISP_DEV_0){
        H42_iic_sel = 1;
    }else{
        H42_iic_sel = 0;
    }

    if(H42_iic_sel)
    {
        extern void  iic_ini(void);
        iic_ini();
    }

	H42_xclk_set(isp_dev);
	H42_iic_set(isp_dev);
	H42_reset(isp_dev);

	if (0 != H42_ID_check())
	{
		return -1;
	}

	return 0;
}


s32 H42_init(u8 isp_dev, u16 *width, u16 *height, u8 *format, u8 *frame_freq)
{
	puts("\n\n H42_init 22 \n\n");


	if (0 != H42_check(isp_dev))
	{
		return -1;
	}

	H42_config_SENSOR(width, height, format, frame_freq);

	return 0;
}




static s32 shutter_gain_async_flag=0;
static s32 cur_ev_level =80;
static s32 aGain_bk = -1;
static s32 dGain_bk = -1;
static s32 exp_time_bk = -1;
static H42_AE_lut_t g_ae_level;
static s32 pre_ev_level;
extern H42_AE_lut_t H42_AE_lut_50Hz[];

static void set_aGain(u32 aGain)
{
    if(aGain==aGain_bk)
        return;
    else
        aGain_bk = aGain;

    wrH42Reg(0xC0, 0x00);
    wrH42Reg(0xC1, aGain);

}

static void set_shutter(u32 exp_time)
{
    int s = h42_get_shutter(exp_time);
//printf("shutter:0x%x",s);
    if(exp_time==exp_time_bk)
        return;
    else
        exp_time_bk = exp_time;

    wrH42Reg(0xC2, 0x01);
    wrH42Reg(0xC3, s&0xff);
    wrH42Reg(0xC4, 0x02);
    wrH42Reg(0xC5, s>>8);

}

static void set_dgain(int temp)
{
    if(temp==dGain_bk)
        return;
    else
        dGain_bk = temp;

    wrH42Reg(0xC6, 0x0d);
    wrH42Reg(0xC7, 0x40|temp);

}

//平滑统计亮度，在快速光照变化的场景中，缓解AE震荡
static u32 h42_isAE_stable = 0;
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





s32 H42_update_cfg(u32 cur_mean)
{
    int pre_level = cur_ev_level;

    int target = H42_TARGET_LUMA;

    cur_mean = ispt_smt_luma(cur_mean);

    if(ae_lut == 0){
        return 0;
    }

    if(abs(cur_mean-target)<= 6 && (h42_isAE_stable == 1)) {
        goto H42_AE_OK;
    }




    if (cur_mean > target && cur_mean-target>=4) {
        if(0) cur_ev_level --;
        else {

        if(cur_mean - target > 40)
            cur_ev_level -=6;
        else

        if(cur_mean - target > 20)
            cur_ev_level -=4;
        else if(cur_mean - target > 10)
            cur_ev_level -=2;
        else
            cur_ev_level --;
        }

        h42_isAE_stable = 0;
    }else if (cur_mean < target && target-cur_mean>=4){
        if(0) cur_ev_level ++;
        else {

        if(target - cur_mean > 40)
            cur_ev_level +=6;
        else
        if(target - cur_mean > 20)
            cur_ev_level +=4;
        else if(target - cur_mean > 10)
            cur_ev_level +=2;
        else
            cur_ev_level ++;
        }

        h42_isAE_stable = 0;
    }else{
        h42_isAE_stable = 1;
    }
//cur_ev_level = 90;

    if (cur_ev_level < 0)
        cur_ev_level = 0;
    else if (cur_ev_level >= h42_ae_lut_size)
        cur_ev_level = h42_ae_lut_size-1;



#if 0
    {

        static int upward=1;
        static int cur_ev_level2 = 0;
        if(upward==1){
            if(cur_ev_level2 < h42_ae_lut_size-1){
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

    g_ae_level = ae_lut[cur_ev_level];

    //g_ae_level.exp_time = 10000;
    //printf("(%3d,%2d,0x%2x,%5d,%2d,%2d)\n",cur_mean, cur_ev_level, g_ae_level.a_gain, g_ae_level.exp_time,g_ae_level.d_gain, ae_lut[cur_ev_level].ev_level);

    if(h42_pre_pll_ratio==1) {

        if(h42_fps==30) {
            if(g_ae_level.exp_time*4>33332){
                u32 frame_exp_rows = (g_ae_level.exp_time*4*h42_pclk)/(H42_CLK_PER_ROW);
                wrH42Reg(0x22,(frame_exp_rows>>0)&0xff);
                wrH42Reg(0x23,(frame_exp_rows>>8)&0xff);
            }else{
                wrH42Reg(0x23,0x2ee>>8);
                wrH42Reg(0x22,0x2ee&0xff);
            }
        }
        if(h42_fps==25) {
            if(g_ae_level.exp_time*4>40000){
                u32 frame_exp_rows = (g_ae_level.exp_time*4*h42_pclk)/(H42_CLK_PER_ROW);
                wrH42Reg(0x22,(frame_exp_rows>>0)&0xff);
                wrH42Reg(0x23,(frame_exp_rows>>8)&0xff);
            }else{
                wrH42Reg(0x23,0x384>>8);
                wrH42Reg(0x22,0x384&0xff);
            }
        }

    }else{
        if(h42_fps==60) {
            if(g_ae_level.exp_time*4<16667){
                wrH42Reg(0x23,0x2ee>>8);
                wrH42Reg(0x22,0x2ee&0xff);
            }else{
                u32 frame_exp_rows = (g_ae_level.exp_time*4*h42_pclk)/(H42_CLK_PER_ROW);
                wrH42Reg(0x22,(frame_exp_rows>>0)&0xff);
                wrH42Reg(0x23,(frame_exp_rows>>8)&0xff);
            }
        }
        if(h42_fps==50) {
            if(g_ae_level.exp_time*4<20000){
                wrH42Reg(0x23,0x384>>8);
                wrH42Reg(0x22,0x384&0xff);
            }else{
                u32 frame_exp_rows = (g_ae_level.exp_time*4*h42_pclk)/(H42_CLK_PER_ROW);
                wrH42Reg(0x22,(frame_exp_rows>>0)&0xff);
                wrH42Reg(0x23,(frame_exp_rows>>8)&0xff);
            }
        }
    }

    // H42 bug
    if(0){//(g_ae_level.a_gain!=aGain_bk && g_ae_level.exp_time!=exp_time_bk){
        set_dgain(g_ae_level.d_gain);
        set_shutter(g_ae_level.exp_time);
        shutter_gain_async_flag = 1;
    }else{
        set_aGain(g_ae_level.a_gain);
        set_dgain(g_ae_level.d_gain);
        set_shutter(g_ae_level.exp_time);
        shutter_gain_async_flag = 0;
    }

H42_AE_OK:
    return ae_lut[cur_ev_level].ev_level;
}


s32 H42_flush_cfg()
{
    u32 r;
    if(shutter_gain_async_flag==1){
        shutter_gain_async_flag=2;
        wrH42Reg(0x12, 0x8);
        return 2;
    }else if(shutter_gain_async_flag==2){
        shutter_gain_async_flag=0;
        set_aGain(g_ae_level.a_gain);
        wrH42Reg(0x12, 0x8);
        return 0;
    }else{
        wrH42Reg(0x12, 0x8);
        return 0;
    }
}



void H42_sleep()
{
}

void H42_wakeup()
{
}

void H42_W_Reg(u16 addr, u16 val)
{
    wrH42Reg((u8)addr,(u8)val);
}
u16 H42_R_Reg(u16 addr)
{
    u8 val;
    rdH42Reg((u8)addr,&val);
    return val;
}
#endif




void reset_H42()
{
    //平滑统计亮度，在快速光照变化的场景中，缓解AE震荡
h42_isAE_stable = 0;
pre_smt_luma = 60;

shutter_gain_async_flag=0;
cur_ev_level =80;
aGain_bk = -1;
dGain_bk = -1;
exp_time_bk = -1;
memset(&g_ae_level, 0, sizeof(H42_AE_lut_t));
pre_ev_level = 0;
h42_pclk = 0;
h42_row_time_us = 0;
h42_pre_pll_ratio = 0;

ae_lut = H42_AE_lut_50Hz_min_33fps;

}


