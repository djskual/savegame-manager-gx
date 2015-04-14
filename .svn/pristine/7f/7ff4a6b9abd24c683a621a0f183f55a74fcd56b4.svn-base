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
#include "prompts/browsers/customFilePathBrowser.h"
#include "prompts/promptwindows.h"
#include "file/fileops.h"
#include "menu/MainWindow.h"
#include "main.h"

BROWSERCUSTOMFILEPATHINFO browserCFP;
BROWSERCUSTOMFILEPATHENTRY * browserCustomFilePath = NULL; // list of files/folders in browser

char CFProot[10];
bool firstCFP = true;

/****************************************************************************
 * ResetCustomFileBrowser()
 * Clears the file browser memory, and allocates one initial entry
 ***************************************************************************/
void ResetCustomFileBrowser()
{
	browserCFP.numEntries = 0;
	browserCFP.selIndex = 0;
	browserCFP.pageIndex = 0;

	// Clear any existing values
	if(browserCustomFilePath != NULL)
	{
		free(browserCustomFilePath);
		browserCustomFilePath = NULL;
	}
	// set aside space for 1 entry
	browserCustomFilePath = (BROWSERCUSTOMFILEPATHENTRY *)malloc(sizeof(BROWSERCUSTOMFILEPATHENTRY));
	memset(browserCustomFilePath, 0, sizeof(BROWSERCUSTOMFILEPATHENTRY));
}

/****************************************************************************
 * UpdateCustomFileDirName()
 * Update curent directory name for file browser
 ***************************************************************************/
int UpdateCustomFileDirName()
{
	int size=0;
	char * test;
	char temp[1024];

	/* current directory doesn't change */
	if (strcmp(browserCustomFilePath[browserCFP.selIndex].filename,".") == 0)
	{
		return 0;
	}
	/* go up to parent directory */
	else if (strcmp(browserCustomFilePath[browserCFP.selIndex].filename,"..") == 0)
	{
		/* determine last subdirectory namelength */
		sprintf(temp,"%s",browserCFP.dir);
		test = strtok(temp,"/");
		while (test != NULL)
		{
			size = strlen(test);
			test = strtok(NULL,"/");
		}

		/* remove last subdirectory name */
		size = strlen(browserCFP.dir) - size - 1;
		browserCFP.dir[size] = 0;

		return 1;
	}
	/* Open a directory */
	else
	{
		/* test new directory namelength */
		if ((strlen(browserCFP.dir)+1+strlen(browserCustomFilePath[browserCFP.selIndex].filename)) < MAXPATHLEN)
		{
			/* update current directory name */
			sprintf(browserCFP.dir, "%s%s/",browserCFP.dir, browserCustomFilePath[browserCFP.selIndex].filename);
			return 1;
		}
		else
		{
			return -1;
		}
	}
}

/****************************************************************************
 * CustomFileSortCallback
 *
 * Quick sort callback to sort file entries with the following order:
 *   .
 *   ..
 *   <dirs>
 *   <files>
 ***************************************************************************/
int CustomFileSortCallback(const void *f1, const void *f2)
{
	/* Special case for implicit directories */
	if(((BROWSERCUSTOMFILEPATHENTRY *)f1)->filename[0] == '.' || ((BROWSERCUSTOMFILEPATHENTRY *)f2)->filename[0] == '.')
	{
		if(strcmp(((BROWSERCUSTOMFILEPATHENTRY *)f1)->filename, ".") == 0) { return -1; }
		if(strcmp(((BROWSERCUSTOMFILEPATHENTRY *)f2)->filename, ".") == 0) { return 1; }
		if(strcmp(((BROWSERCUSTOMFILEPATHENTRY *)f1)->filename, "..") == 0) { return -1; }
		if(strcmp(((BROWSERCUSTOMFILEPATHENTRY *)f2)->filename, "..") == 0) { return 1; }
	}

	/* If one is a file and one is a directory the directory is first. */
	if(((BROWSERCUSTOMFILEPATHENTRY *)f1)->isdir && !(((BROWSERCUSTOMFILEPATHENTRY *)f2)->isdir)) return -1;
	if(!(((BROWSERCUSTOMFILEPATHENTRY *)f1)->isdir) && ((BROWSERCUSTOMFILEPATHENTRY *)f2)->isdir) return 1;

	/* Sort files*/
	return stricmp(((BROWSERCUSTOMFILEPATHENTRY *)f1)->filename, ((BROWSERCUSTOMFILEPATHENTRY *)f2)->filename);
}

/***************************************************************************
 * Browse subdirectories
 **************************************************************************/
int ParseCustomFileDirectory(const char * Path)
{
	DIR_ITER *dir = NULL;
	char fulldir[MAXPATHLEN];
	char filename[MAXPATHLEN];
	struct stat filestat;
	
	// reset browser
	ResetCustomFileBrowser();
	
	// open the directory
	dir = diropen(Path);
	if (dir == NULL)
	{
		sprintf(fulldir, "%s%s", CFProot, browserCFP.dir); // add currentDevice to path
		dir = diropen(fulldir);
	}

	// if we can't open the dir, try opening the root dir
	if (dir == NULL)
	{
		sprintf(browserCFP.dir,"/");
		dir = diropen(CFProot);
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
			BROWSERCUSTOMFILEPATHENTRY * newBrowserCustomFilePath = (BROWSERCUSTOMFILEPATHENTRY *)realloc(browserCustomFilePath, (entryNum+1) * sizeof(BROWSERCUSTOMFILEPATHENTRY));

			if(!newBrowserCustomFilePath) // failed to allocate required memory
			{
				ResetCustomFileBrowser();
				entryNum = -1;
				break;
			}
			else
			{
				browserCustomFilePath = newBrowserCustomFilePath;
			}
			memset(&(browserCustomFilePath[entryNum]), 0, sizeof(BROWSERCUSTOMFILEPATHENTRY)); // clear the new entry

			strncpy(browserCustomFilePath[entryNum].filename, filename, MAXJOLIET);

			if(strcmp(filename,"..") == 0)
			{
				sprintf(browserCustomFilePath[entryNum].displayname, "..");
			}
			else
			{
				strncpy(browserCustomFilePath[entryNum].displayname, filename, MAXDISPLAY);	// crop name for display
			}

			browserCustomFilePath[entryNum].length = filestat.st_size;
			browserCustomFilePath[entryNum].isdir = (filestat.st_mode & _IFDIR) == 0 ? 0 : 1; // flag this as a dir
			
			entryNum++;
		}
	}

	// close directory
	dirclose(dir);

	// Sort the file list
	qsort(browserCustomFilePath, entryNum, sizeof(BROWSERCUSTOMFILEPATHENTRY), CustomFileSortCallback);

	browserCFP.numEntries = entryNum;
	return entryNum;
}

/****************************************************************************
 * FileCustomChangeFolder
 *
 * Update current directory and set new entry list if directory has changed
 ***************************************************************************/
int FileCustomChangeFolder()
{
	if(firstCFP == false)
	{
		if(!UpdateCustomFileDirName())
			return -1;
	}

	char fullpath[200];
	snprintf(fullpath, sizeof(fullpath), "%s%s", CFProot, browserCFP.dir);
	ParseCustomFileDirectory(fullpath);
	
	return browserCFP.numEntries;
}

void ParseCustomFile(char *Path)
{
    char temp[200], name[200], value[200];

    memcpy (temp,Path,strlen(Path)+1);

    char * eq = strchr(temp, '/');

    if(!eq) return;

    *eq = 0;

    TrimCustomFile(name, temp, sizeof(name));
    TrimCustomFile(value, eq+1, sizeof(value));

	sprintf(CFProot, "%s/", name);
	sprintf(browserCFP.dir, "%s", value);
	
	return;
}

void ParseCFP(char *Path)
{
    char temp[200], name[200];

    memcpy (temp,Path,strlen(Path)+1);

    char * eq = strrchr(temp, '/');

    if(!eq) return;

    *eq = 0;

    TrimCustomFile(name, temp, sizeof(name));
    
	sprintf(browserCFP.dir, "%s/", name);	
	
	return;
}

void TrimCustomFile(char *dest, char *src, int size)
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
 * CreateCustomFilePath
 * Displays an onscreen Keyboard to create new path
 ***************************************************************************/
int CreateCustomFilePath(char * customPath)
{
	MainWindow::Instance()->HaltGui();
	GuiImage disabledImg(screenwidth, screenheight, (GXColor){0, 0, 0, 0x70});
	MainWindow::Instance()->Append(&disabledImg);
	MainWindow::Instance()->ResumeGui();
	
	int result = OnScreenKeyboard(customPath, 149, 1);
	if(result == 1)
	{
		if (CheckFile(customPath) == false)
			result = 0;
	}
	
	MainWindow::Instance()->HaltGui();
	MainWindow::Instance()->Remove(&disabledImg);
	MainWindow::Instance()->ResumeGui();
	
	return result;
}

/****************************************************************************
 * FoundCustomFileDevice
 * found the browsed device in customPAthBrowser
 ***************************************************************************/
int FoundCustomFileDevice()
{
	if(!strncmp(CFProot, "sd:/", 4))
		return 0;
	else
		return 1;

}

/****************************************************************************
 * BrowseCustom
 * Displays a list of files on the selected device
 ***************************************************************************/
int BrowseCustomFile(char * Path, char * customPath)
{
	ResetCustomFileBrowser();
	
	ParseCustomFile(Path);
	ParseCustomFileDirectory(CFProot);
	
	ParseCFP(browserCFP.dir);
	FileCustomChangeFolder();

	sprintf(customPath, "%s%s", CFProot, browserCFP.dir);
	
	int Device = FoundCustomFileDevice();
	
	firstCFP = false;
	
	SimpleGuiTrigger trigA(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);
	GuiTrigger trigB;
	trigB.SetButtonOnlyTrigger(-1, WPAD_BUTTON_B | WPAD_CLASSIC_BUTTON_B, PAD_BUTTON_B);
	GuiTrigger trigHome;
    trigHome.SetButtonOnlyTrigger(-1, WPAD_BUTTON_HOME | WPAD_CLASSIC_BUTTON_HOME, 0);
	
	GuiSound btnSoundOver(button_over_wav, button_over_wav_size, cfg.SFXVolume);
	GuiSound btnSoundClick(button_click_wav, button_click_wav_size, cfg.SFXVolume);
	
	GuiImage disabledImg(screenwidth, screenheight, (GXColor){0, 0, 0, 0x70});
	
	GuiImageData browser(custom_file_path_browser_png, custom_file_path_browser_png_size);
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
	AdressText.SetTextf("%s%s", CFProot, browserCFP.dir);
	AdressText.SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	if(device.DeviceCount >= 2) {
		AdressText.SetPosition(185, -165);
		AdressText.SetMaxWidth(240, GuiText::SCROLL);
	}
	else {
		AdressText.SetPosition(168, -165);
		AdressText.SetMaxWidth(255, GuiText::SCROLL);
	}
	
	GuiCustomFilePathBrowser custombrowser(338, 276);
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
				if(browserCustomFilePath[browserCFP.selIndex].isdir)
				{
					if(FileCustomChangeFolder())
					{
						custombrowser.ResetState();
						custombrowser.customList[0]->SetState(STATE_SELECTED);
						custombrowser.TriggerUpdate();
						AdressText.SetTextf("%s%s", CFProot, browserCFP.dir);
					}
				}
				else
				{
					sprintf(customPath, "%s%s%s", CFProot, browserCFP.dir, browserCustomFilePath[browserCFP.selIndex].filename);
					result = 1;
					exit = true;
				}
			}
		}
		
		if(closeBtn.GetState()==STATE_CLICKED)
		{
			closeBtn.ResetState();
			exit = true;
		}
		
		else if(folderBtn.GetState()==STATE_CLICKED)
		{
			folderBtn.ResetState();
			
			customWindow.SetState(STATE_DISABLED);
			
			if(CreateCustomFilePath(customPath))
				result = 1;
			
			exit = true;
		}
		
		else if(sdBtn.GetState()==STATE_CLICKED)
		{
			sdBtn.ResetState();
			sdBtn.SetVisible(false);
			usbBtn.SetVisible(true);
			sdBtn.SetState(STATE_DISABLED);
			usbBtn.SetState(STATE_DEFAULT);
			
			firstCFP = true;
			
			ResetCustomFileBrowser();
			
			ParseCustomFile((char*)"usb:/");
			ParseCustomFileDirectory(CFProot);
			
			ParseCFP(browserCFP.dir);
			FileCustomChangeFolder();
			
			sprintf(customPath, "%s%s", CFProot, browserCFP.dir);
			AdressText.SetTextf(customPath);
			
			Device = 1;
			
			firstCFP = false;
			
		}
		else if(usbBtn.GetState()==STATE_CLICKED)
		{
			usbBtn.ResetState();
			usbBtn.SetVisible(false);
			sdBtn.SetVisible(true);
			usbBtn.SetState(STATE_DISABLED);
			sdBtn.SetState(STATE_DEFAULT);
			
			firstCFP = true;
			
			ResetCustomFileBrowser();
			
			ParseCustomFile((char*)"sd:/");
			ParseCustomFileDirectory(CFProot);
			
			ParseCFP(browserCFP.dir);
			FileCustomChangeFolder();
			
			sprintf(customPath, "%s%s", CFProot, browserCFP.dir);
			AdressText.SetTextf(customPath);
			
			Device = 0;
			
			firstCFP = false;
			
		}
	}
	
	MainWindow::Instance()->HaltGui();
	MainWindow::Instance()->Remove(&customWindow);
	MainWindow::Instance()->ResumeGui();
	
	firstCFP = true;
	
	return result;
}
