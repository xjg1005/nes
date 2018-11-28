/***********************************Jieli tech************************************************
  File : cpu.h
  By   : Juntham
  date : 2014-10-30 16:05
********************************************************************************************/
#ifndef _CPU_LIB_H_
#define _CPU_LIB_H_
#include "cpu.h"

/*
*********************************************************************************************************
*                                              Defines
*********************************************************************************************************
*/

//#define  CPU_SR_ALLOC()  unsigned int cpu_sr
//
///*save and Disable interrupts */
//#define  CPU_INT_DIS()    \
//	asm volatile ("cli %0" : "=d" (cpu_sr))
//
///* pop interrupts values */
//#define  CPU_INT_EN()     \
//	asm volatile ("sti %0": : "d" (cpu_sr))
//
//
//
//
//#define  CPU_LOCK_EN()    asm volatile (".word 0x26")
//#define  CPU_LOCK_DIS()   asm volatile (".word 0x27")
//
//#define  OS_TASK_CLR(a)   DSP_CLR_ILAT = (1L << (30-a))
//#define  OS_TASK_SW(a)    DSP_SET_ILAT = (1L << (30-a))       /* 任务级任务切换函数*/
//
//#if OS_CPU_CORE > 1
//#define OS_ENTER_CRITICAL()  CPU_INT_DIS();CPU_LOCK_EN()            /* 关中断*/
//#define OS_EXIT_CRITICAL()   CPU_LOCK_DIS();CPU_INT_EN()             /* 开中断*/
//#else
//
//
//
//
//#define OS_ENTER_CRITICAL()  \
//	CPU_INT_DIS()
//
//#define OS_EXIT_CRITICAL()  \
//		CPU_INT_EN(); \
//
// #endif


#endif
