#pragma once
#include <stm32f4xx_hal.h>
#include <stm32_hal_legacy.h>

class Fan
{
public:
	void Init()
	{
		__HAL_RCC_TIM1_CLK_ENABLE();
		__HAL_RCC_GPIOE_CLK_ENABLE();

		GPIO_InitTypeDef GPIO_InitStructure;

		GPIO_InitStructure.Pin = GPIO_PIN_9 ;
		GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStructure.Alternate = GPIO_AF1_TIM1;
		GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
		GPIO_InitStructure.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);

		TIM_HandleTypeDef fan_pwm_timer = {};
		fan_pwm_timer.Instance = TIM1;
		fan_pwm_timer.Init.Prescaler = 7;
		fan_pwm_timer.Init.CounterMode = TIM_COUNTERMODE_UP;
		fan_pwm_timer.Init.Period = 20999; //20999
		fan_pwm_timer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		fan_pwm_timer.Init.RepetitionCounter = 0;
		HAL_TIM_PWM_Init(&fan_pwm_timer);

		TIM_OC_InitTypeDef sConfigOC;
		sConfigOC.OCMode = TIM_OCMODE_PWM1;
		sConfigOC.Pulse = 10499; // 10499 = 50%
		sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
		sConfigOC.OCFastMode = TIM_OCFAST_ENABLE;
		sConfigOC.OCIdleState = TIM_OCIDLESTATE_SET;
		
		uint32_t clock = HAL_RCC_GetPCLK2Freq();

		HAL_TIM_PWM_ConfigChannel(&fan_pwm_timer, &sConfigOC, TIM_CHANNEL_1);

		HAL_TIM_PWM_Start(&fan_pwm_timer, TIM_CHANNEL_1);
	}

private:


};