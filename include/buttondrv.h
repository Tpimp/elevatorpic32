#ifndef BUTTONDRV_H
#define	BUTTONDRV_H
/* Hardware include. */
#include <FreeRTOS.h>
#include <plib.h>
#include <stdint.h>
#include <queue.h>
#include <task.h>
#include <semphr.h>

// Define the Buttn Macros
#define PENTHOUSE2 = (0)
#define PENTHOUSE1 = (1)
#define GROUNDFLR =  (2)
#define OPEN_DOOR =  (3)
#define CLOSE_DOOR = (4)

extern xSemaphoreHandle buttonpress;

typedef struct button_control_state
{
    short TARGET_BUTTON;
    short BUTTON_ACTION;
    short ButtonAvailable[5];
    short ButtonState[5];
}BUTTON_CONTROL_MODULE;


typedef struct DEBOUNCE_STRUCT
{
    short TARGET_BUTTON;
    short TARGET_RESULT;
    void (*PRESSED_CALLBACK)(short,short);
    void (*RELEASED_CALLBACK)(short,short);
}DEBOUNCE_DATA;

BUTTON_CONTROL_MODULE BUTTON_CONTROL;


// Prototypes
void initCN(void);

void INIT_BUTTON_CONTROL(void);

void DEBOUNCE_TASK(void * debounce_data);

#endif	/* BUTTONDRV_H */

