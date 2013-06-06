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

extern xSemaphoreHandle TIMER_3_SEMAPHORE;

void INIT_TIMER3(short div,long period_value);
void CLEAR_AND_START_TIMER3();
void CONFIG_TIMER3(short div, long period_value);

#ifdef	__cplusplus
}
#endif

#endif	/* TIMER_H */

