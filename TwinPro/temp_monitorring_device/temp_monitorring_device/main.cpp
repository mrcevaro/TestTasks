

#include "control_device.hpp"


ControlDevice control_device;

int main()
{
	control_device.Init();
	
	while (true)
	{
		control_device.DeviceStateObserver();
	}

}
