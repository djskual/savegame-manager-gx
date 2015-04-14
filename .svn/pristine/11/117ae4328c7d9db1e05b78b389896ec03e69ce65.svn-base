#include <ogcsys.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file/fileops.h"
#include "main.h"

DEVICEINFO device;

void ResetDevice()
{
	device.ConfigDevice = 0;
	device.BootDevice = 0;
	device.DolDevice = 0;
	device.DeviceCount = 0;
	device.BrowsedDevice = 0;
	device.LastBrowsedDevice = 0;
	device.LastBrowsedWii = 3;
}

Device::Device()
{
	
}

Device::~Device()
{
}

int Device::SearchConfig()
{
	char filepath[300];
	
	if (USBDevice_Inserted())
	{
		snprintf(filepath, sizeof(filepath), "usb:/%s%s", CONFIGPATH, CONFIGNAME);
		if (CheckFile(filepath)) {
			device.ConfigDevice = 2;
		}
	}
	if (SDCard_Inserted())
	{
		snprintf(filepath, sizeof(filepath), "sd:/%s%s", CONFIGPATH, CONFIGNAME);
		if (CheckFile(filepath)) {
			device.ConfigDevice = 1;
		}
	}

	return device.ConfigDevice;
}

int Device::SearchBootDevice()
{
	if (USBDevice_Inserted())
		device.BootDevice = 2;
	if (SDCard_Inserted())
		device.BootDevice = 1;
	return device.BootDevice;
}

int Device::SearchDolDevice()
{
	if (USBDevice_Inserted())
	{
		if (CheckFile((char*) "usb:/apps/SaveGameManagerGX/boot.dol") || CheckFile((char*) "usb:/apps/SaveGameManagerGX/boot.elf")
        || CheckFile((char*) "usb:/apps/SaveGame_Manager_GX/boot.dol") || CheckFile((char*) "usb:/apps/SaveGame_Manager_GX/boot.elf")) {
			device.DolDevice = 2;
		}
		
	}
	if (SDCard_Inserted())
	{
		if (CheckFile((char*) "sd:/apps/SaveGameManagerGX/boot.dol") || CheckFile((char*) "sd:/apps/SaveGameManagerGX/boot.elf")
        || CheckFile((char*) "sd:/apps/SaveGame_Manager_GX/boot.dol") || CheckFile((char*) "sd:/apps/SaveGame_Manager_GX/boot.elf")) {
			device.DolDevice = 1;
		}
	}
	
	return device.DolDevice;
}

int Device::GetDeviceCount()
{
	device.DeviceCount = 0;
	if (USBDevice_Inserted())
		device.DeviceCount++;
	if (SDCard_Inserted())
		device.DeviceCount++;
	return device.DeviceCount;
}

bool Device::Init()
{
	ResetDevice();
	
	if (!Device_Inserted())
		return false;
	
	SearchConfig();
	
	if(device.ConfigDevice == 0)
	{
		SearchBootDevice();
		
		if(GetDeviceCount() >= 2)
		{
			if(SearchDolDevice() != 0)
				device.BootDevice = device.DolDevice;
		}
		device.ConfigDevice = device.BootDevice;
	}
	device.BootDevice = device.ConfigDevice;
	device.BrowsedDevice = device.ConfigDevice;
	device.DeviceCount = GetDeviceCount();	
	
	return true;
}
