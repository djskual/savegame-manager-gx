/****************************************************************************
 * SaveAllProgress
 * SaveGame Manager GX 2011
 * by dj_skual
 *
 * SaveAllProgress.h
 ***************************************************************************/

#ifndef _SAVEALLPROGRESS_H_
#define _SAVEALLPROGRESS_H_

#include <string>
#include <gctypes.h>

typedef void (*SaveAllProgressAbortCallback)(void);

#ifdef __cplusplus

void InitSaveAllProgressThread();
void ExitSaveAllProgressThread();
void ShowSaveAllProgress(const char *name, const char *subname, int done, int total, u64 tid, std::string Path = "");

extern "C"
{
#endif

void SaveAllProgressSetAbortCallback(SaveAllProgressAbortCallback callback);
void SaveAllProgressStop();

#ifdef __cplusplus
}
#endif

#endif
