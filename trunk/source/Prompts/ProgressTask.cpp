/****************************************************************************
 * Copyright (C) 2012 Dj_Skual
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
#include "../Controls/Application.h"
#include "ProgressTask.h"

ProgressTask * ProgressTask::instance = NULL;

ProgressTask::ProgressTask()
	: Thread()
	, ExitRequested(false)
{
	startThread();
	resumeThread();
}

ProgressTask::~ProgressTask()
{
	ExitRequested = true;
}

void ProgressTask::executeThread(void)
{
	while(!ExitRequested)
	{
		Application::Instance()->updateEvents();
	}
}
