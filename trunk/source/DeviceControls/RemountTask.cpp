/****************************************************************************
 * Copyright (C) 2011 Dimok
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/
#include "RemountTask.h"
#include "DeviceHandler.hpp"
#include "../Controls/Application.h"
//#include "../Prompts/ProgressWindow.h"

RemountTask::RemountTask(const char *title, int Device)
	: Task(title), RemountDevice(Device)
{
}

RemountTask::~RemountTask()
{
}

void RemountTask::Execute(void)
{
	TaskBegin(this);

	//ProgressWindow::Instance()->StartProgress(this->getTitle().c_str(), tr("Please wait..."));
	//ProgressWindow::Instance()->SetUnit(tr("devices"));

	if(RemountDevice == MAXDEVICES)
	{
		DeviceHandler::Instance()->UnMountAll();

		for(int dev = SD; dev < MAXDEVICES; ++dev)
		{
			//if(ProgressWindow::Instance()->IsCanceled())
				//break;

			//ProgressWindow::Instance()->ShowProgress(dev, MAXDEVICES);
			DeviceHandler::Instance()->Mount(dev);
		}
	}
	else
	{
		DeviceHandler::Instance()->UnMount(RemountDevice);
		DeviceHandler::Instance()->Mount(RemountDevice);
		//ProgressWindow::Instance()->ShowProgress(1, 1);
	}

	//ProgressWindow::Instance()->StopProgress();

	TaskEnd(this);
}
