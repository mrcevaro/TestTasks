#pragma once
#include <stm32f4xx_hal.h>
#include <stm32_hal_legacy.h>

class Device
{
public:
	void Init()
	{
		HAL_Init();

		RCC_OscInitTypeDef RCC_OscInitStruct = {};
		RCC_ClkInitTypeDef RCC_ClkInitStruct = {};
		
		__HAL_RCC_PWR_CLK_ENABLE();
		__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

		RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
		RCC_OscInitStruct.HSEState = RCC_HSE_ON;
		RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
		RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
		RCC_OscInitStruct.PLL.PLLM = RCC_PLLP_DIV4; //4
		RCC_OscInitStruct.PLL.PLLN = 96;
		RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
		RCC_OscInitStruct.PLL.PLLQ = RCC_PLLP_DIV4; //4
		
		if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
		{
		}
			RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
			RCC_OscInitStruct.HSIState = RCC_HSI_ON;
			RCC_OscInitStruct.HSICalibrationValue = 16;
			RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
		
			


		__HAL_RCC_ADC1_CLK_ENABLE();


		/*__GPIOD_CLK_ENABLE();
		GPIO_InitTypeDef GPIO_InitStructure;

		GPIO_InitStructure.Pin = GPIO_PIN_12;

		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
		GPIO_InitStructure.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);*/
	}
};

#ifdef __cplusplus
extern "C"
#endif
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}
