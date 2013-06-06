#include "buttondrv.h"
#include <xc.h>


xSemaphoreHandle buttonpress = NULL;


BUTTON_CONTROL_MODULE BUTTON_CONTROL;

void __attribute__((interrupt(ipl0), vector(_CHANGE_NOTICE_VECTOR))) vCN_ISR_Wrapper(void);

void initCN(void)
{
    // CNCON<15>
    // Set CNPUE<x> for each pin used - Done by main config
    // set CNEN<x> for each pin used
    mCNOpen(CN_ON, CN15_ENABLE | CN16_ENABLE | CN19_ENABLE, CN15_PULLUP_ENABLE | CN16_PULLUP_ENABLE | CN19_PULLUP_ENABLE);

    // Read PORTx to clear ?mismatch? for each PORT having a used pin
    mPORTDRead();

    // Set IPC6<20-18> and <17-16> for interrupt priority and subpriority
    // Clear IFS1<0> to clear interrupt status
    // Set IEC1<0> to unmask/enable interrupt

    ConfigIntCN(CHANGE_INT_PRI_3 | CHANGE_INT_ON);
}


void INIT_BUTTON_CONTROL(void)
{
    BUTTON_CONTROL.BUTTON_ACTION = FALSE;
    buttonpress = xSemaphoreCreateMutex();
    xSemaphoreTake(buttonpress,0);
    BUTTON_CONTROL.ButtonAvailable[0] = TRUE;
    BUTTON_CONTROL.ButtonAvailable[1] = TRUE;
    BUTTON_CONTROL.ButtonAvailable[2] = TRUE;
    BUTTON_CONTROL.ButtonAvailable[3] = TRUE;
    BUTTON_CONTROL.ButtonAvailable[4] = TRUE;
    BUTTON_CONTROL.ButtonState[0] = TRUE;
    BUTTON_CONTROL.ButtonState[1] = TRUE;
    BUTTON_CONTROL.ButtonState[2] = TRUE;
    BUTTON_CONTROL.ButtonState[3] = TRUE;
    BUTTON_CONTROL.ButtonState[4] = TRUE;
    BUTTON_CONTROL.TARGET_BUTTON = 0;
}

void DEBOUNCE_TASK(void * debounce_data)
{
    DEBOUNCE_DATA * mydata = (DEBOUNCE_DATA*)debounce_data;
    vTaskDelay(10/portTICK_RATE_MS);
    taskENTER_CRITICAL();
    BUTTON_CONTROL.ButtonAvailable[mydata->TARGET_BUTTON] = TRUE;
    switch(mydata->TARGET_BUTTON)
    {
        case 0:
        {
            if(PORTDbits.RD6 == mydata->TARGET_RESULT)
            {
                BUTTON_CONTROL.ButtonState[0] = mydata->TARGET_RESULT;
                if(!mydata->TARGET_RESULT)
                {
                    if(mydata->PRESSED_CALLBACK)
                        (*mydata->PRESSED_CALLBACK)(mydata->TARGET_BUTTON,mydata->TARGET_RESULT);
                }
                else
                {
                    if(mydata->RELEASED_CALLBACK)
                        (*mydata->RELEASED_CALLBACK)(mydata->TARGET_BUTTON,mydata->TARGET_RESULT);
                }
            }
            break;
        }
        case 1:
        {
            if(PORTDbits.RD7 == mydata->TARGET_RESULT)
            {
                BUTTON_CONTROL.ButtonState[1] = mydata->TARGET_RESULT;
                if(!mydata->TARGET_RESULT)
                {
                    if(mydata->PRESSED_CALLBACK)
                        (*mydata->PRESSED_CALLBACK)(mydata->TARGET_BUTTON,mydata->TARGET_RESULT);
                }
                else
                {
                    if(mydata->RELEASED_CALLBACK)
                        (*mydata->RELEASED_CALLBACK)(mydata->TARGET_BUTTON,mydata->TARGET_RESULT);
                }
            }
            break;
        }
        case 2:
        {
            if(PORTDbits.RD13 == mydata->TARGET_RESULT)
            {
                BUTTON_CONTROL.ButtonState[2] = mydata->TARGET_RESULT;
                if(!mydata->TARGET_RESULT)
                {
                    if(mydata->PRESSED_CALLBACK)
                        (*mydata->PRESSED_CALLBACK)(mydata->TARGET_BUTTON,mydata->TARGET_RESULT);
                }
                else
                {
                    if(mydata->RELEASED_CALLBACK)
                        (*mydata->RELEASED_CALLBACK)(mydata->TARGET_BUTTON,mydata->TARGET_RESULT);
                }
            }
            break;
        }
        case 3:
        {
            if(PORTCbits.RC1 == mydata->TARGET_RESULT)
            {
                BUTTON_CONTROL.ButtonState[3] = mydata->TARGET_RESULT;
                if(!mydata->TARGET_RESULT)
                {
                    if(mydata->PRESSED_CALLBACK)
                        (*mydata->PRESSED_CALLBACK)(mydata->TARGET_BUTTON,mydata->TARGET_RESULT);
                }
                else
                {
                    if(mydata->RELEASED_CALLBACK)
                        (*mydata->RELEASED_CALLBACK)(mydata->TARGET_BUTTON,mydata->TARGET_RESULT);
                }
            }
            break;
        }
        case 4:
        {
            if(PORTCbits.RC2 == mydata->TARGET_RESULT)
            {
                BUTTON_CONTROL.ButtonState[4] = mydata->TARGET_RESULT;
                if(!mydata->TARGET_RESULT)
                {
                    if(mydata->PRESSED_CALLBACK)
                        (*mydata->PRESSED_CALLBACK)(mydata->TARGET_BUTTON,mydata->TARGET_RESULT);
                }
                else
                {
                    if(mydata->RELEASED_CALLBACK)
                        (*mydata->RELEASED_CALLBACK)(mydata->TARGET_BUTTON,mydata->TARGET_RESULT);
                }
            }
            break;
        }
        default: {break;}
    }
    
    vTaskDelete(NULL);
    taskEXIT_CRITICAL();
}


void vCN_ISR(void)
{
    /* Variables */
    static portBASE_TYPE xHigherPriorityTaskWoken;

    // ISR code

    //ConfigIntCN(CHANGE_INT_PRI_3 | CHANGE_INT_OFF);
    IFS1bits.CNIF = 0;
    
    if(BUTTON_CONTROL.ButtonAvailable[0] && (PORTDbits.RD6 != BUTTON_CONTROL.ButtonState[0]))
    {
        BUTTON_CONTROL.BUTTON_ACTION = PORTDbits.RD6;
        BUTTON_CONTROL.TARGET_BUTTON = 0;
        xSemaphoreGiveFromISR(buttonpress,&xHigherPriorityTaskWoken);
    }
    else if(BUTTON_CONTROL.ButtonAvailable[1] && (PORTDbits.RD7 != BUTTON_CONTROL.ButtonState[1]))
    {
        BUTTON_CONTROL.BUTTON_ACTION = PORTDbits.RD7;
        BUTTON_CONTROL.TARGET_BUTTON = 1;
        xSemaphoreGiveFromISR(buttonpress,&xHigherPriorityTaskWoken);
    }
    else if(BUTTON_CONTROL.ButtonAvailable[2] && (PORTDbits.RD13 != BUTTON_CONTROL.ButtonState[2]))
    {
        BUTTON_CONTROL.BUTTON_ACTION = PORTDbits.RD13;
        BUTTON_CONTROL.TARGET_BUTTON = 2;
        xSemaphoreGiveFromISR(buttonpress,&xHigherPriorityTaskWoken);
    }

    /* If sending or receiving necessitates a context switch, then switch now. */
    portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
}
