#ifndef _DEVICE_H_
#define _DEVICE_H_

enum {
	NO_DEVICE=0,
	SD_DEVICE,
	USB_DEVICE,
	configdevice_max
};

enum {
	NONE=0,
	SD,
	USB,
	bootdevice_max
};

enum {
	NO_DOL=0,
	SD_DOL,
	USB_DOL,
	doldevice_max
};

enum {
	NO_BROWSE=0,
	SD_BROWSE,
	USB_BROWSE,
	WII_BROWSE,
	MII_BROWSE,
	browseddevice_max
};

typedef struct
{
	//!ConfigDevice. 0, SD=1, USB=2
	int ConfigDevice;
	//!BootDevice. 0, SD=1, USB=2
	int BootDevice;
	//!DolDevice. 0, SD=1, USB=2
	int DolDevice;
	//!Current Browsed Device. 0, SD=1, USB=2, SAVES=3, MII=4
	int BrowsedDevice;
	//!last device browsed 0, SD=1, USB=2
	int LastBrowsedDevice;
	//!last saves/mii browsed 0, SAVES=3, MII=4
	int LastBrowsedWii;
	//!DeviceCount
	int DeviceCount;
} DEVICEINFO;

extern DEVICEINFO device;

class Device
{
	public:
		//!Constructor
		Device();
		//!Destructor
		~Device();
		bool Init();
		//!Set Device Settings
		int SearchConfig();
		//!Search on wich device boot
		int SearchBootDevice();
		//!Search dol on devices
		int SearchDolDevice();
		//!return number device connected
		int GetDeviceCount();
};

#endif
