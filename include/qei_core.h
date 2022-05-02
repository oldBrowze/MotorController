/*
 * qei_core.h
 *
 *  Created on: 30 апр. 2022 г.
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

//инициализация пинов/регистров, отвечающих за работу QEI блоков
inline void qei_init()
{

    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;                                       // разблокировка PD7 (исп. для NMI)
    HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= GPIO_PIN_7;
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;


    GPIOPinTypeQEI(GPIO_PORTD_BASE, GPIO_PIN_6);                                                // конфигурируем порт PF0 как вход QEI
    GPIOPinTypeQEI(GPIO_PORTD_BASE, GPIO_PIN_7);                                                // конфигурируем порт PF1 как вход QEI

    GPIOPinTypeQEI(GPIO_PORTC_BASE, GPIO_PIN_5);                                                // конфигурируем порт PF0 как вход QEI
    GPIOPinTypeQEI(GPIO_PORTC_BASE, GPIO_PIN_6);                                                // конфигурируем порт PF1 как вход QEI

    GPIOPinConfigure(GPIO_PD6_PHA0);                                                            // привязываем PD6 к phA (QEI0)
    GPIOPinConfigure(GPIO_PD7_PHB0);                                                            // привязываем PD7 к phB (QEI0)

    GPIOPinConfigure(GPIO_PC5_PHA1);                                                            // привязываем PC5 к phA (QEI1)
    GPIOPinConfigure(GPIO_PC6_PHB1);                                                            // привязываем PC6 к phB (QEI1)

    QEIDisable(QEI0_BASE);                                                                      // отключаем QEI0-модуль
    QEIDisable(QEI1_BASE);                                                                      // отключаем QEI1-модуль

    QEIIntDisable(QEI0_BASE, QEI_INTERROR | QEI_INTDIR | QEI_INTTIMER | QEI_INTINDEX);          // отключаем прерывания модуля QEI0
    QEIIntDisable(QEI1_BASE, QEI_INTERROR | QEI_INTDIR | QEI_INTTIMER | QEI_INTINDEX);          // отключаем прерывания модуля QEI1

    QEIConfigure(QEI0_BASE,                                                                     // настраиваем блок QEI0
                 QEI_CONFIG_CAPTURE_A_B | \
                 QEI_CONFIG_NO_RESET | \
                 QEI_CONFIG_QUADRATURE | \
                 QEI_CONFIG_SWAP | \
                 QEI_CTL_INVA |\
                 QEI_CTL_INVB, _MAX_ENCODER_VALUE);

    QEIConfigure(QEI1_BASE,                                                                     // настраиваем блок QEI1
                     QEI_CONFIG_CAPTURE_A_B | \
                     QEI_CONFIG_NO_RESET | \
                     QEI_CONFIG_QUADRATURE | \
                     QEI_CONFIG_SWAP | \
                     QEI_CTL_INVA |\
                     QEI_CTL_INVB, _MAX_ENCODER_VALUE);

    //QEIFilterConfigure(QEI0_BASE, QEI_FILTCNT_2);
    //QEIFilterEnable(QEI0_BASE);
    QEIIntRegister(QEI0_BASE, QEI0_Handler);                                                    // регистрируем event(указатель на функцию) при прерывании
    QEIIntEnable(QEI0_BASE, QEI_INTTIMER);                                                      // инициируем прерывание при velocity-time

    QEIIntRegister(QEI1_BASE, QEI1_Handler);                                                    // регистрируем event(указатель на функцию) при прерывании
    QEIIntEnable(QEI1_BASE, QEI_INTTIMER);                                                      // инициируем прерывание при velocity-time

    QEIEnable(QEI0_BASE);                                                                       // запускаем QEI0-модуль
    QEIEnable(QEI1_BASE);                                                                       // запускаем QEI1-модуль

    QEIPositionSet(QEI0_BASE, _START_VALUE_QEI0);                                               // указываем начальную позицию QEI0-модуля
    QEIPositionSet(QEI1_BASE, _START_VALUE_QEI1);                                               // указываем начальную позицию QEI0-модуля

    UARTprintf("QUE0/1 is init\n");
}

//обработчик прерывания первого модуля
inline void QEI0_Handler()
{
    QEIIntClear(QEI0_BASE, QEI_INTTIMER);
    //...
}

//обработчик прерывания второго модуля
inline void QEI1_Handler()
{
    QEIIntClear(QEI1_BASE, QEI_INTTIMER);

    //...
}

};

#endif /* INCLUDE_QEI_CORE_H_ */
