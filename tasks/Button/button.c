#include "button.h"
#include "seat/seat.h"
#include "motor/motor.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"


uint8 isLockButtonPressed=0;

void vTaskButton(void *pvParameters)
{
    Seat* seat = (Seat*)pvParameters;
    Motor *motor = seat->motor;
    uint8 flag=0;
    
    for (;;)
    {
        switch (motor->motorState)
        {
            case MOTOR_STOP:
                if (xButtonRead(seat->upCommandWindow)==BUTTON_LOW)
                {
                    if(xButtonRead(seat->upCommandWindow)==BUTTON_LOW)
                    {
                        if(flag==0)
                        {
                            xSemaphoreGive(seat->buttonPressed);
                            flag=1;
                        }
                    }
                }
                else if (xButtonRead(seat->downCommandWindow)==BUTTON_LOW)
                {
                    if(xButtonRead(seat->downCommandWindow)==BUTTON_LOW)
                    {
                        if(flag==0)
                        {
                            xSemaphoreGive(seat->buttonPressed);
                            flag=1;
                        }
                    }
                }
                else
                {
                    flag=0;
                }

                break;
            case MOTOR_CLOCKWISE:       /* going up */
                // if (xButtonRead(seat->upperLimitButton))
                // {
                //     xSemaphoreGive(buttonPressed);
                // }
                break;
            case MOTOR_COUNTERCLOCKWISE:
                break;
        
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    vTaskDelete( NULL );
}
