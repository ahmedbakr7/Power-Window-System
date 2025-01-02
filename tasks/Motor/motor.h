#ifndef MOTOR_H_
#define MOTOR_H_

#include "Button/button.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "UART/uart0.h"

#define JAMMING_DELAY 500UL    // in ticks


typedef enum  // button stats
{
    MOTOR_STOP,
    MOTOR_CLOCKWISE,
    MOTOR_COUNTERCLOCKWISE,
    MOTOR_Jammed
}MOTOR_STATE;

typedef struct  // button stats
{
    Button upperLimitButton;
    Button lowerLimitButton;
    Button Button1;
    Button Button2;
    QueueHandle_t xMotorQueue;
    MOTOR_STATE motorState;
} Motor;


#define xCreateMotor(upperLimitButton,lowerLimitButton,port,pinNumber1,pinNumber2) {\
    upperLimitButton, \
    lowerLimitButton, \
    xCreateButton((port),(pinNumber1)),\
    xCreateButton((port),(pinNumber2)),\
    MOTOR_STOP\
}
void  vTaskMotor(void *pvParameters);

#endif
