#include "config.h"
#include "timer.h"
#include "hwi.h"
#include "msg.h"
#include "os_api.h"
#include "clock.h"
#include "irtc.h"
#include "list.h"

extern volatile u32 delay_cnt;

static timer_work_t time_list ;


void timer1_isr() ;
void timer2_isr();

static u32 timer1_cnt = 0;
static u32 timer2_cnt = 0;

inline u32 OSGetTime(void)
{
    return timer1_cnt/5 ;
}
inline u32 get_os_time(void)
{
    return timer1_cnt/5 ;
}

inline u32 timer_get_sec(void)
{
    return timer1_cnt*T1_PRD/get_apb_clk();
}

inline u32 timer_get_ms(void)
{
    return timer1_cnt*(1000*T1_PRD/get_apb_clk());
}

void timer1_init()
{
	INIT_LIST_HEAD(&time_list.list) ;
    INTALL_HWI(TIME1_INIT, timer1_isr, 1) ; //timer0_isr
    T1_CNT = 0;
    T1_PRD = get_apb_clk() / 500; //2ms
    T1_CON = BIT(0);
}


void timer2_init()
{
    INTALL_HWI(TIME2_INIT, timer2_isr, 2) ; //timer2_isr

    T2_CNT = 0;
    T2_PRD = get_apb_clk() / 500; //2ms
    T2_CON = BIT(0);

}

void timer2_isr()
{
    T2_CON |= BIT(14);

    ++timer2_cnt;

    DSP_CLR_ILAT = BIT(TIME2_INIT);
}
REG_INIT_HANDLE(timer2_isr);

 void timer1_isr(void)
{
    T1_CON |= BIT(14);

    ++timer1_cnt;

    if (delay_cnt > 0)
    delay_cnt--;

#if 0
    if(timer1_cnt%(500*10) == 0)
    {
        u32 reti_addr;
        asm("%0 = RETI ;" : "=r"(reti_addr));

        char *task_name = os_get_currnet_name();

        if(strcmp(task_name, "idle"))
        {
            puts_without_mutex("ctk: ");
            puts_without_mutex(task_name);
            putbyte(' ');
            put_u32hex(reti_addr);
        }
    }
#endif

    struct list_head * pos;
    timer_work_t *timer;

    list_for_each(pos, (&time_list.list))
    {
        timer = list_entry(pos, timer_work_t , list);

        //if((g_os_time % (timer->time)) == 0)
        if((timer1_cnt % (timer->time)) == 0)
        {
            //printf("time handle = %x \r\n",timer->handler);
            timer->handler(timer->param);
        }
    }

    DSP_CLR_ILAT = BIT(TIME1_INIT);

}
REG_INIT_HANDLE(timer1_isr);


/*
time 的时机 是100 ms
*/

timer_handle timer_reg(u32 time , void (*handler)(void *parm) , void *parm)
{
    u32 cpu_sr ;
    timer_handle timer = malloc(sizeof(timer_work_t)) ;

    if(!timer)
    {
        return 0 ;
    }

    printf("timer reg handle = %x \r\n", handler);
    timer->param = parm ;
    timer->time = time ;
    timer->handler = handler ;
    CPU_INT_DIS();
    list_add(&(timer->list), &(time_list.list));
    CPU_INT_EN();
    return timer ;
}

void timer_unreg(timer_handle time)
{
    list_del(&time->list) ;
    free(time) ;
}




