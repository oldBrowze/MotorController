/*
 * qei_core.h
 *
 *  Created on: 30 ���. 2022 �.
 *      Author: oldbrowze
 */

#ifndef INCLUDE_QEI_CORE_H_
#define INCLUDE_QEI_CORE_H_


namespace QEI
{

constexpr uint32_t _MAX_ENCODER_VALUE   = 100u;
constexpr uint8_t _START_VALUE_QEI0     = 0u;
constexpr uint8_t _START_VALUE_QEI1     = 0u;

void QEI0_Handler();
void QEI1_Handler();

//������������� �����/���������, ���������� �� ������ QEI ������
inline void qei_init()
{

    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;                                       // ������������� PD7 (���. ��� NMI)
    HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= GPIO_PIN_7;
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;


    GPIOPinTypeQEI(GPIO_PORTD_BASE, GPIO_PIN_6);                                                // ������������� ���� PF0 ��� ���� QEI
    GPIOPinTypeQEI(GPIO_PORTD_BASE, GPIO_PIN_7);                                                // ������������� ���� PF1 ��� ���� QEI

    GPIOPinTypeQEI(GPIO_PORTC_BASE, GPIO_PIN_5);                                                // ������������� ���� PF0 ��� ���� QEI
    GPIOPinTypeQEI(GPIO_PORTC_BASE, GPIO_PIN_6);                                                // ������������� ���� PF1 ��� ���� QEI

    GPIOPinConfigure(GPIO_PD6_PHA0);                                                            // ����������� PD6 � phA (QEI0)
    GPIOPinConfigure(GPIO_PD7_PHB0);                                                            // ����������� PD7 � phB (QEI0)

    GPIOPinConfigure(GPIO_PC5_PHA1);                                                            // ����������� PC5 � phA (QEI1)
    GPIOPinConfigure(GPIO_PC6_PHB1);                                                            // ����������� PC6 � phB (QEI1)

    QEIDisable(QEI0_BASE);                                                                      // ��������� QEI0-������
    QEIDisable(QEI1_BASE);                                                                      // ��������� QEI1-������

    QEIIntDisable(QEI0_BASE, QEI_INTERROR | QEI_INTDIR | QEI_INTTIMER | QEI_INTINDEX);          // ��������� ���������� ������ QEI0
    QEIIntDisable(QEI1_BASE, QEI_INTERROR | QEI_INTDIR | QEI_INTTIMER | QEI_INTINDEX);          // ��������� ���������� ������ QEI1

    QEIConfigure(QEI0_BASE,                                                                     // ����������� ���� QEI0
                 QEI_CONFIG_CAPTURE_A_B | \
                 QEI_CONFIG_NO_RESET | \
                 QEI_CONFIG_QUADRATURE | \
                 QEI_CONFIG_SWAP | \
                 QEI_CTL_INVA |\
                 QEI_CTL_INVB, _MAX_ENCODER_VALUE);

    QEIConfigure(QEI1_BASE,                                                                     // ����������� ���� QEI1
                     QEI_CONFIG_CAPTURE_A_B | \
                     QEI_CONFIG_NO_RESET | \
                     QEI_CONFIG_QUADRATURE | \
                     QEI_CONFIG_SWAP | \
                     QEI_CTL_INVA |\
                     QEI_CTL_INVB, _MAX_ENCODER_VALUE);

    //QEIFilterConfigure(QEI0_BASE, QEI_FILTCNT_2);
    //QEIFilterEnable(QEI0_BASE);
    QEIIntRegister(QEI0_BASE, QEI0_Handler);                                                    // ������������ event(��������� �� �������) ��� ����������
    QEIIntEnable(QEI0_BASE, QEI_INTTIMER);                                                      // ���������� ���������� ��� velocity-time

    QEIIntRegister(QEI1_BASE, QEI1_Handler);                                                    // ������������ event(��������� �� �������) ��� ����������
    QEIIntEnable(QEI1_BASE, QEI_INTTIMER);                                                      // ���������� ���������� ��� velocity-time

    QEIEnable(QEI0_BASE);                                                                       // ��������� QEI0-������
    QEIEnable(QEI1_BASE);                                                                       // ��������� QEI1-������

    QEIPositionSet(QEI0_BASE, _START_VALUE_QEI0);                                               // ��������� ��������� ������� QEI0-������
    QEIPositionSet(QEI1_BASE, _START_VALUE_QEI1);                                               // ��������� ��������� ������� QEI0-������

    UARTprintf("QUE0/1 is init\n");
}

//���������� ���������� ������� ������
inline void QEI0_Handler()
{
    QEIIntClear(QEI0_BASE, QEI_INTTIMER);
    //...
}

//���������� ���������� ������� ������
inline void QEI1_Handler()
{
    QEIIntClear(QEI1_BASE, QEI_INTTIMER);

    //...
}

};

#endif /* INCLUDE_QEI_CORE_H_ */
