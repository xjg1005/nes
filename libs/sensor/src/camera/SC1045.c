/*******************************************************************************************
 Reis 201510
*******************************************************************************************/
#ifndef SC1045_c
#define  SC1045_c
#include "iic.h"
#include "isp_dev.h"
#include "isp_com.h"
#include "SC1045.h"

S_IIC_FUN SC1045_iic_fun;
static u8 SC1045_iic_sel;

typedef struct
{
    u16 addr;
    u8 value;
}Sensor_reg_ini;

const Sensor_reg_ini SC1045_INI_REG[]=
{
//initial setting

//{0x3003, 0x01}, // soft reset

{0x3000, 0x01}, //pause for reg writing
 //output format 43.2M 1920X750 30fps
//{0x3010, 0x01},
//{0x3011, 0xbe},
//{0x3004, 0x45},

 //output format 36M 1920X750 25fps
{0x3010, 0x31},
{0x3011, 0x46},
{0x3004, 0x44},

 //exp and gain
{0x3e03, 0x03},
{0x3e01, 0x2e},
{0x3e09, 0x00},
{0x3e0f, 0x84},
 //blc rnc
{0x3907, 0x03},
//analog configuration
{0x3600, 0x94}, //0607 reduce power
{0x3610, 0x03},
{0x3634, 0x00}, // reduce power
{0x3620, 0x84},
{0x3631, 0x84}, // txvdd

{0x3622, 0x0e}, //blksun
{0x3633, 0x2c},
{0x3630, 0x88}, //
{0x3635, 0x80}, //sa1_vref

  //pre-charge timing control
{0x3310, 0x83}, //prechg tx auto ctrl [5]
{0x3336, 0x00}, //EXP1  0607
{0x3337, 0x00},
{0x3338, 0x02}, //EXP2
{0x3339, 0xe0},
{0x331E, 0xA0}, // start value 0607
{0x3335, 0x1a}, // end value

   //sample timing
{0x3315, 0x44},
{0x3308, 0x40},
{0x3330, 0x0d}, // sal_en timing,cancel the fpn in low light
{0x3320, 0x05}, //0825
{0x3321, 0x60},
{0x3322, 0x02},
{0x3323, 0xc0},
{0x3303, 0xa0},
{0x3304, 0x60},

  //data output
{0x3d08, 0x00},  //phase
//{0x3640, 0x00}, //pad driver 0-3. longer wire, higher value;


  //0806    vsync gen mode
{0x3d04, 0x04},
{0x3d08, 0x03}, // 0x02 ppk modify on 20151027
      //0825
{0x3310, 0xa3},
{0x331e, 0x56},
{0x3631, 0x88},
{0x3633, 0x2f},
{0x3005, 0xf2},
{0x3416, 0x10},

{0x3000, 0x01}, //recover

};

#define WRCMD 0x60
#define RDCMD 0x61

unsigned char wrSC1045Reg(u16 regID, unsigned char regDat)
{
    if(SC1045_iic_sel)
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

unsigned char rdSC1045Reg(u16 regID, unsigned char *regDat)
{
    if(SC1045_iic_sel)
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

static SC1045_AE_lut_t *ae_lut = SC1045_AE_lut_50Hz_min_30fps;
void set_ae_lut()
{
    ae_lut = SC1045_AE_lut_50Hz_min_30fps;
}


static u32 sc1045_frame_rate=30;

/*************************************************************************************************
    sensor api
*************************************************************************************************/
void SC1045_config_SENSOR(u16 *width, u16 *height, u8 *format, u8 *frame_freq)
{
    u32 i;
    u8 pid = 0x00;
    u32 frame_exp_rows;


    SC1045_set_output_size(width, height, frame_freq);


    for(i=0;i<sizeof(SC1045_INI_REG)/sizeof(Sensor_reg_ini);i++)
    {
        wrSC1045Reg(SC1045_INI_REG[i].addr,SC1045_INI_REG[i].value);
    }
    sc1045_frame_rate = *frame_freq;
    if(sc1045_frame_rate==30){
        //output format 43.2M 1920X750 30fps
        wrSC1045Reg(0x3010,0x01);
        wrSC1045Reg(0x3011,0xbe);
        wrSC1045Reg(0x3004,0x45);
        sc1045_pclk = 43;
        ae_lut = SC1045_AE_lut_50Hz_min_30fps;
        SC1045_MAX_EV_LEVEL = 192;
    }else if(sc1045_frame_rate==25){
         //output format 36M 1920X750 25fps
        wrSC1045Reg(0x3010, 0x31);
        wrSC1045Reg(0x3011,0x46);
        wrSC1045Reg(0x3004,0x44);
        sc1045_pclk = 36;
        ae_lut = SC1045_AE_lut_50Hz_min_25fps;
        SC1045_MAX_EV_LEVEL = 200;
    }

    //ae_lut = SC1045_AE_lut_50Hz_min_25fps;
    //SC1045_MAX_EV_LEVEL = 148;

    frame_exp_rows = 1000000/(SC1045_ROW_TIME_US*sc1045_frame_rate);
    wrSC1045Reg(0x320e,(frame_exp_rows>>8)&0xff); // ppk changing on 20151015
    wrSC1045Reg(0x320f,(frame_exp_rows>>0)&0xff); // ppk changing on 20151015






    printf("SC1045 Frame Rate Preset : (%dfps, 0x%x)\n",sc1045_frame_rate, frame_exp_rows);
   //// *format = SEN_IN_FORMAT_RGRG; // for NT99143
    //*format = SEN_IN_FORMAT_BGBG; // ppk change on 20151027
    *format = SEN_IN_FORMAT_BGBG; // ppk change on 20151027
    	//SEN_IN_FORMAT_GRGR,
	//SEN_IN_FORMAT_GBGB,
	//SEN_IN_FORMAT_BGBG,
   //// wrSC1045Reg(0x3060,0x02); // removed on 20151015 NT99143
      //set_ae_lut(); // ppk add on 20151015
    return;
}



s32 SC1045_set_output_size(u16 *width, u16 *height, u8 *freq)
{
	*width = 1280;
	*height = 720;
	//*freq = 25;   // 25

	return 0;
}



s32 SC1045_power_ctl(u8 isp_dev, u8 is_work)
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


void SC1045_xclk_set(u8 isp_dev)
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



s32 SC1045_ID_check(void)
{

   u8 pid = 0x00;

  #if 1 //5203 JOHNSON 20160509
     u32 i;
//    for (i=0;i < 5; i++)
    {
  #endif
//while(pid != 0x53)
    rdSC1045Reg(0x3400, &pid);// PPK RELEASED ON 20151015
    puts("Sensor PID \n");
    printf("Sensor PID :%x \n",pid);

  #if 0 //old
    puts("\n");

     puts("---ppppkkkk: hello SC1045---\n");
     // PPK RELEASED ON 20151015
  #else //5203 JOHNSON 20160509
    if (pid == 0x53)
        {
            puts("---ppppkkkk: hello SC1045---\n");
            return 0;
        }
    }
  #endif

     #if 1
	if (pid != 0x53) // PPK RELEASED ON 20151015
	{
	    puts("---!SC1045---\n");
		return -1;
			}
			// PPK RELEASED ON 20151015
  #endif
    return 0;


//    u8 pid = 0x00;
//    u8 ver = 0x00;
//    u8 i ;
//    u8 tmp = 0x00;
//
//     for(i = 0;i < 3;i++)//
//    {
//        rdSC1045Reg(0x3107, &pid);
//        rdSC1045Reg(0x3108, &ver);
//    }
//
//
//    puts("SC1045 Sensor PID\n");
//    put_u8hex(pid);
//      puts("\n");
//
//    puts("SC1045 Sensor VER\n");
//    put_u8hex(ver);
//
//    printf("%x\n",tmp);
//
//	if (pid != 0x10 || ver != 0x45)
//	{
//	    puts("---!SC1045---\n");
//		return -1;
//	}
//
//    return 0;

}

void SC1045_reset(u8 isp_dev)
{
    puts("SC1045_reset\n");

    if (isp_dev==ISP_DEV_0)
    {
        SET_RESET_OUT_A();
        SET_RESET_H_A();
        delay(400000);//40000 //解决反复开关机有时不进模组 5203 JOHNSON 20160513
        SET_RESET_L_A();
        delay(400000);//40000
        SET_RESET_H_A();
        delay(400000);//40000
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

puts("---ppppkkkk: ini SC1045---\n");
    //wrSC1045Reg(0x3003, 0x01); // soft reset
}

void SC1045_iic_set(u8 isp_dev)
{
	iic_select(&SC1045_iic_fun, isp_dev);
}

s32 SC1045_check(u8 isp_dev)
{

    if(isp_dev == ISP_DEV_0){
        SC1045_iic_sel = 1;
    }else{
        SC1045_iic_sel = 0;
    }

    if(SC1045_iic_sel)
    {
        extern void  iic_ini(void);
        iic_ini();
    }

	SC1045_xclk_set(isp_dev);
	SC1045_iic_set(isp_dev);
	SC1045_reset(isp_dev);

	if (0 != SC1045_ID_check())
	{
		return -1;
	}

	return 0;
}





static s32 cur_ev_level =80;
static s32 aGain_bk = -1;
static s32 dGain_bk = -1;
static s32 exp_time_bk = -1;
static SC1045_AE_lut_t g_ae_level;
static s32 pre_ev_level;
extern SC1045_AE_lut_t SC1045_AE_lut_50Hz[];

static void set_aGain(u32 aGain)
{


    if(aGain==aGain_bk)
        return;
    else
        aGain_bk = aGain;

    wrSC1045Reg(0x3e09, aGain); // ppk add Again on 20151015

}

static void set_shutter(u32 exp_time)
{
    //int s = SC1045_GET_SHUTTER(exp_time);
    int s= exp_time;

    if(exp_time==exp_time_bk)
        return;
    else
        exp_time_bk = exp_time;


   // if(exp_time==40000 || exp_time==33333)
   //     s-=4;

    //wrSC1045Reg(0x3012, s>>8);
   // wrSC1045Reg(0x3013, s&0xff);
   wrSC1045Reg(0x3e01, s>>4);       // ppk add on 20151015
   wrSC1045Reg(0x3e02, (s<<4)&0xf0); //

  // printf("--- exp : %d\n",s);
}

static void set_dgain(int temp)
{
    if(temp==dGain_bk)
        return;
    else
        dGain_bk = temp;

    wrSC1045Reg(0x3e0f, 0x84|((temp<<5)&0xf0)); // ppk add dgain on 20151015
}

s32 SC1045_init(u8 isp_dev, u16 *width, u16 *height, u8 *format, u8 *frame_freq)
{
	puts("\n\n SC1045_init \n\n");

  #if 0 //5203 JOHNSON 20160509
	if (0 != SC1045_check(isp_dev))
	{
		return -1;
	}
  #endif


	SC1045_config_SENSOR(width, height, format, frame_freq);

	return 0;
}

static u32 sc1045_isAE_stable = 0;
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

s32 SC1045_update_cfg(u32 cur_mean)
{
    int pre_level = cur_ev_level;

    int target = SC1045_TARGET_LUMA;
    int converge_th0,converge_th1;

    if(cur_ev_level < 15) {converge_th0=10;converge_th1=8;}
    else                  {converge_th0= 6;converge_th1=4;}

    cur_mean = ispt_smt_luma(cur_mean);



    if(abs(cur_mean-target)<=converge_th0 && sc1045_isAE_stable == 1){
         goto AE_IS_OK;
    }

    if (cur_mean > target && abs(cur_mean-target)>=converge_th1) {
        if(cur_ev_level<15)  cur_ev_level --; else {
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

            sc1045_isAE_stable = 0;
        }
    }else if (cur_mean < target && abs(cur_mean-target)>=4){
         if(cur_ev_level<15)  cur_ev_level ++; else {
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
         sc1045_isAE_stable = 0;
    }else{
        sc1045_isAE_stable = 1;
    }

    if (cur_ev_level < 0)
        cur_ev_level = 0;
    else if (cur_ev_level > SC1045_MAX_EV_LEVEL)
        cur_ev_level = SC1045_MAX_EV_LEVEL;

AE_IS_OK:
    g_ae_level = ae_lut[cur_ev_level];
    printf("(%3d,%2d,0x%2x,%5d,%2d,%2d)\n",cur_mean, cur_ev_level, g_ae_level.a_gain, g_ae_level.exp_time,g_ae_level.d_gain, ae_lut[cur_ev_level].ev_level);


    if(1){
        u32 frame_exp_rows = 1000000/(SC1045_ROW_TIME_US*sc1045_frame_rate);

              if(g_ae_level.exp_time>=55555 && sc1045_frame_rate>18){
            frame_exp_rows = 1000000/(SC1045_ROW_TIME_US*18);
        }else if(g_ae_level.exp_time>=50000 && sc1045_frame_rate>20){
            frame_exp_rows = 1000000/(SC1045_ROW_TIME_US*20);
        }else if(g_ae_level.exp_time>=40000 && sc1045_frame_rate>25){
            frame_exp_rows = 1000000/(SC1045_ROW_TIME_US*25);
        }else if(g_ae_level.exp_time>=30000 && sc1045_frame_rate>30){
            frame_exp_rows = 1000000/(SC1045_ROW_TIME_US*30);
        }

        frame_exp_rows+=4; //sc1045 bug

        wrSC1045Reg(0x320e,(frame_exp_rows>>8)&0xff); // ppk changing on 20151015
        wrSC1045Reg(0x320f,(frame_exp_rows>>0)&0xff); // ppk changing on 20151015
    }

    return ae_lut[cur_ev_level].ev_level;  // ppk on 20151015using NT99143 tbl for AE&banding
}

s32 SC1045_flush_cfg()
{
    set_aGain(g_ae_level.a_gain);
    set_dgain(g_ae_level.d_gain);
    set_shutter(g_ae_level.exp_time);

    return 0;
}

void SC1045_sleep(){;}
void SC1045_wakeup(){;}

void SC1045_W_Reg(u16 addr, u16 val)
{
    printf("update reg%x with %x\n",addr,val);
    wrSC1045Reg(addr,(u8)val);
}
u16 SC1045_R_Reg(u16 addr)
{
    u8 val;
    printf("read reg%x with %x\n",addr,val);
    rdSC1045Reg(addr,&val);
    return val;
}


void reset_sc1045()
{
sc1045_isAE_stable = 0;
pre_smt_luma = 60;

cur_ev_level =80;
aGain_bk = -1;
dGain_bk = -1;
exp_time_bk = -1;
memset(&g_ae_level, 0, sizeof(SC1045_AE_lut_t));
pre_ev_level = 0;

ae_lut = SC1045_AE_lut_50Hz_min_30fps;


sc1045_frame_rate=30;

}

#endif

