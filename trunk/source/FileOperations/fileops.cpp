 /***************************************************************************
 * Copyright (C) 2009
 * by Dimok
 * modified by Dj Skual
 * for SavegameManager GX 2011
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
 * fileops.cpp
 * File operations for the WiiXplorer
 * Handling all the needed file operations
 ***************************************************************************/
#include <vector>

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <malloc.h>

#include "fileops.h"
#include "DirList.h"

#define BLOCKSIZE			 70*1024      //!70KB
#define VectorResize(List)	if(List.capacity()-List.size() == 0) List.reserve(List.size()+100)


static bool actioncanceled  = false;

/****************************************************************************
 * FindFile
 *
 * Check if file is available in the given directory
 ***************************************************************************/
bool FindFile(std::string filename, std::string path)
{
	if(filename.empty() || path.empty())
		return false;

	DirList * dirList = new DirList(path, NULL, DirList::Files);
	
	for(int i = 0; i < dirList->GetFilecount(); i++)
	{
		if(!filename.compare(dirList->GetFilename(i)))
		{
			delete dirList;
			return true;
		}
	}
	delete dirList;
	
	return false;
}

/****************************************************************************
 * SearchFile
 *
 * Search for a file recursive through all subdirectories
 ***************************************************************************/
bool SearchFile(const char * searchpath, const char * searched_filename, char * outfilepath)
{
	if(!searchpath || !searched_filename)
		return false;
	
	DirList * dirList = new DirList(searchpath, NULL, DirList::Files | DirList::Dirs);
	
	for (int i = 0;  i < dirList->GetFilecount(); i++)
	{
		if(dirList->IsDir(i))
		{
			if(SearchFile(dirList->GetFilepath(i), searched_filename, outfilepath))
			{
				delete dirList;
				return true;
			}
		}
		else
		{
			if(strcasecmp(dirList->GetFilename(i), searched_filename) == 0)
			{
				strcpy(outfilepath, dirList->GetFilepath(i));
				
				delete dirList;
				return true;
			}
		}
	}
	delete dirList;
	
	return false;
}

/****************************************************************************
 * CheckFile
 *
 * Check if file is existing
 ***************************************************************************/
bool CheckFile(std::string filepath)
{
	if(filepath.empty())
		return false;
	
	struct stat filestat;
	
	while(filepath[filepath.size()-1] == '/')
		filepath.erase(filepath.size()-1, 1);
	
	bool isRoot = (filepath.find('/') == std::string::npos);
	if(isRoot)
		filepath += '/';
	
	if (stat(filepath.c_str(), &filestat) == 0)
		return true;
	
	return false;
}

/****************************************************************************
 * FileSize
 *
 * Get filesize in bytes. u64 for files bigger than 4GB
 ***************************************************************************/
u64 FileSize(std::string filepath)
{
  struct stat filestat;

  if (filepath.empty() || stat(filepath.c_str(), &filestat) != 0)
	return 0;

  return filestat.st_size;
}

/****************************************************************************
 * LoadFileToMem
 *
 * Load up the file into a block of memory
 ***************************************************************************/
int LoadFileToMem(std::string filepath, u8 **inbuffer, u64 *size)
{
	if(filepath.empty())
		return -1;

	while(filepath[filepath.size()-1] == '/')
		filepath.erase(filepath.size()-1, 1);
	
	int ret = -1;
	u64 filesize = FileSize(filepath);
	
	*inbuffer = NULL;
	*size = 0;

	FILE *file = fopen(filepath.c_str(), "rb");

	if (file == NULL)
		return -1;

	u8 *buffer = (u8 *) malloc(filesize);
	if (buffer == NULL)
	{
		fclose(file);
		return -2;
	}

	u64 done = 0;
	u32 blocksize = BLOCKSIZE;

	do
	{
		if(actioncanceled)
		{
			free(buffer);
			fclose(file);
			return -10;
		}

		if(blocksize > filesize-done)
			blocksize = filesize-done;

		ret = fread(buffer+done, 1, blocksize, file);
		if(ret < 0)
		{
			free(buffer);
			fclose(file);
			return -3;
		}
		else if(ret == 0)
		{
			//!we are done
			break;
		}

		done += ret;

	}
	while(done < filesize);

	fclose(file);

	if (done != filesize)
	{
		free(buffer);
		return -3;
	}

	*inbuffer = buffer;
	*size = filesize;

	return 1;
}

/****************************************************************************
 * LoadFileToMemWithProgress
 *
 * Load up the file into a block of memory, while showing a progress dialog
 ***************************************************************************/
int LoadFileToMemWithProgress(const char *progressText, std::string filepath, u8 **inbuffer, u64 *size)
{
	return LoadFileToMem(filepath, inbuffer, size);
}

/****************************************************************************
 * CreateSubfolder
 *
 * Create recursive all subfolders to the given path
 ***************************************************************************/
bool CreateSubfolder(std::string fullpath)
{
	if(fullpath.empty())
		return false;
	
	if(CheckFile(fullpath))
		return true;
	
	int length = fullpath.size()-1;
	while(fullpath[length] == '/')
	{
		fullpath.erase(length);
		--length;
	}
	
	std::string subpath(fullpath);
	size_t pos = subpath.rfind('/');
	if(pos == std::string::npos)
		return false;
	
	if(subpath.size()-1 > pos)
		subpath.erase(pos+1);
	
	bool result = CreateSubfolder(subpath);
	if(!result)
		return false;
	
	return (mkdir(fullpath.c_str(), 0777) != -1);
}

/****************************************************************************
 * CopyFile
 *
 * Copy the file from source filepath to destination filepath
 ***************************************************************************/
int CopyFile(const char * src, const char * dest)
{
	int read = 1;
	int wrote = 1;
	
	char * filename = strrchr(src, '/');
	if(filename)
		filename++;
	else
		return -1;
	
	u64 sizesrc = FileSize(src);
	
	FILE * source = fopen(src, "rb");
	
	if(!source)
		return -2;
	
	u32 blksize = BLOCKSIZE;
	
	u8 * buffer = (u8 *) malloc(blksize);
	if(buffer == NULL)
	{
		//!no memory
		fclose(source);
		return -1;
	}
	
	FILE * destination = fopen(dest, "wb");
	if(destination == NULL)
	{
		free(buffer);
		fclose(source);
		return -2;
	}
	
	u64 done = 0;
	
	do
	{
		if(actioncanceled)
		{
			fclose(source);
			fclose(destination);
			free(buffer);
			RemoveFile(dest);
			return -10;
		}
		
		if(blksize > sizesrc - done)
			blksize = sizesrc - done;
		
		read = fread(buffer, 1, blksize, source);
		if(read < 0)
		{
			fclose(source);
			fclose(destination);
			free(buffer);
			RemoveFile(dest);
			return -3;
		}
		
		wrote = fwrite(buffer, 1, read, destination);
		if(wrote < 0)
		{
			fclose(source);
			fclose(destination);
			free(buffer);
			RemoveFile(dest);
			return -4;
		}
		
		done += wrote;
	}
	while (read > 0);
	
	free(buffer);
	fclose(source);
	fclose(destination);
	
	if(sizesrc != done)
		return -5;
	
	return 1;
}

/****************************************************************************
 * CopyDirectory
 *
 * Copy recursive a complete source path to destination path
 ***************************************************************************/
int CopyDirectory(const char * src, const char * dest)
{
	if(!src || !dest)
		return -1;

	DirList * dirList = new DirList(src, NULL, DirList::Files | DirList::Dirs);
	
	std::string Src(src);
	std::string Dest(dest);
	
	while(Src[Src.size()-1] == '/')
		Src.erase(Src.size()-1);
	while(Dest[Dest.size()-1] == '/')
		Dest.erase(Dest.size()-1);
	
	std::string srcName(Src);
	size_t pos = srcName.rfind('/');
	if(srcName.size()-1 > pos)
		srcName.erase(0, pos+1);
	
	Dest += "/";
	Dest += srcName;
	
	CreateSubfolder(Dest.c_str());
	
	for (int i = 0;  i < dirList->GetFilecount(); i++)
	{
		if(actioncanceled)
		{
			delete dirList;
			return -10;
		}
		
		if(dirList->IsDir(i))
			CopyDirectory(dirList->GetFilepath(i), Dest.c_str());
		else
		{
			std::string destName(dirList->GetFilepath(i));
			destName.erase(0, Src.size());
			destName.insert(0, Dest);
			
			CopyFile(dirList->GetFilepath(i), destName.c_str());
		}
	}
	delete dirList;
	
	return 1;
}

/****************************************************************************
 * MoveDirectory
 *
 * Move recursive a complete source path to destination path
 ***************************************************************************/
int MoveDirectory(const char * src, const char * dest)
{
	if(!src || !dest)
		return -1;

	DirList * dirList = new DirList(src, NULL, DirList::Files | DirList::Dirs);
	
	std::string Src(src);
	std::string Dest(dest);
	
	while(Src[Src.size()-1] == '/')
		Src.erase(Src.size()-1);
	while(Dest[Dest.size()-1] == '/')
		Dest.erase(Dest.size()-1);
	
	std::string srcName(Src);
	size_t pos = srcName.rfind('/');
	if(srcName.size()-1 > pos)
		srcName.erase(0, pos+1);
	
	Dest += "/";
	Dest += srcName;
	
	CreateSubfolder(Dest.c_str());
	for (int i = 0;  i < dirList->GetFilecount(); i++)
	{
		if(actioncanceled)
		{
			delete dirList;
			return -10;
		}
		
		if(dirList->IsDir(i))
		{
			std::string tmpSrc(Src);
			tmpSrc += '/';
			tmpSrc += dirList->GetFilename(i);
			
			MoveDirectory(tmpSrc.c_str(), Dest.c_str());
		}
		else
		{
			std::string destName(dirList->GetFilepath(i));
			destName.erase(0, Src.size());
			destName.insert(0, Dest);
			
			MoveFile(dirList->GetFilepath(i), destName.c_str());
		}
	}
	delete dirList;
	
	if(!remove(Src.c_str()))
		return 1;
	
	return -1;
}

/****************************************************************************
 * MoveFile
 *
 * Move a file from srcpath to destdir
 ***************************************************************************/
int MoveFile(const char *srcpath, const char *destdir)
{
	if(CompareDevices(srcpath, destdir))
	{
		if(RenameFile(srcpath, destdir))
			return 1;
		else
			return -1;
	}

	if(CopyFile(srcpath, destdir) < 0)
		return -1;

	if(RemoveFile(srcpath))
		return 1;

	return -1;
}

/****************************************************************************
 * RemoveDirectory
 *
 * Remove a directory and its content recursively
 ***************************************************************************/
int RemoveDirectory(std::string dirpath)
{
	if(dirpath.empty())
		return -1;

	DirList * dirList = new DirList(dirpath, NULL, DirList::Files | DirList::Dirs);
	
	while(dirpath[dirpath.size()-1] == '/')
		dirpath.erase(dirpath.size()-1);
	
	for (int i = 0;  i < dirList->GetFilecount(); i++)
	{
		if(actioncanceled)
		{
			delete dirList;
			return -10;
		}
		
		if(dirList->IsDir(i))
			RemoveDirectory(dirList->GetFilepath(i));
		else
			RemoveFile(dirList->GetFilepath(i));
	}
	delete dirList;
	
	if(!remove(dirpath.c_str()))
		return 1;
	
	return -1;
}

/****************************************************************************
 * RemoveFile
 *
 * Delete the file from a given filepath
 ***************************************************************************/
bool RemoveFile(std::string filepath)
{
	return (remove(filepath.c_str()) == 0);
}

/****************************************************************************
 * RenameFile
 *
 * Rename the file from a given srcpath to a given destpath
 ***************************************************************************/
bool RenameFile(std::string srcpath, std::string destpath)
{
	return (rename(srcpath.c_str(), destpath.c_str()) == 0);
}

/****************************************************************************
 * GetFolderSize
 *
 * Get recursivly complete foldersize
 * The path of directory to get size need to be entered with "/" at the end.
 ***************************************************************************/
void GetFolderSize(std::string folderpath, u64 * foldersize, u32 * filecount)
{
	if(folderpath.empty())
		return;

	DirList * dirList = new DirList(folderpath, NULL, DirList::Files | DirList::Dirs | DirList::CheckSubfolders);
	
	struct stat st;
	
	for(int i = 0; i < dirList->GetFilecount(); ++i)
	{
		if(!dirList->IsDir(i))
		{
			stat(dirList->GetFilepath(i), &st);
			
			if(filecount) *filecount += 1;
			if(foldersize) *foldersize += st.st_size;
		}
	}
	delete dirList;
}

/****************************************************************************
 * CompareDevices
 *
 * Compare if its the devices are equal
 ***************************************************************************/
bool CompareDevices(const char *src, const char *dest)
{
	if(!src || !dest)
		return false;

	char *device1 = strchr(src, ':');
	char *device2 = strchr(dest, ':');

	if(!device1 || !device2)
		return false;

	int position1 = device1-src+1;
	int position2 = device2-dest+1;

	char temp1[50];
	char temp2[50];

	snprintf(temp1, position1, "%s", src);
	snprintf(temp2, position2, "%s", dest);

	if(strcasecmp(temp1, temp2) == 0)
		return true;

	return false;
}

/****************************************************************************
 * VerifyFileType
 *
 * Compare the file type of the path with a type
 ***************************************************************************/
bool VerifyFileType(const char * filepath, const char * type)
{
	if(!filepath || !type )
		return false;
	
	std::string fileext = filepath;
	int index = fileext.find_last_of('.');
	if(index > 0)
	{
		fileext = fileext.substr(index);
		if(!fileext.compare(type))
			return true;
	}
	
	return false;
}

/****************************************************************************
 * GetParentDir
 *
 * Return the parent path of a file or directory
 ***************************************************************************/
std::string GetParentDir(std::string path)
{
	if(path.empty())
		return "";

	while(path[path.size()-1] == '/')
		path.erase(path.size()-1);
	
	return path.erase(path.find_last_of('/'));
}
