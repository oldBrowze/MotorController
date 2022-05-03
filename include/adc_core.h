/*
 * adc_core.h
 *
 *  Created on: 1 мая 2022 г.
 *      Author: oldbrowze
 */

#ifndef INCLUDE_ADC_CORE_H_
#define INCLUDE_ADC_CORE_H_

#include "driverlib/adc.h"
#include "assert.h"

namespace ADC
{
/*
 * Sequencer    |   samples |   depth of buffer
 *      3               1               1
 *      2               4               4
 *      1               4               4
 *      0               8               8
 */


uint32_t transform_adc_value_to_width();

//занимает AIN0, AIN1
constexpr auto voltage_sequencer    = 2;    // напряжение якоря

//занимает AIN2, AIN3
constexpr auto current0_sequencer   = 1;    // ток якоря

//занимает AIN4
constexpr auto speed_sequencer      = 3;    // задание скорости

//занимает AIN6, AIN7
constexpr auto current1_sequencer   = 2;    // ток возбуждения

constexpr auto max_value_adc        = (1 << 12) - 1;


constexpr uint32_t get_size_buffer(const uint8_t sequencer)
{
    switch(sequencer)
    {
        case 0: return 8;
        case 1:
        case 2: return 4;
        default:
            assert(sequencer > 2);
    }
    return 0;
}

uint32_t adc0_voltage_buffer[get_size_buffer(voltage_sequencer)];   //буфер значений напряжений
uint32_t adc0_current0_buffer[get_size_buffer(current0_sequencer)]; //буфер значений тока якоря
uint32_t adc0_current1_buffer[get_size_buffer(current1_sequencer)]; //буфер значений тока возбужедния

uint32_t transform_adc_value_to_width()
{
    static uint32_t buffer;

    buffer = 0;
    for(auto idx = 0; idx < get_size_buffer(voltage_sequencer); ++idx)
        buffer += adc0_voltage_buffer[idx];
    buffer /= 4;

    return buffer * 100 / max_value_adc;
}

inline void ADC_init()
{
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3 | GPIO_PIN_2);                                                // AIN0-1
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_1 | GPIO_PIN_0);                                                // AIN2-3
    GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_3 | GPIO_PIN_0 | GPIO_PIN_1);                                   // AIN4,6,7


    GPIOPinConfigure(GPIO_PCTL_PE1_AIN2);                                                                    // конфигурация портов как AFIO
    GPIOPinConfigure(GPIO_PCTL_PE0_AIN3);

    GPIOPinConfigure(GPIO_PCTL_PE3_AIN0);
    GPIOPinConfigure(GPIO_PCTL_PE2_AIN1);

    GPIOPinConfigure(GPIO_PCTL_PD3_AIN4);

    GPIOPinConfigure(GPIO_PCTL_PD1_AIN6);
    GPIOPinConfigure(GPIO_PCTL_PD0_AIN7);



    //ADCClockConfigSet(ADC0_BASE, ADC_CLOCK_SRC_PIOSC | ADC_CLOCK_RATE_FULL, 1);
    ADCHardwareOversampleConfigure(ADC0_BASE, 4);                                                           // аппаратное усреднение
    ADCHardwareOversampleConfigure(ADC1_BASE, 4);                                                           // аппаратное усреднение

    ADCReferenceSet(ADC0_BASE, ADC_REF_INT);                                                                // опорное напряжение - 3.3 V
    ADCReferenceSet(ADC1_BASE, ADC_REF_INT);                                                                // опорное напряжение - 3.3 V

    //если ADC_TRIGGER_ALWAYS, то нельзя ставить 0
    ADCSequenceConfigure(ADC0_BASE, voltage_sequencer, ADC_TRIGGER_PROCESSOR, 1);
    ADCSequenceConfigure(ADC0_BASE, current0_sequencer, ADC_TRIGGER_PROCESSOR, 2);
    ADCSequenceConfigure(ADC0_BASE, speed_sequencer, ADC_TRIGGER_PROCESSOR, 0);

    ADCSequenceConfigure(ADC1_BASE, current1_sequencer, ADC_TRIGGER_PROCESSOR, 1);



    ADCSequenceStepConfigure(ADC0_BASE, voltage_sequencer, 0, ADC_CTL_CH0 | ADC_CTL_D);
    ADCSequenceStepConfigure(ADC0_BASE, voltage_sequencer, 1, ADC_CTL_CH0 | ADC_CTL_D);
    ADCSequenceStepConfigure(ADC0_BASE, voltage_sequencer, 2, ADC_CTL_CH0 | ADC_CTL_D);
    ADCSequenceStepConfigure(ADC0_BASE, voltage_sequencer, 3, ADC_CTL_CH0 | ADC_CTL_D | ADC_CTL_END);


    ADCSequenceStepConfigure(ADC0_BASE, current0_sequencer, 0, ADC_CTL_CH2 | ADC_CTL_D);
    ADCSequenceStepConfigure(ADC0_BASE, current0_sequencer, 1, ADC_CTL_CH2 | ADC_CTL_D);
    ADCSequenceStepConfigure(ADC0_BASE, current0_sequencer, 2, ADC_CTL_CH2 | ADC_CTL_D);
    ADCSequenceStepConfigure(ADC0_BASE, current0_sequencer, 3, ADC_CTL_CH2 | ADC_CTL_D | ADC_CTL_END);

    ADCSequenceStepConfigure(ADC0_BASE, speed_sequencer, 0, ADC_CTL_CH4 | ADC_CTL_END);

    ADCSequenceStepConfigure(ADC1_BASE, current1_sequencer, 0, ADC_CTL_CH6 | ADC_CTL_D);
    ADCSequenceStepConfigure(ADC1_BASE, current1_sequencer, 1, ADC_CTL_CH6 | ADC_CTL_D);
    ADCSequenceStepConfigure(ADC1_BASE, current1_sequencer, 2, ADC_CTL_CH6 | ADC_CTL_D);
    ADCSequenceStepConfigure(ADC1_BASE, current1_sequencer, 3, ADC_CTL_CH6 | ADC_CTL_D | ADC_CTL_END);

    ADCSequenceEnable(ADC0_BASE, voltage_sequencer);
    ADCSequenceEnable(ADC0_BASE, current0_sequencer);
    ADCSequenceEnable(ADC0_BASE, speed_sequencer);

    ADCSequenceEnable(ADC1_BASE, current1_sequencer);

    UARTprintf("ADC is init\n");
}


};



#endif /* INCLUDE_ADC_CORE_H_ */
