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



#include "utils/uartstdio.h"

//user-defined include
#include "qei_core.h"
#include "pwm_core.h"
#include "adc_core.h"

//������������� ������������ ���� ���������
inline void sysclk_init()
{
    SysCtlClockSet(SYSCTL_USE_PLL |                                                             // ������������ ��
                   SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ |
                   SYSCTL_SYSDIV_2_5);

    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);                                                         // ������� PWM = Fcpu
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);                                                 // ��������� PWM
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);                                                // ��������� GPIOB
    SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0);                                                 // ��������� QEI0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI1);                                                 // ��������� QEI1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);                                                // ��������� GPIOD
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);                                                // ��������� GPIOE
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);                                                // ��������� GPIOC
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);                                                // ��������� UART
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);                                                 // ��������� ADC0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);                                                // ��������� GPIOA
}

inline void interrupt_init()
{
    IntMasterEnable();                                                                          // ��������� ��������� ����������
    IntEnable(INT_QEI0);                                                                        // �������� ���������� QEI0
    IntEnable(INT_QEI1);                                                                        // �������� ���������� QEI1
}



int main()
{
    sysclk_init();
    FPUEnable();
    FPULazyStackingEnable();

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    UARTStdioConfig(0, 115200, SysCtlClockGet());
    UARTprintf("UART is init\n");

    PWM::pwm_init();
    QEI::qei_init();
    ADC::ADC_init();
    //interrupt_init();


    while(true)
    {
        ADCProcessorTrigger(ADC0_BASE, ADC::voltage_sequencer);
        while(ADCBusy(ADC0_BASE));

        ADCSequenceDataGet(ADC0_BASE, ADC::voltage_sequencer, ADC::adc0_voltage_buffer);
        UARTprintf("ADC value AIN0-AIN1 = %u\n", ADC::transform_adc_value_to_width());
    }
}
