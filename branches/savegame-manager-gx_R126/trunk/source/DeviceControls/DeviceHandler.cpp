/****************************************************************************
 * Copyright (C) 2010
 * by Dimok
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any
 * damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any
 * purpose, including commercial applications, and to alter it and
 * redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you
 * must not claim that you wrote the original software. If you use
 * this software in a product, an acknowledgment in the product
 * documentation would be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and
 * must not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 *
 * for WiiXplorer 2010
 ***************************************************************************/
#include <malloc.h>
#include <unistd.h>
#include <string.h>
#include <ogc/mutex.h>
#include <ogc/system.h>
#include <sdcard/wiisd_io.h>

#include "DeviceHandler.hpp"
#include "../Settings/CSettings.h"

DeviceHandler * DeviceHandler::instance = NULL;

DeviceHandler::~DeviceHandler()
{
	UnMountAll();
}

DeviceHandler * DeviceHandler::Instance()
{
	if (instance == NULL)
	{
		instance = new DeviceHandler();
	}
	return instance;
}

void DeviceHandler::DestroyInstance()
{
	if(instance)
	{
		delete instance;
	}
	instance = NULL;
}

bool DeviceHandler::MountAll()
{
	bool result = false;

	for(u32 i = SD; i <= USB8; i++)
	{
		if(Mount(i))
			result = true;
	}

	return result;
}

void DeviceHandler::UnMountAll()
{
	for(u32 i = SD; i <= USB8; i++)
		UnMount(i);

	if(sd)
		delete sd;
	if(usb0)
		delete usb0;
	if(usb1)
		delete usb1;

	sd = NULL;
	usb0 = NULL;
	usb1 = NULL;
}

bool DeviceHandler::Mount(int dev)
{
	if(dev == SD)
		return MountSD();

	else if(dev >= USB1 && dev <= USB8)
		return MountUSB(dev-USB1);

	return false;
}

bool DeviceHandler::IsInserted(int dev)
{
	if(dev == SD)
		return SD_Inserted() && sd->IsMounted(0);

	else if(dev >= USB1 && dev <= USB8)
		return GetUSBFromDev(dev) && GetUSBFromDev(dev)->IsMounted(PartToPortPart(dev-USB1));

	return false;
}

int DeviceHandler::GetDeviceCount()
{
	int count = 0;
	
	if(sd) count++;
	if(usb0) count += usb0->GetPartitionCount();
	if(usb1) count += usb1->GetPartitionCount();
	
	return count;
}

void DeviceHandler::UnMount(int dev)
{
	if(dev == SD)
		UnMountSD();

	else if(dev >= USB1 && dev <= USB8)
		UnMountUSB(dev-USB1);
}

bool DeviceHandler::MountSD()
{
	if(!sd)
		sd = new PartitionHandle(&__io_wiisd);

	if(sd->GetPartitionCount() < 1)
	{
		delete sd;
		sd = NULL;
		return false;
	}

	//! Mount only one SD Partition
	return sd->Mount(0, DeviceName[SD]);
}

bool DeviceHandler::MountUSB(int pos)
{
	if(!usb0)
		usb0 = new PartitionHandle((IOS_GetVersion() > 200) ? &__io_usbstorage2_port0 : &__io_usbstorage);
	if(!usb1 && Settings.USBPort == 1 && IOS_GetVersion() > 200)
		usb1 = new PartitionHandle(&__io_usbstorage2_port1);

	int partCount = 0;
	if(usb0)
		partCount += usb0->GetPartitionCount();
	if(usb1)
		partCount += usb1->GetPartitionCount();

	if(pos >= partCount)
		return false;

	return GetUSBFromDev(USB1+pos)->Mount(PartToPortPart(pos), DeviceName[USB1+pos]);
}

bool DeviceHandler::MountAllUSB()
{
	if(!usb0)
		usb0 = new PartitionHandle((IOS_GetVersion() > 200) ? &__io_usbstorage2_port0 : &__io_usbstorage);
	if(!usb1 && Settings.USBPort == 1 && IOS_GetVersion() > 200)
		usb1 = new PartitionHandle(&__io_usbstorage2_port1);
	
	bool result = false;
	int partCount = 0;
	if(usb0)
		partCount += usb0->GetPartitionCount();
	if(usb1)
		partCount += usb1->GetPartitionCount();
	
	for(int i = 0; i < partCount; i++)
	{
		if(MountUSB(i))
			result = true;
	}

	return result;
}

void DeviceHandler::UnMountUSB(int pos)
{
	if(GetUSBFromDev(USB1+pos))
		GetUSBFromDev(USB1+pos)->UnMount(PartToPortPart(pos));
}

void DeviceHandler::UnMountAllUSB()
{
	int partCount = 0;
	if(usb0)
		partCount += usb0->GetPartitionCount();
	if(usb1)
		partCount += usb1->GetPartitionCount();

	for(int i = 0; i < partCount; i++)
		UnMountUSB(i);

	delete usb0;
	delete usb1;
	usb0 = NULL;
	usb1 = NULL;
}

int DeviceHandler::PathToDriveType(const char * path)
{
	if(!path)
		return -1;

	for(int i = SD; i <= USB8; i++)
	{
		if(strncmp(path, DeviceName[i], strlen(DeviceName[i])) == 0)
			return i;
	}

	return -1;
}

const char * DeviceHandler::GetFSName(int dev)
{
	if(dev == SD && DeviceHandler::instance->sd)
	{
		return DeviceHandler::instance->sd->GetFSName(0);
	}
	else if(dev >= USB1 && dev <= USB8 && instance->GetUSBFromDev(dev))
	{
		return instance->GetUSBFromDev(dev)->GetFSName(instance->PartToPortPart(dev-USB1));
	}

	return NULL;
}

PartitionHandle * DeviceHandler::GetUSBFromDev(int dev)
{
	int usbPart = dev-USB1;

	if(!usb0 || usbPart >= usb0->GetPartitionCount())
		return usb1;
	else
		return usb0;
}

int DeviceHandler::PartToPortPart(int part)
{
	if(!usb0 || part >= usb0->GetPartitionCount())
		return part-usb0->GetPartitionCount();
	else
		return part;
}

int DeviceHandler::PartToPort(int part)
{
	if(!usb0 || part >= usb0->GetPartitionCount())
		return 1;
	else
		return 0;
}