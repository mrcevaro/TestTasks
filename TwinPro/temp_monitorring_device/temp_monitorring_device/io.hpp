#pragma once
#include <stm32f4xx_hal.h>
#include <stm32_hal_legacy.h>
#include "common.hpp"

class DigitalIO
{
public:
    
    void Init()
    {
		ADC_ChannelConfTypeDef config;

		hadc1.Instance = ADC1;
		hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
       // gpio.Pin = 
    }
};