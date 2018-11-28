/*--------------------------------------------------------------------------*/
/**@file     iic.h
   @brief    IICм╥нд╪Ч
   @details
   @author
   @date   2010-12-13
   @note
*/
/*----------------------------------------------------------------------------*/

#ifndef _IIC_H_
#define _IIC_H_

#include "typedef.h"
#include "dv12.h"


//#define iic_data_out()    PORTG_DIR &=~ BIT(14);PORTG_PU |= BIT(14);PORTG_PD &=~BIT(14);
//#define iic_data_in()     PORTG_DIR |= BIT(14);PORTG_PU |= BIT(14);PORTG_PD &=~BIT(14);
//
//#define iic_data_read()   PORTG_IN & BIT(14)
//#define iic_data_h()      do{iic_data_out();PORTG_OUT |= BIT(14);}while(0)
//#define iic_data_l()      do{iic_data_out();PORTG_OUT &=~BIT(14);}while(0)
//
//#define iic_clk_out()     PORTG_DIR &=~ BIT(15);PORTG_PU |= BIT(15);PORTG_PD &=~BIT(15);
//#define iic_clk_h()       do{iic_clk_out();PORTG_OUT |= BIT(15);}while(0)
//#define iic_clk_l()       do{iic_clk_out();PORTG_OUT &=~BIT(15);}while(0)


//SENSOE_IIC不用复用 5203 JOHNSON 20160509
#define iic_data_out()    //PORTA_DIR &=~ BIT(1);PORTA_PU |= BIT(1);PORTA_PD &=~BIT(1);
#define iic_data_in()     //PORTA_DIR |= BIT(1);PORTA_PU |= BIT(1);PORTA_PD &=~BIT(1);

#define iic_data_read()   0//PORTA_IN & BIT(1)
#define iic_data_h()      //do{iic_data_out();PORTA_OUT |= BIT(1);}while(0)
#define iic_data_l()      //do{iic_data_out();PORTA_OUT &=~BIT(1);}while(0)

#define iic_clk_out()     //PORTA_DIR &=~ BIT(2);PORTA_PU |= BIT(2);PORTA_PD &=~BIT(2);
#define iic_clk_h()       //do{iic_clk_out();PORTA_OUT |= BIT(2);}while(0)
#define iic_clk_l()       //do{iic_clk_out();PORTA_OUT &=~BIT(2);}while(0)

#define iic_delay()      delay(300)

void iic_init_io(void);
void iic_start(void);
void iic_stop(void);
u8 r_ack(void);
void s_ack(u8 flag);
u8 iic_revbyte_io( void );
u8 iic_revbyte( u8 para );
void iic_sendbyte_io(u8 byteI2C);
void iic_sendbyte(u8 byte);

void  iic_writen(u8 chip_id,u8 iic_addr,u8 *iic_dat,u8 n);
void  iic_write(u8 chip_id,u8 iic_addr,u8 dat);
void iic_readn(u8 chip_id,u8 iic_addr,u8 *iic_dat,u16 n);
u8 iic_read(u8 chip_id,u8 iic_addr);



#endif
