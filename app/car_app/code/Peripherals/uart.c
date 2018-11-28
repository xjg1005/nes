#include "clock.h"
#include "os_api.h"
#include "uart.h"
#include "app_cfg.h"





#define UART0   0
#define UART1   1
#define UART2   2
#define UART3   3

#ifdef DOUBLE_VEDIO
    //#define DEBUG_UART  UART1
    #define DEBUG_UART  UART2 //5203 JOHNSON 20160509
#else
    #define DEBUG_UART  UART2
#endif


static OS_MUTEX    uart_mutex;
#define UART_WAIT_MUTEX_TIMEOUT       500

int init_uart_mutex()
{
    int err;
    err = os_mutex_create(&uart_mutex);
	if(err != OS_ERR_NONE)
	{
	    printf("create uart mutex err =%d \r\n",err) ;
	    return -1 ;
	}
	return 0 ;
}

inline void printf_enter_mutex(void)
{
//    int err;
//    err = os_mutex_pend(&uart_mutex,UART_WAIT_MUTEX_TIMEOUT,0);
}
inline void printf_exit_mutex(void)
{
//    int err;
//    err = os_mutex_post(&uart_mutex);
}


void uart_init (void)
{

#if UART_DEBUG_EN

    CLK_CON1 |= BIT(5);

#if ( DEBUG_UART == UART0 )

    IOMC3 |= BIT(3) ;//占用相应的IO
    IOMC0 &= ~(BIT(6)|BIT(7)) ;//0 : PC0/PC1 1:PG6/PG7 2:PH12/PH13 3:PD4/PD5


//    PORTC_DIR &= ~BIT(0);
//    PORTC_DIR |= BIT(1);

//    IOMC0 |= BIT(6);
//    PORTG_DIR &= ~BIT(6);
//    PORTG_DIR |= BIT(7);

    IOMC0 |= BIT(7);
    PORTH_DIR &= ~BIT(12);
    PORTH_DIR |= BIT(13);

//    IOMC0 |= (BIT(7) | BIT(6) );
//    PORTD_DIR &= ~BIT(4);
//    PORTD_DIR |= BIT(5);

    UT0_BAUD = (get_apb_clk() / 115200) / 4 - 1;
    UT0_CON =  BIT(0);
#endif


#if ( DEBUG_UART == UART1 )

    IOMC3 |= BIT(7) ;//占用相应的IO
    IOMC1 &= ~(BIT(2)|BIT(3)) ;//0 : PC2/PC3 1:PH2/PH5 2:PH14/PH15 3:PC6/PC7

    PORTC_DIR &= ~BIT(2);  //5203 JOHNSON 20160509
//    PORTC_DIR |= BIT(3);

//    IOMC1 |= BIT(2);
//    PORTH_DIR &= ~BIT(2);
//    PORTH_DIR |= BIT(5);

    //IOMC1 |= BIT(3);   //5203 JOHNSON 20160509
    //PORTH_DIR &= ~BIT(14);
    //PORTH_DIR |= BIT(15);

//    IOMC1 |= (BIT(2) | BIT(3));
//    PORTC_DIR &= ~BIT(6);
//    PORTC_DIR |= BIT(7);

    UT1_BAUD = (get_apb_clk() / 115200) / 4 - 1;
    UT1_CON =  BIT(0);

#endif

#if ( DEBUG_UART == UART2 )

//    IOMC3 |= BIT(11) ;//占用相应的IO
	IOMC3 &= ~(BIT(12)|BIT(13)) ;//0 : PC4/PC5 1:PF0/PF3 2:PD0/PD1 3:PH0/PH1

//    PORTC_DIR &= ~BIT(4);
//    PORTC_DIR |= BIT(5);

    //IOMC3 |= BIT(12);     //5203 JOHNSON 20160509
    //PORTF_DIR &= ~BIT(0);
    //PORTF_DIR |= BIT(3);

//    IOMC3 |= BIT(13);
//    PORTD_DIR &= ~BIT(0);
//    PORTD_DIR |= BIT(1);

    IOMC3 |= (BIT(12) | BIT(13));  //5203 JOHNSON 20160509
    PORTH_DIR &= ~BIT(0);
//    PORTH_DIR |= BIT(1);

    UT2_BAUD = (get_apb_clk() / 115200) / 4 - 1;
    UT2_CON =  BIT(0);

#endif

#if ( DEBUG_UART == UART3 )

    IOMC0 |= BIT(21) ;//占用相应的IO
    IOMC3 &= ~(BIT(14)|BIT(15)) ;//0 : PA7/PA8 1:PB14/PB15 2:PC14/PC15 3:PH10/PH11

    PORTA_DIR &= ~BIT(7);
    PORTA_DIR |= BIT(8);

//    IOMC3 |= BIT(14);
//    PORTB_DIR &= ~BIT(14);
//    PORTB_DIR |= BIT(15);

//    IOMC3 |= BIT(15);
//    PORTC_DIR &= ~BIT(14);
//    PORTC_DIR |= BIT(15);

//    IOMC3 |= (BIT(14) | BIT(15));
//    PORTH_DIR &= ~BIT(10);
//    PORTH_DIR |= BIT(11);

    UT3_BAUD = (get_apb_clk() / 115200) / 4 - 1;
    UT3_CON =  BIT(0);

#endif


#endif


}



#if UART_DEBUG_EN


inline int putbyte(char a)
{

#if ( DEBUG_UART == UART0 )
    if(a == '\n'){
        UT0_BUF = '\r';
        while((UT0_CON & BIT(15)) == 0);
    }
    UT0_BUF = a;
    while((UT0_CON & BIT(15)) == 0);     //TX IDLE
#endif

#if ( DEBUG_UART == UART1 )

    if(a == '\n'){
        UT1_BUF = '\r';
        while((UT1_CON & BIT(15)) == 0);
    }
    UT1_BUF = a;
    while((UT1_CON & BIT(15)) == 0);     //TX IDLE
#endif

#if ( DEBUG_UART == UART2 )

    if(a == '\n'){
        UT2_BUF = '\r';
        while((UT2_CON & BIT(15)) == 0);
    }
    UT2_BUF = a;
    while((UT2_CON & BIT(15)) == 0);     //TX IDLE
#endif

#if ( DEBUG_UART == UART3 )

    if(a == '\n'){
        UT3_BUF = '\r';
        while((UT3_CON & BIT(15)) == 0);
    }
    UT3_BUF = a;
    while((UT3_CON & BIT(15)) == 0);     //TX IDLE
#endif




}

char getchar()
{
    char c=0;

#if ( DEBUG_UART == UART0 )

    if((UT0_CON & BIT(14))){
		c = UT0_BUF;
		UT0_CON |= BIT(12);
	}
#endif

#if ( DEBUG_UART == UART1 )

    if((UT1_CON & BIT(14))){
		c = UT1_BUF;
		UT1_CON |= BIT(12);
	}
#endif

#if ( DEBUG_UART == UART2 )

    if((UT2_CON & BIT(14))){
		c = UT2_BUF;
		UT2_CON |= BIT(12);
	}
#endif

#if ( DEBUG_UART == UART3 )

    if((UT3_CON & BIT(14))){
		c = UT3_BUF;
		UT3_CON |= BIT(12);
	}
#endif

	return c;
}

 void putchar(char a)
{
    if(a == '\n')
    {
        putbyte(0x0d);
        putbyte(0x0a);
    }
    else
        putbyte(a);
}

void put_u4hex(u8 dat)
{
    dat = 0xf & dat;
    if(dat>9)
    {
        putbyte(dat-10 + 'A');
    }
    else
    {
        putbyte(dat + '0');
    }
}

void put_u32hex(u32 dat)
{
    putbyte('0');
    putbyte('x');
    put_u4hex(dat>>28);
    put_u4hex(dat>>24);

    put_u4hex(dat>>20);
    put_u4hex(dat>>16);

    put_u4hex(dat>>12);
    put_u4hex(dat>>8);

    put_u4hex(dat>>4);
    put_u4hex(dat);
    putchar('\n');
}

void put_u16hex(u16 dat)
{
    putbyte('0');
    putbyte('x');


    put_u4hex(dat>>12);
    put_u4hex(dat>>8);

    put_u4hex(dat>>4);
    put_u4hex(dat);
    putchar('\n');
}

void put_u32hex0(u32 dat)
{
    putbyte('0');
    putbyte('x');
    put_u4hex(dat>>28);
    put_u4hex(dat>>24);

    put_u4hex(dat>>20);
    put_u4hex(dat>>16);

    put_u4hex(dat>>12);
    put_u4hex(dat>>8);

    put_u4hex(dat>>4);
    put_u4hex(dat);
    putchar(' ');
}

void put_u8hex0(u8 dat)
{
    putbyte('0');
    putbyte('x');
    put_u4hex(dat>>4);
    put_u4hex(dat);
    putchar(' ');
}

void put_u8hex(u8 dat)
{
    put_u4hex(dat>>4);
    put_u4hex(dat);
    putchar(' ');
}

void put_buf(u8 *buf, u32 len )
{
    u32 i ;

    printf_enter_mutex();

    for(i = 0 ; i < len ; i++)
    {
        if((i%16)==0)
        {
            putbyte('\n') ;
        }
        put_u8hex(buf[i]) ;
    }

  	printf_exit_mutex();
}

void put_buf_without_mutex(u8 *buf, u32 len )
{
    u32 i ;

    for(i = 0 ; i < len ; i++)
    {
        if((i%16)==0)
        {
            putbyte('\n') ;
        }
        put_u8hex(buf[i]) ;
    }
}

inline void printf_buf(u8 *buf, u32 len )
{
    put_buf(buf,len);
}

inline void printf_buf_without_mutex(u8 *buf, u32 len )
{
    u32 i ;

    for(i = 0 ; i < len ; i++)
    {
        if((i%16)==0)
        {
            putbyte('\n') ;
        }
        put_u8hex(buf[i]) ;
    }
}

int puts(char *out)
{
    printf_enter_mutex();

	while(*out != '\0')
	{
		putchar(*out);
		out++;
	}
	printf_exit_mutex();

	return 0;
}
void perror(const char *out)
{
    puts("perror: ");
    printf_enter_mutex();

	while(*out != '\0')
	{
		putchar(*out);
		out++;
	}
	printf_exit_mutex();
}
int puts_without_mutex(char *out)
{
	while(*out != '\0')
	{
		putchar(*out);
		out++;
	}
	return 0;
}

int puts_without_lock(char *out)
{
    return puts_without_mutex(out);
}



#else

inline int putbyte(char a)
{
}
void  putchar(char a)
{
}
char getchar()
{
}
void put_u4hex(u8 dat)
{
}
void put_u32hex(u32 dat)
{
}
void put_u16hex(u16 dat)
{
}
void put_u32hex0(u32 dat)
{
}
void put_u8hex0(u8 dat)
{
}
void put_u8hex(u8 dat)
{
}
void put_buf(u8 *buf, u32 len )
{
}
void perror(const char *out)
{
}
void put_buf_without_mutex(u8 *buf, u32 len )
{
}
inline void printf_buf(u8 *buf, u32 len )
{
}
inline void printf_buf_without_mutex(u8 *buf, u32 len )
{
}
int puts(char *out)
{
}
int puts_without_mutex(char *out)
{
}
int puts_without_lock(char *out)
{
}


#endif


