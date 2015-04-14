/****************************************************************************
 * libwiigui Template
 * Tantric 2009
 *
 * devicebrowser.cpp
 *
 * Generic file routines - reading, writing, browsing
 ***************************************************************************/

#include <gccore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiiuse/wpad.h>
#include <sys/dir.h>
#include <malloc.h>

#include "libwiigui/gui_browsers/gui_browsers.h"
#include "prompts/browsers/customPathBrowser.h"
#include "prompts/promptwindows.h"
#include "saves/savegame.h"
#include "file/fileops.h"
#include "settings/device.h"
#include "menu/MainWindow.h"
#include "main.h"

BROWSERCUSTOMPATHINFO browserCP;
BROWSERCUSTOMPATHENTRY * browserCustomPath = NULL; // list of files/folders in browser

char customroot[10];
bool firstCustom = true;

/****************************************************************************
 * ResetCustomBrowser()
 * Clears the file browser memory, and allocates one initial entry
 ***************************************************************************/
void ResetCustomBrowser()
{
	browserCP.numEntries = 0;
	browserCP.selIndex = 0;
	browserCP.pageIndex = 0;

	// Clear any existing values
	if(browserCustomPath != NULL)
	{
		free(browserCustomPath);
		browserCustomPath = NULL;
	}
	// set aside space for 1 entry
	browserCustomPath = (BROWSERCUSTOMPATHENTRY *)malloc(sizeof(BROWSERCUSTOMPATHENTRY));
	memset(browserCustomPath, 0, sizeof(BROWSERCUSTOMPATHENTRY));
}

/****************************************************************************
 * UpdateCustomDirName()
 * Update curent directory name for file browser
 ***************************************************************************/
int UpdateCustomDirName()
{
	int size=0;
	char * test;
	char temp[1024];

	/* current directory doesn't change */
	if (strcmp(browserCustomPath[browserCP.selIndex].filename,".") == 0)
	{
		return 0;
	}
	/* go up to parent directory */
	else if (strcmp(browserCustomPath[browserCP.selIndex].filename,"..") == 0)
	{
		/* determine last subdirectory namelength */
		sprintf(temp,"%s",browserCP.dir);
		test = strtok(temp,"/");
		while (test != NULL)
		{
			size = strlen(test);
			test = strtok(NULL,"/");
		}

		/* remove last subdirectory name */
		size = strlen(browserCP.dir) - size - 1;
		browserCP.dir[size] = 0;

		return 1;
	}
	/* Open a directory */
	else
	{
		/* test new directory namelength */
		if ((strlen(browserCP.dir)+1+strlen(browserCustomPath[browserCP.selIndex].filename)) < MAXPATHLEN)
		{
			/* update current directory name */
			sprintf(browserCP.dir, "%s%s/",browserCP.dir, browserCustomPath[browserCP.selIndex].filename);
			return 1;
		}
		else
		{
			return -1;
		}
	}
}

/****************************************************************************
 * CustomSortCallback
 *
 * Quick sort callback to sort file entries with the following order:
 *   .
 *   ..
 *   <dirs>
 *   <files>
 ***************************************************************************/
int CustomSortCallback(const void *f1, const void *f2)
{
	/* Special case for implicit directories */
	if(((BROWSERCUSTOMPATHENTRY *)f1)->filename[0] == '.' || ((BROWSERCUSTOMPATHENTRY *)f2)->filename[0] == '.')
	{
		if(strcmp(((BROWSERCUSTOMPATHENTRY *)f1)->filename, ".") == 0) { return -1; }
		if(strcmp(((BROWSERCUSTOMPATHENTRY *)f2)->filename, ".") == 0) { return 1; }
		if(strcmp(((BROWSERCUSTOMPATHENTRY *)f1)->filename, "..") == 0) { return -1; }
		if(strcmp(((BROWSERCUSTOMPATHENTRY *)f2)->filename, "..") == 0) { return 1; }
	}

	/* If one is a file and one is a directory the directory is first. */
	if(((BROWSERCUSTOMPATHENTRY *)f1)->isdir && !(((BROWSERCUSTOMPATHENTRY *)f2)->isdir)) return -1;
	if(!(((BROWSERCUSTOMPATHENTRY *)f1)->isdir) && ((BROWSERCUSTOMPATHENTRY *)f2)->isdir) return 1;

	/* Sort files*/
	return stricmp(((BROWSERCUSTOMPATHENTRY *)f1)->filename, ((BROWSERCUSTOMPATHENTRY *)f2)->filename);
}

/***************************************************************************
 * Browse subdirectories
 **************************************************************************/
int ParseCustomDirectory(const char * Path)
{
	DIR_ITER *dir = NULL;
	char fulldir[MAXPATHLEN];
	char filename[MAXPATHLEN];
	struct stat filestat;
	
	// reset browser
	ResetCustomBrowser();
	
	// open the directory
	dir = diropen(Path);
	if (dir == NULL)
	{
		sprintf(fulldir, "%s%s", customroot, browserCP.dir); // add currentDevice to path
		dir = diropen(fulldir);
	}

	// if we can't open the dir, try opening the root dir
	if (dir == NULL)
	{
		sprintf(browserCP.dir,"/");
		dir = diropen(customroot);
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
			BROWSERCUSTOMPATHENTRY * newBrowserCustomPath = (BROWSERCUSTOMPATHENTRY *)realloc(browserCustomPath, (entryNum+1) * sizeof(BROWSERCUSTOMPATHENTRY));

			if(!newBrowserCustomPath) // failed to allocate required memory
			{
				ResetCustomBrowser();
				entryNum = -1;
				break;
			}
			else
			{
				browserCustomPath = newBrowserCustomPath;
			}
			memset(&(browserCustomPath[entryNum]), 0, sizeof(BROWSERCUSTOMPATHENTRY)); // clear the new entry

			strncpy(browserCustomPath[entryNum].filename, filename, MAXJOLIET);

			if(strcmp(filename,"..") == 0)
			{
				sprintf(browserCustomPath[entryNum].displayname, "..");
			}
			else
			{
				strncpy(browserCustomPath[entryNum].displayname, filename, MAXDISPLAY);	// crop name for display
			}

			browserCustomPath[entryNum].length = filestat.st_size;
			browserCustomPath[entryNum].isdir = (filestat.st_mode & _IFDIR) == 0 ? 0 : 1; // flag this as a dir
			
			if(browserCustomPath[entryNum].isdir)
			{
				char savepath[128];
				sprintf(savepath, "%s%s", navinfo.LastBrowsedPath, browserCustomPath[entryNum].filename);
				
				int ret = Savegame_CheckTitle(savepath, ManageWindow::DEVICE);
				if(!ret)
					browserCustomPath[entryNum].isdir = 0;
			}
			
			if(browserCustomPath[entryNum].isdir)
				entryNum++;
		}
	}

	// close directory
	dirclose(dir);

	// Sort the file list
	qsort(browserCustomPath, entryNum, sizeof(BROWSERCUSTOMPATHENTRY), CustomSortCallback);

	browserCP.numEntries = entryNum;
	return entryNum;
}

/****************************************************************************
 * BrowserCustomChangeFolder
 *
 * Update current directory and set new entry list if directory has changed
 ***************************************************************************/
int BrowserCustomChangeFolder()
{
	if(firstCustom == false)
	{
		if(!UpdateCustomDirName())
			return -1;
	}

	char fullpath[200];
	snprintf(fullpath, sizeof(fullpath), "%s%s", customroot, browserCP.dir);
	ParseCustomDirectory(fullpath);
	
	return browserCP.numEntries;
}

void ParseCustom(char *Path)
{
    char temp[200], name[200], value[200];

    memcpy (temp,Path,strlen(Path)+1);

    char * eq = strchr(temp, '/');

    if(!eq) return;

    *eq = 0;

    TrimCustom(name, temp, sizeof(name));
    TrimCustom(value, eq+1, sizeof(value));

	sprintf(customroot, "%s/", name);
	sprintf(browserCP.dir, "%s", value);
	
	return;
}

void ParseCustomPath(char *Path)
{
    char temp[200], name[200];

    memcpy (temp,Path,strlen(Path)+1);

    char * eq = strrchr(temp, '/');

    if(!eq) return;

    *eq = 0;

    TrimCustom(name, temp, sizeof(name));
    
	sprintf(browserCP.dir, "%s/", name);	
	
	return;
}

void TrimCustom(char *dest, char *src, int size)
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
 * CreateCustomPath
 * Displays an onscreen Keyboard to create new path
 ***************************************************************************/
int CreateCustomPath(char * customPath)
{
	MainWindow::Instance()->HaltGui();
	GuiImage disabledImg(screenwidth, screenheight, (GXColor){0, 0, 0, 0x70});
	MainWindow::Instance()->Append(&disabledImg);
	MainWindow::Instance()->ResumeGui();
	
	int result = OnScreenKeyboard(customPath, 149, 1);
	if(result == 1)
	{
		if (customPath[strlen(customPath)-1] != '/')
			strcat(customPath, "/");
		
		if (CreateSubfolder(customPath) == false)
			result = 0;
	}
	
	MainWindow::Instance()->HaltGui();
	MainWindow::Instance()->Remove(&disabledImg);
	MainWindow::Instance()->ResumeGui();
	
	return result;
}

/****************************************************************************
 * FoundCustomDevice
 * found the browsed device in customPAthBrowser
 ***************************************************************************/
int FoundCustomDevice()
{
	if(!strncmp(customroot, "sd:/", 4))
		return 0;
	else
		return 1;

}

/****************************************************************************
 * BrowseCustom
 * Displays a list of files on the selected device
 ***************************************************************************/
int BrowseCustom(char * Path, char * customPath)
{
	ResetCustomBrowser();
	
	ParseCustom(Path);
	ParseCustomDirectory(customroot);
	
	ParseCustomPath(browserCP.dir);
	BrowserCustomChangeFolder();

	sprintf(customPath, "%s%s", customroot, browserCP.dir);
	
	int Device = FoundCustomDevice();
	
	firstCustom = false;
	
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
	
	GuiImageData validImgData(valid_png, valid_png_size);
	GuiImageData validOverImgData(valid_over_png, valid_over_png_size);
	GuiImage validImg(&validImgData);
	GuiImage validOverImg(&validOverImgData);
	GuiButton validBtn(validImg.GetWidth(), validImg.GetHeight());
	validBtn.SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	validBtn.SetPosition(138, -159);
	validBtn.SetImage(&validImg);
	validBtn.SetImageOver(&validOverImg);
	validBtn.SetSoundOver(&btnSoundOver);
	validBtn.SetSoundClick(&btnSoundClick);
	validBtn.SetTrigger(&trigA);
	
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
	AdressText.SetTextf("%s%s", customroot, browserCP.dir);
	AdressText.SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	if(device.DeviceCount >= 2) {
		AdressText.SetPosition(185, -165);
		AdressText.SetMaxWidth(240, GuiText::SCROLL);
	}
	else {
		AdressText.SetPosition(168, -165);
		AdressText.SetMaxWidth(255, GuiText::SCROLL);
	}
	
	GuiCustomPathBrowser custombrowser(338, 276);
	custombrowser.SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	custombrowser.SetPosition(-19, 0);
	
	MainWindow::Instance()->HaltGui();
	
	GuiWindow customWindow(screenwidth, screenheight);
	customWindow.Append(&disabledImg);
	customWindow.Append(&browserImg);
	customWindow.Append(&custombrowser);
	customWindow.Append(&AdressText);
	customWindow.Append(&folderBtn);
	customWindow.Append(&validBtn);
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
				if(browserCustomPath[browserCP.selIndex].isdir)
				{
					if(BrowserCustomChangeFolder())
					{
						custombrowser.ResetState();
						custombrowser.customList[0]->SetState(STATE_SELECTED);
						custombrowser.TriggerUpdate();
						AdressText.SetTextf("%s%s", customroot, browserCP.dir);
						sprintf(customPath, "%s%s", customroot, browserCP.dir);
					}
				}
			}
		}
		
		if(closeBtn.GetState()==STATE_CLICKED)
		{
			closeBtn.ResetState();
			exit = true;
		}
		
		else if(validBtn.GetState()==STATE_CLICKED)
		{
			validBtn.ResetState();
			result = 1;
			exit = true;
		}
		
		else if(folderBtn.GetState()==STATE_CLICKED)
		{
			folderBtn.ResetState();
			
			customWindow.SetState(STATE_DISABLED);
			
			if(CreateCustomPath(customPath))
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
			
			firstCustom = true;
			
			ResetCustomBrowser();
			
			ParseCustom((char*)"usb:/");
			ParseCustomDirectory(customroot);
			
			ParseCustomPath(browserCP.dir);
			BrowserCustomChangeFolder();
			
			sprintf(customPath, "%s%s", customroot, browserCP.dir);
			AdressText.SetTextf(customPath);
			
			Device = 1;
			
			firstCustom = false;
			
		}
		else if(usbBtn.GetState()==STATE_CLICKED)
		{
			usbBtn.ResetState();
			usbBtn.SetVisible(false);
			sdBtn.SetVisible(true);
			usbBtn.SetState(STATE_DISABLED);
			sdBtn.SetState(STATE_DEFAULT);
			
			firstCustom = true;
			
			ResetCustomBrowser();
			
			ParseCustom((char*)"sd:/");
			ParseCustomDirectory(customroot);
			
			ParseCustomPath(browserCP.dir);
			BrowserCustomChangeFolder();
			
			sprintf(customPath, "%s%s", customroot, browserCP.dir);
			AdressText.SetTextf(customPath);
			
			Device = 0;
			
			firstCustom = false;
			
		}
	}
	
	MainWindow::Instance()->HaltGui();
	MainWindow::Instance()->Remove(&customWindow);
	MainWindow::Instance()->ResumeGui();
	
	firstCustom = true;
	
	return result;
}
