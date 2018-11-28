#ifndef __ARCH_SYS_ARCH_H__
#define __ARCH_SYS_ARCH_H__

#include "cc.h" //包含cc.h头文件
#include "os_api.h"


#ifdef SYS_ARCH_GLOBALS
#define SYS_ARCH_EXT
#else
#define SYS_ARCH_EXT extern
#endif



/*-------------critical region protection (depends on uC/OS-II setting)-------*/
#if      OS_CRITICAL_METHOD == 3
#define SYS_ARCH_DECL_PROTECT(lev)  CPU_SR_ALLOC()
#define  SYS_ARCH_PROTECT(lev)      OS_ENTER_CRITICAL()
#define  SYS_ARCH_UNPROTECT(lev)    OS_EXIT_CRITICAL()
#else
#error("not define other OS_CRITICAL_METHOD!");
#endif


/*-----------------macros-----------------------------------------------------*/
//max number of lwip tasks (TCPIP) note LWIP TASK start with 1

#define MAX_QUEUE_ENTRIES (128/*+(sizeof(void *)+sizeof(QS)*4)/4*/)	// the max size of each mailbox

#define sys_arch_mbox_tryfetch(mbox,msg)    sys_arch_mbox_fetch(mbox,msg,1)

/*-----------------type define------------------------------------------------*/
/** struct of LwIP mailbox */
typedef struct {
    OS_EVENT   pQ;
//    void*       pvQEntries[MAX_QUEUE_ENTRIES];
} TQ_DESCR;
/** struct of LwIP mailbox */

typedef OS_SEM sys_sem_t; // type of semiphores
typedef OS_MUTEX sys_mutex_t; // type of mutex
typedef TQ_DESCR sys_mbox_t; // type of mailboxes
typedef u32_t sys_thread_t; // type of id of the new thread
typedef u8_t sys_prot_t;

//#define     SYS_MBOX_NULL NULL
#define     SYS_SEM_NULL NULL

#endif /* __SYS_RTXC_H__ */
