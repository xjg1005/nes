/*******************************************************************************************
 File Name: iic.h

 Version: 1.00

 Discription:  IIC 驱动

 Author:yulin deng

 Email :flowingfeeze@163.com

 Date:星期五, 四月 19 2013

 Copyright:(c)JIELI  2011  @ , All Rights Reserved.
*******************************************************************************************/
#ifndef iic_h

#define  iic_h
#include "config.h"

//第一个软件IIC接口
#define SCCB_SID_H_A()   do{PORTB_DIR &= ~BIT(14); PORTB_PU |= BIT(14); PORTB_OUT |= BIT(14);}while(0)    //数据线高电平
#define SCCB_SID_L_A()   do{PORTB_DIR &= ~BIT(14); PORTB_PU |= BIT(14); PORTB_OUT &= ~BIT(14);}while(0)
#define SCCB_SIC_H_A()   do{PORTB_DIR &= ~BIT(12); PORTB_PU |= BIT(12); PORTB_OUT |= BIT(12);}while(0)
#define SCCB_SIC_L_A()   do{PORTB_DIR &= ~BIT(12); PORTB_PU |= BIT(12); PORTB_OUT &= ~BIT(12);}while(0)


#define SCCB_SID_IN_A    PORTB_DIR |= BIT(14);PORTB_PU |= BIT(14)
#define SCCB_SID_OUT_A   PORTB_DIR &= ~BIT(14);PORTB_PU |= BIT(14)
#define SCCB_SIC_OUT_A   PORTB_DIR &= ~BIT(12);PORTB_PU |= BIT(12)
#define SCCB_SID_STATE_A (PORTB_IN & BIT(14))

#define SET_SID_H_A()    PORTB_OUT |= BIT(14)
#define SET_SID_L_A()    PORTB_OUT &= ~BIT(14)
#define SCCB_SID_PU_A    PORTB_PU |= BIT(14)

                           //5203 JOHNSON 20160509
#define SET_RESET_OUT_A()  //PORTD_DIR &=~BIT(1)
#define SET_RESET_H_A()    DAA_CON0 |= BIT(2)//PORTD_OUT |=BIT(1)
#define SET_RESET_L_A()    DAA_CON0 &=~ BIT(2)//PORTD_OUT &=~BIT(1)


//#define SCCB_SID_H_A()   do{PORTD_DIR &= ~BIT(5); PORTD_OUT |= BIT(5);PORTD_HD |= BIT(5);}while(0)    //数据线高电平
//#define SCCB_SID_L_A()   do{PORTD_DIR &= ~BIT(5); PORTD_OUT &= ~BIT(5);PORTD_HD |= BIT(5);}while(0)
//#define SCCB_SIC_H_A()   do{PORTD_DIR &= ~BIT(4); PORTD_OUT |= BIT(4);}while(0)
//#define SCCB_SIC_L_A()   do{PORTD_DIR &= ~BIT(4); PORTD_OUT &= ~BIT(4);}while(0)
//
//
//#define SCCB_SID_IN_A    PORTD_DIR |= BIT(5)
//#define SCCB_SID_OUT_A   PORTD_DIR &= ~BIT(5)
//#define SCCB_SIC_OUT_A   PORTD_DIR &= ~BIT(4)
//#define SCCB_SID_STATE_A (PORTD_IN & BIT(5))
//
//#define SET_SID_H_A()    PORTD_OUT |= BIT(5)
//#define SET_SID_L_A()    PORTD_OUT &= ~BIT(5)
//#define SCCB_SID_PU_A    PORTD_PU |= BIT(5)
//
//#define SET_RESET_OUT_A()  PORTD_DIR &=~BIT(2)
//#define SET_RESET_H_A()    PORTD_OUT |=BIT(2)
//#define SET_RESET_L_A()    PORTD_OUT &=~BIT(2)


//第二个软件IIC接口

                         //5203 JOHNSON 20160509
#define SCCB_SID_H_B()   //do{PORTC_DIR &= ~BIT(3); PORTC_PU |= BIT(3); PORTC_OUT |= BIT(3);}while(0)    //数据线高电平
#define SCCB_SID_L_B()   //do{PORTC_DIR &= ~BIT(3); PORTC_PU |= BIT(3); PORTC_OUT &= ~BIT(3);}while(0)
#define SCCB_SIC_H_B()   //do{PORTC_DIR &= ~BIT(2); PORTC_PU |= BIT(2); PORTC_OUT |= BIT(2);}while(0)
#define SCCB_SIC_L_B()   //do{PORTC_DIR &= ~BIT(2); PORTC_PU |= BIT(2); PORTC_OUT &= ~BIT(2);}while(0)

#define SCCB_SID_IN_B    //PORTC_DIR |= BIT(3);PORTC_PU |= BIT(3)
#define SCCB_SID_OUT_B   //PORTC_DIR &= ~BIT(3);PORTC_PU |= BIT(3)
#define SCCB_SID_STATE_B  0//(PORTC_IN & BIT(3))
#define SCCB_SIC_OUT_B   //PORTC_DIR &= ~BIT(2);PORTC_PU |= BIT(2)

#define SET_SID_H_B()    //PORTC_OUT |= BIT(3)
#define SET_SID_L_B()    //PORTC_OUT &= ~BIT(3)

#define SET_RESET_OUT_B()  //PORTC_DIR &=~BIT(4)
#define SET_RESET_H_B()    //PORTC_OUT |=BIT(4)
#define SET_RESET_L_B()    //PORTC_OUT &=~BIT(4)


//#define SCCB_SID_H_B()   do{PORTA_DIR &= ~BIT(5); PORTA_OUT |= BIT(5);}while(0)    //数据线高电平
//#define SCCB_SID_L_B()   do{PORTA_DIR &= ~BIT(5); PORTA_OUT &= ~BIT(5);}while(0)
//#define SCCB_SIC_H_B()   do{PORTA_DIR &= ~BIT(6); PORTA_OUT |= BIT(6);}while(0)
//#define SCCB_SIC_L_B()   do{PORTA_DIR &= ~BIT(6); PORTA_OUT &= ~BIT(6);}while(0)
//
//#define SCCB_SID_IN_B    PORTA_DIR |= BIT(5)
//#define SCCB_SID_OUT_B   PORTA_DIR &= ~BIT(5)
//#define SCCB_SID_STATE_B (PORTA_IN & BIT(5))
//#define SCCB_SIC_OUT_B   PORTA_DIR &= ~BIT(6)
//
//#define SET_SID_H_B()    PORTA_OUT |= BIT(5)
//#define SET_SID_L_B()    PORTA_OUT &= ~BIT(5)
//
//#define SET_RESET_OUT_B()  PORTA_DIR &=~BIT(9)
//#define SET_RESET_H_B()    PORTA_OUT |=BIT(9)
//#define SET_RESET_L_B()    PORTA_OUT &=~BIT(9)




typedef struct {
    void    (* startSCCB)(void);
    void    (* stopSCCB)(void);
    u8      (* Wait_Ack)(void);
    void    (* Ack)(void);
    void    (* noAck)(void);
    unsigned char    (* SCCBwriteByte)(unsigned char m_data);
    unsigned char    (* SCCBreadByte)(void);
} S_IIC_FUN;

extern const S_IIC_FUN iic_fun[2];

extern void iic_ini (void) ;
extern void iic_tx (u8 dat);
extern void iic_tx_ws (u8 dat) ;
extern void iic_tx_we (u8 dat) ;
extern u8 iic_rx (void) ;
extern u8 iic_rx_ws (void);
extern u8 iic_rx_we (void);
void   iic_write_dma(u8 *buf, u16 len);


#endif
