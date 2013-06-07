#include "led.h"

short LED_CURRENT_VALUE = 0;

/*******************************************************************
| led_drive - takes three parameters and performs 1 of
| four commands on an led sensor:
|           1.Read LED current state
|           2&3.SET LED ON or OFF
|           4.TOGGLE the LED state
| Parameters:
|   lednum: a value between 0 and 2 PORTD<2-0>
|   cmd: a value between 0 and 2 indicating which command
|   LED_ON: Default is off (0) for LED_ON value, will be used with
|       SET command (1).
| Return: return_value indicates if the command request was
|       successful and if it wasn't, why not.
*******************************************************************/
short led_drive(int lednum, int cmd, int LED_ON)
{
    short return_value;
    return_value = -1;
    // switch on the current command
    switch(cmd)
    {
        case 0: // READ the value for return
        {
           return_value =  READ_LED_STATE(lednum);
           break;
        }
        case 1: // set the led on or off
        {
           if(LED_ON) 
           { 
               // SET LED_ON
               TURN_ON_LED(lednum);
           }
           else
           {
               // SET LED_OFF
               TURN_OFF_LED(lednum);
           }
           return_value = 2; // return success
           break;
        }
        case 2: // TOGGLE the bits
        {
            // Toggle LED_ON/LED_OFF
            TOGGLE_LED(lednum);
            return_value = 2; // return success
            break;
        }
        // return invalid cmd
        default : { return_value = INVALID_LED_COMMAND; break;}
    }
    return return_value; // EXIT drive_led with return status
}

short READ_LED_STATE(short lednumber)
{
    short ret_val;
    ret_val = -1;
    switch(lednumber)
    {
        case 0:{}
        case 1:{}
        case 2:
        {
            ret_val = mPORTDReadBits(1 << lednumber);
            break;
        }
        case 3:{}
        case 4:
        {
            ret_val = mPORTBReadBits(1 << (lednumber-3));
        }
        default: {break;}
    }
    return ret_val;
}

void TOGGLE_LED(short lednumber)
{
    switch(lednumber)
    {
        case 0:{}
        case 1:{}
        case 2:
        {
            mPORTDToggleBits(1 << lednumber);
            break;
        }
        case 3:{}
        case 4:
        {
            mPORTBToggleBits(1 << (lednumber+1));
        }
        default: {break;}
    }
}

void TURN_OFF_LED(short lednumber)
{
    switch(lednumber)
    {
        case 0:{}
        case 1:{}
        case 2:
        {
            mPORTDClearBits(1 << lednumber);
            break;
        }
        case 3:{}
        case 4:
        {
            mPORTBClearBits(1 << (lednumber+1));
        }
        default: {break;}
    }
}

void NextIteration(void)
{
    switch(LED_CURRENT_VALUE)
    {
        case 0:{TURN_OFF_LED(1); LED_CURRENT_VALUE++; break;}
        case 1:{TURN_OFF_LED(0);TURN_OFF_LED(2); LED_CURRENT_VALUE++; break;}
        case 2:{}
        case 3:{}
        case 4:{}
        case 5:{}
        case 6:{}
        case 7:
        {
            LED_CURRENT_VALUE++;
            break;
        }
        case 8:{TURN_ON_LED(0); TURN_ON_LED(2); LED_CURRENT_VALUE++; break;}
        case 9:{TURN_ON_LED(1); LED_CURRENT_VALUE++; break;}
        default:
        {
            T3CONbits.ON = 0;
            LED_CURRENT_VALUE = 0;
            TMR3 = 0;
            break;
        }

    }
}


void TURN_ON_LED(short lednumber)
{
    switch(lednumber)
    {
        case 0:{}
        case 1:{}
        case 2:
        {
            mPORTDSetBits(1 << lednumber);
            break;
        }
        case 3:{}
        case 4:
        {
            mPORTBSetBits(1 << (lednumber+1));
        }
        default: {break;}
    }
}

void LED_OPEN_SEQUENCE(void)
{
    static short LED_CURRENT_STATE;
    LED_CURRENT_STATE = 0;
    while(LED_CURRENT_STATE >= 0)
    {
        CLEAR_AND_START_TIMER3();
        if(xSemaphoreTake(TIMER3_RDY, portMAX_DELAY))
        {
            switch(LED_CURRENT_STATE)
            {
                case 0:{}
                case 1:{}
                case 2:{TURN_OFF_LED(LED_CURRENT_STATE); LED_CURRENT_STATE++; break;}
                default:
                {
                    LED_CURRENT_STATE = -1;
                    break;
                }
            }
        }
    }
    vTaskDelete(NULL);
}

void LED_CLOSE_SEQUENCE(void)
{
    static short LED_CURRENT_STATE;
    LED_CURRENT_STATE = 2;
    while(LED_CURRENT_STATE >= 0)
    {
        CLEAR_AND_START_TIMER3();
        if(xSemaphoreTake(TIMER3_RDY, portMAX_DELAY))
        {
            switch(LED_CURRENT_STATE)
            {
                case 0:{}
                case 1:{}
                case 2:{TURN_ON_LED(LED_CURRENT_STATE); LED_CURRENT_STATE--; break;}
                default:
                {
                    LED_CURRENT_STATE = -1;
                    break;
                }
            }
        }
    }
    vTaskDelete(NULL);
}

void LED_CLOSE_BLOCKED_SEQUENCE(void)
{
    static short LED_CURRENT_STATE;
    LED_CURRENT_STATE = 0;
    while(LED_CURRENT_STATE >= 0)
    {
        CLEAR_AND_START_TIMER3();
        if(xSemaphoreTake(TIMER3_RDY, portMAX_DELAY))
        {
            switch(LED_CURRENT_STATE)
            {
                case 0:{TURN_ON_LED(2); LED_CURRENT_STATE++; break;}
                case 1:{TURN_ON_LED(1); LED_CURRENT_STATE++; break;}
                case 2:{TURN_OFF_LED(1); LED_CURRENT_STATE++; break;}
                case 3:{TURN_ON_LED(1); LED_CURRENT_STATE++; break;}
                case 4:{TURN_OFF_LED(1); LED_CURRENT_STATE++; break;}
                case 5:{TURN_OFF_LED(2); LED_CURRENT_STATE++; break;}
                default:
                {
                    T3CONbits.ON = 0;
                    IFS0bits.T3IF = 0;
                    LED_CURRENT_STATE = -1;
                    TMR3 = 0;
                    break;
                }
            }
        }
    }
    vTaskDelete(NULL);
}