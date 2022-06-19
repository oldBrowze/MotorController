/*
 * rs485.h
 *
 *  Created on: 19 θών. 2022 γ.
 *      Author: oldbrowze
 */

#ifndef INC_RS485_H_
#define INC_RS485_H_

namespace RS485
{


constexpr uint32_t gpio_port = GPIO_PORTE_BASE;
constexpr uint8_t gpio_pins = GPIO_PIN_0 | GPIO_PIN_1;

void init()
{
    GPIOPinConfigure(GPIO_PE0_U7RX);
    GPIOPinConfigure(GPIO_PE1_U7TX);
    GPIOPinTypeUART(gpio_port, gpio_pins);

    //UARTConfigSetExpClk(ui32Base, ui32UARTClk, ui32Baud, ui32Config)
}



}



#endif /* INC_RS485_H_ */
