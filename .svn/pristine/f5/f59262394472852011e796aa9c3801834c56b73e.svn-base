#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ogcsys.h>
#include <sys/dir.h>
#include <sys/stat.h>

#include "tools/StringTools.h"
#include "file/fileops.h"
#include "saves/savegame.h"
#include "saves/utils.h"
#include "main.h"

using namespace std;

static bool firstuse = true;
extern bool progresscanceled;

static int fileCount = 0;
static string isfsPath[200];
static string devicePath[200];

void deleteAllNandSaveDatas(const char *path)
{
	s32 fd;
	u32 num = 0;
	
	u32 i=0, j=0, k=0;

	if(ISFS_ReadDir(path, NULL, &num) == ISFS_OK)
	{
		char *nbuf = (char *)allocate_memory((ISFS_MAXPATH + 1) * num);
		
		if(nbuf != NULL)
		{
			if(ISFS_ReadDir(path, nbuf, &num) == ISFS_OK)
			{
				char ebuf[ISFS_MAXPATH + 1];
				for(i = 0, k=0; i < num; i++)
				{
					for(j = 0; nbuf[k] != 0; j++, k++)
						ebuf[j] = nbuf[k];
					k++;
					ebuf[j] = 0;
					
					char pbuf[ISFS_MAXPATH + 1];
					snprintf(pbuf, sizeof(pbuf),"%s/%s",path,ebuf);
					
					fd = ISFS_Open(pbuf, ISFS_OPEN_READ);
					if(fd < 0)
						deleteAllNandSaveDatas(pbuf);
					else {
						ISFS_Close(fd);
						bool Needed = false;
						for(int cur = 0; cur < fileCount && Needed == false; cur++) {
							if(strcmp(pbuf, isfsPath[cur].c_str()) == 0)
								Needed = true;
						}
						if(Needed == false)
							ISFS_Delete(pbuf);
					}
				}
			}
			free(nbuf);
		}
	}
}

void createAllNandSavePaths(const char *path)
{
	fileCount = 0;
	string tmpdevicePath[200];
	
	char indexpath[MAXPATHLEN];
	sprintf(indexpath,  "%s/index.sgmgx", path);
	
	ifstream filestr;
	filestr.open(indexpath);
	if (filestr.fail())
        return;
	
	while (!filestr.eof())
	{
		getline(filestr,isfsPath[fileCount]);
		if(!filestr.fail())
		{
			char isfspath[MAXPATHLEN];
			char devpath[MAXPATHLEN];
			char tmpdevname[MAXPATHLEN];
			
			sprintf(isfspath, "%s", isfsPath[fileCount].c_str());
			
			tmpdevicePath[fileCount] = isfsPath[fileCount].substr(30);
			sprintf(tmpdevname, "%s", tmpdevicePath[fileCount].c_str());
			fix_filename(tmpdevname);
			sprintf(devpath, "%s/%s", path, tmpdevname);
			devicePath[fileCount] = devpath;
			
			fileCount++;
		}
	}
	filestr.close();
}

void createAllNandSaveDatas()
{
	char isfspath[MAXPATHLEN];
	for(int i = 0; i<fileCount; i++)
	{
		sprintf(isfspath, "%s", isfsPath[i].c_str());
		s32 fd = ISFS_Open(isfspath, ISFS_OPEN_READ);
		if(fd < 0)
			ISFS_CreateFile(isfspath, 0, 3, 1, 1);
		else
			ISFS_Close(fd);
	}
}

int Savegame_Install(const char *devpath, const char *nandpath)
{
	static u8 buffer[1024] ATTRIBUTE_ALIGN(0x20);
	int result = 0;
	
	createAllNandSavePaths(devpath);
	deleteAllNandSaveDatas(nandpath);
	createAllNandSaveDatas();
	
	for(int i = 0; (i < fileCount) && (result == 0); i++)
	{
		result = -1;
		
		FILE * infp = fopen(devicePath[i].c_str(),  "rb");
		if (infp)
		{
			s32 outfp = ISFS_Open(isfsPath[i].c_str(), ISFS_OPEN_WRITE);
			if (outfp >= 0)
			{
				for (;;)
				{
					int size = fread(buffer, 1, sizeof(buffer), infp);
					if (size < 0)
						break;
					
					if (!size) {
						result = 0;
						break;
					}
					
					if(ISFS_Write(outfp, buffer, size) >= 0)
						result = 0;
				}
				ISFS_Close(outfp);
			}
			fclose(infp);
		}
	}
	
	if(progresscanceled)
		result =  -10;
	
	return result;
}

bool WriteIndexFile(const char * indexPath, const char * indextext)
{
	FILE * indexfp = fopen(indexPath, "a");
	if (indexfp)
	{
		if(fprintf(indexfp, "%s", indextext) >= 0) {
			fclose(indexfp);
			return true;
		}
		fclose(indexfp);
	}
	return false;
}

int Savegame_Extract(const char *nandpath, const char *devpath, const char *indexpath)
{
	bool result = false;
	u32 num = 0;
	
	char savefolder[MAXPATHLEN];
	char indexPath[MAXPATHLEN];
	snprintf(savefolder, sizeof(savefolder), "%s/", devpath);
	snprintf(indexPath, sizeof(indexPath), "%s/index.sgmgx", indexpath);
	
	if(firstuse == true) {
		RemoveDirectory(savefolder);
		firstuse = false;
	}
	mkdir(devpath, 777);
	
	if(ISFS_ReadDir(nandpath, NULL, &num) == ISFS_OK)
	{
		char *nbuf = (char *)allocate_memory((ISFS_MAXPATH + 1) * num);
		
		if(nbuf != NULL)
		{
			if(ISFS_ReadDir(nandpath, nbuf, &num) == ISFS_OK)
			{
				static fstats stats ATTRIBUTE_ALIGN(32);
				char filename[ISFS_MAXPATH + 1];
				u32 i=0, j=0, k=0;
				
				for(i = 0, k=0; i < num; i++)
				{
					for(j = 0; nbuf[k] != 0; j++, k++)
						filename[j] = nbuf[k];
					k++;
					filename[j] = 0;
					
					/* Generate paths */
					char inpath[ISFS_MAXPATH+1];
					char outpath[MAXPATHLEN+1];
					char indextext[100];
					
					sprintf(inpath, "%s/%s", nandpath, filename);
					sprintf(indextext, "%s\n", inpath);
					fix_filename(filename);
					sprintf(outpath, "%s/%s", devpath, filename);
					
					s32 fd = ISFS_Open(inpath, ISFS_OPEN_READ);
					if (fd >= 0)
					{
						result = false;
						if (ISFS_GetFileStats(fd, &stats) >= 0)
						{
							u32* data = NULL;
							
							if (stats.file_length > 0)
								data = (u32*)memalign(32, round_up(stats.file_length, 32));
							
							if (data)
							{
								if (ISFS_Read(fd, (char*)data, stats.file_length) == (int)stats.file_length)
								{
									FILE * outfp = fopen(outpath, "wb");
									if (outfp)
									{
										if ((int)fwrite((char*)data, 1, stats.file_length, outfp) >= 0)
										{
											if(WriteIndexFile(indexPath, indextext) == true)
												result = true;
										}
										fclose(outfp);
									}
								}
								free(data);
							}
						}
						ISFS_Close(fd);
						
						if(result == false) {
							free(nbuf);
							firstuse = true;
							return -1;
						}
						if (progresscanceled) {
							free(nbuf);
							firstuse = true;
							return -10;
						}
					}
					else {
						Savegame_Extract(inpath, outpath, indexpath);
					}
				}
			}
		}
		free(nbuf);
		
		if(result == false) {
			firstuse = true;
			return -1;
		}
	}
	
	firstuse = true;
	
	return 0;//succes
}

int Savegame_GetNandPath(u64 tid, char *outbuf)
{
	static char buffer[1024] ATTRIBUTE_ALIGN(32);
	
	if (ES_GetDataDir(tid, buffer) < 0)
		return -1;

	strcpy(outbuf, buffer);

	return 0;
}

int Savegame_CheckTitle(const char *path, short source)
{
	char filepath[128];

	sprintf(filepath, "%s/banner.bin", path);

	if(source == ManageWindow::WII)
	{
		s32 fd = ISFS_Open(filepath, ISFS_OPEN_READ);
		if(fd < 0)
			return -1;
		else
			ISFS_Close(fd);
		
		return 0;
	}
	else if(source == ManageWindow::DEVICE)
	{
		FILE * fp = fopen(filepath, "rb");
		if (!fp)
			return -1;
		
		fclose(fp);
		
		return 0;
	}
	
	return -1;
}
