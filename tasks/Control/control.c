#include "control.h"

#define AUTO_FUNCTION_DELAY 1000UL


extern Button lockButton;

void vControlTask(void *pvParameters)
{
    Seat* seat = (Seat*)pvParameters;
    Motor* motor = seat->motor;
    
    for (;;)
    {
        xSemaphoreTake(seat->buttonPressed, portMAX_DELAY);
        if ((seat->id==DRIVER?TRUE:!(xButtonRead(lockButton)==BUTTON_LOW))&&(xButtonRead(seat->upCommandWindow)==BUTTON_LOW))  // up button is pressed
        {
            motor->motorState = MOTOR_CLOCKWISE;
            xQueueSend(motor->xMotorQueue, &motor->motorState, 0);
            seat->timeStart=xTaskGetTickCount();
            while(xButtonRead(seat->upCommandWindow)==BUTTON_LOW)  // will exit the loop when the button is released
                vTaskDelay(100);
            if((seat->timeStart +AUTO_FUNCTION_DELAY)>xTaskGetTickCount()) // auto function delay didnt pass, meaning auto function is enabled
            {
                while ((xButtonRead(seat->detectionSensor)&&BUTTON_HIGH) && motor->motorState==MOTOR_CLOCKWISE);   // will loop until the detection sensor is pressed or the motor is stopped
                if (xButtonRead(seat->detectionSensor)==BUTTON_LOW) 
                {
                    motor->motorState = MOTOR_Jammed;
                    xQueueSend(motor->xMotorQueue, &motor->motorState, 0);
                    vTaskSuspend(seat->buttonTaskHandle);
                    vTaskDelay(500);    // 0.5 sec delay
                    vTaskResume(seat->buttonTaskHandle);
                }
                xSemaphoreGive(seat->buttonPressed);
            }
            else {
                motor->motorState = MOTOR_STOP;
                xQueueSend(motor->xMotorQueue, &motor->motorState, 0);
            }
        }
        else if ((seat->id==DRIVER?TRUE:!(xButtonRead(lockButton)==BUTTON_LOW))&&(xButtonRead(seat->downCommandWindow)==BUTTON_LOW))
        {
            motor->motorState = MOTOR_COUNTERCLOCKWISE;
            xQueueSend(motor->xMotorQueue, &motor->motorState, 0);
            seat->timeStart=xTaskGetTickCount();
            while(xButtonRead(seat->downCommandWindow)==BUTTON_LOW)  // will exit the loop when the button is released
                vTaskDelay(100);
            if((seat->timeStart +AUTO_FUNCTION_DELAY)>xTaskGetTickCount()) // auto function delay didnt pass, meaning auto function is enabled
            {
                xSemaphoreGive(seat->buttonPressed);
                while(motor->motorState!=MOTOR_STOP)
                    vTaskDelay(pdMS_TO_TICKS(50));
            }
            else {
                motor->motorState = MOTOR_STOP;
                xQueueSend(motor->xMotorQueue, &motor->motorState, 0);
            }

        }
    }
    vTaskDelete( NULL );
}
