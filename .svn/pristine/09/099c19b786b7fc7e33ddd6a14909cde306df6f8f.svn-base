/****************************************************************************
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
 * DirList.cpp
 *
 * DirList Class
 * for Wii-FileXplorer 2009
 ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dir.h>

#include "file/DirList.h"

DirList::DirList(const char * path, const char *filter)
{
    filecount = 0;

    FileInfo = (FileInfos *) malloc(sizeof(FileInfos));
    if (!FileInfo) {
        return;
    }

    memset(&FileInfo[filecount], 0, sizeof(FileInfos));

    this->LoadPath(path, filter);
    this->SortList();
}

DirList::~DirList()
{
    for(int i = 0; i < filecount; i++)
    {
        if(FileInfo[i].FilePath) {
            free(FileInfo[i].FilePath);
            FileInfo[i].FilePath = NULL;
        }
        if(FileInfo[i].FileName) {
            free(FileInfo[i].FileName);
            FileInfo[i].FileName = NULL;
        }
    }

    if (FileInfo) {
        free(FileInfo);
        FileInfo = NULL;
    }

}

bool DirList::LoadPath(const char * folderpath, const char *filter)
{
    struct stat st;
    DIR_ITER *dir = NULL;
    char filename[1024];
	char *filterCopy, *filterTok;

    dir = diropen(folderpath);
    if (dir == NULL) {
        return false;
    }

	size_t size = strlen(filter);
	filterCopy = (char *) malloc(size + 1);
	memset(filterCopy, 0, size + 1);

    while (dirnext(dir,filename,&st) == 0)
    {
        if (strcmp(filename,".") != 0 && strcmp(filename,"..") != 0)
        {
            if(filter) {

                char *fileext = strrchr(filename, '.');
                if(fileext && ((st.st_mode & S_IFDIR) == 0)) {
					strncpy(filterCopy, filter, size);
					filterTok = strtok(filterCopy, ",");

					while (filterTok != NULL)
					{
						if ((strcasecmp(fileext, filterTok) == 0 || strcasecmp(fileext, filterTok) == 0))
						{
							FileInfos *TempFileInfo = (FileInfos *) realloc(FileInfo, (filecount+1)*sizeof(FileInfos));

							if (!TempFileInfo)
							{
								for(int i = 0; i < filecount; i++)
								{
									if(FileInfo[i].FilePath) {
										free(FileInfo[i].FilePath);
										FileInfo[i].FilePath = NULL;
									}
									if(FileInfo[i].FileName) {
										free(FileInfo[i].FileName);
										FileInfo[i].FileName = NULL;
									}
								}
								free(FileInfo);
								FileInfo = NULL;
								free(TempFileInfo);
								TempFileInfo = NULL;
								filecount = 0;
								dirclose(dir);

								free(filterCopy);
								return false;

							} else
                                FileInfo = TempFileInfo;

							memset(&(FileInfo[filecount]), 0, sizeof(FileInfo));

							FileInfo[filecount].FilePath = (char *) malloc(strlen(folderpath)+2);
							FileInfo[filecount].FileName = (char *) malloc(strlen(filename)+2);

							if (!FileInfo[filecount].FilePath || !FileInfo[filecount].FileName)
							{
								for(int i = 0; i < filecount; i++)
								{
									if(FileInfo[i].FilePath) {
										free(FileInfo[i].FilePath);
										FileInfo[i].FilePath = NULL;
									}
									if(FileInfo[i].FileName) {
										free(FileInfo[i].FileName);
										FileInfo[i].FileName = NULL;
									}
								}
								free(FileInfo);
								FileInfo = NULL;
								filecount = 0;
								dirclose(dir);

								free(filterCopy);
								return false;
							}

							//!Set the values
							snprintf(FileInfo[filecount].FilePath, strlen(folderpath)+1, "%s", folderpath);
							snprintf(FileInfo[filecount].FileName, strlen(filename)+1, "%s", filename);
							FileInfo[filecount].FileSize = st.st_size;
							FileInfo[filecount].isDir = false;
							filecount++;
						}

						filterTok = strtok(NULL, ",");
					}
                }
            } else {

                FileInfos *TempFileInfo = (FileInfos *) realloc(FileInfo, (filecount+1)*sizeof(FileInfos));

                if (!TempFileInfo)
                {
                    for(int i = 0; i < filecount; i++)
                    {
                        if(FileInfo[i].FilePath) {
                            free(FileInfo[i].FilePath);
                            FileInfo[i].FilePath = NULL;
                        }
                        if(FileInfo[i].FileName) {
                            free(FileInfo[i].FileName);
                            FileInfo[i].FileName = NULL;
                        }
                    }
                    free(FileInfo);
                    FileInfo = NULL;
                    free(TempFileInfo);
                    TempFileInfo = NULL;
                    filecount = 0;
                    dirclose(dir);
					free(filterCopy);
                    return false;

                } else
                    FileInfo = TempFileInfo;

                memset(&(FileInfo[filecount]), 0, sizeof(FileInfo));

                FileInfo[filecount].FilePath = (char *) malloc(strlen(folderpath)+2);
                FileInfo[filecount].FileName = (char *) malloc(strlen(filename)+2);

                if (!FileInfo[filecount].FilePath || !FileInfo[filecount].FileName)
                {
                    for(int i = 0; i < filecount; i++)
                    {
                        if(FileInfo[i].FilePath) {
                            free(FileInfo[i].FilePath);
                            FileInfo[i].FilePath = NULL;
                        }
                        if(FileInfo[i].FileName) {
                            free(FileInfo[i].FileName);
                            FileInfo[i].FileName = NULL;
                        }
                    }
                    free(FileInfo);
                    FileInfo = NULL;
                    filecount = 0;
                    dirclose(dir);
					free(filterCopy);
                    return false;
                }

                //!Set the values
                snprintf(FileInfo[filecount].FilePath, strlen(folderpath)+1, "%s", folderpath);
                snprintf(FileInfo[filecount].FileName, strlen(filename)+1, "%s", filename);
                FileInfo[filecount].FileSize = st.st_size;

                if ((st.st_mode & S_IFDIR) != 0)
                    FileInfo[filecount].isDir = true;
                else
                    FileInfo[filecount].isDir = false;

                filecount++;
            }
        }
    }
    dirclose(dir);
	free(filterCopy);

    return true;
}

char *DirList::GetFilename(int ind)
{
    if (ind > filecount || ind < 0)
        return NULL;
    else
        return FileInfo[ind].FileName;
}

char *DirList::GetFilepath(int ind)
{
    if (ind > filecount || ind < 0)
        return NULL;
    else
        return FileInfo[ind].FilePath;
}

unsigned int DirList::GetFilesize(int ind)
{
    if (ind > filecount || !filecount || !FileInfo)
        return 0;
    else
        return FileInfo[ind].FileSize;
}

bool DirList::IsDir(int ind)
{
    if (ind > filecount || !filecount || !FileInfo)
        return false;
    else
        return FileInfo[ind].isDir;
}

static int ListCompare(const void *a, const void *b)
{
    FileInfos *ab = (FileInfos*) a;
    FileInfos *bb = (FileInfos*) b;

    return stricmp((char *) ab->FileName, (char *) bb->FileName);
}

void DirList::SortList()
{
    qsort(FileInfo, filecount, sizeof(FileInfos), ListCompare);
}

int DirList::GetFileIndex(const char *filename)
{
	for (int i = 0; i < filecount; i++)
	{
		if (strcmp(FileInfo[i].FileName, filename) == 0)
		{
			return i;
		}
	}
	return -1;
}
