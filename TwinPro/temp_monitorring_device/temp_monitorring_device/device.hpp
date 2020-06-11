#pragma once
#include <stm32f4xx_hal.h>
#include <stm32_hal_legacy.h>

class Device
{
public:
	void Init()
	{
		HAL_Init();
		__enable_irq();

		RCC_OscInitTypeDef RCC_OscInitStruct = {};
		RCC_ClkInitTypeDef RCC_ClkInitStruct = {};
		
		__HAL_RCC_PWR_CLK_ENABLE();
		__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

		RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
		RCC_OscInitStruct.HSEState = RCC_HSE_ON;
		RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
		RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
		RCC_OscInitStruct.PLL.PLLM = 4; //4
		RCC_OscInitStruct.PLL.PLLN = 168;
		RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
		RCC_OscInitStruct.PLL.PLLQ = 4; //4
		
		if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
		{

		}

		RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
		RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
		RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
		RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
		
		if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
		{
		
		}
		
		uint32_t clock = HAL_RCC_GetHCLKFreq();

		HAL_SYSTICK_Config(clock / 1000);
		HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	

		__HAL_RCC_GPIOA_CLK_ENABLE();

		GPIO_InitTypeDef GPIO_InitStructure;

		GPIO_InitStructure.Pin = GPIO_PIN_0;
		GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
		GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
		GPIO_InitStructure.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	}

	void ResetDevice()
	{
		NVIC_SystemReset();
	}
};

