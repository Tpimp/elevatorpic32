#include "DriveControl.h"

//Initialize the Elevator to valid values
TRIP_DATA ELEVATOR;
xTaskHandle ELEVATOR_TASK = NULL;
void DriveControl(void)
{
    float destination_floor_in_feet = 0;
    while(1)
    {
        //tx Beginning trip from , going to ->
        while(ELEVATOR.TRAVELING)
        {
            CLEAR_AND_START_TIMER5();
            // Sleep until TIMER 5 is ready
            if(xSemaphoreTake(TIMER5_RDY,portMAX_DELAY))
            {
                // fires every 500 ms not every second so CurrentVelocity 
                // is divided by 2 and breakingdecel is a division of 4 
                if(!ELEVATOR.BRAKING)
                {
                    if( ELEVATOR.CurrentVelocity + (ELEVATOR.CurrentAccelPerSec + ELEVATOR.CurrentVelocity) < MAX_SPEED)
                        ELEVATOR.CurrentVelocity += (ELEVATOR.CurrentAccelPerSec + ELEVATOR.CurrentVelocity);
                    else
                         ELEVATOR.CurrentVelocity = MAX_SPEED;
                    if(ELEVATOR.UP)
                    {
                        ELEVATOR.CurrentLocation += ELEVATOR.CurrentVelocity;
                        if((ELEVATOR.CurrentLocation + 
                           (ELEVATOR.CurrentAccelPerSec + ELEVATOR.CurrentVelocity/2))
                            >= ELEVATOR.BrakingStartLocation )
                        {
                            ELEVATOR.BRAKING = true;
                            destination_floor_in_feet = ELEVATOR.DestinationFloor *10;
                        }
                        
                    }
                    else
                    {
                        ELEVATOR.CurrentLocation -= ELEVATOR.CurrentVelocity;
                        if((ELEVATOR.CurrentLocation + 
                           (ELEVATOR.CurrentAccelPerSec - ELEVATOR.CurrentVelocity/2))
                            <= ELEVATOR.BrakingStartLocation )
                        {
                            ELEVATOR.BRAKING = true;
                            destination_floor_in_feet = ELEVATOR.DestinationFloor *10;
                        }                    
                    }
                }
                else // ELSE BRAKING!
                {
                    if(ELEVATOR.UP)
                    {

                        if(destination_floor_in_feet - ELEVATOR.CurrentLocation > 10)
                        {
                            ELEVATOR.CurrentLocation += ELEVATOR.CurrentVelocity;
                            static float percent_complete;
                            percent_complete = (float)(ELEVATOR.CurrentLocation - ELEVATOR.BrakingStartLocation)/(float)((destination_floor_in_feet - (ELEVATOR.LastFloorStopped *10)));
                            ELEVATOR.BrakingDecelPerSec = (ELEVATOR.CurrentVelocity * percent_complete);
                            if(ELEVATOR.CurrentVelocity - ELEVATOR.BrakingDecelPerSec > MINIMUM_SPEED)
                                ELEVATOR.CurrentVelocity -= ELEVATOR.BrakingDecelPerSec;
                        }
                        else
                        {
                            if(destination_floor_in_feet - ELEVATOR.CurrentLocation < ELEVATOR.CurrentVelocity)
                            {
                                ELEVATOR.CurrentLocation = destination_floor_in_feet;
                                ELEVATOR.CurrentVelocity = 0;
                                ELEVATOR.TRAVELING = false;
                                ELEVATOR.BRAKING = false;
                                ELEVATOR.BrakingDecelPerSec = 0;
                            }
                            else
                            {
                                if(ELEVATOR.CurrentVelocity > 1.0f)
                                    ELEVATOR.CurrentVelocity /= 2;
                                ELEVATOR.CurrentLocation += ELEVATOR.CurrentVelocity;
                            }
                        }
                                            
                    }
                    else
                    {

                        if(destination_floor_in_feet - ELEVATOR.CurrentLocation > 10)
                        {

                            ELEVATOR.CurrentLocation -= ELEVATOR.CurrentVelocity;
                            ELEVATOR.BrakingDecelPerSec = ELEVATOR.CurrentVelocity/4;
                            if(ELEVATOR.CurrentVelocity > MINIMUM_SPEED)
                                ELEVATOR.CurrentVelocity -= ELEVATOR.BrakingDecelPerSec;
                        }
                        else
                        {
                            if(destination_floor_in_feet - ELEVATOR.CurrentLocation < 1.5)
                            {
                                ELEVATOR.CurrentLocation = destination_floor_in_feet;
                                ELEVATOR.CurrentVelocity = 0;
                                ELEVATOR.TRAVELING = false;
                                ELEVATOR.BRAKING = false;
                                ELEVATOR.BrakingDecelPerSec = 0;
                            }
                            else
                            {
                                ELEVATOR.CurrentLocation -= ELEVATOR.CurrentVelocity;

                            }
                        }
                    }
                }
            }
            // TX Current Location Data
        }
        TOGGLE_LED(2);
        vTaskSuspend(NULL); 
    }
}

void InitializeElevator()
{
    ELEVATOR.BRAKING = false;
    ELEVATOR.BrakingDecelPerSec = 1.0f;
    ELEVATOR.BrakingStartLocation = 0;
    ELEVATOR.CurrentAccelPerSec = 4.0f;
    ELEVATOR.CurrentLocation = 0.0;
    ELEVATOR.CurrentVelocity = 0;
    ELEVATOR.DestinationFloor = 0;
    ELEVATOR.IN_EMERGENCY_STATE = false;
    ELEVATOR.LastFloorStopped = 0;
    ELEVATOR.TRAVELING = false;
    ELEVATOR.UP = false;
}

bool RequestTripElevator(unsigned short destinationfloor)
{
    bool ret_val = false;
    // If not already moving
    if(!ELEVATOR.TRAVELING)
    {
        if(ELEVATOR.DestinationFloor != destinationfloor)
        {
            short distance_to_travel = 0;
            ELEVATOR.LastFloorStopped = ELEVATOR.DestinationFloor;
            ELEVATOR.DestinationFloor = destinationfloor;
            if(destinationfloor > ELEVATOR.LastFloorStopped)
            {
                ELEVATOR.UP  = true;
                distance_to_travel = (destinationfloor - ELEVATOR.LastFloorStopped)*10;
                ELEVATOR.BrakingStartLocation = ELEVATOR.CurrentLocation + (distance_to_travel/2) -1;
            }
            else
            {
                ELEVATOR.UP = false;
                distance_to_travel = (destinationfloor - ELEVATOR.LastFloorStopped)*10;
                ELEVATOR.BrakingStartLocation = ELEVATOR.CurrentLocation - (distance_to_travel/2) -1;
            }
            ELEVATOR.TRAVELING = true;
            vTaskResume(ELEVATOR_TASK);
        }
    }
    return ret_val;
}

bool SetEmergencyStateElevator(bool in_emergency_state)
{
    ELEVATOR.IN_EMERGENCY_STATE = in_emergency_state;
}
