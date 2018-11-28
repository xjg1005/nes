#ifndef TIMER_H
#define TIMER_H

#include "typedef.h"


#include "list.h"
typedef struct timer_work{
    struct list_head list;
	unsigned int time;
	void * param;
	void (*handler)(void * param);
	u32 cnt;
}timer_work_t, *timer_handle;

void timer0_init();

timer_handle  timer_reg(u32 time ,void (*handler)(void *parm) ,void *parm);
void timer_init() ;
void timer_unreg(timer_handle timer) ;
void timer_delay_2ms(u32 cnt) ;
u32 os_get_time();
#endif

