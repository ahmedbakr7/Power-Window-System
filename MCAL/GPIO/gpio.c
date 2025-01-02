#include "gpio.h"

void GPIO_BuiltinButtonsLedsInit(void)
{
    /*
     * PF0 --> SW2
     * PF1 --> Red LED
     * PF2 --> Blue LED
     * PF3 --> Green LED
     * PF4 --> SW1
     */

    /* Enable clock for PORTF and wait for clock to start */
    SYSCTL_RCGCGPIO_R |= 0x20;
    while(!(SYSCTL_PRGPIO_R & 0x20));

    GPIO_PORTF_LOCK_R   = 0x4C4F434B;                       /* Unlock the GPIO_PORTF_CR_R */
    GPIO_PORTF_CR_R    |= (1<<0);                           /* Enable changes on PF0 */
    GPIO_PORTF_AMSEL_R &= 0xE0;                             /* Disable Analog on PF0, PF1, PF2, PF3 and PF4 */
    GPIO_PORTF_PCTL_R  &= 0xFFF00000;                       /* Clear PMCx bits for PF0, PF1, PF2, PF3 and PF4 to use it as GPIO pins */
    GPIO_PORTF_DIR_R   &= ~(1<<0) & ~(1<<4);                /* Configure PF0 & PF4 as input pins */
    GPIO_PORTF_DIR_R   |= ((1<<1) | (1<<2) | (1<<3));       /* Configure PF1, PF2 & PF3 as output pins */
    GPIO_PORTF_AFSEL_R &= 0xE0;                             /* Disable alternative function on PF0, PF1, PF2, PF3 and PF4 */
    GPIO_PORTF_PUR_R   |= ((1<<0)|(1<<4));                  /* Enable pull-up on PF0 & PF4 */
    GPIO_PORTF_DEN_R   |= 0x1F;                             /* Enable Digital I/O on PF0, PF1, PF2, PF3 and PF4 */
    GPIO_PORTF_DATA_R  &= ~(1<<1) & ~(1<<2) & ~(1<<3);      /* Clear bits 1, 2 & 3 in Data register to turn off the LEDs */
}

void GPIO_RedLedOn(void)
{
    GPIO_PORTF_DATA_R |= (1<<1);  /* Red LED ON */
}

void GPIO_BlueLedOn(void)
{
    GPIO_PORTF_DATA_R |= (1<<2);  /* Blue LED ON */
}

void GPIO_GreenLedOn(void)
{
    GPIO_PORTF_DATA_R |= (1<<3);  /* Green LED ON */
}

void GPIO_RedLedOff(void)
{
    GPIO_PORTF_DATA_R &= ~(1<<1);  /* Red LED OFF */
}

void GPIO_BlueLedOff(void)
{
    GPIO_PORTF_DATA_R &= ~(1<<2);  /* Blue LED OFF */
}

void GPIO_GreenLedOff(void)
{
    GPIO_PORTF_DATA_R &= ~(1<<3);  /* Green LED OFF */
}

void GPIO_RedLedToggle(void)
{
    GPIO_PORTF_DATA_R ^= (1<<1);  /* Red LED is toggled */
}

void GPIO_BlueLedToggle(void)
{
    GPIO_PORTF_DATA_R ^= (1<<2);  /* Blue LED is toggled */
}

void GPIO_GreenLedToggle(void)
{
    GPIO_PORTF_DATA_R ^= (1<<3);  /* Green LED is toggled */
}

uint8 GPIO_SW1GetState(void)
{
    return ((GPIO_PORTF_DATA_R >> 4) & 0x01);
}

uint8 GPIO_SW2GetState(void)
{
    return ((GPIO_PORTF_DATA_R >> 0) & 0x01);
}

void GPIO_SW1EdgeTriggeredInterruptInit(void)
{
    GPIO_PORTF_IS_R    &= ~(1<<4);      /* PF4 detect edges */
    GPIO_PORTF_IBE_R   &= ~(1<<4);      /* PF4 will detect a certain edge */
    GPIO_PORTF_IEV_R   &= ~(1<<4);      /* PF4 will detect a falling edge */
    GPIO_PORTF_ICR_R   |= (1<<4);       /* Clear Trigger flag for PF4 (Interrupt Flag) */
    GPIO_PORTF_IM_R    |= (1<<4);       /* Enable Interrupt on PF4 pin */
    /* Set GPIO PORTF priority as 5 by set Bit number 21, 22 and 23 with value 2 */
    NVIC_PRI7_R = (NVIC_PRI7_R & GPIO_PORTF_PRIORITY_MASK) | (GPIO_PORTF_INTERRUPT_PRIORITY<<GPIO_PORTF_PRIORITY_BITS_POS);
    NVIC_EN0_R         |= 0x40000000;   /* Enable NVIC Interrupt for GPIO PORTF by set bit number 30 in EN0 Rister */
}

void GPIO_SW2EdgeTriggeredInterruptInit(void)
{
    GPIO_PORTF_IS_R    &= ~(1<<0);      /* PF0 detect edges */
    GPIO_PORTF_IBE_R   &= ~(1<<0);      /* PF0 will detect a certain edge */
    GPIO_PORTF_IEV_R   &= ~(1<<0);      /* PF0 will detect a falling edge */
    GPIO_PORTF_ICR_R   |= (1<<0);       /* Clear Trigger flag for PF0 (Interrupt Flag) */
    GPIO_PORTF_IM_R    |= (1<<0);       /* Enable Interrupt on PF0 pin */
    /* Set GPIO PORTF priority as 5 by set Bit number 21, 22 and 23 with value 2 */
    NVIC_PRI7_R = (NVIC_PRI7_R & GPIO_PORTF_PRIORITY_MASK) | (GPIO_PORTF_INTERRUPT_PRIORITY<<GPIO_PORTF_PRIORITY_BITS_POS);
    NVIC_EN0_R         |= 0x40000000;   /* Enable NVIC Interrupt for GPIO PORTF by set bit number 30 in EN0 Register */
}


void GPIO_Init(uint8 portNumber,uint8 pinNumber,GPIO_PinDirectionType direction,uint8 pullup )
{
    SYSCTL_RCGCGPIO_R|=(1<<portNumber);
    while ((SYSCTL_PRGPIO_R & (1<<portNumber))==0);
    switch (portNumber)
    {
    case PORTA_ID:
        GPIO_PORTA_LOCK_R= 0X4C4F434B;
        GPIO_PORTA_CR_R|=(1<<pinNumber);
        GPIO_PORTA_DIR_R|=(direction<<pinNumber);
        if(direction==PIN_INPUT) GPIO_PORTA_PUR_R|=(pullup<<pinNumber);
        GPIO_PORTA_DEN_R|=(1<<pinNumber);
        break;
        
    case PORTB_ID:
        GPIO_PORTB_LOCK_R= 0X4C4F434B;
        GPIO_PORTB_CR_R|=(1<<pinNumber);
        GPIO_PORTB_DIR_R|=(direction<<pinNumber);
        if(direction==PIN_INPUT) GPIO_PORTB_PUR_R|=(pullup<<pinNumber);
        GPIO_PORTB_DEN_R|=(1<<pinNumber);
        break;
        
    case PORTC_ID:
        GPIO_PORTC_LOCK_R= 0X4C4F434B;
        GPIO_PORTC_CR_R|=(1<<pinNumber);
        GPIO_PORTC_DIR_R|=(direction<<pinNumber);
        if(direction==PIN_INPUT) GPIO_PORTC_PUR_R|=(pullup<<pinNumber);
        GPIO_PORTC_DEN_R|=(1<<pinNumber);
        break;
        
    case PORTD_ID:
        GPIO_PORTD_LOCK_R= 0X4C4F434B;
        GPIO_PORTD_CR_R|=(1<<pinNumber);
        GPIO_PORTD_DIR_R|=(direction<<pinNumber);
        if(direction==PIN_INPUT) GPIO_PORTD_PUR_R|=(pullup<<pinNumber);
        GPIO_PORTD_DEN_R|=(1<<pinNumber);
        break;
        
    case PORTE_ID:
        GPIO_PORTE_LOCK_R= 0X4C4F434B;
        GPIO_PORTE_CR_R|=(1<<pinNumber);
        GPIO_PORTE_DIR_R|=(direction<<pinNumber);
        if(direction==PIN_INPUT) GPIO_PORTE_PUR_R|=(pullup<<pinNumber);
        GPIO_PORTE_DEN_R|=(1<<pinNumber);
        break;
        
    case PORTF_ID:
        GPIO_PORTF_LOCK_R= 0X4C4F434B;
        GPIO_PORTF_CR_R|=(1<<pinNumber);
        GPIO_PORTF_DIR_R|=(direction<<pinNumber);
        if(direction==PIN_INPUT) GPIO_PORTF_PUR_R|=(pullup<<pinNumber);
        GPIO_PORTF_DEN_R|=(1<<pinNumber);
        break;
    
    default:
        break;
    }
}

void GPIO_writePin(uint8 portNumber,uint8 pinNumber,uint8 value)
{
    switch (portNumber)
    {
    case PORTA_ID:
        value?SET_BIT(GPIO_PORTA_DATA_R,pinNumber):CLEAR_BIT(GPIO_PORTA_DATA_R,pinNumber);
        break;
    case PORTB_ID:
        value?SET_BIT(GPIO_PORTB_DATA_R,pinNumber):CLEAR_BIT(GPIO_PORTB_DATA_R,pinNumber);
        break;
    case PORTC_ID:
        value?SET_BIT(GPIO_PORTC_DATA_R,pinNumber):CLEAR_BIT(GPIO_PORTC_DATA_R,pinNumber);
        break;
    case PORTD_ID:
        value?SET_BIT(GPIO_PORTD_DATA_R,pinNumber):CLEAR_BIT(GPIO_PORTD_DATA_R,pinNumber);
        break;
    case PORTE_ID:
        value?SET_BIT(GPIO_PORTE_DATA_R,pinNumber):CLEAR_BIT(GPIO_PORTE_DATA_R,pinNumber);
        break;
    case PORTF_ID:
        value?SET_BIT(GPIO_PORTF_DATA_R,pinNumber):CLEAR_BIT(GPIO_PORTF_DATA_R,pinNumber);
        break;
    
    default:
        break;
    }
}

void GPIO_writePort(uint8 portNumber,uint8 value)
{
    switch (portNumber)
    {
    case PORTA_ID:
        GPIO_PORTA_DATA_R=value;
        break;
    case PORTB_ID:
        GPIO_PORTB_DATA_R=value;
        break;
    case PORTC_ID:
        GPIO_PORTC_DATA_R=value;
        break;
    case PORTD_ID:
        GPIO_PORTD_DATA_R=value;
        break;
    case PORTE_ID:
        GPIO_PORTE_DATA_R=value;
        break;
    case PORTF_ID:
        GPIO_PORTF_DATA_R=value;
        break;
    
    default:
        break;
    }
}

uint8 GPIO_ReadPin(uint8 portNumber,uint8 pinNumber)
{
    switch (portNumber)
    {
    case PORTA_ID:
        return BIT_IS_SET(GPIO_PORTA_DATA_R,pinNumber)?LOGIC_HIGH:LOGIC_LOW;
        break;
    case PORTB_ID:
        return BIT_IS_SET(GPIO_PORTB_DATA_R,pinNumber)?LOGIC_HIGH:LOGIC_LOW;
        break;
    case PORTC_ID:
        return BIT_IS_SET(GPIO_PORTC_DATA_R,pinNumber)?LOGIC_HIGH:LOGIC_LOW;
        break;
    case PORTD_ID:
        return BIT_IS_SET(GPIO_PORTD_DATA_R,pinNumber)?LOGIC_HIGH:LOGIC_LOW;
        break;
    case PORTE_ID:
        return BIT_IS_SET(GPIO_PORTE_DATA_R,pinNumber)?LOGIC_HIGH:LOGIC_LOW;
        break;
    case PORTF_ID:
        return BIT_IS_SET(GPIO_PORTF_DATA_R,pinNumber)?LOGIC_HIGH:LOGIC_LOW;
        break;
    
    default:
        break;
    }
}

uint8 GPIO_ReadPort(uint8 portNumber)
{
    
    switch (portNumber)
    {
    case PORTA_ID:
        return GPIO_PORTA_DATA_R;
        break;
    case PORTB_ID:
        return GPIO_PORTB_DATA_R;
        break;
    case PORTC_ID:
        return GPIO_PORTC_DATA_R;
        break;
    case PORTD_ID:
        return GPIO_PORTD_DATA_R;
        break;
    case PORTE_ID:
        return GPIO_PORTE_DATA_R;
        break;
    case PORTF_ID:
        return GPIO_PORTF_DATA_R;
        break;
    
    default:
        break;
    }
}

void GPIO_TogglePin(uint8 PortNumber, uint8 PinNumber)
{
    
    switch (PortNumber)
    {
    case PORTA_ID:
        TOGGLE_BIT(GPIO_PORTA_DATA_R, PinNumber);
        break;

    case PORTB_ID:
        TOGGLE_BIT(GPIO_PORTB_DATA_R, PinNumber);
        break;

    case PORTC_ID:
        TOGGLE_BIT(GPIO_PORTC_DATA_R, PinNumber);
        break;

    case PORTD_ID:
        TOGGLE_BIT(GPIO_PORTD_DATA_R, PinNumber);
        break;

    case PORTE_ID:
        TOGGLE_BIT(GPIO_PORTE_DATA_R, PinNumber);
        break;

    case PORTF_ID:
        TOGGLE_BIT(GPIO_PORTF_DATA_R, PinNumber);
        break;
    }
}