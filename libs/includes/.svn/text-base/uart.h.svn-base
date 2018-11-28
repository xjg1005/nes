#ifndef _UART_
#define _UART_

#include "typedef.h"





extern  int init_uart_mutex();
extern  void uart_init (void);


//#if UART_DEBUG_EN

extern  int putbyte(char a);
extern  void  putchar(char a);

extern  void put_u4hex(u8 dat);
extern  void put_u32hex(u32 dat);
extern  void put_u16hex(u16 dat);
extern  void put_u32hex0(u32 dat);
extern  void put_u8hex0(u8 dat);
extern  void put_u8hex(u8 dat);

extern  void put_buf(u8 *buf, u32 len );
extern  void put_buf_without_mutex(u8 *buf, u32 len );
extern  void printf_buf(u8 *buf, u32 len );
extern  void printf_buf_without_mutex(u8 *buf, u32 len );

extern  int puts(char *out);
extern  int puts_without_mutex(char *out);
extern int puts_without_lock(char *out);


//#else
//
//#define  putbyte
//#define  putchar
//
//#define put_u4hex
//#define put_u32hex
//#define put_u16hex
//#define put_u32hex0
//#define put_u32hex
//#define put_u8hex0
//#define put_u8hex
//
//#define put_buf
//#define put_buf_without_mutex
//#define printf_buf
//#define printf_buf_without_mutex
//
//#define puts
//#define puts_without_mutex
//#define puts_without_lock
//
//#endif


#endif

