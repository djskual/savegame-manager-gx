/****************************************************************************
 * fileops.h
 * by dimok
 *
 * File operations for the WiiXplorer
 * Handling all the needed file operations
 ***************************************************************************/

#ifndef _FILEOPS_H_
#define _FILEOPS_H_

#include <gctypes.h>

#include "file/foldersize.h"

bool CreateSubfolder(const char * fullpath);
bool FindFile(const char * filename, const char * path);
bool CheckFile(const char * filepath);
int FileSize(const char * filepath);
int LoadFileToMem(const char * filepath, u8 **buffer, u64 *size);
int LoadFileToMemWithProgress(const char *progressText, const char *filePath, u8 **buffer, u64 *size);
int CopyFile(const char * src, const char * dest);
int CopyDirectory(const char * src, const char * dest);
int MoveDirectory(char * src, const char * dest);
int MoveFile(const char *srcpath, char *destdir);
int RemoveDirectory(char * dirpath);
bool RemoveFile(const char * filepath);
void GetFolderSize(const char * folderpath, u64 &foldersize, u32 &filenumber);
bool CompareDevices(const char *src, const char *dest);
bool VerifyFileType(char * filepath, const char * type);
void TrimFileTypePath(char *dest, char *src, int size);

#endif
