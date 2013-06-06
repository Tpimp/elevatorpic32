/* 
 * File:   SystemControl.h
 * Author: christopher.dean
 *
 * Created on April 9, 2013, 4:07 PM
 */

#ifndef SYSTEMCONTROL_H
#define	SYSTEMCONTROL_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

/* Hardware include. */
#include <xc.h>
/* Standard includes. */
#include <stdint.h>
#include <plib.h>
#include "buttondrv.h"



// create worker function -Acts as main task for button CN and LED
void SystemControl();


void ScanRCButtons(DEBOUNCE_DATA * debounce_info, char * task_name);


// create static call wrapper (not necessary for next time)
static void StaticSystemControl()
{
    // cast back to unsigned int, then pass by value
    SystemControl();
}

// Define LED_FlashTask
void LED_FlashTask(void * lednum);

void RCButtonReleased(short buttonreleased, short buttonstate);
void ButtonPressed(short buttonpressed,short bottonstate);
void ButtonReleased(short buttonreleased, short buttonstate);
#endif	/* SYSTEMCONTROL_H */

