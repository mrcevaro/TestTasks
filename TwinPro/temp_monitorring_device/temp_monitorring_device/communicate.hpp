#pragma once
#include <stm32f4xx_hal.h>
#include <stm32_hal_legacy.h>


uint8_t firstByteWait = 1;

uint8_t timeOut = 0;
UART_HandleTypeDef _husart2;


enum CorrectMessage
{
	validly,
	wrong
};

enum HeadPacket
{
	recive = 0xCD,
	transmit = 0xEA
};

enum TypePacket
{
	 write = 0x01,
	 read = 0x02,
};

enum RegAddress
{
	control = 0x10,
	status = 0x20,
	temperature = 0x21,
	fan_power = 0x22,
};

enum StatusFlags
{
	on,
	off,
	overheat,
	none
};
//struct Register
//{
//	RegType type;
//	uint8_t value;
//};

struct Register
{
	int8_t CTRL; //0x10
	int8_t ST;   // 0x20
	int8_t TEMP;  // 0x21
	int8_t PWM;  // 0x22
};

//// 0x00 - start temp control, 0x02 - stop temp control, 0x04 - reset chip
//Register reg_control = { RegType::control, 0x02 };
//
//// 0x00 - state temp control (0-OFF, 1-ON), 0x02 - overheat(0-OFF, 1-ON)
//Register reg_status{ RegType::status,  };
//
//// Return current Temp (-...+)
// Register reg_temperature{ RegType::temperature,  };
//
//// Return current pwm of Fun (0-100%)
//Register reg_fan_power{ RegType::fan_power,  };


class Mcu
{
public:
	
	void RunTempControl()
	{
		_mcu_reg.CTRL = (1 << 0);
	}

	void StopTempControl()
	{
		_mcu_reg.CTRL = (1 << 1);
	}

	void Reset()
	{
		_mcu_reg.CTRL = (2 << 1);
	}

	int8_t GetValueReg(RegAddress reg)
	{
		switch (reg)
		{
		case status:
			return _mcu_reg.ST;
		case temperature:
			return _mcu_reg.TEMP;
		case fan_power:
			return _mcu_reg.PWM;
		default:
			break;
		}
		
	}

private:
	Register _mcu_reg;

	void SetStatus(StatusFlags fl)
	{
		switch (fl)
		{
		case on:
			_mcu_reg.ST = (1 << 0);
			break;
		case off:
			_mcu_reg.ST = (0 << 1);
			break;
		case overheat:
			_mcu_reg.ST = 
			break;
		case none:
			break;
		default:
			break;
		}

	}
	

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
		

		return _recive_message;
	}


	CorrectMessage MessageValidly(uint8_t* message)
	{
		if (IsParseMessage(message))
		{
			return CorrectMessage::validly;
		}
		return CorrectMessage::wrong;
	}

	bool IsParseMessage(uint8_t* message)
	{
		if (!IsHeadCorrect(message)) return false;
		_recive_message._head = (HeadPacket)message[0]; //?

		if (!IsCrcCorrect(message, message[4])) return false;
		_recive_message._crc = message[4];

		

		if (!IsTypePacketCorrect(message)) return false;
		_recive_message._type_packet = (TypePacket) message[1]; //?

		if (!IsRegCorrect(message)) return false;
		_recive_message._reg = (RegAddress) message[2]; //?

		_recive_message._data = message[3];

	}


	uint8_t buffer[5] = {};



private:
	
	uint32_t _baudrate = 9600;
	Packet _recive_message = {};

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

SerialPort _serial_port;

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
			HAL_UART_Receive_IT(&_husart2, _serial_port.buffer + 1, 4);
		}
		else
		{
			if (_serial_port.MessageValidly(_serial_port.buffer) == CorrectMessage::validly)
			{
				_serial_port.ClearBuffer();
				firstByteWait = 1;
				HAL_UART_Receive_IT(&_husart2, _serial_port.buffer, 1);
			}
			else {
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






