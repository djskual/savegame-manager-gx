/****************************************************************************
 * Copyright (C) 2011
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
 * HomebrewReceiver.cpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#include <ogcsys.h>
#include <unistd.h>
#include <zip/unzip.h>

#include "HomebrewReceiver.hpp"
#include "BootHomebrew.h"
#include "../Language/gettext.h"
#include "../Network/networkops.h"
#include "../Prompts/PromptWindows.h"
#include "../Prompts/ProgressWindow.h"

extern u32 infilesize;
extern u32 uncfilesize;
extern char wiiloadVersion[2];
extern int connection;

HomebrewReceiver::HomebrewReceiver()
{
	loaded = false;
}

HomebrewReceiver::~HomebrewReceiver()
{
	
}

void HomebrewReceiver::CheckIncoming()
{
	if(!IsNetworkInit())
		return;
	
	if (GetCheckIncomming() && !loaded)
	{
		ResumeNetworkWait();
		loaded = true;
	}
	
	if (infilesize > 0 && loaded)
	{
		if (ReceiveFile() != 0)
			return;
		CloseConnection();
		ResumeNetworkWait();
	}
}

int HomebrewReceiver::ReceiveFile()
{
	char filesizetxt[50];
	char temp[50];
	u32 filesize = 0;

	if (infilesize < MB_SIZE)
		snprintf(filesizetxt, sizeof(filesizetxt), tr( "Incoming file %0.2fKB" ), infilesize / KB_SIZE);
	else snprintf(filesizetxt, sizeof(filesizetxt), tr( "Incoming file %0.2fMB" ), infilesize / MB_SIZE);

	snprintf(temp, sizeof(temp), tr( "Load file from: %s ?" ), GetIncommingIP());

	int choice = WindowPrompt(filesizetxt, temp, tr( "Load" ), tr( "Cancel" ));

	if (choice == 0)
		return 0;

	u32 read = 0;
	int len = NET_BLOCKSIZE;
	filesize = infilesize;
	u8 * buffer = (u8 *) malloc(infilesize);
	if(!buffer)
	{
		CloseConnection();
		WindowPrompt(tr( "Not enough memory." ), 0, tr( "OK" ));
		return 0;
	}

	bool error = false;
	while (read < infilesize)
	{
		ShowProgress(tr( "Receiving file from:" ), GetIncommingIP(), NULL, read, infilesize, true);
		
		if (infilesize - read < (u32) len)
			len = infilesize - read;
		else len = NET_BLOCKSIZE;
		
		int result = network_read(connection, buffer+read, len);
		
		if (result < 0)
		{
			CloseConnection();
			ProgressStop();
			WindowPrompt(tr( "Error while transfering data." ), 0, tr( "OK" ));
			free(buffer);
			return 0;
		}
		if (!result)
		{
			break;
		}
		
		read += result;
	}

	char filename[101];
	memset(filename, 0, sizeof(filename));
	
	network_read(connection, (u8*) filename, 100);
	
	//! Uncompress Wiiload
	if (wiiloadVersion[0] > 0 || wiiloadVersion[1] > 4)
	{
		u8 *unc = (u8 *) malloc(uncfilesize);
		if(!unc)
		{
			free(buffer);
			CloseConnection();
			ProgressStop();
			WindowPrompt(tr( "Not enough memory." ), 0, tr( "OK" ));
			return 0;
		}
		
		uLongf f = uncfilesize;
		error = uncompress(unc, &f, buffer, infilesize) != Z_OK;
		uncfilesize = f;
		filesize = uncfilesize;
		
		free(buffer);
		buffer = unc;
		free(unc);
	}

	CopyHomebrewMemory(buffer, 0, filesize);
	
	ProgressStop();

	if (error || read != infilesize)
	{
		WindowPrompt(tr( "Error:" ), tr( "No data could be read." ), tr( "OK" ));
		FreeHomebrewBuffer();
		return 0;
	}

	CloseConnection();
	
	AddBootArgument(filename);

	return BootHomebrewFromMem();
}
