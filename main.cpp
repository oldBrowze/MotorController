#include <stdint.h>
#include <inc/tm4c123gh6pm.h>
#include <driverlib/gpio.h>
#include <driverlib/fpu.h>
#include <driverlib/pwm.h>
#include <driverlib/qei.h>
#include <driverlib/interrupt.h>
#include <driverlib/sysctl.h>
#include <driverlib/pin_map.h>
#include <inc/hw_memmap.h>
#include <inc/hw_gpio.h>
#include <inc/hw_types.h>
#include <driverlib/uart.h>
//#include <inc/hw_ints.h>


#include "utils/uartstdio.h"

//user-defined include
#include "inc/adc_core.h"
#include "inc/ports_core.h"
#include "inc/pwm_core.h"
#include "inc/qei_core.h"
#include "inc/tm4c123_ps2.h"
#include "inc/rs485.h"

inline void sysclk_init()
{
    SysCtlClockSet(SYSCTL_USE_PLL |
                   SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ |
                   SYSCTL_SYSDIV_2_5);

    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART7);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
}

void button_handler()
{

    /*
     * Для проверки стенда
     *
     */

    uint32_t status = GPIOIntStatus(GPIO_PORTF_BASE, true);
    GPIOIntClear(GPIO_PORTF_BASE, status);

    /*
    if(status & GPIO_INT_PIN_4)
        PWM::pulse_width = (PWM::pulse_width < 90) ? PWM::pulse_width + 10 : 90;
    else if(status & GPIO_INT_PIN_0)
        PWM::pulse_width = (PWM::pulse_width > 10) ? PWM::pulse_width - 10 : 10;

    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWM::get_normalize_width(PWM::pulse_width));
    */


    /*
    Для проверки дискретных выводов
    GPIOPinWrite(PORTS::gpio_base, PORTS::gpio_pin_contactor,  ~GPIOPinRead(PORTS::gpio_base, PORTS::gpio_pin_contactor));
    GPIOPinWrite(PORTS::gpio_base, PORTS::gpio_pin_brake,  ~GPIOPinRead(PORTS::gpio_base, PORTS::gpio_pin_brake));
    */



    //ADCProcessorTrigger(ADC0_BASE, ADC::speed_sequencer); для вызова
    //ADCProcessorTrigger(ADC0_BASE, ADC::voltage_sequencer);
    //ADCProcessorTrigger(ADC0_BASE, ADC::current_anchor_sequencer);
    //ADCProcessorTrigger(ADC0_BASE, ADC::current_winding_sequencer);

    UARTprintf("%x\n", PS2::get_value_from_buffer());
}

void crSet(unsigned int base, unsigned char value)
{
    unsigned long v = (HWREG(GPIO_PORTF_BASE + GPIO_O_CR) & 0xFFFFFF00) | value;
    HWREG(base + GPIO_O_CR) = v;
}


int main()
{
    sysclk_init();
    FPUEnable();
    FPULazyStackingEnable();

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTStdioConfig(0, 115200, SysCtlClockGet());
    UARTprintf("UART is init\n");

    PWM::pwm_init();
    QEI::qei_init();
    ADC::ADC_init();
    PORTS::ports_init();

    PS2::init();

    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);

    //разблокировка PF0
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    crSet(GPIO_PORTF_BASE, 1);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    crSet(GPIO_PORTF_BASE, 0);
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;

    GPIOIntRegister(GPIO_PORTF_BASE, button_handler);
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_FALLING_EDGE);
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
    IntEnable(INT_GPIOF);
    IntPrioritySet(INT_GPIOF, 0);

    while(true){}
}
