#pragma once
#include <stm32f4xx_hal.h>
#include <stm32_hal_legacy.h>


UART_HandleTypeDef huart2;
//
//struct Pin 
//{
//    GPIO_TypeDef* _port = nullptr;
//    uint16_t _gpio_pin = {};
//};
//
//static constexpr   Pin  T_SENSOR = { GPIOB, GPIO_PIN_0 };
//static constexpr   Pin  UART1_TX = { GPIOB, GPIO_PIN_0 };
//static constexpr   Pin  UART1_RX = { GPIOB, GPIO_PIN_0 };
//static constexpr   Pin  PWM = { GPIOB, GPIO_PIN_0 };