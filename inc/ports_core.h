#ifndef INC_PORTS_CORE_H_
#define INC_PORTS_CORE_H_



namespace PORTS
{

constexpr auto gpio_base            = GPIO_PORTA_BASE;
constexpr auto gpio_pin_gercon1     = GPIO_PIN_5;
constexpr auto gpio_pin_gercon2     = GPIO_PIN_6;
constexpr auto gpio_pin_brake       = GPIO_PIN_3;
constexpr auto gpio_pin_contactor   = GPIO_PIN_4;
constexpr auto gpio_pins_output     = gpio_pin_brake | gpio_pin_contactor;
constexpr auto gpio_pins_input      = gpio_pin_gercon1 | gpio_pin_gercon2;

inline void ports_init()
{
    GPIOPinTypeGPIOOutput(gpio_base, gpio_pins_output);
    GPIOPinTypeGPIOInput(gpio_base, gpio_pins_input);
}


};


#endif /* INC_PORTS_CORE_H_ */
