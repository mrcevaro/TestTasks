#pragma once
#include <stm32f4xx_hal.h>
#include <stm32_hal_legacy.h>

#include "mcu.hpp"


enum HeadPacket : uint8_t
{
	recive = 0xCD,
	transmit = 0xEA
};

enum TypePacket : uint8_t
{
	write = 0x01,
	read = 0x02,
};

enum CommandToMcu : uint8_t
{
	start_monitorring = 0x01,
	stop_monitorring = 0x00,
	recet_mcu = 0x02,
};

struct DataPacketMcu
{
	HeadPacket _head;
	TypePacket _type_packet;
	RegAddress _reg;
	uint8_t _data = 0;
	uint8_t _crc = 0;
};
