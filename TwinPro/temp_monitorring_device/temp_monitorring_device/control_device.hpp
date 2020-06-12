#pragma once

#include "common.hpp"
#include "temperature_sensor.hpp"
#include "fan.hpp"
#include "communicate.hpp"
#include "device.hpp"

volatile uint16_t systick_ms = 0;

class ControlDevice
{
public:

	void Init()
	{
		_device.Init();
		_temp_sensor.Init();
		_fan.Init();
		serial_port.Init();
	}

	void DeviceStateObserver()
	{
		if (serial_port.IsRequestToMcu())
		{
			WriteRecivedCommandToMcu(serial_port.GetRecivePacket());
			serial_port.ClearRequest();
		}

		if (IsOnResetDevice())
		{
			SetOnResetDevice();
		}

		if (IsOnMonitoring())
		{
			if (IsUpMonitoringTime())
			{
				if (_prev_temp != GetCurrentTemp())
				{
					if ((GetCurrentTemp() - _prev_temp) >= _overheat_temp)
					{
						WriteOverheatStateToMcu(StatusFlags::overheat);
					}
					else
					{
						WriteOverheatStateToMcu(StatusFlags::normalheat);
					}

					if ((GetCurrentTemp() - _prev_temp) >= _fun_start_temp)
					{
						_fan.On();
					}

					if (GetCurrentTemp() >= _critical_temp)
					{
						WriteOverheatStateToMcu(StatusFlags::overheat);
						SetSpeedFan(_max_speed_fan);
					}
					else if (GetCurrentTemp() <= _fun_stop_temp && IsFanOn())
					{
						SetSpeedFan(_min_speed_fan);
						_fan.Off();
					}
					_prev_temp = GetCurrentTemp();
				}
			}
			WriteCurrentTempMcu();
		}
	}

private:
	void WriteOverheatStateToMcu(StatusFlags st)
	{
		mcu.SetStatusMcu(st);
	}

	void WriteCurrentTempMcu()
	{
		mcu.WriteValueToReg(RegAddress::temperature, _temp_sensor.GetTemp());
	}

	float GetCurrentTemp()
	{
		return _temp_sensor.GetTemp();
	}

	void WriteRecivedCommandToMcu(const DataPacketMcu& packet)
	{
		if (packet._type_packet == write)
		{
			if (packet._data == CommandToMcu::recet_mcu)
			{
				mcu.Reset();
			}
			else if (packet._data == CommandToMcu::start_monitorring)
			{
				mcu.RunTempControl();
			}
			else if (packet._data == CommandToMcu::stop_monitorring)
			{
				mcu.StopTempControl();
			}
		}
	}

	bool IsFanOn()
	{
		return _fan.IsOn();
	}

	void SetSpeedFan(uint8_t speed)
	{
		mcu.WriteValueToReg(RegAddress::fan_power, speed);
		_fan.SetPower(speed);
	}

	bool IsOnMonitoring()
	{
		if (mcu.GetStatusMcu() & 0x01)
		{
			return true;
		}
		return false;
	}

	bool IsOnOverheat()
	{
		if (mcu.GetStatusMcu() & 0x02)
		{
			return true;
		}
		return false;
	}

	void SetOnResetDevice()
	{
		_device.ResetDevice();
	}

	bool IsOnResetDevice()
	{
		if (mcu.ReadValueReg(control) & (2 << 1))
		{
			return true;
		}
		return false;
	}

	bool IsUpMonitoringTime()
	{
		if (systick_ms >= _control_time_ms)
		{
			systick_ms = 0;
			return true;
		}
		return false;
	}

private:

	InternalTemperatureSensor _temp_sensor;
	Fan _fan;
	Device _device;

	DataPacketMcu _packet;

	float _prev_temp = 0.0;
	const float _overheat_temp = 20.0;
	const float _fun_start_temp = 2.0;
	const float _fun_stop_temp = 25.0;
	const float _critical_temp = 50.0;

	static const uint8_t _max_speed_fan = 100;
	static const uint8_t _min_speed_fan = 0;

	static const uint16_t _control_time_ms = 5000; //5s

	bool _state_monitoring = false;
};

#ifdef __cplusplus
extern "C"
#endif
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
	++systick_ms;
}
