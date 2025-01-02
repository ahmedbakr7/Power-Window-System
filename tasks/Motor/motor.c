#include "motor.h"
#include "seat/seat.h"
#include "GPIO/gpio.h"


extern Button lockButton;

void  stopMotor(Motor *motor)
{
    xButtonWrite(motor->Button1, BUTTON_LOW);
    xButtonWrite(motor->Button2, BUTTON_LOW);
    motor->motorState = MOTOR_STOP;
    GPIO_RedLedOff();
    GPIO_GreenLedOff();
    GPIO_BlueLedOff();
}

void vTaskMotor(void *pvParameters)
{
    Seat *seat = (Seat *)pvParameters;
    Motor *motor = seat->motor;

    for (;;)
    {
        xQueueReceive(motor->xMotorQueue, &motor->motorState, portMAX_DELAY);
        switch (motor->motorState) 
        {
            case MOTOR_STOP:
                stopMotor(motor);
                break;

            case MOTOR_CLOCKWISE:   /* go up until limit switch */
                xButtonWrite(motor->Button1, BUTTON_HIGH);
                xButtonWrite(motor->Button2, BUTTON_LOW);
                UART0_SendString("up\n\r");
                GPIO_BlueLedOn();
                while ((seat->id==DRIVER?TRUE:!(xButtonRead(lockButton)==BUTTON_LOW))&&(xButtonRead(seat->upperLimitButton)&&BUTTON_HIGH )&& motor->motorState==MOTOR_CLOCKWISE)
                    vTaskDelay(100);
                stopMotor(motor);   /* stop motor, upon reaching limit switch */
                break;


            case MOTOR_COUNTERCLOCKWISE:    /* go up until limit switch */
                xButtonWrite(motor->Button1, BUTTON_LOW);
                xButtonWrite(motor->Button2, BUTTON_HIGH);
                UART0_SendString("down\n\r");
                GPIO_GreenLedOn();
                while ((seat->id==DRIVER?TRUE:!(xButtonRead(lockButton)==BUTTON_LOW))&&(xButtonRead(seat->lowerLimitButton)&&BUTTON_HIGH )&& motor->motorState==MOTOR_COUNTERCLOCKWISE)
                    vTaskDelay(100);
                stopMotor(motor);   /* stop motor, upon reaching limit switch */
                break;

            case MOTOR_Jammed:    /* go up until limit switch */
                xButtonWrite(motor->Button1, BUTTON_LOW);
                xButtonWrite(motor->Button2, BUTTON_HIGH);
                UART0_SendString("J\n\r");
                GPIO_RedLedOn();
                TickType_t currentTime=xTaskGetTickCount();
                seat->timeStart=currentTime;
                while ((seat->timeStart +JAMMING_DELAY)>currentTime&& xButtonRead(seat->lowerLimitButton)&&BUTTON_HIGH )
                {
                    vTaskDelay(50);
                    currentTime=xTaskGetTickCount();
                }
                stopMotor(motor);   /* stop motor, upon reaching limit switch, or completing a 0.5sec delay */
                seat->timeStart=0;
                break;
                
        }
    }
    vTaskDelete( NULL );
}

