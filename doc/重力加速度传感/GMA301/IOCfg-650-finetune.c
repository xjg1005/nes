/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       IOCfg.c
    @ingroup    mIPRJAPCommonIO

    @brief      IO config module
                This file is the IO config module

    @note       Nothing.

    @date       2005/12/24
*/

/** \addtogroup mIPRJAPCommonIO */
//@{

#include "Type.h"
#include "DrvExt.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include "debug.h"
#include "IOCfg.h"
#include "Utility.h"
#include "Pll.h"
#include "IOInit.h"
//#include "Timer.h"
#include "GSensor.h"
#include "Gpio_i2c.h"
#include "i2c.h"


///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#if (USE_VIO == ENABLE)
UINT32 Virtual_IO[VIO_MAX_ID] = {0};
UINT32 vio_getpin(UINT32 id){if(id>=VIO_MAX_ID)return 0; return Virtual_IO[id];}
void vio_setpin(UINT32 id, UINT32 v){if(id>=VIO_MAX_ID)return; Virtual_IO[id] = v;}
#endif

#define GPIO_SET_NONE           0xffffff
#define GPIO_SET_OUTPUT_LOW     0x0
#define GPIO_SET_OUTPUT_HI      0x1

#define GSENSOR_TYPE_NONE    0XFF
#define GSENSOR_TYPE_DMTARD07   0x0
#define GSENSOR_TYPE_MM3A311     0x1

#define GSENSOR_TPYE   GSENSOR_TYPE_DMTARD07 

#if(GSENSOR_TPYE==GSENSOR_TYPE_MM3A311)
////for mm3a310_da311 IC.
enum{
    DA311_PAGE_NO = 0x00,    
	DA311_OSC_REG,    
	DA311_TEST_REG1,    
	DA311_TEST_REG2,    
	DA311_OTP_PG,    
	DA311_OTP_PTM,    
	DA311_LDO_REG,    
	DA311_TEMP_OUT_L = 0x0d,    
	DA311_TEMP_OUT_H,    
	DA311_WHO_AM_I,    
	DA311_OVRN_DURATION = 0x1e,    
	DA311_TEMP_CFG_REG,
	DA311_CTRL_REG1,    
	DA311_CTRL_REG2,    
	DA311_CTRL_REG3,    
	DA311_CTRL_REG4,    
	DA311_CTRL_REG5,    
	DA311_CTRL_REG6,    
	DA311_REFERENCE,    
	DA311_STATUS_REG,    
	DA311_OUT_X_L,    
	DA311_OUT_X_H,    
	DA311_OUT_Y_L,    
	DA311_OUT_Y_H,    
	DA311_OUT_Z_L,    
	DA311_OUT_Z_H,    
	DA311_FIFO_CTRL_REG,    
	DA311_FIFO_SRC,        
	DA311_INT1_CFG,    
	DA311_INT1_SRC,    
	DA311_INT1_THS,    
	DA311_INT1_DURATION,    
	DA311_INT2_CFG,    
	DA311_INT2_SRC,    
	DA311_INT2_THS,    
	DA311_INT2_DURATION,    
	DA311_CLICK_CFG,    
	DA311_CLICK_SRC,    
	DA311_CLICK_THS,    
	DA311_TIME_LIMIT,    
	DA311_TIME_LATENCY,    
	DA311_TIME_WINDOW = 0x3d,
#ifndef RELEASE_VER /* reserved */    
	DA311_SOFT_RESET = 0x0105,    
	DA311_OTP_FLAG = 0x0109,    
	DA311_OTP_XOFF_L = 0x0110,    
	DA311_OTP_XOFF_H,    
	DA311_OTP_YOFF_L,    
	DA311_OTP_YOFF_H,    
	DA311_OTP_ZOFF_L,    
	DA311_OTP_ZOFF_H,    
	DA311_OTP_XSO,    
	DA311_OTP_YSO,    
	DA311_OTP_ZSO,    
	DA311_OTP_TRIM_THERM_L,    
	DA311_OTP_TRIM_THERM_H,    
	DA311_OTP_TRIM_OSC,    
	DA311_LPF_ABSOLUTE,    
	DA311_LPF_COEF_A1_L,    
	DA311_LPF_COEF_A1_H,    
	DA311_LPF_COEF_A2_L,    
	DA311_LPF_COEF_A2_H,    
	DA311_LPF_COEF_B0_L,    
	DA311_LPF_COEF_B0_H,    
	DA311_LPF_COEF_B1_L,    
	DA311_LPF_COEF_B1_H,    
	DA311_LPF_COEF_B2_L,    
	DA311_LPF_COEF_B2_H,        
	DA311_TEMP_OFF1,    
	DA311_TEMP_OFF2,    
	DA311_TEMP_OFF3,    
	DA311_OTP_SO_COEFF = 0x012a
#endif /* RELEASE_VER */
};
enum {
    MODE_2G = 0x00,
    MODE_4G = 0x10,
    MODE_8G = 0x20,
    MODE_16G = 0x30,
};
#endif
GPIO_INIT_OBJ uiGPIOMapInitTab[] = {
    //CARD
    //{  GPIO_CARD_POWER,         GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,   GPIO_SET_NONE         },
    {  GPIO_CARD_DETECT,        GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_CARD_DETECT       },
    {  GPIO_CARD_WP,            GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_CARD_WP           },
    //LCD
    {  GPIO_LCD_BLG_PCTL,       GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_LOW,  PAD_LCD_BLG_PCTL      },
    //LED
    {  GPIO_GREEN_LED,          GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,  GPIO_SET_NONE         },
    {  GPIO_KEY_SHUTTER2,          GPIO_DIR_INPUT,    PAD_PULLUP,  PAD_KEY_SHUTTER2             },
    {  GPIO_TV_PLUG,           GPIO_DIR_INPUT,      PAD_PULLDOWN,           PAD_TV_PLUG           },
   // {  GPIO_KEY_MODE,           GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_MODE          },
   //Gsensor
   {  GPIO_GS_INT1,      GPIO_DIR_INPUT,    PAD_PULLUP,   PAD_GS_INT1      },  
    //Sensor
    {  GPIO_SENSOR_PWM1,        GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_LOW,  PAD_PIN_NOT_EXIST     },
    {  GPIO_SENSOR_PWM2,        GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_LOW,  PAD_PIN_NOT_EXIST     },
    {  GPIO_SENSOR_RESET,       GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,   PAD_PIN_NOT_EXIST     },
    {  GPIO_WHITE_LIGHT,      GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_LOW,   PAD_WHITE_LIGHT      },    
#if 0
     {  GPIO_KEY_ZOOMOUT,       GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_ZOOMOUT       },
     {  GPIO_KEY_ZOOMIN,        GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_ZOOMIN       },
     {  GPIO_KEY_LEFT,          GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_LEFT       },
     {  GPIO_KEY_ENTER,         GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_ENTER       },
     {  GPIO_KEY_UP,            GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_UP       },
     {  GPIO_KEY_RIGHT,         GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_RIGHT       },
     {  GPIO_KEY_PLAYBACK,      GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_PLAYBACK       },
     {  GPIO_KEY_DOWN,          GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_DOWN       },
     {  GPIO_KEY_MENU,          GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_MENU       },

     //#NT#2012/07/25#Isiah Chang -begin
     //#NT#Added GPIO map for Lens&motor driver.
     {  GPIO_LENS_ZOOM_PI,      GPIO_DIR_INPUT,     PAD_PULLDOWN,         PAD_LENS_ZOOM_PI       },
     {  GPIO_LENS_ZOOM_PR,      GPIO_DIR_INPUT,     PAD_PULLDOWN,         PAD_LENS_ZOOM_PR       },
     {  GPIO_LENS_FOCUS_PI,     GPIO_DIR_INPUT,     PAD_PULLDOWN,         PAD_LENS_FOCUS_PI      },

     {  GPIO_LENS_IN1A,         GPIO_DIR_OUTPUT,    PAD_NONE,             PAD_PIN_NOT_EXIST      },
     {  GPIO_LENS_IN1B,         GPIO_DIR_OUTPUT,    PAD_NONE,             PAD_PIN_NOT_EXIST      },
     {  GPIO_LENS_IN2A,         GPIO_DIR_OUTPUT,    PAD_NONE,             PAD_PIN_NOT_EXIST      },
     {  GPIO_LENS_IN2B,         GPIO_DIR_OUTPUT,    PAD_NONE,             PAD_PIN_NOT_EXIST      },
     {  GPIO_LENS_IN3A,         GPIO_DIR_OUTPUT,    PAD_NONE,             PAD_PIN_NOT_EXIST      },
     {  GPIO_LENS_IN3B,         GPIO_DIR_OUTPUT,    PAD_NONE,             PAD_PIN_NOT_EXIST      },
     {  GPIO_LENS_IN4A,         GPIO_DIR_OUTPUT,    PAD_NONE,             PAD_PIN_NOT_EXIST      },
     {  GPIO_LENS_IN4B,         GPIO_DIR_OUTPUT,    PAD_NONE,             PAD_PIN_NOT_EXIST      },
     //#NT#2012/07/25#Isiah Chang -end
#endif
};

UINT32 totalGpioInitCount = sizeof(uiGPIOMapInitTab)/sizeof((uiGPIOMapInitTab)[0]);

void IO_InitGensor(void);

#include "rtc.h"

//should be call after rtc_open()
void IO_GetPowerSrc(void)
{
    UINT32 pwrlost, pwsrc;
    pwrlost = rtc_isPowerLost();
    if(pwrlost)
    {
        DBG_DUMP("^GPowerOn Pwr Lost!\r\n"); //"firs time power-on" or "lost power of Gold capacitor"

        //should notify user to configure current date-time!
    }

    pwsrc = rtc_getPWROnSource();
    if(pwsrc == RTC_PWRON_SRC_PWR_SW)
    {
        DBG_DUMP("^GPowerOn Src = PWR key\r\n");
    }
    else if(pwsrc == RTC_PWRON_SRC_PWR_SW2)
    {
        DBG_DUMP("^GPowerOn Src = PB Key\r\n");
    }
    else if(pwsrc == RTC_PWRON_SRC_PWR_SW3)
    {
        DBG_DUMP("^GPowerOn Src = USB plug\r\n");
    }
    else if(pwsrc == RTC_PWRON_SRC_PWR_SW4)
    {
        DBG_DUMP("^GPowerOn Src = DC plug\r\n");
    }
    else if(pwsrc == RTC_PWRON_SRC_PWR_ALM)
    {
        DBG_DUMP("^GPowerOn Src = PWR alarm\r\n");
    }
}

/**
  Do GPIO initialization

  Initialize input/output pins, and pin status

  @param void
  @return void
*/
void IO_InitGPIO(void)
{
    UINT32 iSValue;

    DBG_IND("GPIO START\r\n");
    //--------------------------------------------------------------------
    // Open GPIO driver
    //--------------------------------------------------------------------
    #if 1 //_MIPS_TODO
    gpio_open();
    for(iSValue=0 ; iSValue<totalGpioInitCount ; iSValue++)
    {
        if (uiGPIOMapInitTab[iSValue].GpioDir == GPIO_DIR_INPUT)
        {
            gpio_setDir(uiGPIOMapInitTab[iSValue].GpioPin, GPIO_DIR_INPUT);
            pad_setPullUpDown(uiGPIOMapInitTab[iSValue].PadPin, uiGPIOMapInitTab[iSValue].PadDir);
        }
        else
        {
            gpio_setDir(uiGPIOMapInitTab[iSValue].GpioPin, GPIO_DIR_OUTPUT);
            if (uiGPIOMapInitTab[iSValue].PadDir == GPIO_SET_OUTPUT_HI)
            {
                gpio_setPin(uiGPIOMapInitTab[iSValue].GpioPin);
            }
            else
            {
                gpio_clearPin(uiGPIOMapInitTab[iSValue].GpioPin);
            }
        }
    }
    #endif

    //--------------------------------------------------------------------
    // Use Non-Used PWM to be Delay Timer
    //--------------------------------------------------------------------
    #if defined(PWMID_TIMER)
    Delay_setPwmChannels(PWMID_TIMER);
    #endif

    DBG_IND("GPIO END\r\n");

}


/**
  Initialize Gsensor IIC bus

  Initialize Gsensor IIC bus

  @param void
  @return void
*/
#if(GSENSOR_TPYE==GSENSOR_TYPE_DMTARD07)	
        /*
        GSensor_I2C_ReadReg(0x53);//soft reset,clear out all reg

        debug_msg("devID:%x\r\n",GSensor_I2C_ReadReg(0x0F));

        // init setting
        // Low Power:32Hz, XYZ enable 
        GSensor_I2C_WriteReg(0x44, 0x47);
        // xy enable
        //GSensor_I2C_WriteReg(0x44, 0x46);

        // 2G mode, High Pass Filter for INT1, Low pass filter for data
        //DMARD07_WriteReg(0x45, 0x24);
        // 2G mode, High Pass Filter for INT1, High pass filter for data
        GSensor_I2C_WriteReg(0x45, 0x14);
        // 2G mode, Low Pass Filter for INT1, High pass filter for data
        //GSensor_I2C_WriteReg(0x45, 0x10);

        //  High-pass Filter Cutoff for 0.6 Hz 
        GSensor_I2C_WriteReg(0x46, 0x00);

        // No latch, INT SRC1 enable, active 1 
        GSensor_I2C_WriteReg(0x47, 0x04);
        // No latch, INT SRC1 enable, active 0
        //GSensor_I2C_WriteReg(0x47, 0x44);
        // latch int 1, int 2, Int SRC1 enable
        //GSensor_I2C_WriteReg(0x47, 0x34);
        // latch int 1
        //GSensor_I2C_WriteReg(0x47, 0x24);

        GSensor_I2C_WriteReg(0x48, 0x00);

        //  OR all interrupt events, High-G XYZ 
        // xyz +G
        //GSensor_I2C_WriteReg(0x4A, 0x2A);
        // xyz -G
        GSensor_I2C_WriteReg(0x4A, 0x15);
        // xy +-G
        //GSensor_I2C_WriteReg(0x4A, 0x3c);
        // xy +G
        //GSensor_I2C_WriteReg(0x4A, 0x28);
        // xy -G
        //GSensor_I2C_WriteReg(0x4A, 0x14);
        // xyz +-G
        //GSensor_I2C_WriteReg(0x4A, 0x3f);

        // Threshold = 755.9 mg 
        GSensor_I2C_WriteReg(0x4C, 0x30);
        //  Duration = 47.1 ms 
        GSensor_I2C_WriteReg(0x4D, 0x10);
*/
#if 0
void IO_InitGensor(void)
{
  GSENSOR_INFO GsensorInfo;


    GsensorInfo.I2C_RegBytes = GSENSOR_I2C_REGISTER_1BYTE;
    GsensorInfo.I2C_PinMux = I2C_PINMUX_1ST;
    GsensorInfo.I2C_BusClock = I2C_BUS_CLOCK_400KHZ;
    // DMARD07 GSensor I2C slave addresses
    GsensorInfo.I2C_Slave_WAddr = 0x38;
    GsensorInfo.I2C_Slave_RAddr = 0x39;
    if (GSensor_I2C_Init(GsensorInfo) == TRUE)
    {
        //DARMD07 init register setting
        #if 1
        GSensor_I2C_ReadReg(0x53);//soft reset,clear out all reg
        Delay_DelayMs(10);
        GSensor_I2C_WriteReg(0x44,0x27);//Normal Mode, 342Hz,XYZ Int enable
        GSensor_I2C_WriteReg(0x45,0x24);//2G mode,High Pass Filter for INT1
        GSensor_I2C_WriteReg(0x46,0x00);//High-pass Filter Cutoff for 6.4Hz
        GSensor_I2C_WriteReg(0x47,0x04);//INT1 latch,INT SRC1 enable,Int pin high active
        GSensor_I2C_WriteReg(0x48,0x00);
        GSensor_I2C_WriteReg(0x4A,0x2A);//0x7F //OR all interrupt events,High-G XYZ
        GSensor_I2C_WriteReg(0x4C,0x1);//Threshold = 16*15.7 = 251.2 mg
        GSensor_I2C_WriteReg(0x4D,0x01);//Duration = 16*2.9 = 46.4ms

        #else
        GSensor_I2C_ReadReg(0x53);//soft reset,clear out all reg
        Delay_DelayMs(10);        
        GSensor_I2C_WriteReg(0x44, 0x47);
        GSensor_I2C_WriteReg(0x45, 0x14);
        GSensor_I2C_WriteReg(0x46, 0x00);
        GSensor_I2C_WriteReg(0x47, 0x04);
        GSensor_I2C_WriteReg(0x48, 0x00);
        GSensor_I2C_WriteReg(0x4A, 0x15);
        GSensor_I2C_WriteReg(0x4C, 0x30);
        GSensor_I2C_WriteReg(0x4D, 0x10);
/*
            GSensor_I2C_WriteReg(0x47,0x04);//0x64 INT1 latch,INT SRC1 enable,Int pin high active
            GSensor_I2C_WriteReg(0x48,0x00);
            GSensor_I2C_WriteReg(0x4A,0x6A);//0x7F //OR all interrupt events,High-G XYZ
            GSensor_I2C_WriteReg(0x4C,0x2); //0x15 Threshold = 16*15.7 = 251.2 mg
            */
        #endif

        debug_msg("devID:%x\r\n",GSensor_I2C_ReadReg(0x0F));
        debug_msg("reg 0x44:%x\r\n",GSensor_I2C_ReadReg(0x44));
        debug_msg("reg 0x45:%x\r\n",GSensor_I2C_ReadReg(0x45));
        debug_msg("reg 0x46:%x\r\n",GSensor_I2C_ReadReg(0x46));
        debug_msg("reg 0x47:%x\r\n",GSensor_I2C_ReadReg(0x47));
        debug_msg("reg 0x48:%x\r\n",GSensor_I2C_ReadReg(0x48));
        debug_msg("reg 0x4A:%x\r\n",GSensor_I2C_ReadReg(0x4A));
        debug_msg("reg 0x4C:%x\r\n",GSensor_I2C_ReadReg(0x4C));
        debug_msg("reg 0x4D:%x\r\n",GSensor_I2C_ReadReg(0x4D));        
    }
    else {
        debug_msg("G Sensor Init failed !!\r\n");
    }
}
#else 
GSENSOR_INFO ui_GsensorInfo;
static I2C_STS GsSensor_I2C_Receive(UINT32 *value, BOOL bNACK, BOOL bStop)
{
    I2C_DATA I2cData;
    I2C_STS ret;

    I2cData.VersionInfo = DRV_VER_96650;
    I2cData.ByteCount = I2C_BYTE_CNT_1;
    I2cData.Byte[0].Param = I2C_BYTE_PARAM_NONE;
    if ( bNACK == TRUE )
        I2cData.Byte[0].Param |= I2C_BYTE_PARAM_NACK;
    if ( bStop == TRUE )
        I2cData.Byte[0].Param |= I2C_BYTE_PARAM_STOP;

    ret = i2c_receive(&I2cData);
    if ( ret != I2C_STS_OK )
    {
        DBG_ERR("i2c ret = %02x!!\r\n", ret);
    }

    *value = I2cData.Byte[0].uiValue;

    return ret;
}

static I2C_STS GsSensor_I2C_Transmit(UINT32 value, BOOL bStart, BOOL bStop)
{
    I2C_DATA I2cData;
    I2C_STS ret;

    I2cData.VersionInfo = DRV_VER_96650;
    I2cData.ByteCount = I2C_BYTE_CNT_1;
    I2cData.Byte[0].uiValue = value & 0xff;
    I2cData.Byte[0].Param = I2C_BYTE_PARAM_NONE;
    if ( bStart == TRUE )
        I2cData.Byte[0].Param |= I2C_BYTE_PARAM_START;
    if ( bStop == TRUE )
        I2cData.Byte[0].Param |= I2C_BYTE_PARAM_STOP;
    ret = i2c_transmit(&I2cData);
    if ( ret != I2C_STS_OK )
    {
            DBG_ERR("i2c ret = %d!!\r\n", ret);
    }
    return ret;
}
UINT32 GSensor_I2C_ReadReg_2B(UINT32 uiAddr, UINT32 uidata)
{
  UINT32      ulWriteAddr, ulReadAddr, ulReg1;
  static UINT32 ulData1,ulData2;

    ulWriteAddr =0x30;
    ulReadAddr  =0x31;
    ulReg1      =(uiAddr&0x000000ff);
    ulData1     = 0;
    ulData2 = 0;
debug_err((">>> read register\r\n"));
    if(i2c_lock(ui_GsensorInfo.I2C_Channel) != E_OK)
    {
        debug_err(("GSensor: readReg I2C Lock Fail\r\n"));
        return ulData1;
    }

    if (GsSensor_I2C_Transmit(ulWriteAddr, 1, 0) != I2C_STS_OK)
    {
        debug_err(("Error transmit data1!!\r\n"));
        i2c_unlock(ui_GsensorInfo.I2C_Channel);
        return  ulData1;
    }

    if (GsSensor_I2C_Transmit(ulReg1, 0, 0) != I2C_STS_OK)
    {
        debug_err(("Error transmit data2!!\r\n"));
        i2c_unlock(ui_GsensorInfo.I2C_Channel);
        return  ulData1;
    }

    if (GsSensor_I2C_Transmit(ulReadAddr, 1, 0) != I2C_STS_OK)
    {
        debug_err(("Error transmit data3!!\r\n"));
        i2c_unlock(ui_GsensorInfo.I2C_Channel);
        return  ulData1;
    }

    if (GsSensor_I2C_Receive(&ulData1, 0, 0) != I2C_STS_OK)
    {
        debug_err(("Error Receive data!!\r\n"));
        i2c_unlock(ui_GsensorInfo.I2C_Channel);
        return  ulData1;
    }

    if (GsSensor_I2C_Receive(&ulData2, 1, 1) != I2C_STS_OK)
    {
        debug_err(("Error Receive data!!\r\n"));
        i2c_unlock(ui_GsensorInfo.I2C_Channel);
        return  ulData1;
    }
    if(i2c_unlock(ui_GsensorInfo.I2C_Channel) != E_OK)
    {
        debug_err(("GSensor: readReg I2C UnLock Fail\r\n"));
        return  ulData1;
    }
    debug_err((">>>>>uidata = %d %d\r\n",ulData1,ulData2));

   uidata =  (ulData1<<8 + ulData2);
   debug_err((">>>uidata = %d\r\n",uidata));
    return  uidata;
}

void IO_InitGensor(void)
{
  GSENSOR_INFO GsensorInfo;
  UINT32 value;

    GsensorInfo.I2C_RegBytes = GSENSOR_I2C_REGISTER_1BYTE;
    GsensorInfo.I2C_PinMux = I2C_PINMUX_1ST;
    GsensorInfo.I2C_BusClock = I2C_BUS_CLOCK_400KHZ;
    GsensorInfo.I2C_Slave_WAddr = 0x30;
    GsensorInfo.I2C_Slave_RAddr = 0x31;
    debug_err((">>> GSM301\r\n"));
    if (GSensor_I2C_Init(GsensorInfo) == TRUE)
    {
		//init
		GSensor_I2C_WriteReg(0x21,0x52);//
		GSensor_I2C_WriteReg(0x00,0x02);//
		GSensor_I2C_WriteReg(0x00,0x12);//
		GSensor_I2C_WriteReg(0x00,0x02);//
		GSensor_I2C_WriteReg(0x00,0x82);//
		GSensor_I2C_WriteReg(0x00,0x02);//
		GSensor_I2C_WriteReg(0x1F,0x28);//
		GSensor_I2C_WriteReg(0x0C,0x8F);//
		GSensor_I2C_WriteReg(0x00,0x06);//
	
		//interrupt setup
		GSensor_I2C_WriteReg(0x11,0x04);//IIC 0X07 for no pullup //0x06 High active  0x04 low active
		//Gsensor_WriteReg(0x11,0x06);//IIC 0X06 for no pullup 
		//set Gsensor Level 
		//0x08-->0.5G 
		//0X10-->1G
		GSensor_I2C_WriteReg(0x38,0X00);//
		GSensor_I2C_WriteReg(0x39,0X30);//10 1g 20 2g 30 3g 40 4g 50 5g 60 6g
		
		GSensor_I2C_WriteReg(0x0F,0x00);//
		GSensor_I2C_WriteReg(0x0E,0x00);//0x1C//0x00 // 0x00:disable interrupt
		GSensor_I2C_WriteReg(0x1F,0x28);//To disable micro motion interrupt.
		//TimerDelayMs(10);
		//Latched reference data of micro motion.
		GSensor_I2C_ReadReg(0x12);
		GSensor_I2C_ReadReg(0x13);
		GSensor_I2C_ReadReg(0x14);
		GSensor_I2C_ReadReg(0x15);
		GSensor_I2C_ReadReg(0x16);
		GSensor_I2C_ReadReg(0x17);
		GSensor_I2C_ReadReg(0x18);
		GSensor_I2C_ReadReg(0x19);
		//GSensor_I2C_ReadReg(0x1A);
		//GSensor_I2C_ReadReg(0x1B);
        
        debug_msg("reg 0x14:%x\r\n",GSensor_I2C_ReadReg(0x14));//X-L
        debug_msg("reg 0x15:%x\r\n",GSensor_I2C_ReadReg(0x15));//X-H
        debug_msg("reg 0x16:%x\r\n",GSensor_I2C_ReadReg(0x16));//Y-L
        debug_msg("reg 0x17:%x\r\n",GSensor_I2C_ReadReg(0x17));//Y-H
        debug_msg("reg 0x18:%x\r\n",GSensor_I2C_ReadReg(0x18));//Z-L
		debug_msg("reg 0x19:%x\r\n",GSensor_I2C_ReadReg(0x19));//Z-H

		GSensor_I2C_WriteReg(0x1F,0x38);//To enable micro motion interrupt.
		Delay_DelayMs(1); //2014_0819 added 1ms delay for micro motion setup itself.

		GSensor_I2C_WriteReg(0x0E,0x1C);//To enable interrupt.
		GSensor_I2C_ReadReg_2B(0x1C,value);
	
    }
    else {
        debug_msg("G Sensor Init failed !!\r\n");
    }
}
#endif

#elif(GSENSOR_TPYE==GSENSOR_TYPE_MM3A311)
void IO_InitGensor(void)
{
  GSENSOR_INFO GsensorInfo;

    GsensorInfo.I2C_RegBytes = GSENSOR_I2C_REGISTER_1BYTE;
    GsensorInfo.I2C_PinMux = I2C_PINMUX_1ST;
    GsensorInfo.I2C_BusClock = I2C_BUS_CLOCK_400KHZ;
    // MM3A311 GSensor I2C slave addresses
    GsensorInfo.I2C_Slave_WAddr = 0x4e;
    GsensorInfo.I2C_Slave_RAddr = 0x4d;
    if (GSensor_I2C_Init(GsensorInfo) == TRUE)
    {
        //DARMD07 init register setting
        debug_msg("------------devID:%x\r\n",GSensor_I2C_ReadReg(0x0f));

    GSensor_I2C_WriteReg(0x00, 1);////reset start.
    GSensor_I2C_WriteReg(0x05, 0xAA);   
    Delay_DelayMs(5);////5ms
    GSensor_I2C_WriteReg(0x05, 0x00);   
    Delay_DelayMs(5);////5ms
    GSensor_I2C_WriteReg(0x00, 0);////reset end.
    Delay_DelayMs(10);////10ms
    /* load OTP. */
    GSensor_I2C_WriteReg(DA311_TEMP_CFG_REG, 0x08);
    GSensor_I2C_WriteReg(DA311_CTRL_REG5, 0x80);
    /* Full scale: 2G */
    GSensor_I2C_WriteReg(DA311_CTRL_REG4, MODE_8G);
    /* ODR=200Hz, X,Y,Z axis enable */
    GSensor_I2C_WriteReg(DA311_CTRL_REG1, 0x6f);
    /* Power on, DATA measurement enable, lDO swtich all on */
    GSensor_I2C_WriteReg(DA311_TEMP_CFG_REG, 0x88);
    GSensor_I2C_WriteReg(DA311_LDO_REG, 0x02);
    /* set Single Click interrupt I1_DRDY1 */
    /*
    CTRL_REG3=80h
    CTRL_REG5=08h
    CLICK_CFG=15h
    CLICK_THS=40h////1 LSB = full scale/128
    TIME_LIMIT=0Eh////1 LSB = 1/ODR(Now:200Hz)
    TIME_LANTENCY=10h////for dobule click.
    TIME_WINDOW=12h////for dobule click.
    */
    GSensor_I2C_WriteReg(DA311_CTRL_REG3, 0x80);////Single Click on DRDY1
    /* CTRL_REG5:[D3:LIR_INT1]
    Latch interrupt request on INT1_SRC register, with INT1_SRC register
    Cleared by reading INT1_SRC itself. Default value: 0.
    (0: interrupt request not latched; 1: interrupt request latched)
    */
    GSensor_I2C_WriteReg(DA311_CTRL_REG5, 0x08);////interrupt request latched
    GSensor_I2C_WriteReg(DA311_CLICK_CFG, 0x15);////CLICK_CFG
    GSensor_I2C_WriteReg(DA311_CLICK_THS, 0x60);////CLICK_THS 0x60<==>1.5G
    GSensor_I2C_WriteReg(DA311_TIME_LIMIT, 0x0E);////TIME_LIMIT
    /* CTRL_REG6 [D1:H_LACTIVE] 0(default): interrupt active high; 1: interrupt active low */
///    GSensor_I2C_WriteReg(DA311_CTRL_REG6, 0x02);////Low active.
    GSensor_I2C_WriteReg(DA311_TIME_LATENCY, 0x10);////TIME_LANTENCY for dobule click.
    GSensor_I2C_WriteReg(DA311_TIME_WINDOW, 0x12);////TIME_WINDOW for dobule click.

    GSensor_I2C_WriteReg(0x00, 1);
    GSensor_I2C_WriteReg(0x1b, 0x27);   
    GSensor_I2C_WriteReg(0x1c, 0x30);   
    GSensor_I2C_WriteReg(0x27, 0x3f);   
    GSensor_I2C_WriteReg(0x28, 0xff);   
    GSensor_I2C_WriteReg(0x29, 0x0f);   
    GSensor_I2C_WriteReg(0x00, 0);
    } else {
        debug_msg("---------G Sensor Init failed !!\r\n");
    }
}
#else
void IO_InitGensor(void)
{}
#endif		

/**
  Initialize voltage detection

  Initialize voltage detection for battery and flash

  @param void
  @return void
*/

void IO_InitADC(void)
{

    if (adc_open(ADC_CH_VOLDET_BATTERY) != E_OK)
    {
        DBG_ERR("Can't open ADC channel for battery voltage detection\r\n");
        return;
    }

    if (adc_open(ADC_CH_VOLDET_KEY1) != E_OK)
    {
        DBG_ERR("Can't open ADC channel for key key1 detection\r\n");
        return;
    }
    if (adc_open(ADC_CH_VOLDET_KEY2) != E_OK)
    {
        DBG_ERR("Can't open ADC channel for key key1 detection\r\n");
        return;
    }

    //650 Range is 250K Hz ~ 2M Hz
    adc_setConfig(ADC_CONFIG_ID_OCLK_FREQ, 250000); //250K Hz

    //battery voltage detection
    adc_setChConfig(ADC_CH_VOLDET_BATTERY, ADC_CH_CONFIG_ID_SAMPLE_FREQ, 10000); //10K Hz, sample once about 100 us for CONTINUOUS mode
    adc_setChConfig(ADC_CH_VOLDET_BATTERY, ADC_CH_CONFIG_ID_SAMPLE_MODE, (VOLDET_ADC_MODE) ? ADC_CH_SAMPLEMODE_CONTINUOUS : ADC_CH_SAMPLEMODE_ONESHOT);
    adc_setChConfig(ADC_CH_VOLDET_BATTERY, ADC_CH_CONFIG_ID_INTEN, FALSE);

    //key key1 detection
    adc_setChConfig(ADC_CH_VOLDET_KEY1, ADC_CH_CONFIG_ID_SAMPLE_FREQ, 10000); //10K Hz, sample once about 100 us for CONTINUOUS mode
    adc_setChConfig(ADC_CH_VOLDET_KEY1, ADC_CH_CONFIG_ID_SAMPLE_MODE, (VOLDET_ADC_MODE) ? ADC_CH_SAMPLEMODE_CONTINUOUS : ADC_CH_SAMPLEMODE_ONESHOT);
    adc_setChConfig(ADC_CH_VOLDET_KEY1, ADC_CH_CONFIG_ID_INTEN, FALSE);

    //key key2 detection
    adc_setChConfig(ADC_CH_VOLDET_KEY2, ADC_CH_CONFIG_ID_SAMPLE_FREQ, 10000); //10K Hz, sample once about 100 us for CONTINUOUS mode
    adc_setChConfig(ADC_CH_VOLDET_KEY2, ADC_CH_CONFIG_ID_SAMPLE_MODE, (VOLDET_ADC_MODE) ? ADC_CH_SAMPLEMODE_CONTINUOUS : ADC_CH_SAMPLEMODE_ONESHOT);
    adc_setChConfig(ADC_CH_VOLDET_KEY2, ADC_CH_CONFIG_ID_INTEN, FALSE);

    // Enable adc control logic
    adc_setEnable(TRUE);

    Delay_DelayMs(15); //wait ADC stable  //for pwr on speed up
}

#if _MIPS_TODO
static char ADCStr1[32];
static char ADCStr2[32];

char* VolDet_GetStatusString1(void)
{
    sprintf(ADCStr1, "A0=%ld,A1=%ld,A2=%ld,A3=%ld", adc_readData(0), adc_readData(1),adc_readData(2),adc_readData(3));
    return ADCStr1;
}

char* VolDet_GetStatusString2(void)
{
    sprintf(ADCStr2, "A4=%ld,A5=%ld,A6=%ld,A7=%ld", adc_readData(4), adc_readData(5),adc_readData(6),adc_readData(7));
    return ADCStr2;
}
#endif

void IO_InitEncryption(void)
{
    ENDE_DEVICE_OBJ g_ENDECtrlObj;

    g_ENDECtrlObj.ENDECtrl_Pin.uigpio_clk     = GPIO_CK235_CLK;
    g_ENDECtrlObj.ENDECtrl_Pin.uigpio_data    = GPIO_CK235_DAT;

    InitEnDecry(&g_ENDECtrlObj);
}
void Dx_InitIO(void)  // Config IO for external device
{
    IO_InitPinmux(); //initial PINMUX config
    IO_InitGPIO(); //initial GPIO pin status
    IO_InitADC(); //initial ADC pin status
#if(GSENSOR_TPYE!=GSENSOR_TYPE_NONE)  
    IO_InitGensor();
#endif
#if (_HDMITYPE_ == _HDMI_ON_)
    //thse two pins are default pull-up for GPIO,but must turn off for HDMI DDC I2C
    pad_setPullUpDown(PAD_PIN_PGPIO28, PAD_NONE);
    pad_setPullUpDown(PAD_PIN_PGPIO29, PAD_NONE);
#endif    
    //IO_InitEncryption();
}

void Dx_UninitIO(void)  // Config IO for external device
{

    // Disable adc control logic
    adc_setEnable(FALSE);
    adc_close(ADC_CH_VOLDET_BATTERY);
    adc_close(ADC_CH_VOLDET_KEY1);
    adc_close(ADC_CH_VOLDET_KEY2);
}

BOOL GPIOMap_GsensorDet(void)
{

return gpio_getPin(GPIO_GS_INT1);
#if(GSENSOR_TPYE!=GSENSOR_TYPE_NONE)  
    return (gpio_getPin(GPIO_GS_INT1)? TRUE : FALSE);
#else
    return FALSE;
#endif
}
#if(GSENSOR_TPYE == GSENSOR_TYPE_DMTARD07)
void GSensorSensitivity(UINT32 level)
{
return;
    switch(level)
    {
    case 0:
        GSensor_I2C_WriteReg(0x44, 0x00);
        debug_err(("GSensorSensitivity()==================>0\r\n"));
        break;
    case 1:
        GSensor_I2C_WriteReg(0x44, 0x47);
        // Threshold = 755.9 mg 
        GSensor_I2C_WriteReg(0x4C, 0x70);
        //  Duration = 47.1 ms 
        GSensor_I2C_WriteReg(0x4D, 0x10);
        debug_err(("GSensorSensitivity()==================>1\r\n"));
        break;
    case 2:
        GSensor_I2C_WriteReg(0x44, 0x47);
        // Threshold = 755.9 mg 
        GSensor_I2C_WriteReg(0x4C, 0x50);
        //  Duration = 47.1 ms 
        GSensor_I2C_WriteReg(0x4D, 0x10);

        debug_err(("GSensorSensitivity()==================>2\r\n"));
        break;
    case 3:
        GSensor_I2C_WriteReg(0x44, 0x47);
        // Threshold = 755.9 mg 
        GSensor_I2C_WriteReg(0x4C, 0x30);
        //  Duration = 47.1 ms 
        GSensor_I2C_WriteReg(0x4D, 0x10);

        debug_err(("GSensorSensitivity()==================>3\r\n"));
        break;

    default:
        break;
    }
}

void GSensorReadXYZ(void)
{
    UINT32 value;
        GSensor_I2C_ReadReg_2B(0x1C,value);
        debug_msg("reg 0x12:%x\r\n",GSensor_I2C_ReadReg(0x12));//*****2014_0819 added for GMA301 X, Y, Z data read.*****
        debug_msg("reg 0x14:%x\r\n",GSensor_I2C_ReadReg(0x14));
        debug_msg("reg 0x15:%x\r\n",GSensor_I2C_ReadReg(0x15));
        debug_msg("reg 0x16:%x\r\n",GSensor_I2C_ReadReg(0x16));
        debug_msg("reg 0x17:%x\r\n",GSensor_I2C_ReadReg(0x17));
        debug_msg("reg 0x18:%x\r\n",GSensor_I2C_ReadReg(0x18));   
        debug_msg("reg 0x19:%x\r\n",GSensor_I2C_ReadReg(0x19));//*****2014_0819 modified because it ReadReg(0x12) is incorrectly.*****
}

#elif(GSENSOR_TPYE==GSENSOR_TYPE_MM3A311)
void GSensorSensitivity(UINT32 level)
{
    switch(level)
    {
    case 0:
	  GSensor_I2C_WriteReg(DA311_TEMP_CFG_REG, 0x20);		
        debug_err(("GSensorSensitivity()==================>0\r\n"));
        break;
    case 1:
        GSensor_I2C_WriteReg(DA311_CLICK_THS, 0x7A);////CLICK_THS 0.95x8G
        debug_err(("GSensorSensitivity()==================>1\r\n"));
        break;
    case 2:
        GSensor_I2C_WriteReg(DA311_CLICK_THS, 0x4D);////CLICK_THS 0.60x8G
        debug_err(("GSensorSensitivity()==================>2\r\n"));
        break;
    case 3:
        GSensor_I2C_WriteReg(DA311_CLICK_THS, 0x26);////CLICK_THS 0.30x8G
        debug_err(("GSensorSensitivity()==================>3\r\n"));
        break;

    default:
        break;
    }
}
#else
void GSensorSensitivity(UINT32 level)
{}
#endif

#if(GSENSOR_TPYE!=GSENSOR_TYPE_NONE)
void GSensorSensitivityReset(void)
{
    //GSensor_I2C_ReadReg(0x0F);
}
#else
void GSensorSensitivityReset(void)
{}
#endif

BOOL GPIOMap_IsBatteryFull(void)
{
    return (gpio_getPin(GPIO_BATTERY_FULL) == 1 ? FALSE : TRUE);
}

void GPIOMap_TurnOnWhiteLight(void)
{
	gpio_setPin(GPIO_WHITE_LIGHT);
}
void GPIOMap_TurnOffWhiteLight(void)
{
	gpio_clearPin(GPIO_WHITE_LIGHT);
}
UINT32 GPIOMap_IsWhiteLight(void)
{
	return gpio_getPin(GPIO_WHITE_LIGHT);
}
