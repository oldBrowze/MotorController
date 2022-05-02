/*
 * pwm_core.h
 *
 *  Created on: 30 апр. 2022 г.
 *      Author: oldbrowze
 */

#ifndef INCLUDE_PWM_CORE_H_
#define INCLUDE_PWM_CORE_H_


namespace PWM
{

constexpr uint32_t F_CPU                = 80000000;
constexpr auto _FREQUENCY               = 20000;
constexpr auto _PERIOD                  = F_CPU / _FREQUENCY;

constexpr uint32_t get_normalize_width(const uint32_t width)
{
    return _PERIOD * width / 100;
}


inline void pwm_init()
{
    GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6);                                                // устанавливаем pb6 как выход pwm
    GPIOPinConfigure(GPIO_PB6_M0PWM0);                                                          // привязываем pb6 к каналу M0PWM0

    PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN);                                   // устанавливаем генератор pwm0 в режим счета вниз
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, _PERIOD);                                             // устанавливаем период по константам
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, get_normalize_width(50));                            // устанавливаем заполнение на 50
    PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT, true);                                             // разрешаем выход m0pwm0
    PWMGenEnable(PWM0_BASE, PWM_GEN_0);                                                         // запускаем генератор

    UARTprintf("PWM0 is init\n");
}

};


#endif /* INCLUDE_PWM_CORE_H_ */
