#ifndef _MANGO_DEV_UART_H_
#define _MANGO_DEV_UART_H_

#include "typedef.h"
#include "ioctl.h"
#include "device_drive.h"

enum uart_clk_src
{
    LSB_CLK,
    OSC_CLK,
    PLL_48M,
}uart_clk_src;

enum _uart_port_out
{
    //uart0
    PORTC_0_1 = 0x00001000,
    PORTG_6_7 = 0x00002000,
    PORTH_12_13 = 0x00003000,
    PORTD_4_5 = 0x00004000,
    //uart1
    PORTC_2_3 = 0x00005000,
    PORTH_2_5 = 0x00006000,
    PORTH_14_15 = 0x00007000,
    PORTC_6_7 = 0x00008000,
};

typedef struct _uart_init_typedef
{
    enum _uart_port_out port;
    u32 baudrate;
    u32  circular_buffer_addr;
    u16  circular_buffer_lenth;
    u16 max_continue_recv_cnt;
    u32 idle_sys_clk_cnt;
    enum uart_clk_src clk_src;
}uart_init_typedef;

enum
{
    UART_CIRCULAR_BUFFER_WRITE_OVERLAY = -1,
    UART_RECV_TIMEOUT = -2,
    UART_RECV_EXIT = -3,
};

extern const struct DEV_IO * dev_reg_uart0(void *parm);
extern const struct DEV_IO * dev_reg_uart1(void *parm);

#define UART_MAGIC                          'U'
#define UART_FLUSH                          _IO(UART_MAGIC,1)
#define UART_SET_RECV_ALL                   _IOW(UART_MAGIC,2,bool)
#define UART_SET_RECV_BLOCK                 _IOW(UART_MAGIC,3,bool)
#define UART_SET_RECV_TIMEOUT               _IOW(UART_MAGIC,4,u32)
#define UART_SET_RECV_TIMEOUT_CB            _IOW(UART_MAGIC,5,int (*)(void))

#endif //_MANGO_DEV_UART0_H_

