#include "common.hpp"

#include "monitoring.hpp"


TempMonitoring _monitoring;

int main()
{


	_monitoring.Init();


	while (true)
	{
		
		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET)
		{
			
		}
	//HAL_Delay(1000);

	}

	//HAL_Init();

	//__GPIOD_CLK_ENABLE();
	//GPIO_InitTypeDef GPIO_InitStructure;

	//GPIO_InitStructure.Pin = GPIO_PIN_12;

	//GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	//GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	//GPIO_InitStructure.Pull = GPIO_NOPULL;
	//HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);

	//for (;;)
	//{
	//	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
	//	HAL_Delay(500);
	//	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
	//	HAL_Delay(500);
	//}
}
