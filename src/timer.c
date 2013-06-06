#include "timer.h"
#include <xc.h>

xSemaphoreHandle TIMER_3_SEMAPHORE = NULL;

void __attribute__((interrupt(ipl1), vector(_TIMER_3_VECTOR))) vTIMER_3_ISR_Wrapper(void);

void INIT_TIMER3(short div,long period_value)
{
    T3CONbits.ON = 0;
    TMR3 = 0;
    PR3 = period_value;
    T3CONbits.TCKPS = div;
    IEC0bits.T3IE = 1;
    IFS0bits.T3IF = 0;
    IPC3bits.T3IP = 3;
    IPC3bits.T3IS = 0;

}

void CLEAR_AND_START_TIMER3()
{
   T3CONbits.ON = 0;
   TMR3 = 0;
   T3CONbits.ON = 1;
}

void CONFIG_TIMER3(short div, long period_value)
{
    TMR3 = 0;
    PR3 = period_value;
    T3CONbits.TCKPS = div;
}

void vTimer_3_ISR(void)
{
    T3CONbits.ON = 0;
    IFS0bits.T3IF = 0;
    TMR3 = 0;
    T3CONbits.ON = 1;
    NextIteration();    

}