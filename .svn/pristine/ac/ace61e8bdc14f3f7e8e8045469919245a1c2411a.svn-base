 /***************************************************************************
 * Copyright (C) 2009
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
 * fileops.h
 * File operations for the WiiXplorer
 * Handling all the needed file operations
 ***************************************************************************/
#ifndef _FILEOPS_H_
#define _FILEOPS_H_

#include <string>
#include <gctypes.h>

bool CreateSubfolder(std::string fullpath);
bool FindFile(std::string filename, std::string path);
bool SearchFile(const char * searchpath, const char * searched_filename, char * outfilepath);
bool CheckFile(std::string filepath);
u64 FileSize(std::string filepath);
int LoadFileToMem(std::string filepath, u8 **buffer, u64 *size);
int LoadFileToMemWithProgress(const char *progressText, std::string filePath, u8 **buffer, u64 *size);
int CopyFile(const char * src, const char * dest);
int CopyDirectory(const char * src, const char * dest);
int MoveDirectory(const char * src, const char * dest);
int MoveFile(const char *srcpath, const char *destdir);
int RemoveDirectory(std::string dirpath);
bool RenameFile(std::string srcpath, std::string destpath);
bool RemoveFile(std::string filepath);
void GetFolderSize(std::string folderpath, u64 * foldersize, u32 * filenumber);
bool CompareDevices(const char *src, const char *dest);
bool VerifyFileType(const char * filepath, const char * type);
std::string GetParentDir(std::string path);

#endif
