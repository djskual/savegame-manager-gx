/****************************************************************************
 * ProgressWindow
 * WiiXplorer 2009
 * by dimok
 * Modified by Dj_Skual
 * for SaveGame Manager GX
 *
 * ProgressWindow.h
 ***************************************************************************/

#ifndef _PROGRESSWINDOW_H_
#define _PROGRESSWINDOW_H_

#include <string>

#include "libwiigui/gui.h"
#include "tools/timer.h"

extern bool actioncanceled;

enum
{
    PROGRESSBAR = 1,
    THROBBER,
	LANGUAGE,
	CONNECT
};

class ProgressWindow : public GuiWindow
{
    public:
		static ProgressWindow * Instance();
		static void DestroyInstance();

        void StartProgress(const char *title, int mode, bool reset);
        void ShowProgress(u64 done, u64 total, const char *msg = NULL);
        void StopProgress();
        const char * GetTitle() { return ProgressTitle.c_str(); };
        void SetTotalValues(u64 Size, u32 FileCount UNUSED) { TotalSize = Size; };
        void ResetValues() { progressTotal = 0; TotalDone = 0; TotalSize = 0; };
        int GetProgressMode() { return showProgress; };
    protected:
        ProgressWindow();
        ~ProgressWindow();
		static void * ThreadCallback(void *arg);
		void InternalUpdate();
		void SetupProgressbar();
		void SetupThrobber();
		void MainLoop();
		void LoadWindow();
		void UpdateValues();
		void ClearMemory();
        static ProgressWindow * instance;
		
        u64 progressDone;
        u64 progressTotal;
        u64 TotalDone;
        u64 TotalSize;
        u64 TimerSize;
        int showProgress;
        bool Changed;
        bool resetstate;
		lwp_t ProgressThread;
		
		Timer ProgressTimer;
		
		std::string ProgressTitle;
		char ProgressMsg[200];
		
		GuiImageData * dialogBox;
		GuiImageData * progressbarOutline;
		GuiImageData * progressbarEmpty;
		GuiImageData * progressbar;
		GuiImageData * throbber;
		GuiImageData * btnOutline;
		
		GuiImage * dialogBoxImg;
		GuiImage * progressbarOutlineImg;
		GuiImage * progressbarEmptyImg;
		GuiImage * progressbarImg;
		GuiImage * throbberImg;
		GuiImage * buttonImg;
		
        GuiText * titleTxt;
        GuiText * msgTxt;
        GuiText * prTxt;
        GuiText * prsTxt;
        GuiText * speedTxt;
        GuiText * sizeTxt;
        GuiText * TimeTxt;;
        GuiText * AbortTxt;
        
        GuiButton * AbortBtn;
        
        GuiSound * soundClick;
        GuiSound * soundOver;
		
		GuiTrigger * trigA;
};

void StartProgress(const char *title, int mode, bool reset);
void ShowProgress(u64 done, u64 total, const char *msg = NULL);
void StopProgress();

#endif
