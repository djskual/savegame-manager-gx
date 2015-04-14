/****************************************************************************
 * ManageProgress
 * SaveGame Manager GX 2010
 * by dj_skual
 *
 * ManageProgress.h
 ***************************************************************************/

#ifndef _MANAGEPROGRESS_H_
#define _MANAGEPROGRESS_H_

typedef void (*ManageProgressAbortCallback)(void);

#ifdef __cplusplus

void InitManageProgressThread();
void ExitManageProgressThread();

extern "C"
{
#endif

void StartManageProgress(const char * title, const char * msg = NULL);
void ManageProgressSetAbortCallback(ManageProgressAbortCallback callback);
void ManageProgressStop();

#ifdef __cplusplus
}
#endif

#endif
