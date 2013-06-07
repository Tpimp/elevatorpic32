/* 
 * File:   ButtonControl.h
 * Author: christopher.dean
 *
 * Created on April 9, 2013, 4:07 PM
 * ButtonControl- Provides interrupt handeling and button polling for the
 * PIC32MX Starter Kit and PIC32 Companion Board eflightworks board.
 */

#ifndef BUTTONCONTROL_H
#define	BUTTONCONTROL_H

/* Hardware include. */
#include <xc.h>
/* Standard includes. */
#include <stdint.h>
#include <plib.h>
#include "buttondrv.h"


// create worker function -Acts as main task for button CN and LED
void ButtonControl();


void ScanRCButtons(DEBOUNCE_DATA * debounce_info, char * task_name);


// create static call wrapper (not necessary for next time)
static void StaticButtonControl()
{
    // cast back to unsigned int, then pass by value
    ButtonControl();
}


void RCButtonReleased(short buttonreleased, short buttonstate);
void ButtonPressed(short buttonpressed,short bottonstate);
void ButtonReleased(short buttonreleased, short buttonstate);
#endif	/* BUTTONCONTROL_H */

