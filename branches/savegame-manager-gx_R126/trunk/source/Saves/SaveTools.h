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
 * SaveTools.h
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#ifndef _SAVETOOLS_H_
#define _SAVETOOLS_H_

#include <gccore.h>
#include <string.h>
#include <string>

#define BLOCK_SIZE			1024
#define TITLE_UPPER(x)		((u32)((x) >> 32))
#define TITLE_LOWER(x)		((u32)(x))

//! Check if the folder is a save folder
bool CheckSave(std::string path);
//! Check if the save is a installed on nand
bool CheckIfInstalled(u64 tid);
//! Check if the title is a save folder on nand
bool CheckTitle(std::string path);
//! Get The Save NAND Path based on titleid
bool GetNandPath(u64, char *);
//! Get The Game title ID based on folder name
u64 StrToHex64(const char *);
//! Get The GameID[4] based on titleid
char *titleText(u32 kind, u32 title);

//! Get The Save Name & Subname on NAND based on titleid
std::string GetNandName(u64 tid);
std::string GetNandSubname(u64 tid);
//! Get The Save Name & Subname on Storage Device based on path
std::string GetDeviceName(std::string savepath);
std::string GetDeviceSubname(std::string savepath);

//! Old invalid fat character fix
void RestoreSaveFile(std::string Filepath);

#endif
