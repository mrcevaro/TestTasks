#pragma once
#include <stm32f4xx_hal.h>
#include <stm32_hal_legacy.h>


uint8_t firstByteWait = 1;

uint8_t timeOut = 0;
UART_HandleTypeDef _husart2;


enum HeadPacket
{
	recive = 0xCD,
	transmit = 0xEA
};

enum TypePacket
{
	 write = 0x01,
	 read = 0x02
};

enum Reg
{
	control = 0x10,
	status = 0x20,
	temperature = 0x21,
	fan_power = 0x22
};

struct Packet 
{
	 HeadPacket _head ;
	 TypePacket _type_packet ;
	 Reg _reg ;
	 uint8_t _data = 0;
	 uint8_t _crc = 0;
};

//Packet   _recive_message = { 0xCD };
//Packet _transmite_message = { 0xEA };




class SerialPort
{
public:
	void Init() 
	{
		
		__HAL_RCC_USART2_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();

		GPIO_InitTypeDef GPIO_InitStructure;

		GPIO_InitStructure.Pin = GPIO_PIN_2 | GPIO_PIN_3;
		GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStructure.Alternate = GPIO_AF7_USART2;
		GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
		GPIO_InitStructure.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

		_husart2.Instance = USART2;
		_husart2.Init.BaudRate = _baudrate;
		_husart2.Init.WordLength = UART_WORDLENGTH_8B;
		_husart2.Init.StopBits = UART_STOPBITS_1;
		_husart2.Init.Parity = UART_PARITY_NONE;
		_husart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
		_husart2.Init.Mode = UART_MODE_TX_RX;

		if (HAL_UART_Init(&_husart2) != HAL_OK)
		{
		}

		HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(USART2_IRQn);
	    
		firstByteWait = 1;
		HAL_UART_Receive_IT(&_husart2, buffer, 1);
	
	}

	void TransmitMessage(uint8_t* msg)
	{
		HAL_UART_Transmit(&_husart2, msg, sizeof(msg)+1, HAL_MAX_DELAY);
	}


	bool IsCrcCorrect(uint8_t* packet, uint8_t crc)
	{
		uint8_t xorw = 0;
		for (int i = 0; i < sizeof(packet) - 1; i++) {
			xorw = xorw ^ packet[i];
		}

		return crc == xorw;
	}

	void ClearBuffer()
	{
		for (int i = 0; i < sizeof(buffer); i++)
		{
			buffer[i] = 0;
		}
	}

	Packet GetPacket()
	{
		
		_recive_message._type_packet = GetTypePacket();
		_recive_message._crc = buffer[4];

		return _recive_message;
	}

	uint8_t buffer[5] = {};



private:
	
	uint32_t _baudrate = 9600;
	Packet _recive_message = {};

	TypePacket GetTypePacket()
	{
		switch (buffer[1])
		{
		case 0x01:
			return TypePacket::write;
			break;
		case 0x02:
			return TypePacket::read;
			break;
		}
	}
};

SerialPort _serial_port;


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	
	if (huart == &_husart2)
	{
		if (firstByteWait != 0)
		{
			firstByteWait = 0;
			
			HAL_UART_Receive_IT(&_husart2, _serial_port.buffer + 1, 4);
		}
		else
		{
			if (_serial_port.buffer[0] == HeadPacket::recive &&  _serial_port.IsCrcCorrect(_serial_port.buffer, _serial_port.buffer[4]))
			{
				_serial_port.ClearBuffer();
				firstByteWait = 1;
				HAL_UART_Receive_IT(&_husart2, _serial_port.buffer, 1);
			}
			
		
		}
	}
}

#ifdef __cplusplus
extern "C"
#endif
void USART2_IRQHandler()
{
	HAL_UART_IRQHandler(&_husart2);
	__HAL_UART_CLEAR_OREFLAG(&_husart2);
}






