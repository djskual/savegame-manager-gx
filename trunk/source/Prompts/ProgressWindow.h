/****************************************************************************
 * Copyright (C) 2009-2011 Dimok
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/
#ifndef _PROGRESSWINDOW_H_
#define _PROGRESSWINDOW_H_

#include "../GUI/gui.h"
#include "ProgressBar.h"

class ProgressWindow : public GuiFrame, public sigslot::has_slots<>
{
	public:
		static ProgressWindow * Instance() { if(!instance) instance = new ProgressWindow; return instance; }
		static void DestroyInstance() { delete instance; instance = NULL; }
		
		void OpenWindow(void) { OpenRequest = true; CloseRequest = false; }
		void CloseWindow(void) { OpenRequest = false; CloseRequest = true; }
		
		void StartProgress(const char *title, const char *msg = NULL);
		void ShowProgress(const u64 &done, const u64 &total, const char *filename);
		void ShowProgress(const u64 &done, const u64 &total);
		void StartThrobber(const char *title, const char *msg = NULL);
		void ShowThrobber(const char *msg);
		void FinishProgress();
		void StopProgress();
		void SetTitle(const char *title);
		void SetMessage(const char *msg);
		
		const char * GetTitle() const { return ProgressTitle; };
		bool IsCanceled() const { return Canceled; }
		bool IsRunning() const { return !WindowClosed; }
		void Draw();
		
		enum
		{
			PROGRESSBAR,
			THROBBER,
		};
		
	private:
		ProgressWindow();
		virtual ~ProgressWindow();
		static ProgressWindow * instance;
		
		void OnCancelClick(GuiButton *sender UNUSED, int pointer UNUSED, const POINT &p UNUSED) { Canceled = true; }
		void TryOpenWindow(void);
		void TryCloseWindow(void);
		void AdvanceRotation(void);
		
		int type;
		u64 progressDone;
		u64 progressTotal;
		bool Changed;
		bool Canceled;
		bool WindowClosed;
		bool OpenRequest;
		bool CloseRequest;

		Timer delayTimer;
		Timer completeTimer;
		
		char ProgressTitle[50];
		char ProgressMsg[80];
		
		ProgressBar * progressBar;
		
		GuiImageData * dialogBox;
		GuiImageData * btnOutline;
		GuiImageData * throbber;
		
		GuiImage * dialogBoxImg;
		GuiImage * buttonImg;
		GuiImage * throbberImg;
		
		GuiText * titleTxt;
		GuiText * msgTxt;
		GuiText * AbortBtnTxt;
		
		GuiButton * AbortBtn;
		
		GuiSound * soundClick;
		GuiSound * soundOver;

		GuiTrigger * trigA;
};

#define StartProgress		ProgressWindow::Instance()->StartProgress
#define StartThrobber		ProgressWindow::Instance()->StartThrobber
#define ShowProgress		ProgressWindow::Instance()->ShowProgress
#define ShowThrobber		ProgressWindow::Instance()->ShowThrobber
#define FinishProgress		ProgressWindow::Instance()->FinishProgress
#define StopProgress		ProgressWindow::Instance()->StopProgress
#define ProgressCanceled	ProgressWindow::Instance()->IsCanceled

#endif
