/* 
 * File:   led.h
 * Author: christopher.dean
 *
 * Created on April 2, 2013, 3:58 PM
 */

#ifndef LED_H
#define	LED_H

/* Hardware include. */
#include <xc.h>
/* Standard includes. */
#include <FreeRTOS.h>
#include <plib.h>
#include <stdint.h>
#include <task.h>
#include <semphr.h>
#include "timer.h"

// Define elevator LED types
#define ELEVATOR_TOP = (0)
#define ELEVATOR_MID = (1)
#define ELEVATOR_BTM = (2)
#define ELEVATOR_UP =  (3)
#define ELEVATOR_DWN = (4)

static const unsigned int INVALID_LED_INDEX = 1;
static const unsigned int INVALID_LED_COMMAND = 2;

// Define programmable LED wrapper for SFR Macros
short led_drive(int lednum, int cmd, int LED_ON);
extern short LED_CURRENT_VALUE;

void TURN_OFF_LED(short lednumber);
void TOGGLE_LED(short lednumber);
void TURN_ON_LED(short lednumber);
short READ_LED_STATE(short lednumber);
void NextIteration(void);
void LED_OPEN_SEQUENCE(void);
void LED_CLOSE_SEQUENCE(void);
void LED_CLOSE_BLOCKED_SEQUENCE(void);


#endif	/* LED_H */

