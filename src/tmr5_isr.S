#include "ISR_Support.h"

    .set	nomips16
	.set 	noreorder

    .extern vTimer_5_ISR
	.extern xISRStackTop
    .global vTIMER_5_ISR_Wrapper
    .set noreorder
    .set noat
    .ent vTIMER_5_ISR_Wrapper

vTIMER_5_ISR_Wrapper:
    portSAVE_CONTEXT

    jal vTimer_5_ISR
    nop

    portRESTORE_CONTEXT

    .end vTIMER_5_ISR_Wrapper


