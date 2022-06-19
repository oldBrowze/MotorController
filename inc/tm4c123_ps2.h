/*
 * tm4c123_ps2.h
 *
 *  Created on: 17 θών. 2022 γ.
 *      Author: oldbrowze
 */

#ifndef INC_TM4C123_PS2_H_
#define INC_TM4C123_PS2_H_

#include <inc/tm4c123gh6pm.h>
#include <driverlib/gpio.h>
#include <stdint.h>

namespace PS2
{

void pin_clock_IT_Handler();

volatile uint8_t active_code = 0x0;
volatile bool isBusy = true;
volatile uint8_t counter = 1;

constexpr uint32_t
    gpio_base = GPIO_PORTB_BASE,
    gpio_port_data = GPIO_PIN_0,
    gpio_port_clock = GPIO_PIN_1;



void init()
{
    GPIOPinTypeGPIOInput(gpio_base, gpio_port_clock | gpio_port_data);
    //GPIOPadConfigSet(gpio_base, gpio_port_clock, GPIO_STRENGTH_6MA, GP)
    GPIOIntRegister(gpio_base, PS2::pin_clock_IT_Handler);
    GPIOIntTypeSet(gpio_base, gpio_port_clock, GPIO_FALLING_EDGE);
    GPIOIntEnable(gpio_base, gpio_port_clock);
    IntEnable(INT_GPIOA);
    //IntPrioritySet(INT_GPIOF, 0);
}

uint8_t get_value_from_buffer()
{
    uint8_t buffer = active_code;
    if(isBusy)
    {
        active_code = 0x0;
        return buffer;
    }
    return 0x1;
}
void receive_enable();
void reveive_disable();

void transmit();


void pin_clock_IT_Handler()
{
    uint32_t status = GPIOIntStatus(gpio_base, true);
    GPIOIntClear(gpio_base, status);
    if(counter == 33)
    {
        counter = 1;
        isBusy = true;
        return;
    }
    isBusy = false;

    if(counter > 1 && counter < 10)
        active_code |= GPIOPinRead(gpio_base, gpio_port_data) << (counter - 2);



    ++counter;
}

}


#endif /* INC_TM4C123_PS2_H_ */
