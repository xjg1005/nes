/***********************************Jieli tech************************************************
  File : os_cpu.h
  By   : Juntham
  date : 2014-07-03 09:06
********************************************************************************************/
#ifndef _OS_CPU_H
#define _OS_CPU_H
#include "cpu.h"


#define  OS_CPU_EXT           extern

void OSCtxSw(void);
#define os_ctx_sw OSCtxSw

/*
*********************************************************************************************************
*                                              DATA TYPES
*                                         (Compiler Specific)
*********************************************************************************************************
*/


#define OS_CRITICAL_METHOD          3
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
//#define     CPU_SR_ALLOC()  OS_CPU_SR  cpu_sr
#endif


#endif                                           /*_OS_CPU_H                                            */
