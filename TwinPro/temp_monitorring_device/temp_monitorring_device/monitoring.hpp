#pragma once

#include "temperature_sensor.hpp"
#include "fan.hpp"
#include "communicate.hpp"
#include "device.hpp"

Device _device;


volatile uint16_t systick_ms = 0;



class TempMonitoring
{
public:


	void TempObserver()
	{
		
	}

	void GetStateMcu()
	{

	}

	void Init()
	{
		_device.Init();
		_temp_sensor.Init();
		_fan.Init();
	
		serial_port.Init();
		_fan.On();
	}

	
	void SetSpeedFan(uint8_t speed)
	{
		mcu.WriteValueToReg(RegAddress::fan_power, speed);
		_fan.SetPower(speed);
	}

	void OnMonitoring() 
	{
		mcu.RunTempControl();
	}

	void StopMonitoring()
	{
		mcu.StopTempControl();
	}

	bool IsOnMonitoring()
	{
		if (mcu.GetStatusMcu() & 0x01)
		{
			return true;
		}
		return false;
	}



	   
private:
	InternalTemperatureSensor _temp_sensor;
	Fan _fan;
	
	Packet _packet;
	
	int8_t _cur_power = 0;

	static const uint16_t _control_time_ms = 5000; //5s
	bool _state_monitoring = false;

	static const uint8_t _max_speed_fan = 100;
	static const uint8_t _min_speed_fan = 0;

	bool IsUpMonitoringTime()
	{
		if (systick_ms >= _control_time_ms)
		{
			systick_ms = 0;
			return true;
		}
		return false;
	}
};

TempMonitoring message;

#ifdef __cplusplus
extern "C"
#endif
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
	++systick_ms;
}
