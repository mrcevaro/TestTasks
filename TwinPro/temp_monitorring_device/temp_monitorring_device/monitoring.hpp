#pragma once

#include "temperature_sensor.hpp"
#include "fan.hpp"
#include "communicate.hpp"


class Monitoring
{
public:

	void Init()
	{
		_temp_sensor.Init();
		_fan.Init();
		_serial_port.Init();
	}

	bool IsOnMonitoring()
	{
		_packet = _serial_port.GetPacket();
	}


private:
	InternalTemperatureSensor _temp_sensor;
	Fan _fan;

	Packet _packet;
};

Monitoring message;