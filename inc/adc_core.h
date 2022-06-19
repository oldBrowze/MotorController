#ifndef INC_ADC_CORE_H_
#define INC_ADC_CORE_H_

#include "driverlib/adc.h"
#include <driverlib/pin_map.h>
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

/*
 * Sequencer    |   Pins    |
 *
 * speed            PE3(signal)
 * voltage          PD3(+), PD2(-)
 * current_anchor   PD1(+), PD0(-)
 * current_winding  PE5(+), PE4(-)
 *
 */

constexpr uint8_t
    speed_sequencer = 3,
    current_anchor_sequencer = 2,
    current_winding_sequencer = 1,
    voltage_sequencer = 0;

uint32_t
    speed_value = 0,
    current_anchor_value = 0,
    current_winding_value = 0,
    voltage_value = 0;

void ADC0_voltage_IT_Handler();
void ADC0_speed_IT_Handler();
void ADC0_current_anchor_IT_Handler();
void ADC0_current_winding_IT_Handler();


inline void ADC_init()
{
    ADCReferenceSet(ADC0_BASE, ADC_REF_INT);

    ADCSequenceConfigure(ADC0_BASE, speed_sequencer, ADC_TRIGGER_PROCESSOR, 1);
    ADCSequenceConfigure(ADC0_BASE, voltage_sequencer, ADC_TRIGGER_PROCESSOR, 1);
    ADCSequenceConfigure(ADC0_BASE, current_anchor_sequencer, ADC_TRIGGER_PROCESSOR, 1);
    ADCSequenceConfigure(ADC0_BASE, current_winding_sequencer, ADC_TRIGGER_PROCESSOR, 1);

    ADCSequenceStepConfigure(ADC0_BASE, speed_sequencer, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH0);
    ADCSequenceStepConfigure(ADC0_BASE, voltage_sequencer, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH2 | ADC_CTL_D);
    ADCSequenceStepConfigure(ADC0_BASE, current_anchor_sequencer, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH3 | ADC_CTL_D);
    ADCSequenceStepConfigure(ADC0_BASE, current_winding_sequencer, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH4 | ADC_CTL_D);

    ADCHardwareOversampleConfigure(ADC0_BASE, 4);


    ADCIntEnable(ADC0_BASE, speed_sequencer);
    ADCIntEnable(ADC0_BASE, voltage_sequencer);
    ADCIntEnable(ADC0_BASE, current_anchor_sequencer);
    ADCIntEnable(ADC0_BASE, current_winding_sequencer);

    ADCIntRegister(ADC0_BASE, speed_sequencer, ADC0_speed_IT_Handler);
    ADCIntRegister(ADC0_BASE, voltage_sequencer, ADC0_voltage_IT_Handler);
    ADCIntRegister(ADC0_BASE, current_anchor_sequencer, ADC0_current_anchor_IT_Handler);
    ADCIntRegister(ADC0_BASE, current_winding_sequencer, ADC0_current_winding_IT_Handler);

    ADCSequenceEnable(ADC0_BASE, speed_sequencer);
    ADCSequenceEnable(ADC0_BASE, voltage_sequencer);
    ADCSequenceEnable(ADC0_BASE, current_anchor_sequencer);
    ADCSequenceEnable(ADC0_BASE, current_winding_sequencer);


    UARTprintf("ADC is init\n");
}


void ADC0_voltage_IT_Handler()
{
    ADCIntClear(ADC0_BASE, voltage_sequencer);

    ADCSequenceDataGet(ADC0_BASE, voltage_sequencer, &voltage_value);
    UARTprintf("ADC0 = %u\n", voltage_value);

}
void ADC0_speed_IT_Handler()
{
    ADCIntClear(ADC0_BASE, speed_sequencer);

    //ADCSequenceDataGet(ADC0_BASE, voltage_sequencer, &voltage_value);
    //UARTprintf("ADC0 = %u\n", voltage_value);
}
void ADC0_current_anchor_IT_Handler()
{
    ADCIntClear(ADC0_BASE, current_anchor_sequencer);

    ADCSequenceDataGet(ADC0_BASE, current_anchor_sequencer, &current_anchor_value);
    UARTprintf("ADC0 = %u\n", current_anchor_value);

}
void ADC0_current_winding_IT_Handler()
{
    ADCIntClear(ADC0_BASE, current_winding_sequencer);

    ADCSequenceDataGet(ADC0_BASE, current_winding_sequencer, &current_winding_value);
    UARTprintf("ADC0 = %u\n", current_winding_value);

}

};



#endif /* INC_ADC_CORE_H_ */
