#ifndef _OS_COMPAT_H_
#define _OS_COMPAT_H_

#include "os_api.h"

enum kill_mode
{
    KILL_WAIT,
    KILL_REQ,
    KILL_FORCE,
};

int thread_fork_init(void);
int thread_fork(char *thread_name, int prio, int stk_size, u32 q_size, int *pid, void (*func)(void*), void *parm);
void thread_kill(volatile int *pid, enum kill_mode mode);
int thread_kill_req(void);
u16 thread_put_msg_fifo(u32 msg,u8 type,int *pid);
u16 thread_put_msg_lifo(u32 msg,u8 type,int *pid);

#endif //_OS_COMPAT_H_
