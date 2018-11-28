#ifndef _CONFIG_
#define _CONFIG_

#include "dv12.h"
#include "typedef.h"

/* For writing into SPR. */

#define CLR_WDT(X)

//#ifndef LITTLE_ENDIAN
//#define LITTLE_ENDIAN 1234
//#endif

//#define     INT_EN()       // mtspr(SPR_SR, mfspr(SPR_SR)|BIT(2))
//#define     INT_DIS()      // mtspr(SPR_SR, mfspr(SPR_SR)&~BIT(2))
//
//#define     ENTER_CRITICAL(SR)      //do{SR=mfspr(SPR_SR)&BIT(2);INT_DIS();}while(0)
//#define     LEAVE_CRITICAL(SR)      //mtspr(SPR_SP,SR|mfspr(SPR_SR))

#define     ENTER_CRITICAL(SR)      asm volatile ("cli %0 ; .word 0x26 ;nop; nop ; nop;" : "=r" (SR) );  //(cpu_sr = cli())            		/* Disable interrupts                       */ //do{SR=mfspr(SPR_SR)&BIT(2);INT_DIS();}while(0)
#define     LEAVE_CRITICAL(SR)      asm volatile (" .word 0x27 ;sti %0": : "d" (SR)); //(sti(cpu_sr))               		/* Enable  interrupts                      *///mtspr(SPR_SP,SR|mfspr(SPR_SR))

//
//#define puts(a) do{}while(0)
//#define printf(...) do{}while(0)

#endif
