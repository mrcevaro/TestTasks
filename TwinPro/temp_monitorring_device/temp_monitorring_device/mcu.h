#pragma once
#include <stm32f4xx_hal.h>
#include <stm32_hal_legacy.h>




enum RegAddress : uint8_t
{
	control = 0x10,
	status = 0x20,
	temperature = 0x21,
	fan_power = 0x22,
};

enum StatusFlags
{
	on_temp_control,
	off_temp_control,
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
	int8_t _ctrl; //0x10
	int8_t _st;   // 0x20
	int8_t _temp;  // 0x21
	int8_t _pwm;  // 0x22
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
		_reg._ctrl = (1 << 0);
		_reg._ctrl &= (0 << 1);
		SetStatusMcu(StatusFlags::on_temp_control);
	}

	void StopTempControl()
	{
		_reg._ctrl = (1 << 1);
		_reg._ctrl &= (0 << 0);
		SetStatusMcu(StatusFlags::off_temp_control);
	}

	void Reset()
	{
		_reg._ctrl |= (2 << 1);

	}

	const int8_t GetStatusMcu()
	{
		return _reg._st;
	}

	const int8_t ReadValueReg(RegAddress reg)
	{
		switch (reg)
		{
		case temperature:
			return _reg._temp;
		case fan_power:
			return _reg._pwm;
		}
	}

	void WriteValueToReg(RegAddress reg, int8_t val)
	{
		switch (reg)
		{
		case temperature:
			_reg._temp = val;
			break;
		case fan_power:
			_reg._pwm = val;
			break;
		}

	}

	void SetStatusMcu(StatusFlags fl)
	{
		switch (fl)
		{
		case on_temp_control:
			_reg._st |= (1 << 0);
			break;
		case off_temp_control:
			_reg._st &= (0 << 0);
			break;
		case overheat:
			_reg._st |= (1 << 1);
			break;
		case none:
			_reg._st &= (0 << 1);
			break;
		}

	}

private:
	Register _reg;
};

Mcu mcu;