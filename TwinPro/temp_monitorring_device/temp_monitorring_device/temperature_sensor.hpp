#pragma once
#include <stm32f4xx_hal.h>
#include <stm32_hal_legacy.h>
#include "common.hpp"

class InternalTemperatureSensor
{
public:
    
    void Init()
    {
		
		ADC_ChannelConfTypeDef config;

		__HAL_RCC_ADC1_CLK_ENABLE();

		hadc1.Instance = ADC1;
		hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
		hadc1.Init.Resolution = ADC_RESOLUTION12b;
		hadc1.Init.ScanConvMode = DISABLE;
		hadc1.Init.ContinuousConvMode = ENABLE;
		hadc1.Init.DiscontinuousConvMode = DISABLE;
		hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
		hadc1.Init.NbrOfConversion = 1;
		hadc1.Init.DMAContinuousRequests = DISABLE;
		hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
		HAL_ADC_Init(&hadc1);

		config.Channel = ADC_CHANNEL_TEMPSENSOR;
		config.Rank = 1;
		config.SamplingTime = ADC_SAMPLETIME_480CYCLES;
		HAL_ADC_ConfigChannel(&hadc1, &config);

		HAL_ADC_Start(&hadc1);
    }

	float GetTemp()
	{
		uint16_t value = GetValueFromAdc();
		float temp = ((float)value) / 4096 * 3300;
		
		return  temp = ((temp - 760.0) / 2.5) + 25;
	}

private:

	uint16_t GetValueFromAdc()
	{
		HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
		return HAL_ADC_GetValue(&hadc1);
	}

	ADC_HandleTypeDef hadc1;
};