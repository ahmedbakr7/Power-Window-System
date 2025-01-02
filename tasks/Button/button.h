
#ifndef BUTTON_H_
#define BUTTON_H_

#include "FreeRTOS.h"
#include "std_types.h"
#include "common_macros.h"
#include "../../tm4c123gh6pm.h"
#include "semphr.h"
#include "gpio.h"


#define BUTTON_LOW  0X00000000
#define BUTTON_HIGH 0XFFFFFFFF

// bitbanding
#define xCreateButton(port,pinNumber)  ((Button)(port)+( 1<<pinNumber ))
#define xButtonRead(button)            ((*button))
#define xButtonWrite(button,value)     (((*button))=(value))

typedef uint32* Button;

extern xSemaphoreHandle lockChanging;  // mutex to protect the lock button while debouncing
extern xSemaphoreHandle buttonPressed;  // mutex to protect the lock button while debouncing

extern uint8 isLockButtonPressed;

void vTaskButton(void *pvParameters);

#endif
