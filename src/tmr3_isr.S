#include "ISR_Support.h"

    .set	nomips16
	.set 	noreorder

    .extern vTimer_3_ISR
	.extern xISRStackTop
    .global vTIMER_3_ISR_Wrapper
    .set noreorder
    .set noat
    .ent vTIMER_3_ISR_Wrapper

vTIMER_3_ISR_Wrapper:
    portSAVE_CONTEXT

    jal vTimer_3_ISR
    nop

    portRESTORE_CONTEXT

    .end vTIMER_3_ISR_Wrapper


