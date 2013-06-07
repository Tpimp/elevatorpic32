/* 
 * File:   timer.h
 * Author: Christopher
 *
 * Created on June 6, 2013, 12:54 AM
 */

#ifndef TIMER_H
#define	TIMER_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <FreeRTOS.h>
#include <semphr.h>

// TIMER 3 Functions and Semaphore Definition
extern xSemaphoreHandle TIMER3_RDY;
void CLEAR_AND_START_TIMER3();
void CONFIG_TIMER3(short div, long period_value);
void INIT_TIMER3(short div,long period_value);


// TIMER 5 Functions and Semaphore Definition
extern xSemaphoreHandle TIMER5_RDY;
void CLEAR_AND_START_TIMER5();
void CONFIG_TIMER5(short div, long period_value);
void INIT_TIMER5(short div,long period_value);



#ifdef	__cplusplus
}
#endif

#endif	/* TIMER_H */

