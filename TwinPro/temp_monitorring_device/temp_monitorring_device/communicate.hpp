#pragma once
#include <stm32f4xx_hal.h>
#include <stm32_hal_legacy.h>
#include "mcu.h"

uint8_t firstByteWait = 1;

uint8_t timeOut = 0;
UART_HandleTypeDef _husart2;


enum CorrectMessage
{
	validly,
	wrong
};

enum HeadPacket : uint8_t
{
	recive = 0xCD,
	transmit = 0xEA
};

enum TypePacket
{
	 write = 0x01,
	 read = 0x02,
};



struct Packet 
{
	 HeadPacket _head ;
	 TypePacket _type_packet ;
	 RegAddress _reg ;
	 uint8_t _data = 0 ;
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
		HAL_UART_Receive_IT(&_husart2, _recive_buffer, 1);
	
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

	void ClearBuffer(uint8_t* buffer)
	{
		for (int i = 0; i < sizeof(buffer); i++)
		{
			buffer[i] = 0;
		}
	}

	Packet GetRecivePacket()
	{

		return _recive_packet;
	}


	Packet GetTranssmitPacket()
	{

		return _transsmit_packet;
	}

	void CreatePacket(uint8_t* message, Packet& packet)
	{
		packet._head = (HeadPacket)message[0]; //?
		packet._type_packet = (TypePacket) message[1]; //?
		packet._reg = (RegAddress) message[2]; //?
		packet._data = message[3];
		packet._crc = message[4];
	}


	uint8_t GetCrc(uint8_t* packet)
	{
		uint8_t xorw = 0;
		for (int i = 0; i < sizeof(packet) - 1; i++) {
			xorw = xorw ^ packet[i];
		}
		return xorw;
	}

	void CreateAnswerMessage()
	{
		_transsmit_packet._head = HeadPacket::transmit;
		_transsmit_packet._type_packet = _recive_packet._type_packet;
		_transsmit_packet._reg = _recive_packet._reg;

		if (_transsmit_packet._type_packet == TypePacket::write)
		{
			_transsmit_packet._data = _recive_packet._data;
		}
		else
		{
			if (_recive_packet._reg == RegAddress::status)
			{
				_transsmit_packet._data = mcu.GetStatusMcu();
			}
			else
			{
				_transsmit_packet._data = mcu.ReadValueReg(_recive_packet._reg);
			}
		}

		_transsmit_packet._crc = GetCrc((uint8_t*)&_transsmit_packet);
	}

	void SendAnswerMessage()
	{
		CreateAnswerMessage();
		HAL_UART_Transmit_IT(&_husart2, (uint8_t*)&_transsmit_packet, 5);
	}

	CorrectMessage MessageValidly(uint8_t* message)
	{
		if (IsParseMessage(message))
		{
			CreatePacket(message, _recive_packet);
			return CorrectMessage::validly;
		}
		return CorrectMessage::wrong;
	}

	bool IsParseMessage(uint8_t* message)
	{
		if (!IsHeadCorrect(message)) return false;
		//_recive_packet._head = (HeadPacket)message[0]; //?

		if (!IsCrcCorrect(message, message[4])) return false;
		//_recive_packet._crc = message[4];

		

		if (!IsTypePacketCorrect(message)) return false;
		//_recive_packet._type_packet = (TypePacket) message[1]; //?

		if (!IsRegCorrect(message)) return false;
		//_recive_packet._reg = (RegAddress) message[2]; //?

		//_recive_packet._data = message[3];
		return true;
	}




	uint8_t _recive_buffer[5] = {};
	uint8_t _transsmite_buffer[5] = {};


private:
	

	uint32_t _baudrate = 9600;
	Packet _recive_packet = {};
	Packet _transsmit_packet = {};

	bool IsHeadCorrect(uint8_t* message)
	{
		if (message[0] == HeadPacket::recive || message[0] == HeadPacket::transmit)
		{
			return true;
		}
		return false;
	}

	bool IsTypePacketCorrect(uint8_t* message)
	{
		if (message[1] == TypePacket::read || message[1] == TypePacket::write)
		{
			return true;
		}
		return false;
	}

	bool IsRegCorrect(uint8_t* message)
	{
		uint8_t regs[4] = { RegAddress::control, RegAddress::fan_power, RegAddress::status, RegAddress::temperature };
		for (size_t i = 0; i < 4; i++)
		{
			if (message[2] == regs[i]) return true;
		}
		return false;
	}
};

SerialPort serial_port;


#ifdef __cplusplus
extern "C"
#endif
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	if (huart == &_husart2)
	{
		if (firstByteWait != 0)
		{
			firstByteWait = 0;
			HAL_UART_Receive_IT(&_husart2, serial_port._recive_buffer + 1, 4);
		}
		else
		{
			if (serial_port.MessageValidly(serial_port._recive_buffer) == CorrectMessage::validly)
			{
				serial_port.SendAnswerMessage();
				serial_port.ClearBuffer(serial_port._recive_buffer);
				firstByteWait = 1;
				HAL_UART_Receive_IT(&_husart2, serial_port._recive_buffer, 1);
			}
			else {
				serial_port.ClearBuffer(serial_port._recive_buffer);
				firstByteWait = 1;
				HAL_UART_Receive_IT(&_husart2, serial_port._recive_buffer, 1);
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






