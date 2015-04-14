/****************************************************************************
 * Copyright (C) 2012
 * by Dj_Skual
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
 * SdPort.cpp
 *
 * for SaveGame Manager GX 2012
 ***************************************************************************/
#include <unistd.h>
#include <sdcard/wiisd_io.h>

#include "SdPort.hpp"
#include "../Language/gettext.h"
#include "../DeviceControls/DeviceHandler.hpp"
#include "../Prompts/ProgressWindow.h"
#include "../Settings/CSettings.h"

SdPort::SdPort()
{
	state = __io_wiisd.isInserted();
	new_state = state;
	is_config_device = false;
	IsConfigDevice();
}

SdPort::~SdPort()
{
	
}

void SdPort::IsConfigDevice()
{
	std::string config_path = Settings.ConfigPath;
	config_path.erase(config_path.find_first_of(":"));
	
	if( !config_path.compare("sd") )
		is_config_device = true;
}

bool SdPort::StateChanged()
{
	if(!DeviceHandler::Instance()->IsInserted(SD))
	{
		if(__io_wiisd.startup())
		{
			new_state = __io_wiisd.isInserted();
			if(!is_config_device) __io_wiisd.shutdown();
		}
	}
	else
		new_state = __io_wiisd.isInserted();
	
	if(state != new_state)
	{
		if(new_state == NOT_INSERTED && is_config_device)
			return WaitInsert();
		
		state = new_state;
		
		DeviceHandler::DestroyInstance();
		
		DeviceHandler::Instance()->MountSD();
		DeviceHandler::Instance()->MountAllUSB();
		
		return true;
	}
	
	return false;
}

bool SdPort::WaitInsert()
{
	ShowProgress(tr("SD Card removed."), NULL, NULL, 0, 1, false, false, THROBBER);
	
	while( __io_wiisd.isInserted() == NOT_INSERTED) usleep(50);
	
	__io_wiisd.shutdown();
	DeviceHandler::DestroyInstance();
	
	DeviceHandler::Instance()->MountSD();
	DeviceHandler::Instance()->MountAllUSB();
	
	ProgressStop();
	
	return true;
}