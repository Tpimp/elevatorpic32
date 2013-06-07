#include <FreeRTOS.h>
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "ButtonControl.h"
#include "led.h"
#include "buttondrv.h"



void ButtonPressed(short buttonpressed,short bottonstate)
{
    //led_drive(buttonpressed,1,1);
}

void ButtonReleased(short buttonreleased, short buttonstate)
{
    xTaskCreate(LED_OPEN_SEQUENCE,
                    "DR_OPN",
                    configMINIMAL_STACK_SIZE,
                    NULL,
                    3,
                    NULL);
}

void RCButtonReleased(short buttonreleased, short buttonstate)
{
    xTaskCreate(LED_CLOSE_BLOCKED_SEQUENCE,
                    "DR_CLS",
                    configMINIMAL_STACK_SIZE,
                    NULL,
                    3,
                    NULL);
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

void ButtonControl()
{
    // INITIALIZE and Create local Variables for ButtonControl Task
    char task_name[] = "DBNCE0"; // Task name for debounce task create

    DEBOUNCE_DATA DEBOUNCE_LIST[3]; // used to track debounce state (INTs)
    DEBOUNCE_DATA RC_DEBOUNCE_LIST[2]; // used to track debounce state (Poll)

    // Set the target_button to debounce
    // one for each button to bebounced (can all happen synchronously)
    DEBOUNCE_LIST[0].TARGET_BUTTON =0;
    DEBOUNCE_LIST[1].TARGET_BUTTON =1;
    DEBOUNCE_LIST[2].TARGET_BUTTON =2;
    RC_DEBOUNCE_LIST[0].TARGET_BUTTON = 3;
    RC_DEBOUNCE_LIST[1].TARGET_BUTTON = 4;

    // Initialize the function pointer CALLBACKS for the button press
    // Call backs will recieve the button number and state,
    // the CALLBACK then can perform any actions necessary with the current info
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

    // Initially all buttons are sitting at 1 (pull-ups)
    DEBOUNCE_LIST[0].TARGET_RESULT = 1;
    DEBOUNCE_LIST[1].TARGET_RESULT = 1;
    DEBOUNCE_LIST[2].TARGET_RESULT = 1;
    RC_DEBOUNCE_LIST[0].TARGET_RESULT = 1;
    RC_DEBOUNCE_LIST[1].TARGET_RESULT = 1;
    
    /************************************************************************
     * ButtonControl- Monitors the buttonpress semaphore then handles
     * button presses by creating a debounce task for each new button press.
     * The debounce function can also be passed a callback and in this case
     * is passed the ButtonPressed and ButtonReleased methods above.
     ***********************************************************************/
    while(1) // while the task is running
    {    
        
        if(pdTRUE == xSemaphoreTake(buttonpress, 1/portTICK_RATE_MS )) // if not waiting for a key to be released
        {
            taskENTER_CRITICAL();
            // Set the new debounce target
            DEBOUNCE_LIST[BUTTON_CONTROL.TARGET_BUTTON].TARGET_RESULT = BUTTON_CONTROL.BUTTON_ACTION;
            // Mark the button as "not" available so the ISR will ignore it
            BUTTON_CONTROL.ButtonAvailable[BUTTON_CONTROL.TARGET_BUTTON] = FALSE;
            // Create the task name for the current debounce task
            task_name[5] = (char)('0' + BUTTON_CONTROL.TARGET_BUTTON );
            // Create the DEBOUNCE Task to debounce the current button
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
            // if no interrupts to handle, scan RC buttons
            ScanRCButtons(RC_DEBOUNCE_LIST,task_name);
        }
        //vTaskDelay((portTickType)50);
    }
}
