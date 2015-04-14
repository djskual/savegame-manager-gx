#ifndef STARTUPPROCESS_H_
#define STARTUPPROCESS_H_

#include "../GUI/gui.h"

class StartUpProcess
{
	public:
		static bool Run(int argc, char *argv[]);
		
	private:
		StartUpProcess();
		~StartUpProcess();
		void ShowProgress(int done);
		bool Execute(int argc, char *argv[]);
		void TextFade(int direction);
		void SetTextf(const char * format, ...);
		void Draw();
		static void ParseArguments(int argc, char *argv[]);
		
		bool showProgress;
		
		GuiImageData * GXImageData;
		GuiImageData * ProgressBarOutlineData;
		GuiImageData * ProgressBarEmptyData;
		GuiImageData * ProgressBarData;
		
		GuiImage * background;
		GuiImage * GXImage;
		GuiImage * ProgressBarOutline;
		GuiImage * ProgressBarEmpty;
		GuiImage * ProgressBar;
		
		GuiText * titleTxt;
		GuiText * messageTxt;
};

#endif
