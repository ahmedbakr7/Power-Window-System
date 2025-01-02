#ifndef CONTROL_H
#define CONTROL_H

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "seat/seat.h"
#include "motor/motor.h"
#include "button/button.h"



void vControlTask(void *pvParameters);

#endif