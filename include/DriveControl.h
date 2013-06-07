/* 
 * File:   DriveControl.h
 * Author: Christopher
 *
 * Created on June 7, 2013, 6:13 AM
 */

#ifndef DRIVECONTROL_H
#define	DRIVECONTROL_H
#include <stdbool.h>
#include "timer.h"
#include <task.h>


static const unsigned short MAX_SPEED  = 40;
static const  float MINIMUM_SPEED = 4.5;
extern xTaskHandle ELEVATOR_TASK;

typedef struct TRIP_DATA_T
{  
    bool TRAVELING;
    bool BRAKING;
    bool UP;
    bool IN_EMERGENCY_STATE;
    float DestinationFloor;
    float BrakingStartLocation;
    float CurrentLocation;
    float CurrentVelocity;
    unsigned LastFloorStopped;
    float CurrentAccelPerSec;
    float BrakingDecelPerSec;
}TRIP_DATA;

extern TRIP_DATA ELEVATOR;

void DriveControl(void);
bool RequestTripElevator(unsigned short destinationfloor);
bool SetEmergencyStateElevator(bool in_emergency_state);



#endif 