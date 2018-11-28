/*****************************************************************************************************************

(C) Copyright 2003 - Analog Devices, Inc.  All rights reserved.

File Name:		startup.asm

Date Modified:	2/26/03		LB		Rev 0.2

Software:       GNU DCC 4.1+

Hardware:		CM-BF533

Purpose:		Generic Startup file

************************************************************************************************/


#include "startup.h"

.extern _main;
.extern _sdram_bss_vma;
.extern _sdram_bss_size;

.extern _video_ram_data_vma;
.extern _video_ram_data_lma;
.extern _video_ram_data_size;
.extern _video_ram_bss_vma;
.extern _video_ram_bss_size;

.extern _ram0_data_vma;
.extern _ram0_data_lma;
.extern _ram0_data_size;
.extern _ram0_bss_vma;
.extern _ram0_bss_size;


#define STACK_SIZE    0x4000

.section .stack, "aw", @nobits

	    .space  STACK_SIZE

_stack:


.section .text;
.org 0
_START:

r5=r0;
r6=r1;
r7=r2;

jump start1;
csync ;


start1:

    B0 = 0;
    B1 = 0;
    B2 = 0;
    B3 = 0;

    L0 = 0;
    L1 = 0;
    L2 = 0;
    L3 = 0;

    I0 = 0 ;
    I1 = 0 ;
    I2 = 0 ;
    I3 = 0 ;


    SP.l = _stack-12;
    SP.h = _stack-12;
    FP = SP;
	R1 = 0;

    P2 = 0;
    P3 = 0;
    P4 = 0;
    P5 = 0;

    M0 = 4;


/************************************** MOVE VGRAM DATA **************************************/

    P0.l= _video_ram_data_lma;
    P0.h= _video_ram_data_lma ;
    p1.l= _video_ram_data_size ;
    p1.h= _video_ram_data_size ;
    P1 = P1>>2;
    p2.l= _video_ram_data_vma ;
    p2.h= _video_ram_data_vma ;

    LSETUP(.init_video_ram_data_0, .init_video_ram_data_1) LC0 = P1;
.init_video_ram_data_0:
    r0 = [p0++] ;
.init_video_ram_data_1:
    [p2++]= r0


/************************************** MOVE RAM0 DATA **************************************/

    P0.l= _ram0_data_lma;
    P0.h= _ram0_data_lma ;
    p1.l= _ram0_data_size ;
    p1.h= _ram0_data_size ;
    P1 = P1>>2;
    p2.l= _ram0_data_vma ;
    p2.h= _ram0_data_vma ;

    LSETUP(.init_ram0_data_0, .init_ram0_data_1) LC0 = P1;
.init_ram0_data_0:
    r0 = [p0++] ;
.init_ram0_data_1:
    [p2++]= r0

/************************************** CLEAN VGRAM BSS **************************************/
    P0.L= _video_ram_bss_vma;
    P0.H= _video_ram_bss_vma ;
    P1.L= _video_ram_bss_size ;
    P1.H= _video_ram_bss_size ;
    P1 = P1>>2;
    R0 = 0;

    LSETUP(.clr_video_ram_bss,.clr_video_ram_bss) LC0 = P1;
.clr_video_ram_bss:
    [P0++] = R0;


/************************************** CLEAN RAM0 BSS **************************************/

    P0.L= _ram0_bss_vma;
    P0.H= _ram0_bss_vma ;
    P1.L= _ram0_bss_size ;
    P1.H= _ram0_bss_size ;
    P1 = P1>>2;
    R0 = 0;

    LSETUP(.clr_ram0_bss,.clr_ram0_bss) LC0 = P1;
.clr_ram0_bss:
    [P0++] = R0;

/************************************** CLEAN SDRAM BSS **************************************/

P0.L= _sdram_bss_vma;
P0.H= _sdram_bss_vma ;
P1.L= _sdram_bss_size ;
P1.H= _sdram_bss_size ;
P1 = P1>>2;
R0 = 0;

LSETUP(.clr_sdram_bss,.clr_sdram_bss) LC0 = P1;
.clr_sdram_bss:
[P0++] = R0;

/*******************************************************************************************/


p1 = 0 ;
p2 = 0 ;
r0 = 0 ;

r0 = r5 ;
r1 = r6 ;
r2 = r7 ;

p0.l = _main;
p0.h = _main;
call (p0);
csync ;


