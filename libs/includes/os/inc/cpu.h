/***********************************Jieli tech************************************************
  File : cpu.h
  By   : Juntham
  date : 2014-10-30 16:05
********************************************************************************************/
#ifndef _CPU_H_
#define _CPU_H_
#include "dv12.h"



#define LITTLE_ENDIAN
#define  OS_STK_GROWTH        1                             /* Stack grows from HIGH to LOW memory on 80x86  */

#define OS_CPU_CORE               1
#define OS_CPU_MMU                0
#define SDRAM                     0
#define MEM_MALLOC                1

#ifndef __ASSEMBLY__
/*typedef unsigned char   u8, bool, BOOL;
typedef char            s8;
typedef unsigned short  u16;
typedef signed short    s16;
typedef unsigned int    u32 , tu8, tu16 , tbool , tu32;
typedef signed int      s32;*/

inline static int processor_id()
{
    int cpu_index ;
    asm("%0 = SEQSTAT ;" : "=r" (cpu_index));
    return cpu_index ;
}

#if OS_CPU_CORE > 1
#define OS_CPU_ID                 processor_id()
#else
#define OS_CPU_ID                 0
#endif

#endif
/*
*********************************************************************************************************
*                                              Defines
*********************************************************************************************************
*/

#define  CPU_SR_ALLOC()  unsigned int cpu_sr

/*save and Disable interrupts */
#define  CPU_INT_DIS()    \
	asm volatile ("cli %0" : "=d" (cpu_sr))

/* pop interrupts values */
#define  CPU_INT_EN()     \
	asm volatile ("sti %0": : "d" (cpu_sr))




#define  CPU_LOCK_EN()    asm volatile (".word 0x26")
#define  CPU_LOCK_DIS()   asm volatile (".word 0x27")

#define  OS_TASK_CLR(a)   DSP_CLR_ILAT = (1L << (30-a))
#define  OS_TASK_SW(a)    DSP_SET_ILAT = (1L << (30-a))       /* 任务级任务切换函数*/

#if OS_CPU_CORE > 1
#define OS_ENTER_CRITICAL()  CPU_INT_DIS();CPU_LOCK_EN()            /* 关中断*/
#define OS_EXIT_CRITICAL()   CPU_LOCK_DIS();CPU_INT_EN()             /* 开中断*/
#else




#define OS_ENTER_CRITICAL()  \
	CPU_INT_DIS()

#define OS_EXIT_CRITICAL()  \
		CPU_INT_EN(); \



#endif




#define MULU(Rm,Rn) asm volatile (".word 0xc512;divq(%0, %1);" :: "d" (Rm), "d" (Rn) );
#define MUL(Rm,Rn)  asm volatile (".word 0xc512;divs(%0, %1);" :: "d" (Rm), "d" (Rn) );
#define MAC(Rm,Rn)  asm volatile (".word 0xc512;%0*=%1;" ::"d" (Rm), "d" (Rn) );
#define MAS(Rm,Rn)  asm volatile (".word 0xc512;%0=~%1;" :: "d"(Rm), "d" (Rn) );
#define MRSIS(Rm,Rn) asm volatile (".word 0xc512;%0+=%1;" : "=&d" (Rm): "i" (Rn) );  //mac read shift imm&sat
#define MRSI(Rm,Rn) asm volatile (".word 0xc512;%0=%1;" : "=&d" (Rm): "i" (Rn) );    //mac read shift imm
#define MRSRS(Rm,Rn) asm volatile (".word 0xc512;%0>>>=%1;" : "=&d" (Rm): "d" (Rn) );  //mac read shift reg&sat
#define MRSR(Rm,Rn) asm volatile (".word 0xc512;%0>>=%1;" : "=&d" (Rm): "d" (Rn) );    //mac read shift register
#define MUL32(Ro,Rm,Rn) asm volatile (".word 0xc512;%0=%1&%2;": "=&d" (Ro):"d" (Rm), "d" (Rn) );

#define MACCLR()  asm volatile (".word 0xc512;divs(r0, r0);.word 0xc512;r0=~r0");
#define MACSET(h,l) asm volatile ("SEQSTAT = %0; USP = %1"::"d" (l), "d" (h));
#define MACRL(l) asm volatile ("%0 = SEQSTAT":"=d" (l));
#define MACRH(h) asm volatile ("%0 = USP":"=d" (h));

#define MULSI(Ro,Rm,Rn,Rp)  asm volatile (".word 0xc512;divs(%1, %2);.word 0xc512;%0=%3;" : "=&d" (Ro) : "d"(Rm), "d" (Rn), "i" (Rp) );
#define MULSIS(Ro,Rm,Rn,Rp) asm volatile (".word 0xc512;divs(%1, %2);.word 0xc512;%0+=%3;" : "=&d" (Ro) : "d"(Rm), "d" (Rn), "i" (Rp) );
#define MULSR(Ro,Rm,Rn,Rp)  asm volatile (".word 0xc512;divs(%1, %2);.word 0xc512;%0>>=%3;" : "=&d" (Ro) : "d"(Rm), "d" (Rn), "d" (Rp) );
#define MULSRS(Ro,Rm,Rn,Rp) asm volatile (".word 0xc512;divs(%1, %2);.word 0xc512;%0>>>=%3;" : "=&d" (Ro) : "d"(Rm), "d" (Rn), "d" (Rp) );



/* void EnableOtherCpu(void); */
/* #define enable_other_cpu EnableOtherCpu */


#endif
