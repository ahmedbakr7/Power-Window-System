#ifndef SEAT_H_
#define  SEAT_H_

#include "Button/button.h"
#include "Motor/motor.h"
#include "FreeRTOS.h"
#include "semphr.h"

#define DRIVER 1
#define PASSENGER 2

typedef struct  // Seat stats
{
    uint8 id;
    Button upperLimitButton;
    Button lowerLimitButton;
    Button upCommandWindow;
    Button downCommandWindow;
    Button detectionSensor;
    // Button lockButton;           /* in case of driver */
    Motor * motor;
    xSemaphoreHandle buttonPressed;
    TickType_t timeStart;
    xTaskHandle buttonTaskHandle;
    // xSemaphoreHandle SeatM;
} Seat;


#endif
 