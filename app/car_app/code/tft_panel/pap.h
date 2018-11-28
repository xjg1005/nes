#ifndef _PAP_H_
#define _PAP_H_

#include "typedef.h"
#include "dv12.h"

#define PAP_8BIT_MODE_EN	      1//0:16BIT 1:8BIT(DB[10..17] <-> PG[8..15])
#define DW_16BIT                  0//1//8bit模式有效  0:8BIT 1:16BIT(8BIT读写两次)
#define PAP_16BIT_ENDIAN          0//16bit模式有效 1:BIG_ENDIAN 0:SMALL_ENDIAN
								   //Little Endian 0:DB[0..7] <-> PG[0..7],  DB[8..15] <-> PG[8..15]
								   //Big Endian    1:DB[0..7] <-> PG[8..15], DB[8..15] <-> PG[0..7]

#define PAP_WAIT()			do{while(!(PAP_CON&BIT(7)));PAP_CON |= BIT(6);}while(0)//等PND并清PND
#define PAP_WAIT_NCLR()     do{while(!(PAP_CON&BIT(7)));}while(0)   //等PND但不清PND

#define PAPDEN  14  //PAP接口数据信号引脚(PGx)使能
#define PAPREN  13  //PAP接口读信号引脚(PH4)使能
#define PAPWEN  12  //PAP接口写信号引脚(PH3)使能

#define PAP_PORT_EN()   IOMC0 |= ((1L<<PAPDEN)|(1L<<PAPREN)|(1L<<PAPWEN))   //使能PAP接口
#define PAP_RD_OUT()	//PORTH_DIR &= ~BIT(4)
#define PAP_WR_OUT()	PORTH_DIR &= ~BIT(3)

#define PAP_EXT_EN()    PAP_CON |= BIT(16)      //使能PAP扩展模式
#define PAP_EXT_DIS()   PAP_CON &= ~BIT(16)     //禁止PAP扩展模式
#define PAP_EXT_M2L()   PAP_CON |= BIT(17)  //扩展模式顺序MSB到LSB
#define PAP_EXT_L2M()   PAP_CON &= ~BIT(17) //扩展模式顺序LSB到MSB
#define PAP_IE(x)       PAP_CON = (PAPCON & ~BIT(18)) | ((x & 0x1)<<18)


#if PAP_8BIT_MODE_EN
    #define		PAP_DIR_O()			do{PORTG_DIR &= 0x00ff;PAP_CON &=~BIT(1);}while(0)
    #define		PAP_DIR_I()			do{PORTG_DIR |= 0xff00;PAP_CON |= BIT(1);}while(0)
#else
    #define		PAP_DIR_O()			do{PORTG_DIR &= 0x0000;PAP_CON &=~BIT(1);}while(0)
    #define		PAP_DIR_I()			do{PORTG_DIR |= 0xffff;PAP_CON |= BIT(1);}while(0)
#endif

extern void pap_init();
extern void pap_write(u16 w);
extern u16  pap_read();
extern void pap_dma_read(void *pBuf, u16 len);
extern void pap_dma_write(void *pBuf, u16 len);

#endif
