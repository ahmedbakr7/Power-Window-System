#ifndef GPIO_H_
#define GPIO_H_

#include "tm4c123gh6pm.h"
#include "std_types.h"
#include "common_macros.h"

#define PORTA_ID               0
#define PORTB_ID               1
#define PORTC_ID               2
#define PORTD_ID               3
#define PORTE_ID               4
#define PORTF_ID               5

#define PIN0_ID                0
#define PIN1_ID                1
#define PIN2_ID                2
#define PIN3_ID                3
#define PIN4_ID                4
#define PIN5_ID                5
#define PIN6_ID                6
#define PIN7_ID                7

#define GPIO_PORTA_BASE GPIO_PORTA_DATA_BITS_R
#define GPIO_PORTB_BASE GPIO_PORTB_DATA_BITS_R
#define GPIO_PORTC_BASE GPIO_PORTC_DATA_BITS_R
#define GPIO_PORTD_BASE GPIO_PORTD_DATA_BITS_R
#define GPIO_PORTE_BASE GPIO_PORTE_DATA_BITS_R
#define GPIO_PORTF_BASE GPIO_PORTF_DATA_BITS_R

#define GPIO_PORTF_PRIORITY_MASK      0xFF1FFFFF
#define GPIO_PORTF_PRIORITY_BITS_POS  21
#define GPIO_PORTF_INTERRUPT_PRIORITY 5

#define PRESSED                ((uint8)0x00)
#define RELEASED               ((uint8)0x01)


typedef enum
{
	PIN_INPUT,PIN_OUTPUT
}GPIO_PinDirectionType;

void GPIO_BuiltinButtonsLedsInit(void);

void GPIO_RedLedOn(void);
void GPIO_BlueLedOn(void);
void GPIO_GreenLedOn(void);

void GPIO_RedLedOff(void);
void GPIO_BlueLedOff(void);
void GPIO_GreenLedOff(void);

void GPIO_RedLedToggle(void);
void GPIO_BlueLedToggle(void);
void GPIO_GreenLedToggle(void);

uint8 GPIO_SW1GetState(void);
uint8 GPIO_SW2GetState(void);

void GPIO_SW1EdgeTriggeredInterruptInit(void);
void GPIO_SW2EdgeTriggeredInterruptInit(void);


void GPIO_Init(uint8 portNumber,uint8 pinNumber,GPIO_PinDirectionType direction,uint8 pullup );

void GPIO_writePin(uint8 portNumber,uint8 pinNumber,uint8 value);

void GPIO_writePort(uint8 portNumber,uint8 value);

uint8 GPIO_ReadPin(uint8 portNumber,uint8 pinNumber);

uint8 GPIO_ReadPort(uint8 portNumber);

void GPIO_TogglePin(uint8 PortNumber, uint8 PinNumber);


#endif

