
#ifndef __LCD_H__
#define __LCD_H__


#include "config.h"
#include "tft.h"
#include "app_cfg.h"

#ifdef DOUBLE_VEDIO
    //#define ALCD_BL_ON()       PORTH_DIR &= ~BIT(10);PORTH_OUT |= BIT(10)//高电平
    //#define ALCD_BL_OFF()      PORTH_DIR &= ~BIT(10);PORTH_OUT &= ~BIT(10)//低电平
    //5203 JOHNSON 20160509
    #define ALCD_BL_ON()       PORTD_DIR &= ~BIT(0);PORTD_OUT |= BIT(0)
    #define ALCD_BL_OFF()      PORTD_DIR &= ~BIT(0);PORTD_OUT &= ~BIT(0)

#else
    #define ALCD_BL_ON()     PORTH_DIR &= ~BIT(11);PORTH_OUT |= BIT(11)
    #define ALCD_BL_OFF()    PORTH_DIR &= ~BIT(11);PORTH_OUT &= ~BIT(11)
#endif

#define AVOUT_DET_IO_INIT()    PORTF_DIR |= BIT(0);PORTF_PU |= BIT(0)
#define AVOUT_DET_IO_STATUS()  ((PORTF_IN & BIT(0)) ? FALSE : TRUE )


//#if LCD_43
//    #define LCD_W       480//TFT_WIDTH//480
//    #define LCD_H       272//TFT_HIGHT//272
//    #define TFT_LCD     0
//    #define NEW_LCD     1
//#endif
//
//#if LCD_C6
//    #define LCD_W       320//480//TFT_WIDTH//480
//    #define LCD_H       240//272//TFT_HIGHT//272
//    #define TFT_LCD    1
//    #define NEW_LCD    0
//#endif

#if 1 //AVOUT JOHNSON 201160530 ADD
extern  u16 LCD_W;
extern  u16 LCD_H;
#define TFT_LCD    1
#define NEW_LCD    0
u16 LCD_MODE ;
#endif

//void lcd_init();  //old
void lcd_init(u8 mode); //AVOUT JOHNSON 20160530
u32 lcd_get_size(u16 * width, u16 * heigh);
u32 lcd_get_bottom_color();
void lcd_pwm_init(void);
void lcd_brightness_control(u8 level);
#endif

