#ifndef __DEBUG_H__
#define __DEBUG_H__
#include "typedef.h"

#define    DEBUG_SD0	(0x00)
#define    DEBUG_SD1	(0x01)


#define    DEBUG_SPI0	(0x04)
#define    DEBUG_SPI1	(0x05)


#define    DEBUG_UART0WR	(0x08)
#define    DEBUG_UART0RD	(0x09)

#define    DEBUG_UART1WR	(0x0a)
#define    DEBUG_UART1RD	(0x0b)

#define    DEBUG_ALNK		(0x10)
#define    DEBUG_AUDIO		(0x11)
#define    DEBUG_PAP		(0x12)
#define    DEBUG_IIC		(0x13)
#define    DEBUG_CHECKSUM	(0x14)
#define    DEBUG_FUSB		(0x20)
#define    DEBUG_HUSB		(0x21)
#define    DEBUG_IMC_CH0	(0x80)
#define    DEBUG_IMC_CH1	(0x81)
#define    DEBUG_IMC_SRC	(0x82)
#define    DEBUG_IMB		(0x83)
#define    DEBUG_JPG0		(0x84)
#define    DEBUG_JPG1		(0x85)

#define    DEBUG_CPU		(0xfe)

#define    DEBUG_INVALD		(0xff)


/*
注意：不可以使用一个窗口选择一片内存允许外设A写，同时使用另一个窗口禁止外设B写。

    limit_index:
        debug窗口索引
    low_addr:
        窗口的低地址
    high_addr:
        窗口的高地址
    is_allow_write:
        1--指定外设写窗口内部不触发异常
        0--任意外设写窗口内部触发异常
        当参数【is_allow_write】为0的时候，参数【dev】无效
    dev:
        指定外设的编号
 */
void dev_write_range_limit(u32 limit_index, //debug窗口索引
                           void*low_addr,   //范围内的低地址
                           void *high_addr, //窗口的高地址
                           u32 is_allow_write,  //1--指定外设写窗口内部不触发异常 0--任意外设写窗口内部触发异常 当参数【is_allow_write】为0的时候，参数【dev】无效
                           u32 dev  //指定外设的编号
                          );

void cpu_write_range_limit(u32 limit_index,void*low_addr,void *high_addr);

void pc_rang_limit(void*low_addr,void *high_addr);

void exception_isr(void);

#endif

