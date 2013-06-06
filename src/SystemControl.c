#include <FreeRTOS.h>
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "SystemControl.h"
#include "led.h"
#include "buttondrv.h"



void ButtonPressed(short buttonpressed,short bottonstate)
{
    //led_drive(buttonpressed,1,1);
}

void ButtonReleased(short buttonreleased, short buttonstate)
{
    TOGGLE_LED(buttonreleased);
}

void RCButtonReleased(short buttonreleased, short buttonstate)
{
    TOGGLE_LED(buttonreleased);
    CLEAR_AND_START_TIMER3();
}

void ScanRCButtons(DEBOUNCE_DATA * debounce_info, char * task_name)
{
    if(BUTTON_CONTROL.ButtonAvailable[3] && BUTTON_CONTROL.ButtonState[3] != PORTCbits.RC1)
    {
        debounce_info[0].TARGET_RESULT = PORTCbits.RC1;
        BUTTON_CONTROL.ButtonAvailable[3] = FALSE;
        task_name[5] = '3';
        xTaskCreate(DEBOUNCE_TASK,
                    task_name,
                    configMINIMAL_STACK_SIZE,
                    &debounce_info[0],
                    1,
                    NULL);
    }
    else if(BUTTON_CONTROL.ButtonAvailable[4] && BUTTON_CONTROL.ButtonState[4] != PORTCbits.RC2)
    {
        debounce_info[1].TARGET_RESULT = PORTCbits.RC2;
        BUTTON_CONTROL.ButtonAvailable[4] = FALSE;
        task_name[5] = '4';
        xTaskCreate(DEBOUNCE_TASK,
                    task_name,
                    configMINIMAL_STACK_SIZE,
                    &debounce_info[1],
                    1,
                    NULL);
    }
}

void SystemControl()
{
    // system control params and

    char task_name[] = "DBNCE0";
    DEBOUNCE_DATA DEBOUNCE_LIST[3];
    DEBOUNCE_DATA RC_DEBOUNCE_LIST[2];
    DEBOUNCE_LIST[0].TARGET_BUTTON =0;
    DEBOUNCE_LIST[1].TARGET_BUTTON =1;
    DEBOUNCE_LIST[2].TARGET_BUTTON =2;
    RC_DEBOUNCE_LIST[0].TARGET_BUTTON = 3;
    RC_DEBOUNCE_LIST[1].TARGET_BUTTON = 4;
    DEBOUNCE_LIST[0].PRESSED_CALLBACK = 0;
    DEBOUNCE_LIST[1].PRESSED_CALLBACK = 0;
    DEBOUNCE_LIST[2].PRESSED_CALLBACK = 0;
    RC_DEBOUNCE_LIST[0].PRESSED_CALLBACK = 0;
    RC_DEBOUNCE_LIST[1].PRESSED_CALLBACK = 0;
    DEBOUNCE_LIST[0].RELEASED_CALLBACK = &ButtonReleased;
    DEBOUNCE_LIST[1].RELEASED_CALLBACK = &ButtonReleased;
    DEBOUNCE_LIST[2].RELEASED_CALLBACK = &ButtonReleased;
    RC_DEBOUNCE_LIST[0].RELEASED_CALLBACK = &RCButtonReleased;
    RC_DEBOUNCE_LIST[1].RELEASED_CALLBACK = &RCButtonReleased;
    DEBOUNCE_LIST[0].TARGET_RESULT = 1;
    DEBOUNCE_LIST[1].TARGET_RESULT = 1;
    DEBOUNCE_LIST[2].TARGET_RESULT = 1;
    RC_DEBOUNCE_LIST[0].TARGET_RESULT = 1;
    RC_DEBOUNCE_LIST[1].TARGET_RESULT = 1;
    // create the task handle
    //create state tracking variables

    /************************************************************************
     * SystemControl- Monitors the buttonpress semaphore then handles
     * button presses by creating a debounce task for each new button press.
     * The debounce function can also be passed a callback and in this case
     * is passed the ButtonPressed and ButtonReleased methods above.
     ***********************************************************************/
    while(1) // while the task is running
    {    
        
        if(pdTRUE == xSemaphoreTake(buttonpress, 1/portTICK_RATE_MS )) // if not waiting for a key to be released
        {
            taskENTER_CRITICAL();
            DEBOUNCE_LIST[BUTTON_CONTROL.TARGET_BUTTON].TARGET_RESULT = BUTTON_CONTROL.BUTTON_ACTION;
            BUTTON_CONTROL.ButtonAvailable[BUTTON_CONTROL.TARGET_BUTTON] = FALSE;
            task_name[5] = (char)('0' + BUTTON_CONTROL.TARGET_BUTTON );
            xTaskCreate(DEBOUNCE_TASK,
                        task_name,
                        configMINIMAL_STACK_SIZE,
                        &DEBOUNCE_LIST[BUTTON_CONTROL.TARGET_BUTTON],
                        1,
                        NULL);
            taskEXIT_CRITICAL();
        }
        else
        {
            ScanRCButtons(RC_DEBOUNCE_LIST,task_name);
        }
    }
}


// LED_FlashTask takes a void * (casted integer pointer)
// While task is running continuosly toggle the LED on and off
// hand off to idle task for 500 Ms in between each toggle
void LED_FlashTask(void * lednum)
{
    int led = *(int*)lednum;
    while(1)
    {
        // toggl the led number on
        led_drive(led,2,0);
        // Nothing left to do so
        // idle the process for 500MS
        vTaskDelay(500 /portTICK_RATE_MS);
    }
}