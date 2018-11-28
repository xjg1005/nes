

#ifndef _I2C_H
#define _I2C_H

#include "dv12.h"
#include "typedef.h"
#include "app_cfg.h"

extern void delay(unsigned int  t);

#if 0
#define I2C_DELAY()    delay(2000)
#else
#define I2C_DELAY()    delay(200)
#endif

#ifdef DOUBLE_VEDIO
  #if 0 //old
    #define A_SDA_H()  do{PORTC_DIR &= ~BIT(3); PORTC_OUT |= BIT(3);}while(0)    //数据线高电平
    #define A_SDA_L()  do{PORTC_DIR &= ~BIT(3); PORTC_OUT &= ~BIT(3);}while(0)
    #define A_SCL_H()  do{PORTC_DIR &= ~BIT(2); PORTC_OUT |= BIT(2);}while(0)
    #define A_SCL_L()  do{PORTC_DIR &= ~BIT(2); PORTC_OUT &= ~BIT(2);}while(0)

    #define A_SDA_PU() PORTC_PU |= BIT(3)  //配置内部上来
    #define A_SCL_PU() PORTC_PU |= BIT(2)

    #define A_SDA_IN()      PORTC_DIR |= BIT(3)
    #define A_SDA_OUT()     PORTC_DIR &= ~BIT(3)
    #define A_SCL_OUT()     PORTC_DIR &= ~BIT(2)
    #define A_SDA_STATE()   ((PORTC_IN & BIT(3)) >> 3)

    #define SET_SDA_H()     PORTC_OUT |= BIT(3)
    #define SET_SDA_L()     PORTC_OUT &= ~BIT(3)
  #else //5203 JOHNSON 20160509
    #define A_SDA_H()  do{PORTA_DIR &= ~BIT(1); PORTA_OUT |= BIT(1);}while(0)    //数据线高电平
    #define A_SDA_L()  do{PORTA_DIR &= ~BIT(1); PORTA_OUT &= ~BIT(1);}while(0)
    #define A_SCL_H()  do{PORTA_DIR &= ~BIT(2); PORTA_OUT |= BIT(2);}while(0)
    #define A_SCL_L()  do{PORTA_DIR &= ~BIT(2); PORTA_OUT &= ~BIT(2);}while(0)

    #define A_SDA_PU() PORTA_PU |= BIT(1)  //配置内部上来
    #define A_SCL_PU() PORTA_PU |= BIT(2)

    #define A_SDA_IN()      PORTA_DIR |= BIT(1)
    #define A_SDA_OUT()     PORTA_DIR &= ~BIT(1)
    #define A_SCL_OUT()     PORTA_DIR &= ~BIT(2)
    #define A_SDA_STATE()   ((PORTA_IN & BIT(1)) >> 1)

    #define SET_SDA_H()     PORTA_OUT |= BIT(1)
    #define SET_SDA_L()     PORTA_OUT &= ~BIT(1)
  #endif


//    #define A_INT_IN()      PORTH_DIR |= BIT(13)//; PORTI_PU |=  BIT(1);PORTI_PD &= ~ BIT(1)
//    #define A_INT_OUT()     PORTH_DIR &= ~BIT(13)//; PORTI_PU &= ~ BIT(1);PORTI_PD &= ~ BIT(1)
//    #define A_INT_STATE()   ((PORTH_IN & BIT(13)) >> 13)
//    #define SET_INT_H()     PORTH_OUT |= BIT(13)
//    #define SET_INT_L()     PORTH_OUT &= ~BIT(13)

#else
    #define A_SDA_H()  do{PORTC_DIR &= ~BIT(3); PORTC_OUT |= BIT(3);}while(0)    //数据线高电平
    #define A_SDA_L()  do{PORTC_DIR &= ~BIT(3); PORTC_OUT &= ~BIT(3);}while(0)
    #define A_SCL_H()  do{PORTC_DIR &= ~BIT(4); PORTC_OUT |= BIT(4);}while(0)
    #define A_SCL_L()  do{PORTC_DIR &= ~BIT(4); PORTC_OUT &= ~BIT(4);}while(0)

    #define A_SDA_PU() PORTC_PU |= BIT(3)  //配置内部上来
    #define A_SCL_PU() PORTC_PU |= BIT(4)

    #define A_SDA_IN()      PORTC_DIR |= BIT(3)
    #define A_SDA_OUT()     PORTC_DIR &= ~BIT(3)
    #define A_SCL_OUT()     PORTC_DIR &= ~BIT(4)
    #define A_SDA_STATE()   ((PORTC_IN & BIT(3)) >> 3)

    #define SET_SDA_H()     PORTC_OUT |= BIT(4)
    #define SET_SDA_L()     PORTC_OUT &= ~BIT(4)


//    #define A_INT_IN()      PORTH_DIR |= BIT(13)//; PORTI_PU |=  BIT(1);PORTI_PD &= ~ BIT(1)
//    #define A_INT_OUT()     PORTH_DIR &= ~BIT(13)//; PORTI_PU &= ~ BIT(1);PORTI_PD &= ~ BIT(1)
//    #define A_INT_STATE()   ((PORTH_IN & BIT(13)) >> 13)
//    #define SET_INT_H()     PORTH_OUT |= BIT(13)
//    #define SET_INT_L()     PORTH_OUT &= ~BIT(13)
#endif


extern void init_i2c_io(void);
extern void start_I2c(void);
extern void stop_I2c(void);
extern u8 wait_Ack(void);
extern void Ack(void);
extern void no_Ack(void);
extern u8 write_byte_to_slave(u8 m_data);
extern u8 read_byte_from_slave(void);

#endif
