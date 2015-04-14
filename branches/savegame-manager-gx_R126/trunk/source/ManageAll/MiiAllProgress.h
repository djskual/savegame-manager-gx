/****************************************************************************
 * MiiAllProgress
 * SaveGame Manager GX 2011
 * by dj_skual
 *
 * MiiAllProgress.h
 ***************************************************************************/

#ifndef _MIIALLPROGRESS_H_
#define _MIIALLPROGRESS_H_

#include <string>

typedef void (*MiiAllProgressAbortCallback)(void);

#ifdef __cplusplus

void InitMiiAllProgressThread();
void ExitMiiAllProgressThread();
void ShowMiiAllProgress(const char *name,
						int done,
						int total,
						int selected = -1,
						int emu_selected = -1,
						std::string creator = "",
						int female = 0,
						int day = 0,
						int month = 0,
						int favColor = 0);

extern "C"
{
#endif

void MiiAllProgressSetAbortCallback(MiiAllProgressAbortCallback callback);
void MiiAllProgressStop();

#ifdef __cplusplus
}
#endif

#endif
