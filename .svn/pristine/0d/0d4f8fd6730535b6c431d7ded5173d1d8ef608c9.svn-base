/****************************************************************************
 * SaveGame Manager GX
 * Dj_Skual 2010
 *
 * customFilePathBrowser.cpp
 *
 ***************************************************************************/

#include <gccore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiiuse/wpad.h>
#include <sys/dir.h>
#include <malloc.h>

#include "libwiigui/gui_browsers/gui_browsers.h"
#include "language/langbrowser.h"
#include "language/UpdateLanguage.h"
#include "prompts/promptwindows.h"
#include "file/fileops.h"
#include "menu/MainWindow.h"
#include "main.h"

BROWSERLANGINFO browserl;
BROWSERLANGENTRY * browserLang = NULL;

char langroot[10];
bool firstlang = true;

/****************************************************************************
 * ResetLangBrowser()
 * Clears the file browser memory, and allocates one initial entry
 ***************************************************************************/
void ResetLangBrowser()
{
	browserl.numEntries = 0;
	browserl.selIndex = 0;
	browserl.pageIndex = 0;

	// Clear any existing values
	if(browserLang != NULL)
	{
		free(browserLang);
		browserLang = NULL;
	}
	// set aside space for 1 entry
	browserLang = (BROWSERLANGENTRY *)malloc(sizeof(BROWSERLANGENTRY));
	memset(browserLang, 0, sizeof(BROWSERLANGENTRY));
}

/****************************************************************************
 * UpdateLangDirName()
 * Update curent directory name for file browser
 ***************************************************************************/
int UpdateLangDirName()
{
	int size=0;
	char * test;
	char temp[1024];

	/* current directory doesn't change */
	if (strcmp(browserLang[browserl.selIndex].filename,".") == 0)
	{
		return 0;
	}
	/* go up to parent directory */
	else if (strcmp(browserLang[browserl.selIndex].filename,"..") == 0)
	{
		/* determine last subdirectory namelength */
		sprintf(temp,"%s",browserl.dir);
		test = strtok(temp,"/");
		while (test != NULL)
		{
			size = strlen(test);
			test = strtok(NULL,"/");
		}

		/* remove last subdirectory name */
		size = strlen(browserl.dir) - size - 1;
		browserl.dir[size] = 0;

		return 1;
	}
	/* Open a directory */
	else
	{
		/* test new directory namelength */
		if ((strlen(browserl.dir)+1+strlen(browserLang[browserl.selIndex].filename)) < MAXPATHLEN)
		{
			/* update current directory name */
			sprintf(browserl.dir, "%s%s/",browserl.dir, browserLang[browserl.selIndex].filename);
			return 1;
		}
		else
		{
			return -1;
		}
	}
}

/****************************************************************************
 * LangSortCallback
 *
 * Quick sort callback to sort file entries with the following order:
 *   .
 *   ..
 *   <dirs>
 *   <files>
 ***************************************************************************/
int LangSortCallback(const void *f1, const void *f2)
{
	/* Special case for implicit directories */
	if(((BROWSERLANGENTRY *)f1)->filename[0] == '.' || ((BROWSERLANGENTRY *)f2)->filename[0] == '.')
	{
		if(strcmp(((BROWSERLANGENTRY *)f1)->filename, ".") == 0) { return -1; }
		if(strcmp(((BROWSERLANGENTRY *)f2)->filename, ".") == 0) { return 1; }
		if(strcmp(((BROWSERLANGENTRY *)f1)->filename, "..") == 0) { return -1; }
		if(strcmp(((BROWSERLANGENTRY *)f2)->filename, "..") == 0) { return 1; }
	}

	/* If one is a file and one is a directory the directory is first. */
	if(((BROWSERLANGENTRY *)f1)->isdir && !(((BROWSERLANGENTRY *)f2)->isdir)) return -1;
	if(!(((BROWSERLANGENTRY *)f1)->isdir) && ((BROWSERLANGENTRY *)f2)->isdir) return 1;

	/* Sort files*/
	return stricmp(((BROWSERLANGENTRY *)f1)->filename, ((BROWSERLANGENTRY *)f2)->filename);
}

/***************************************************************************
 * Browse subdirectories
 **************************************************************************/
int ParseLangDirectory(const char * Path)
{
	DIR_ITER *dir = NULL;
	char fulldir[MAXPATHLEN];
	char filename[MAXPATHLEN];
	struct stat filestat;
	
	// reset browser
	ResetLangBrowser();
	
	// open the directory
	dir = diropen(Path);
	if (dir == NULL)
	{
		sprintf(fulldir, "%s%s", langroot, browserl.dir); // add currentDevice to path
		dir = diropen(fulldir);
	}

	// if we can't open the dir, try opening the root dir
	if (dir == NULL)
	{
		sprintf(browserl.dir,"/");
		dir = diropen(langroot);
		if (dir == NULL)
		{
			return -1;
		}
	}
	
	
	// index files/folders
	int entryNum = 0;

	while(dirnext(dir,filename,&filestat) == 0)
	{
		if(strcmp(filename,".") != 0)
		{
			BROWSERLANGENTRY * newBrowserLang = (BROWSERLANGENTRY *)realloc(browserLang, (entryNum+1) * sizeof(BROWSERLANGENTRY));

			if(!newBrowserLang) // failed to allocate required memory
			{
				ResetLangBrowser();
				entryNum = -1;
				break;
			}
			else
			{
				browserLang = newBrowserLang;
			}
			memset(&(browserLang[entryNum]), 0, sizeof(BROWSERLANGENTRY)); // clear the new entry

			strncpy(browserLang[entryNum].filename, filename, MAXJOLIET);

			if(strcmp(filename,"..") == 0)
			{
				sprintf(browserLang[entryNum].displayname, "..");
			}
			else
			{
				strncpy(browserLang[entryNum].displayname, filename, MAXDISPLAY);	// crop name for display
			}

			browserLang[entryNum].length = filestat.st_size;
			browserLang[entryNum].isdir = (filestat.st_mode & _IFDIR) == 0 ? 0 : 1; // flag this as a dir
			
			if (browserLang[entryNum].isdir ||
				VerifyFileType(browserLang[entryNum].displayname, "lang"))
				entryNum++;
		}
	}

	// close directory
	dirclose(dir);

	// Sort the file list
	qsort(browserLang, entryNum, sizeof(BROWSERLANGENTRY), LangSortCallback);

	browserl.numEntries = entryNum;
	return entryNum;
}

/****************************************************************************
 * BrowserLangChangeFolder
 *
 * Update current directory and set new entry list if directory has changed
 ***************************************************************************/
int BrowserLangChangeFolder()
{
	if(firstlang == false)
	{
		if(!UpdateLangDirName())
			return -1;
	}

	char fullpath[200];
	snprintf(fullpath, sizeof(fullpath), "%s%s", langroot, browserl.dir);
	ParseLangDirectory(fullpath);
	
	return browserl.numEntries;
}

void ParseLang(char *Path)
{
    char temp[200], name[200], value[200];

    memcpy (temp,Path,strlen(Path)+1);

    char * eq = strchr(temp, '/');

    if(!eq) return;

    *eq = 0;

    TrimLang(name, temp, sizeof(name));
	TrimLang(value, eq+1, sizeof(value));

	sprintf(langroot, "%s/", name);
	sprintf(browserl.dir, "%s", value);
	
	return;
}

void ParseLangFile(char *Path)
{
    char temp[200], name[200];

    memcpy (temp,Path,strlen(Path)+1);

    char * eq = strrchr(temp, '/');

    if(!eq) return;

    *eq = 0;

    TrimLang(name, temp, sizeof(name));
    
	sprintf(browserl.dir, "%s/", name);	
	
	return;
}

void TrimLang(char *dest, char *src, int size)
{
	int len;
	while (*src == ' ') src++;
	len = strlen(src);
	while (len > 0 && strchr(" \r\n", src[len-1])) len--;
	if (len >= size) len = size-1;
	strncpy(dest, src, len);
	dest[len] = 0;
}

/****************************************************************************
 * CreateLangPath
 * Displays an onscreen Keyboard to create new path
 ***************************************************************************/
int CreateLangPath(char * Path)
{
	MainWindow::Instance()->HaltGui();
	GuiImage disabledImg(screenwidth, screenheight, (GXColor){0, 0, 0, 0x70});
	MainWindow::Instance()->Append(&disabledImg);
	MainWindow::Instance()->ResumeGui();
	
	int result = OnScreenKeyboard(Path, 149, 1);
	if(result == 1)
	{
		mkdir(Path, 777);
		
		WindowPrompt(tr("Language Path changed."), 0, tr("OK"), 0, 0, 0, false);
		if ((Path[strlen(Path)-1] != '/'))
			strcat(Path, "/");
		snprintf(cfg.LanguagePath, sizeof(cfg.LanguagePath), "%s", Path);
	}
	
	MainWindow::Instance()->HaltGui();
	MainWindow::Instance()->Remove(&disabledImg);
	MainWindow::Instance()->ResumeGui();
	
	return result;
}

/****************************************************************************
 * FoundLangDevice
 * found the browsed device in langbrowser
 ***************************************************************************/
int FoundLangDevice()
{
	if(!strncmp(langroot, "sd:/", 4))
		return 0;
	else
		return 1;
}

/****************************************************************************
 * RefreshBrowser
 ***************************************************************************/
void RefreshBrowser(char * Path)
{
	firstlang = true;
	
	ResetLangBrowser();
	
	ParseLang(Path);
	ParseLangDirectory(langroot);
	
	ParseLang(Path);
	ParseLangFile(browserl.dir);
	
	BrowserLangChangeFolder();
	
	firstlang = false;
}

/****************************************************************************
 * BrowseLangDevice
 * Displays a list of files on the selected device
 ***************************************************************************/
int BrowseLangDevice()
{
	ResetLangBrowser();
	
	char langPath[MAXPATHLEN];
	sprintf(langPath, "%s", cfg.LanguagePath);
	
	ParseLang(langPath);
	ParseLangDirectory(langroot);
	
	ParseLangFile(browserl.dir);
	BrowserLangChangeFolder();
	
	int Device = FoundLangDevice();
	
	firstlang = false;
	
	SimpleGuiTrigger trigA(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);
	GuiTrigger trigB;
	trigB.SetButtonOnlyTrigger(-1, WPAD_BUTTON_B | WPAD_CLASSIC_BUTTON_B, PAD_BUTTON_B);
	GuiTrigger trigHome;
    trigHome.SetButtonOnlyTrigger(-1, WPAD_BUTTON_HOME | WPAD_CLASSIC_BUTTON_HOME, 0);
	
	GuiSound btnSoundOver(button_over_wav, button_over_wav_size, cfg.SFXVolume);
	GuiSound btnSoundClick(button_click_wav, button_click_wav_size, cfg.SFXVolume);
	
	GuiImage disabledImg(screenwidth, screenheight, (GXColor){0, 0, 0, 0x70});
	
	GuiImageData browser(custom_path_browser_png, custom_path_browser_png_size);
	GuiImage browserImg(&browser);
	browserImg.SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	
	GuiImageData folderImgData(icon_folder_png, icon_folder_png_size);
	GuiImage folderImg(&folderImgData);
	GuiButton folderBtn(folderImg.GetWidth(), folderImg.GetHeight());
	folderBtn.SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	folderBtn.SetPosition(-181, -164);
	folderBtn.SetImage(&folderImg);
	folderBtn.SetSoundOver(&btnSoundOver);
	folderBtn.SetSoundClick(&btnSoundClick);
	folderBtn.SetTrigger(&trigA);
	folderBtn.SetEffectGrow();
	
	GuiImageData closeImgData(close_png, close_png_size);
	GuiImageData closeOverImgData(close_over_png, close_over_png_size);
	GuiImage closeImg(&closeImgData);
	GuiImage closeOverImg(&closeOverImgData);
	GuiButton closeBtn(closeImg.GetWidth(), closeImg.GetHeight());
	closeBtn.SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	closeBtn.SetPosition(176, -160);
	closeBtn.SetImage(&closeImg);
	closeBtn.SetImageOver(&closeOverImg);
	closeBtn.SetSoundOver(&btnSoundOver);
	closeBtn.SetSoundClick(&btnSoundClick);
	closeBtn.SetTrigger(&trigA);
	closeBtn.SetTrigger(&trigB);
	closeBtn.SetTrigger(&trigHome);
	closeBtn.SetEffectGrow();
	
	GuiImageData wifiImgData(wifi_png, wifi_png_size);
	GuiImageData wifiOverImgData(wifi_over_png, wifi_over_png_size);
	GuiImage wifiImg(&wifiImgData);
	GuiImage wifiOverImg(&wifiOverImgData);
	GuiButton wifiBtn(wifiImg.GetWidth(), wifiImg.GetHeight());
	wifiBtn.SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	wifiBtn.SetPosition(137, -160);
	wifiBtn.SetImage(&wifiImg);
	wifiBtn.SetImageOver(&wifiOverImg);
	wifiBtn.SetSoundOver(&btnSoundOver);
	wifiBtn.SetSoundClick(&btnSoundClick);
	wifiBtn.SetTrigger(&trigA);
	
	GuiImageData sdCardData(sd_card_png, sd_card_png_size);
	GuiImage sdCardImg(&sdCardData);
	GuiButton sdBtn(sdCardImg.GetWidth(), sdCardImg.GetHeight());
	sdBtn.SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	sdBtn.SetPosition(166, -164);
	sdBtn.SetImage(&sdCardImg);
	sdBtn.SetSoundOver(&btnSoundOver);
	sdBtn.SetSoundClick(&btnSoundClick);
	sdBtn.SetVisible(Device ? false : true);
	sdBtn.SetTrigger(&trigA);
	sdBtn.SetEffectGrow();
	sdBtn.SetState(Device ? STATE_DISABLED : STATE_DEFAULT);
	
	GuiImageData usbStorageData(usb_storage_png, usb_storage_png_size);
	GuiImage usbStorageImg(&usbStorageData);
	GuiButton usbBtn(usbStorageImg.GetWidth(), usbStorageImg.GetHeight());
	usbBtn.SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	usbBtn.SetPosition(165, -164);
	usbBtn.SetImage(&usbStorageImg);
	usbBtn.SetSoundOver(&btnSoundOver);
	usbBtn.SetSoundClick(&btnSoundClick);
	usbBtn.SetVisible(Device ? true : false);
	usbBtn.SetTrigger(&trigA);
	usbBtn.SetEffectGrow();
	usbBtn.SetState(Device ? STATE_DEFAULT : STATE_DISABLED);
	
	GuiText AdressText(NULL, 18, (GXColor) { 0, 0, 0, 255});
	AdressText.SetTextf("%s%s", langroot, browserl.dir);
	AdressText.SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	if(device.DeviceCount >= 2) {
		AdressText.SetPosition(185, -165);
		AdressText.SetMaxWidth(240, GuiText::SCROLL);
	}
	else {
		AdressText.SetPosition(168, -165);
		AdressText.SetMaxWidth(255, GuiText::SCROLL);
	}
	
	GuiLangBrowser custombrowser(338, 276);
	custombrowser.SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	custombrowser.SetPosition(-19, 0);
	
	MainWindow::Instance()->HaltGui();
	
	GuiWindow customWindow(screenwidth, screenheight);
	customWindow.Append(&disabledImg);
	customWindow.Append(&browserImg);
	customWindow.Append(&custombrowser);
	customWindow.Append(&AdressText);
	customWindow.Append(&folderBtn);
	customWindow.Append(&closeBtn);
	customWindow.Append(&wifiBtn);
	if(device.DeviceCount >= 2) {
		customWindow.Append(&sdBtn);
		customWindow.Append(&usbBtn);
	}
	
	MainWindow::Instance()->Append(&customWindow);
	
	MainWindow::Instance()->ResumeGui();
	
	bool exit = false;
	int result = 0;
	
	while(!exit)
	{
		for(int i=0; i < CUSTOM_PAGESIZE; i++)
		{
			if(custombrowser.customList[i]->GetState() == STATE_CLICKED)
			{
				custombrowser.customList[i]->ResetState();
				if(browserLang[browserl.selIndex].isdir)
				{
					if(BrowserLangChangeFolder())
					{
						custombrowser.ResetState();
						custombrowser.customList[0]->SetState(STATE_SELECTED);
						custombrowser.TriggerUpdate();
						sprintf(langPath, "%s%s", langroot, browserl.dir);	
						AdressText.SetTextf(langPath);		
					}
				}
				else
				{
					customWindow.SetState(STATE_DISABLED);
					int choice = WindowPrompt(browserLang[browserl.selIndex].filename, tr("Do you want to load this language ?"), tr("Yes"), tr("Cancel"), 0, 0, false);
					if(choice == 1)
					{
						char filepath[MAXPATHLEN];
						snprintf(filepath, sizeof(filepath), "%s%s", langPath, browserLang[browserl.selIndex].filename);
						if(gettextLoadLanguage(filepath))
						{
							sprintf(cfg.LanguagePath, "%s", langPath);
							sprintf(cfg.LanguageFile, "%s", browserLang[browserl.selIndex].filename);
							result = 2;
							exit = true;
						}
					}
					customWindow.SetState(STATE_DEFAULT);
				}
			}
		}
		
		if(closeBtn.GetState()==STATE_CLICKED)
		{
			closeBtn.ResetState();
			exit = true;
		}
		
		else if(wifiBtn.GetState()==STATE_CLICKED)
		{
			wifiBtn.ResetState();
			
			int choice = WindowPrompt(tr("Update all Language Files to:"), langPath, tr("Yes"), tr("Cancel"));
		    if (choice == 1) {
				updateAllLanguageFiles(langPath);
				
				RefreshBrowser(langPath);
			}
		}
		else if(folderBtn.GetState()==STATE_CLICKED)
		{
			folderBtn.ResetState();
			
			customWindow.SetState(STATE_DISABLED);
			
			result = CreateLangPath(langPath);
			
			if(result == 1)
			{
				RefreshBrowser(cfg.LanguagePath);
				
				sprintf(langPath, "%s%s", langroot, browserl.dir);	
				AdressText.SetTextf(langPath);
			}
			
			result = 0;
			customWindow.SetState(STATE_DEFAULT);
		}
		
		else if(sdBtn.GetState()==STATE_CLICKED)
		{
			sdBtn.ResetState();
			sdBtn.SetVisible(false);
			usbBtn.SetVisible(true);
			sdBtn.SetState(STATE_DISABLED);
			usbBtn.SetState(STATE_DEFAULT);
			
			RefreshBrowser((char*)"usb:/");
			
			sprintf(langPath, "%s%s", langroot, browserl.dir);
			AdressText.SetTextf(langPath);
			
			Device = 1;
		}
		else if(usbBtn.GetState()==STATE_CLICKED)
		{
			usbBtn.ResetState();
			usbBtn.SetVisible(false);
			sdBtn.SetVisible(true);
			usbBtn.SetState(STATE_DISABLED);
			sdBtn.SetState(STATE_DEFAULT);
			
			RefreshBrowser((char*)"sd:/");
			
			sprintf(langPath, "%s%s", langroot, browserl.dir);
			AdressText.SetTextf(langPath);
			
			Device = 0;
		}
	}
	
	MainWindow::Instance()->HaltGui();
	MainWindow::Instance()->Remove(&customWindow);
	MainWindow::Instance()->ResumeGui();
	
	firstlang = true;
	
	return result;
}
