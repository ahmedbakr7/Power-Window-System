/* Kernel includes. */
#include "FreeRTOS.h"
#include "seat/seat.h"
#include "task.h"
#include "queue.h"
#include "common/std_types.h"
#include "common_macros.h"
#include "semphr.h"
#include "button/button.h"
#include "motor/motor.h"
#include "control/control.h"
#include "GPIO/gpio.h"
#include "UART/uart0.h"

Button lockButton;

void prvSetupHardware(void)
{
/* 
    TODO: initialize all required GPIOs pins
 */
    GPIO_Init(PORTB_ID,PIN0_ID,PIN_INPUT ,1);
    GPIO_Init(PORTB_ID,PIN1_ID,PIN_INPUT ,1 );
    GPIO_Init(PORTA_ID,PIN5_ID,PIN_INPUT ,1 );
    GPIO_Init(PORTA_ID,PIN6_ID,PIN_INPUT ,1 );
    GPIO_Init(PORTF_ID,PIN4_ID,PIN_INPUT ,1 );

    GPIO_Init(PORTB_ID,PIN4_ID,PIN_INPUT ,1);
    GPIO_Init(PORTA_ID,PIN7_ID,PIN_INPUT ,1 );
    GPIO_Init(PORTE_ID,PIN4_ID,PIN_INPUT ,1 );
    GPIO_Init(PORTE_ID,PIN5_ID,PIN_INPUT ,1 );
    GPIO_Init(PORTF_ID,PIN0_ID,PIN_INPUT ,1 );

    GPIO_Init(PORTE_ID,PIN2_ID,PIN_INPUT ,1 );  // lock

    GPIO_Init(PORTA_ID,PIN2_ID,PIN_OUTPUT ,1 );  // motor pins
    GPIO_Init(PORTA_ID,PIN3_ID,PIN_OUTPUT ,1 );  // motor pins

    GPIO_BuiltinButtonsLedsInit();
    UART0_Init();
}

int main(void)
{
   /* Setup the hardware for use with the Tiva C board. */
    prvSetupHardware();

    static Seat passenger={
            PASSENGER,                                  // id
            xCreateButton(GPIO_PORTB_BASE,PIN0_ID),         // upper limit Switch
            xCreateButton(GPIO_PORTB_BASE,PIN1_ID),         // lower limit Switch
            xCreateButton(GPIO_PORTA_BASE,PIN5_ID),         // up command button
            xCreateButton(GPIO_PORTA_BASE,PIN6_ID),         // down command button
            xCreateButton(GPIO_PORTF_BASE,PIN4_ID),         // JAMMING button
            NULL/* xCreateButton(PORTF_ID,PIN0_ID) */,      // LOCK Button incase of driver
            NULL,                                           // Motor
            NULL,             // buttonPressed
            0,                                              // tickStart
            NULL,                                           // button task handle
        };

    static Seat driver={
            DRIVER,                                  // id
            xCreateButton(GPIO_PORTB_BASE,PIN4_ID),         // upper limit Switch
            xCreateButton(GPIO_PORTA_BASE,PIN7_ID),         // lower limit Switch
            xCreateButton(GPIO_PORTE_BASE,PIN4_ID),         // up command button
            xCreateButton(GPIO_PORTE_BASE,PIN5_ID),         // down command button
            xCreateButton(GPIO_PORTF_BASE,PIN0_ID),         // JAMMING button
            NULL,                                           // LOCK Button
            NULL,                                           // Motor
            NULL,             // buttonPressed
            0,                                              // tickStart
            NULL,                                           // button task handle
        };

    static Motor motorPassenger;
    static Motor motorDriver;

    
    motorPassenger.upperLimitButton=passenger.upperLimitButton;
    motorPassenger.lowerLimitButton=passenger.lowerLimitButton;
    motorPassenger.Button1=xCreateButton(GPIO_PORTA_BASE,PIN2_ID);  // N1
    motorPassenger.Button2=xCreateButton(GPIO_PORTA_BASE,PIN3_ID);  // N2
    motorPassenger.xMotorQueue=xQueueCreate(10,sizeof(MOTOR_STATE));
    motorPassenger.motorState=MOTOR_STOP;
    
    motorDriver.upperLimitButton=driver.upperLimitButton;
    motorDriver.lowerLimitButton=driver.lowerLimitButton;
    motorDriver.Button1=xCreateButton(GPIO_PORTA_BASE,PIN2_ID);
    motorDriver.Button2=xCreateButton(GPIO_PORTA_BASE,PIN3_ID);
    motorDriver.xMotorQueue=xQueueCreate(10,sizeof(MOTOR_STATE));
    motorDriver.motorState=MOTOR_STOP;

    driver.motor=&motorDriver;
    driver.buttonPressed=xSemaphoreCreateBinary();
    passenger.buttonPressed=xSemaphoreCreateBinary();
    passenger.motor=&motorPassenger;




    
    lockButton=xCreateButton(GPIO_PORTE_BASE,PIN2_ID);

    
    xTaskCreate(vTaskButton , "buttonTask" , 100   , (void*)&passenger    , 1 , &(passenger.buttonTaskHandle));
    xTaskCreate(vControlTask , "controlTask" , 100   , (void*)&passenger    , 2 , NULL);
    xTaskCreate(vTaskMotor , "motorTask" , 100   , (void*)&passenger    , 2 , NULL);
    
    xTaskCreate(vTaskButton , "buttonTask" , 100   , (void*)&driver    , 1 , &(driver.buttonTaskHandle));
    xTaskCreate(vControlTask , "controlTask" , 100   , (void*)&driver    , 2 , NULL);
    xTaskCreate(vTaskMotor , "motorTask" , 100   , (void*)&driver    , 2 , NULL);
    

    vTaskStartScheduler();

    for (;;);
    vTaskSuspendAll();

}
