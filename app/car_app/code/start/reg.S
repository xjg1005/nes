
.global _ISR_PUSH_REG

_ISR_PUSH_REG:

[--SP] = ASTAT;
[--SP] = reti;
[--SP] = (R7:0,P5:0);
[--sp] = FP;
[--SP] = I0;
[--SP] = I1;
[--SP] = I2;
[--SP] = I3;
[--SP] = B0;
[--SP] = B1;
[--SP] = B2;
[--SP] = B3;
[--SP] = L0;
[--SP] = L1;
[--SP] = L2;
[--SP] = L3;
[--SP] = M0;
[--SP] = M1;
[--SP] = M2;
[--SP] = M3;
R1.L = A0.X;
[--SP] = R1;
//R1 = A0.W;
[--SP] = A0.W;
R1.L = A1.X;
[--SP] = R1;
//R1 = A1.W;
[--SP] = A1.W;
[--SP] = LC0;
//R3 = 0x0 (X);		/*		R3=0x0(  0) */
//LC0 = R3;
[--SP] = LC1;
//LC1 = R3;
[--SP] = LT0;
[--SP] = LT1;
[--SP] = LB0;
[--SP] = LB1;
//L0 = 0x0 (X);		/*		L0=0x0 <_cnt.1320>(  0) */
//L1 = 0x0 (X);		/*		L1=0x0 <_cnt.1320>(  0) */
//L2 = 0x0 (X);		/*		L2=0x0 <_cnt.1320>(  0) */
//L3 = 0x0 (X);		/*		L3=0x0 <_cnt.1320>(  0) */
RTS;


.global _ISR_POP_REG

_ISR_POP_REG:
LB1 = [SP++];
LB0 = [SP++];
LT1 = [SP++];
LT0 = [SP++];
LC1 = [SP++];
LC0 = [SP++];
R0 = [SP++];
A1 = R0;
R0 = [SP++];
A1.X = R0.L;
R0 = [SP++];
A0 = R0;
R0 = [SP++];
A0.X = R0.L;
M3 = [SP++];
M2 = [SP++];
M1 = [SP++];
M0 = [SP++];
L3 = [SP++];
L2 = [SP++];
L1 = [SP++];
L0 = [SP++];
B3 = [SP++];
B2 = [SP++];
B1 = [SP++];
B0 = [SP++];
I3 = [SP++];
I2 = [SP++];
I1 = [SP++];
I0 = [SP++];
FP = [SP++];
(R7:0,P5:0) = [SP++];
RETI = [SP++];
ASTAT = [SP++];

#ifdef USE_RTOS
rts ;
#else
UNLINK ;
rti ;
#endif


.global ___udivsi3
___udivsi3:
divq(r0,r1) ;
RTS ;

.global ___divsi3
___divsi3:
divs(r0,r1) ;
RTS ;
