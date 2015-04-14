/****************************************************************************
 * ProgressWindow
 * USB Loader GX 2009
 *
 * ProgressWindow.h
 ***************************************************************************/

#ifndef _PROGRESSWINDOW_H_
#define _PROGRESSWINDOW_H_

enum
{
	PROGRESSBAR,
	THROBBER,
	UPDATE
};

typedef void (*ProgressAbortCallback)(void);

#ifdef __cplusplus

void InitProgressThread();
void ExitProgressThread();
void ShowProgress(const char *title, const char *msg1 = NULL, const char *msg2 = NULL, s64 done = 0, s64 total = 1, bool swSize = false, bool swTime = false, int mode = PROGRESSBAR);

extern "C"
{
#endif

void StartProgress(const char * title, const char * msg1 = NULL, const char * msg2 = NULL, bool swSize = false, bool swTime = false, int mode = PROGRESSBAR);
void ShowProgress(s64 done, s64 total);
void ProgressSetAbortCallback(ProgressAbortCallback callback);
void ProgressStop();

#ifdef __cplusplus
}
#endif

#endif
