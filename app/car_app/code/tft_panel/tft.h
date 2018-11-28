#ifndef __TFT_H__
#define __TFT_H__

#include "typedef.h"
#include "pap.h"

#define TFT_DELAY(cnt)  delay(cnt)

#define LCD_ST7735S			0
#define LCD_ILI9325			1
#define LCD_SSD1963     	2
#define LCD_OTA5182A    	3
#define LCD_ILI8961     	4
#define LCD_ILI8960     	5
#define LCD_LQ026B3UX01 	6
#define LCD_JC4301      	7
#define LCD_43INCH			8
#define LCD_8INCH			9
#define LCD_RM68116         10
#define LCD_ILI9163         11
#define LCD_AVOUT           12

#define LCD_TYPE			LCD_ILI9325
//#define LCD_TYPE			LCD_AVOUT

#if (LCD_TYPE == LCD_RM68116)

#define SPI_SDA_OUT()	PORTF_DIR &=~ BIT(14)
#define SPI_SDA_IN()	PORTF_DIR |= BIT(14)
#define SPI_SDA_R()		PORTF_IN & BIT(14)
#define SPI_SDA_H()		PORTF_OUT |= BIT(14)
#define SPI_SDA_L()		PORTF_OUT &=~ BIT(14)

#define SPI_CSX_OUT()	PORTF_DIR &=~ BIT(13)
#define SPI_CSX_H()		PORTF_OUT |= BIT(13)
#define SPI_CSX_L()		PORTF_OUT &=~ BIT(13)

#define SPI_SCL_OUT()	PORTF_DIR &=~ BIT(12)
#define SPI_SCL_H()		PORTF_OUT |= BIT(12)
#define SPI_SCL_L()		PORTF_OUT &=~ BIT(12)

#define SPI_DCX_OUT()	PORTF_DIR &=~ BIT(11)
#define SPI_DCX_H()		PORTF_OUT |= BIT(11)
#define SPI_DCX_L()		PORTF_OUT &=~ BIT(11)

#define SPI_RST_OUT()   PORTF_DIR &=~ BIT(10)
#define SPI_RST_H()     PORTF_OUT |= BIT(10)
#define SPI_RST_L()     PORTF_OUT &=~BIT(10)

#endif

//--------------------------LCD_ILI9163----------------------------//
#if (LCD_TYPE == LCD_ILI9163)

//PF12
#define 	TFT_RS_OUT()    do{PORTF_DIR &= ~BIT(13); }while(0)
#define		TFT_RS_L()		do{PORTF_DIR &= ~BIT(13); PORTF_OUT &=~BIT(13);}while(0)
#define		TFT_RS_H()		do{PORTF_DIR &= ~BIT(13); PORTF_OUT |= BIT(13);}while(0)
//PF14
#define 	TFT_RST_OUT()   do{PORTF_DIR &= ~BIT(12); }while(0)
#define		TFT_RST_L()	    do{PORTF_DIR &= ~BIT(12); PORTF_OUT &= ~BIT(12);}while(0)
#define		TFT_RST_H()	    do{PORTF_DIR &= ~BIT(12); PORTF_OUT |= BIT(12);}while(0)
//PF13
#define 	TFT_CS_OUT()    do{PORTF_DIR &= ~BIT(11); }while(0)
#define		TFT_CS_EN()		do{PORTF_DIR &= ~BIT(11); PORTF_OUT &=~BIT(11);}while(0)
#define		TFT_CS_DIS()	do{PORTF_DIR &= ~BIT(11); PORTF_OUT |= BIT(11);}while(0)

#define TFT_CS_L()  TFT_CS_EN()
#define TFT_CS_H()  TFT_CS_DIS()

#endif

//--------------------------LCD_ILI8960----------------------------//
#if (LCD_TYPE == LCD_ILI8960)

#define LCD_CS_OUT()    PORTF_DIR &=~BIT(12)
#define LCD_CS_DIS()    PORTF_OUT |= BIT(12)
#define LCD_CS_EN()     PORTF_OUT &=~BIT(12)

#define LCD_SDA_OUT()   PORTF_DIR &=~BIT(13)
#define LCD_SDA_IN()    PORTF_DIR |= BIT(13);//PORTA_PU |= BIT(1);PORTA_PD &=~BIT(1)
#define LCD_SDA_H()     PORTF_OUT |= BIT(13)
#define LCD_SDA_L()     PORTF_OUT &=~BIT(13)
#define LCD_SDA_R()     PORTF_IN & BIT(13)

#define LCD_SCL_OUT()   PORTF_DIR &=~BIT(14)
#define LCD_SCL_H()     PORTF_OUT |= BIT(14)
#define LCD_SCL_L()     PORTF_OUT &=~BIT(14)

void lcd_init();
void ILI8960_initial(void);

#define RGB_8BIT                        0
#define RGB_8BIT_DUMMY_320x240_NTSC     1
#define RGB_8BIT_DUMMY_320x288_PAL      2
#define RGB_8BIT_DUMMY_360x240_NTSC     3
#define RGB_8BIT_DUMMY_360x288_PAL      4
#define YUV720x240_NTSC                 5
#define YUV720x288_PAL                  6
#define YUV640x240_NTSC                 7
#define YUV640x288_PAL                  8
#define CCIR656_PAL                     9
#define CCIR656_NTSC                    10

#define DRIVE_MODE  CCIR656_NTSC
#endif
//=============================================================//

//--------------------------ST7735S----------------------------//
#if (LCD_TYPE == LCD_ST7735S)
//LCD引脚接口(用于PAP接口的引脚在PAP.h里面定义)
//PF12
#define 	TFT_RS_OUT()    do{PORTF_DIR &= ~BIT(12); }while(0)
#define		TFT_RS_L()		do{PORTF_DIR &= ~BIT(12); PORTF_OUT &=~BIT(12);}while(0)
#define		TFT_RS_H()		do{PORTF_DIR &= ~BIT(12); PORTF_OUT |= BIT(12);}while(0)
//PF14
#define 	TFT_RST_OUT()   do{PORTF_DIR &= ~BIT(14); }while(0)
#define		TFT_RST_L()	    do{PORTF_DIR &= ~BIT(14); PORTF_OUT &= ~BIT(14);}while(0)
#define		TFT_RST_H()	    do{PORTF_DIR &= ~BIT(14); PORTF_OUT |= BIT(14);}while(0)
//PF13
#define 	TFT_CS_OUT()    do{PORTF_DIR &= ~BIT(13); }while(0)
#define		TFT_CS_EN()		do{PORTF_DIR &= ~BIT(13); PORTF_OUT &=~BIT(13);}while(0)
#define		TFT_CS_DIS()	do{PORTF_DIR &= ~BIT(13); PORTF_OUT |= BIT(13);}while(0)

#define TFT_CS_L()  do{PORTF_DIR &= ~BIT(13);PORTF_OUT &=~BIT(13);}while(0)
#define TFT_CS_H()  do{PORTF_DIR &= ~BIT(13);PORTF_OUT |= BIT(13);}while(0)

#define TFT_WIDTH      128
#define TFT_HIGHT      160

#endif
//=============================================================//

//--------------------------ILI9325----------------------------//
#if (LCD_TYPE == LCD_ILI9325)
//LCD引脚接口(用于PAP接口的引脚在PAP.h里面定义)
#if 1 //插接&焊接 9325 JOHNSON 20160526
    //PH2//PH8
    #define 	TFT_RS_OUT()    do{PORTH_DIR &= ~BIT(2); }while(0)
    #define		TFT_RS_L()		do{PORTH_DIR &= ~BIT(2); PORTH_OUT &=~BIT(2);}while(0)
    #define		TFT_RS_H()		do{PORTH_DIR &= ~BIT(2); PORTH_OUT |= BIT(2);}while(0)

    //PH5//PH0
    #define 	TFT_RST_OUT()   do{PORTH_DIR &= ~BIT(5); }while(0)
    #define		TFT_RST_L()	    do{PORTH_DIR &= ~BIT(5); PORTH_OUT &=~BIT(5);}while(0)
    #define		TFT_RST_H()	    do{PORTH_DIR &= ~BIT(5); PORTH_OUT |= BIT(5);}while(0)

    //PH0//PH1
    #define 	TFT_CS_OUT()    do{PORTH_DIR &= ~BIT(0); }while(0)
    #define		TFT_CS_EN()		do{PORTH_DIR &= ~BIT(0); PORTH_OUT &=~BIT(0);}while(0)
    #define		TFT_CS_DIS()	do{PORTH_DIR &= ~BIT(0); PORTH_OUT |= BIT(0);}while(0)
    #define TFT_CS_L()  do{PORTH_DIR &= ~BIT(0);PORTH_OUT &=~BIT(0);}while(0)
    #define TFT_CS_H()  do{PORTH_DIR &= ~BIT(0);PORTH_OUT |= BIT(0);}while(0)
    //PH9
    #define TFT_TE_L()  //PORTH_DIR &=~ BIT(9);PORTH_OUT |=  BIT(9);
    #define TFT_TE_H()  //PORTH_DIR &=~ BIT(9);PORTH_OUT &=~ BIT(9);

#else //old

#define 	TFT_RST_OUT()   do{PORTG_DIR &= ~BIT(2); }while(0)
#define		TFT_RST_L()	    do{PORTG_DIR &= ~BIT(2); PORTG_OUT &=~BIT(2);delay(100);}while(0)
#define		TFT_RST_H()	    do{PORTG_DIR &= ~BIT(2); PORTG_OUT |= BIT(2);delay(100);}while(0)

//#define 	TFT_CS_OUT()    do{PORTG_DIR &= ~BIT(6); }while(0)
//#define		TFT_CS_EN()		do{PORTG_DIR &= ~BIT(6); PORTG_OUT &=~BIT(6);delay(100);}while(0)
//#define		TFT_CS_DIS()	do{PORTG_DIR &= ~BIT(6); PORTG_OUT |= BIT(6);delay(100);}while(0)
//
//#define 	TFT_RS_OUT()    do{PORTG_DIR &= ~BIT(7); }while(0)
//#define		TFT_RS_L()		do{PORTG_DIR &= ~BIT(7); PORTG_OUT &=~BIT(7);delay(100);}while(0)
//#define		TFT_RS_H()		do{PORTG_DIR &= ~BIT(7); PORTG_OUT |= BIT(7);delay(100);}while(0)

#define 	TFT_CS_OUT()    do{PORTH_DIR &= ~BIT(0); }while(0)
#define		TFT_CS_EN()		do{PORTH_DIR &= ~BIT(0); PORTH_OUT &=~BIT(0);delay(100);}while(0)
#define		TFT_CS_DIS()	do{PORTH_DIR &= ~BIT(0); PORTH_OUT |= BIT(0);delay(100);}while(0)

#define 	TFT_RS_OUT()    do{PORTH_DIR &= ~BIT(1); }while(0)
#define		TFT_RS_L()		do{PORTH_DIR &= ~BIT(1); PORTH_OUT &=~BIT(1);delay(100);}while(0)
#define		TFT_RS_H()		do{PORTH_DIR &= ~BIT(1); PORTH_OUT |= BIT(1);delay(100);}while(0)

#endif

#define TFT_WIDTH       400//320
#define TFT_HIGHT       240//240

#define SCREEN_ROTATE

#endif
//=============================================================//

//--------------------------SSD1963----------------------------//
#if (LCD_TYPE == LCD_SSD1963)
//LCD引脚接口(用于PAP接口的引脚在PAP.h里面定义)
//PF12
#define 	TFT_RS_OUT()    do{PORTF_DIR &= ~BIT(12); }while(0)
#define		TFT_RS_L()		do{PORTF_DIR &= ~BIT(12); PORTF_OUT &=~BIT(12);}while(0)
#define		TFT_RS_H()		do{PORTF_DIR &= ~BIT(12); PORTF_OUT |= BIT(12);}while(0)
//PF14
#define 	TFT_RST_OUT()   do{PORTF_DIR &= ~BIT(14); }while(0)
#define		TFT_RST_L()	    do{PORTF_DIR &= ~BIT(14); PORTF_OUT &= ~BIT(14);}while(0)
#define		TFT_RST_H()	    do{PORTF_DIR &= ~BIT(14); PORTF_OUT |= BIT(14);}while(0)
//PF13
#define 	TFT_CS_OUT()    do{PORTF_DIR &= ~BIT(13); }while(0)
#define		TFT_CS_EN()		do{PORTF_DIR &= ~BIT(13); PORTF_OUT &=~BIT(13);}while(0)
#define		TFT_CS_DIS()	do{PORTF_DIR &= ~BIT(13); PORTF_OUT |= BIT(13);}while(0)

#define TFT_CS_L()  do{PORTF_DIR &= ~BIT(13);PORTF_OUT &=~BIT(13);}while(0)
#define TFT_CS_H()  do{PORTF_DIR &= ~BIT(13);PORTF_OUT |= BIT(13);}while(0)

#define TFT_WIDTH       800
#define TFT_HIGHT       480
#endif
//=============================================================//

//--------------------------OTA5182A----------------------------//
#if (LCD_TYPE == LCD_OTA5182A)
//PC3 PF12
#define TFT_CS_OUT()    PORTF_DIR &= ~BIT(12);PORTF_HD |= BIT(12)
#define TFT_CS_EN()     PORTF_OUT &= ~BIT(12)
#define TFT_CS_DIS()    PORTF_OUT |= BIT(12)
//PD1 PH10
#define TFT_SDA_OUT()   PORTF_DIR &= ~BIT(13);PORTF_HD |= BIT(13)
#define TFT_SDA_IN()    PORTF_DIR |=  BIT(13)
#define TFT_SDA_H()     PORTF_OUT |=  BIT(13)
#define TFT_SDA_L()     PORTF_OUT &= ~BIT(13)
#define TFT_SDA_R()     PORTF_IN&BIT(13)
//PC1 PH11
#define TFT_SCL_OUT()   PORTF_DIR &= ~BIT(14);PORTF_HD |= BIT(14)
#define TFT_SCL_H()     PORTF_OUT |= BIT(14)
#define TFT_SCL_L()     PORTF_OUT &=~BIT(14)

//PH5
#define TFT_VSYNC_OUT() PORTH_DIR &= ~BIT(5)
#define TFT_VSYNC_H()   TFT_VSYNC_OUT();PORTH_OUT |= BIT(5)
#define TFT_VSYNC_L()   TFT_VSYNC_OUT();PORTH_OUT &=~BIT(5)

#define TFT_RESET_H()   PORTG_DIR &= ~BIT(6);PORTG_OUT |= BIT(6)
#define TFT_RESET_L()   PORTG_DIR &= ~BIT(6);PORTG_OUT &=~BIT(6)

#define RAW_MODE

//RGB 8BIT + dummy
#define RGB_320x240_NTSC    0
#define RGB_320x288_PAL     1
#define RGB_360x240_NTSC    2
#define RGB_360x288_PAL     3
#define RGB_FORMAT RGB_360x288_PAL      //RGB_MODE有效

#endif
//=============================================================//

//--------------------------ILI8961----------------------------//
#if (LCD_TYPE == LCD_ILI8961)
//    PG3
//#define TFT_CS_OUT()    PORTG_DIR &= ~BIT(7);PORTG_HD |= BIT(7)
#define TFT_CS_OUT()    PORTH_DIR &= ~BIT(3);PORTH_HD |= BIT(3)
#define TFT_CS_EN()     PORTH_OUT &= ~BIT(3)
#define TFT_CS_DIS()    PORTH_OUT |= BIT(3)
////PD1 PH10
//#define TFT_SDA_OUT()   PORTH_DIR &= ~BIT(10);PORTH_HD |= BIT(10)
//#define TFT_SDA_IN()    PORTH_DIR |=  BIT(10)
//#define TFT_SDA_H()     PORTH_OUT |=  BIT(10)
//#define TFT_SDA_L()     PORTH_OUT &= ~BIT(10)
//#define TFT_SDA_R()     PORTH_IN&BIT(10)
////PC1 PH11
//#define TFT_SCL_OUT()   PORTH_DIR &= ~BIT(11);PORTH_HD |= BIT(11)
//#define TFT_SCL_H()     PORTH_OUT |= BIT(11)
//#define TFT_SCL_L()     PORTH_OUT &=~BIT(11)


//PD1 PH10
//#define TFT_SDA_OUT()   PORTG_DIR &= ~BIT(4);PORTG_HD |= BIT(4)
#define TFT_SDA_OUT()   PORTH_DIR &= ~BIT(7);PORTH_HD |= BIT(7)
#define TFT_SDA_IN()    PORTH_DIR |=  BIT(7)
#define TFT_SDA_H()     PORTH_OUT |=  BIT(7)
#define TFT_SDA_L()     PORTH_OUT &= ~BIT(7)
#define TFT_SDA_R()     PORTH_IN&BIT(4)
//PC1 PH11
//#define TFT_SCL_OUT()   PORTG_DIR &= ~BIT(5);PORTG_HD |= BIT(5)
#define TFT_SCL_OUT()   PORTH_DIR &= ~BIT(6);PORTH_HD |= BIT(6)
#define TFT_SCL_H()     PORTH_OUT |= BIT(6)
#define TFT_SCL_L()     PORTH_OUT &=~BIT(6)

//PH5
#define TFT_VSYNC_OUT() PORTH_DIR &= ~BIT(5)
#define TFT_VSYNC_H()   TFT_VSYNC_OUT();PORTH_OUT |= BIT(5)
#define TFT_VSYNC_L()   TFT_VSYNC_OUT();PORTH_OUT &=~BIT(5)

#define TFT_RESET_H()   //PORTG_DIR &= ~BIT(6);PORTG_OUT |= BIT(6)
#define TFT_RESET_L()   //PORTG_DIR &= ~BIT(6);PORTG_OUT &=~BIT(6)

#define RGB_8BIT                        0
#define RGB_8BIT_DUMMY_320x240_NTSC     1
#define RGB_8BIT_DUMMY_320x288_PAL      2
#define RGB_8BIT_DUMMY_360x240_NTSC     3
#define RGB_8BIT_DUMMY_360x288_PAL      4
#define YUV720x240_NTSC                 5
#define YUV720x288_PAL                  6
#define YUV640x240_NTSC                 7
#define YUV640x288_PAL                  8
#define CCIR656_PAL                     9
#define CCIR656_NTSC                    10


#define DRIVE_MODE          RGB_8BIT_DUMMY_320x240_NTSC

#define TFT_WIDTH       320
#define TFT_HIGHT       240

#endif
//=============================================================//

//--------------------------LQ026B3UX01------------------------//
#if (LCD_TYPE == LCD_LQ026B3UX01)

#define TFT_CS_OUT()    PORTF_DIR &=~BIT(13)
#define TFT_CS_EN()     PORTF_OUT &=~BIT(13)
#define TFT_CS_DIS()    PORTF_OUT |= BIT(13)

#define TFT_RST_OUT()   PORTF_DIR &=~BIT(14)
#define TFT_RST_H()     PORTF_OUT |= BIT(14)
#define TFT_RST_L()     PORTF_OUT &=~BIT(14)

#define TFT_RS_OUT()    PORTF_DIR &= ~BIT(12)
#define TFT_RS_H()      PORTF_OUT |= BIT(12)
#define TFT_RS_L()      PORTF_OUT &=~BIT(12)

#endif

//常用颜色表
#define COLOUR_RED       0xF800
#define COLOUR_BLUE      0x001F
#define COLOUR_GREEN     0x07E0
#define COLOUR_WHITE     0xFFFF
#define COLOUR_BLACK     0x0000

extern void tft_init();
extern void tft_ClearScreen(u16 color);

#endif
//=============================================================//
