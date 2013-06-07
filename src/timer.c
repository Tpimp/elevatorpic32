#include "timer.h"
#include <xc.h>
#include <FreeRTOS.h>
#include <semphr.h>
xSemaphoreHandle TIMER3_RDY = NULL;
void __attribute__((interrupt(ipl1), vector(_TIMER_3_VECTOR), nomips16)) vTIMER_3_ISR_Wrapper(void);
void __attribute__((interrupt(ipl2), vector(_TIMER_5_VECTOR), nomips16)) vTIMER_5_ISR_Wrapper(void);
void INIT_TIMER3(short div,long period_value)
{
    // Timer initializes as OFF and with 0 counts
    T3CONbits.ON = 0;
    TMR3 = 0;
    // Set the period to count to
    PR3 = period_value;
    // Set the division prescaler
    /* 111 = 1:256
     * 110 = 1:64
     * 101 = 1:32
     * 100 = 1:16
     * 011 = 1:8
     * 010 = 1:4
     * 001 = 1:2
     * 000 = 1:1
     */
    T3CONbits.TCKPS = div;
    // Enable interrupt
    IEC0bits.T3IE = 1;
    IFS0bits.T3IF = 0;
    // and initialize priority 3 and sub 0
    IPC3bits.T3IP = 3;
    IPC3bits.T3IS = 0;
    TIMER3_RDY = xSemaphoreCreateCounting(2,0);
    xSemaphoreTake(TIMER3_RDY, 0);
}

void CLEAR_AND_START_TIMER3()
{
   T3CONbits.ON = 0;
   TMR3 = 0;
   xSemaphoreTake(TIMER3_RDY, 1/portTICK_RATE_MS);
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
    static signed portBASE_TYPE xHigherPriorityTaskWoken;
    xHigherPriorityTaskWoken = pdFALSE;
    // Handle the interrupt
    T3CONbits.ON = 0;
    IFS0bits.T3IF = 0;
    // Notify Task waiting on TIMER_3
    xSemaphoreGiveFromISR(TIMER3_RDY,&xHigherPriorityTaskWoken);
    portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
}



// TIMER1 Definitions


xSemaphoreHandle TIMER5_RDY = NULL;


void INIT_TIMER5(short div,long period_value)
{
    // Timer initializes as OFF and with 0 counts
    T5CONbits.ON = 0;
    TMR5 = 0;
    // Set the period to count to
    PR5 = period_value;
    // Set the division prescaler
    /* 111 = 1:256
     * 110 = 1:64
     * 101 = 1:32
     * 100 = 1:16
     * 011 = 1:8
     * 010 = 1:4
     * 001 = 1:2
     * 000 = 1:1
     */
    T5CONbits.TCKPS = div;
    // Enable interrupt
    IEC0bits.T5IE = 1;
    IFS0bits.T5IF = 0;
    // and initialize priority 3 and sub 0
    IPC5bits.T5IP = 3;
    IPC5bits.T5IS = 0;
    TIMER5_RDY = xSemaphoreCreateCounting(2,0);
    xSemaphoreTake(TIMER5_RDY, 0);
}

void CLEAR_AND_START_TIMER5()
{
   T5CONbits.ON = 0;
   TMR5 = 0;
   xSemaphoreTake(TIMER5_RDY, 1/portTICK_RATE_MS);
   T5CONbits.ON = 1;
}

void CONFIG_TIMER5(short div, long period_value)
{
    TMR5 = 0;
    PR5 = period_value;
    T5CONbits.TCKPS = div;
}

void vTimer_5_ISR(void)
{
    static signed portBASE_TYPE xHigherPriorityTaskWoken;
    xHigherPriorityTaskWoken = pdFALSE;
    // Handle the interrupt
    T5CONbits.ON = 0;
    IFS0bits.T5IF = 0;
    // Notify Task waiting on TIMER_3
    xSemaphoreGiveFromISR(TIMER5_RDY,&xHigherPriorityTaskWoken);
    portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
}
