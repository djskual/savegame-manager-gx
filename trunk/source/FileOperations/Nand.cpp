#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <algorithm>

#include "Nand.hpp"
#include "../FileOperations/fileops.h"
#include "../Memory/MemTools.h"

bool Nand::Initialized = false;

int Nand::Startup() 
{
	if (Initialized)
		return 0;
	
    int ret = ISFS_Initialize();
	Initialized = (ret == ISFS_OK);
	
	return ret;
}

void Nand::Shutdown()
{
	if (!Initialized)
		return;
	
	ISFS_Deinitialize();
	Initialized = false;
}

int Nand::CreateDir(std::string path)
{
	int ret = ISFS_CreateDir(path.c_str(), 0, ISFS_OPEN_RW, ISFS_OPEN_RW, ISFS_OPEN_RW);
	if (ret <= -1)
	{
		while(path.at(path.size()-1) == '/')
			path.erase(path.size()-1);
		
		std::string parent = path;
		size_t pos = parent.find_last_of("/");
		if(pos != std::string::npos)
		{
			parent.erase(pos);
			ret = Nand::CreateDir(parent);
			if (ret <= -1)
				return ret;
			else
				ret = ISFS_CreateDir(path.c_str(), 0, ISFS_OPEN_RW, ISFS_OPEN_RW, ISFS_OPEN_RW);
		}
	}
	
	if (ret <= -1)
		return ret;
	
	ret = ISFS_SetAttr(path.c_str(), 0, 0, 0, ISFS_OPEN_RW, ISFS_OPEN_RW, ISFS_OPEN_RW);
	
	return ret;
}

int Nand::CreateFile(std::string filepath, u8 attributes, u8 ownerPerm, u8 groupPerm, u8 otherPerm, u32 uid, u16 gid)
{
	int ret = ISFS_CreateFile(filepath.c_str(), attributes, ownerPerm, groupPerm, otherPerm);
	
	if (ret <= -1)
	   return ret;
	
	ret = ISFS_SetAttr(filepath.c_str(), uid, gid, attributes, ownerPerm, groupPerm, otherPerm);
	
	return ret;
}

int Nand::Open(std::string filepath, u8 mode)
{
	return ISFS_Open(filepath.c_str(), mode);
}

int Nand::OpenRead(std::string filepath)
{
	return Nand::Open(filepath, ISFS_OPEN_READ);	
}

int Nand::OpenReadWrite(std::string filepath)
{
	return Nand::Open(filepath, ISFS_OPEN_RW);
}

u32 Nand::GetFileSize(int fp)
{
	if (fp < 1)
		return 0;
	
	fstats *stats = (fstats *)MemTools::AllocateMemory_64(sizeof(fstats));
	int ret = ISFS_GetFileStats(fp, stats);
	if (ret >= 0)
		ret = stats->file_length;
	
	free(stats);
	
	return ((ret < 0) ? 0 : ret);
}

u32 Nand::GetFileSize(std::string filepath)
{
	if (filepath.empty())
		return 0;
	
	u32 filesize = 0;
	
	int file = Nand::OpenRead(filepath);
	if (file >= 0)
	{
		filesize = Nand::GetFileSize(file);
		Nand::Close(file);
	}
	
	return filesize;
}

int Nand::GetDirSize(std::string path)
{
	int totalSize = 0;
	std::vector<std::string> list;
	
	int res = Nand::ReadDir(path, list);
	if(res != ISFS_OK)
		return -1;
	if(list.empty())
		return 0;
	
	for(u32 i = 0; i < list.size(); i++)
	{
		std::string filepath = path;
		filepath += "/";
		filepath += list.at(i);
		
		res = Nand::GetFileSize(filepath);
		if(res <= 0)
		{
			res = Nand::GetDirSize(filepath);
			if(res <= 0)
			{
				totalSize = -1;
				break;
			}
		}
		
		if(res>0)
			totalSize += res;
	}
	
	return totalSize;
}

int Nand::Close(int fp) 
{
	if (fp < 1)
		return 0;
	
	return ISFS_Close(fp);
}

bool Nand::CheckFile(std::string filepath)
{
	bool ret = false;
	
	if(filepath.empty())
		return ret;
	
	int file = Nand::OpenRead(filepath);
	if (file >= 0)
	{
		ret = true;
		Nand::Close(file);
	}
	
	return ret;
}

int Nand::Delete(std::string filepath)
{
	return ISFS_Delete(filepath.c_str());
}

int Nand::Write(int file, u8 *buffer, u32 length) 
{
	return ISFS_Write(file, buffer, length);
}

int Nand::Write(std::string filepath, u8 *buffer, u32 length)
{
	//! file need to exist before write
	if (!buffer || !length)
		return -1;
	
	int file = Nand::OpenReadWrite(filepath);
	if (file < 0)
		return file;

	int ret = Nand::Write(file, buffer, length);
	
	Nand::Close(file);
	return ret;
}

int Nand::Read(int fp, u8 *buffer, u32 length) 
{
	return ISFS_Read(fp, buffer, length);
}

int Nand::Seek(int fp, int where, int whence)
{
	return ISFS_Seek(fp, where, whence);
}

int Nand::ReadDir(std::string path, std::vector<std::string> &list)
{
	u32 count = 0;
	
	int ret = ISFS_ReadDir(path.c_str(), NULL, &count);
	if (ret < ISFS_OK)
		return ret;

	if (count == 0)
		return ISFS_OK;
	
	char *dirlist = (char*)MemTools::AllocateMemory_64(ISFS_MAXPATH * count);
	if (!dirlist)
		return ISFS_ENOMEM;

	ret = ISFS_ReadDir(path.c_str(), dirlist, &count);
	if (ret == ISFS_OK)
	{
		u32 index = 0;
		
		for (u32 i = 0; i < count; i++)
		{
			const char *name = dirlist + index;
			list.push_back(name);
			index += strlen(name) + 1;
		}
		sort(list.begin(), list.end());
	}

	delete dirlist;
	dirlist = NULL;
	
	return ret;
}

int Nand::LoadFile(std::string filepath, u8 **outbuffer, u32 *outfilesize)
{
	if(filepath.empty())
		return -1;
	
	int ret = -1;
	u32 filesize = 0;
	u8* buffer = NULL;
	
	int file = Nand::OpenRead(filepath);
	if (file >= 0)
	{
		filesize = Nand::GetFileSize(file);
		if (filesize)
		{
			buffer = (u8 *)MemTools::AllocateMemory_32(filesize);
			if (buffer)
			{
				ret = Nand::Read(file, buffer, filesize);
				if (!buffer)
					ret = -1;
			}
		}
		Nand::Close(file);
	}
	
	if (ret < 0)
	{
		if(buffer) free(buffer);
		return ret;
	}

	*outbuffer = buffer;
	*outfilesize = filesize;
	
	return ret;
}

typedef struct _ReplaceStruct
{
	const char * replace;
	char orig;
	short size;
} ReplaceStruct;

//! More replacements can be added if needed
static const ReplaceStruct Replacements[] =
{
	{ "&gt;", '>', 4 },
	{ "&lt;", '<', 4 },
	{ "&st;", '*', 4 },
	{ "&cl;", ':', 4 },
	{ "&qt;", '\"', 6 },
	{ "&qm;", '?', 6 },
	{ "&vb;", '|', 5 },
	{ NULL, '\0', 0 }
};

static void ConvertInvalidCharacters(std::string &filename)
{
	size_t startPos = 0;
	size_t pos;

	for(int i = 0; Replacements[i].replace != 0; ++i)
	{
		while((pos = filename.find(Replacements[i].orig, startPos)) != std::string::npos)
		{
			filename.erase(pos, 1);
			filename.insert(pos, Replacements[i].replace);
		}
	}
}

bool Nand::ExtractFile(std::string nandPath, std::string filepath)
{
	bool ret = false;
	
	if(nandPath.empty() || filepath.empty())
		return ret;
	
	u8* buffer = NULL;
	u32 size = 0;
	
	Nand::LoadFile(nandPath, &buffer, &size);
	if(buffer)
	{
		std::string destDir = filepath;
		size_t pos = destDir.find_last_of("/");
		if(pos != std::string::npos)
		{
			destDir.erase(pos+1);
			if(CreateSubfolder(destDir))
			{
				FILE *pFile = fopen(filepath.c_str(), "wb");
				if(pFile)
				{
					if(fwrite(buffer, 1, size, pFile) == size)
						ret = true;
					
					fclose(pFile);
				}
			}
		}
		free(buffer);
	}
	
	return ret;
}

bool Nand::ExtractDir(std::string wiiPath, std::string devicePath)
{
	int ret = false;
	
	if(wiiPath.empty() || devicePath.empty())
		return ret;
	
	while(wiiPath.at(wiiPath.size()-1) == '/')
		wiiPath.erase(wiiPath.size()-1);
	while(devicePath.at(devicePath.size()-1) == '/')
		devicePath.erase(devicePath.size()-1);
	
	std::vector<std::string> List;
	if( Nand::ReadDir(wiiPath, List) == ISFS_OK)
	{
		ret = true;
		for (int i = 0; (u32)i < List.size(); i++)
		{
			std::string nandPath = wiiPath;
			nandPath += "/";
			nandPath += List.at(i);
			
			ConvertInvalidCharacters(List.at(i));
			std::string destPath = devicePath;
			destPath += "/";
			destPath += List.at(i);
			
			if(Nand::CheckFile(nandPath))
			{
				if(!Nand::ExtractFile(nandPath, destPath))
					ret = false;
			}
			else
			{
				if(!Nand::ExtractDir(nandPath, destPath))
					ret = false;
			}
			
			if(ret == false)
				break;
		}
	}
	
	return ret;
}
