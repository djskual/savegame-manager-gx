/****************************************************************************
 * by Dj_Skual
 * for SaveGame Manager GX 2011
 *
 * menu.cpp
 ***************************************************************************/

#include <gccore.h>
#include <ogcsys.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <wiiuse/wpad.h>
#include <time.h>

#include "libwiigui/gui_browsers/gui_browsers.h"
#include "boot_homebrew/BootHomebrew.h"
#include "prompts/promptwindows.h"
#include "prompts/ProgressWindow.h"
#include "prompts/browsers/browsers.h"
#include "saves/manage/manage.h"
#include "saves/manage/ManageProgress.h"
#include "manage_all/manage_all.h"
#include "manage_all/ManageAllProgress.h"
#include "manage_all/ManageMiiProgress.h"
#include "saves/wii_list.h"
#include "file/fileops.h"
#include "settings/settingsprompts.h"
#include "network/networkops.h"
#include "text/TextViewer.h"
#include "menu/InfoWindow.h"
#include "menu/menu.h"
#include "menu/MainWindow.h"
#include "mii/mii_list.h"
#include "mii/mii_tools.h"
#include "mii/manage_mii/manage_mii.h"
#include "main.h"

extern FreeTypeGX *fontClock;
extern bool refreshMainMenu;

Settings Settings;

/****************************************************************************
 * SaveGameBrowser
 ***************************************************************************/
int SaveGameBrowser()
{
	int i;
	int dataed = -1;
    int oldsel = -1;
	refreshMainMenu = false;
	
	//Screensaver
    int check = 0; //to skip the first cycle when wiimote isn't completely connected

	//Clock
    struct tm * timeinfo;
    char theTime[80]="";
    time_t lastrawtime=0;

	ShutoffRumble();

	GuiImageData bgImgData(cfg.Widescreen? wbackground_png : background_png,
							cfg.Widescreen? wbackground_png_size : background_png_size);
	GuiImage bgImg(&bgImgData);
	MainWindow::Instance()->Append(&bgImg);
	
	int ret = GetPathToBrowse();
	if(ret == -1)
		return MENU_EXIT;
	else if(ret == 1)
		return MENU_SAVEGAME;
	
	int menu = MENU_NONE;
	
	GuiSound btnSoundOver(button_over_wav, button_over_wav_size, cfg.SFXVolume);
	GuiSound btnSoundClick(button_click_wav, button_click_wav_size, cfg.SFXVolume);
	
	SimpleGuiTrigger trigA(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);
	GuiTrigger trigB;
	trigB.SetButtonOnlyTrigger(-1, WPAD_BUTTON_B | WPAD_CLASSIC_BUTTON_B, PAD_BUTTON_B);
	GuiTrigger trigHome;
    trigHome.SetButtonOnlyTrigger(-1, WPAD_BUTTON_HOME | WPAD_CLASSIC_BUTTON_HOME, 0);

	GuiDeviceBrowser deviceBrowser(280, 276);
	deviceBrowser.SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	deviceBrowser.SetPosition(330, -70);
	deviceBrowser.SetState(STATE_DISABLED);
	
	GuiSaveBrowser saveBrowser(280, 276);
	saveBrowser.SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	saveBrowser.SetPosition(30, -70);
	saveBrowser.SetState(STATE_DISABLED);
	
	GuiMiiBrowser miiBrowser(280, 276);
	miiBrowser.SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	miiBrowser.SetPosition(30, -70);
	miiBrowser.SetState(STATE_DISABLED);
	
	InfoWindow InfoWindow(wiiSaveList[0].name);
	
	if(device.BrowsedDevice <= 2) {
		deviceBrowser.SetState(STATE_DEFAULT);
		InfoWindow.SetVisible(false);
	}
	else if(device.BrowsedDevice == 3){
		saveBrowser.SetState(STATE_DEFAULT);
		InfoWindow.SetVisible(cfg.InfoWindow ? true : false);
	}
	else if(device.BrowsedDevice == 4){
		miiBrowser.SetState(STATE_DEFAULT);
		InfoWindow.SetVisible(false);
	}
	
	GuiImageData btnAllManage(button_switch_png, button_switch_png_size);
	GuiImageData btnAllManageOver(button_switch_over_png, button_switch_over_png_size);
	GuiImage AllManageBtnImg(&btnAllManage);
	GuiImage AllManageBtnImgOver(&btnAllManageOver);
	
	GuiImageData settings(settings_button_png, settings_button_png_size);
	GuiImageData settingsOver(settings_button_over_png, settings_button_over_png_size);
	GuiImage settingsBtnImg(&settings);
	GuiImage settingsBtnImgOver(&settingsOver);
	
	GuiImageData shortcut(menu_button_png, menu_button_png_size);
	GuiImageData shortcutOver(menu_button_over_png, menu_button_over_png_size);
	GuiImage shortcutBtnImg(&shortcut);
	GuiImage shortcutBtnImgOver(&shortcutOver);
	
	GuiImageData home(button_home_png, button_home_png_size);
	GuiImageData homeOver(button_home_over_png, button_home_over_png_size);
	GuiImage homeBtnImg(&home);
	GuiImage homeBtnImgOver(&homeOver);
	
	GuiImageData tabImgData(tab_png, tab_png_size);
	
	if (cfg.Widescreen == 1)
	{
		AllManageBtnImg.SetWidescreen(true);
		AllManageBtnImgOver.SetWidescreen(true);
		settingsBtnImg.SetWidescreen(true);
		settingsBtnImgOver.SetWidescreen(true);
		shortcutBtnImg.SetWidescreen(true);
		shortcutBtnImgOver.SetWidescreen(true);
		homeBtnImg.SetWidescreen(true);
		homeBtnImgOver.SetWidescreen(true);
		
	}
	
	GuiTooltip AllManageBtnTT(tr("Manage All"));
    AllManageBtnTT.SetAlpha(255);
    GuiTooltip settingsBtnTT(tr("Settings"));
    settingsBtnTT.SetAlpha(255);
    GuiTooltip shortcutBtnTT(tr("Shortcuts"));
    shortcutBtnTT.SetAlpha(255);
    GuiTooltip homeBtnTT(tr("Home Menu"));
    homeBtnTT.SetAlpha(255);
	GuiTooltip wiiTabTT(tr("Browse Saves"));
    wiiTabTT.SetAlpha(255);
	GuiTooltip miiTabTT(tr("Browse Miis"));
    miiTabTT.SetAlpha(255);
	GuiTooltip sdTabTT(tr("Browse SD"));
    sdTabTT.SetAlpha(255);
	GuiTooltip usbTabTT(tr("Browse USB"));
    usbTabTT.SetAlpha(255);
	
	GuiButton AllManageBtn(btnAllManage.GetWidth(), btnAllManage.GetHeight());
	AllManageBtn.SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	AllManageBtn.SetPosition(16, 355);
	AllManageBtn.SetImage(&AllManageBtnImg);
	AllManageBtn.SetImageOver(&AllManageBtnImgOver);
	AllManageBtn.SetSoundOver(&btnSoundOver);
	AllManageBtn.SetSoundClick(&btnSoundClick);
	AllManageBtn.SetTrigger(&trigA);
	AllManageBtn.SetEffectGrow();
	
	GuiButton settingsBtn(settings.GetWidth(), settings.GetHeight());
	settingsBtn.SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	settingsBtn.SetPosition(64, 371);
	settingsBtn.SetImage(&settingsBtnImg);
	settingsBtn.SetImageOver(&settingsBtnImgOver);
	settingsBtn.SetSoundOver(&btnSoundOver);
	settingsBtn.SetSoundClick(&btnSoundClick);
	settingsBtn.SetTrigger(&trigA);
	settingsBtn.SetEffectGrow();
	
	GuiButton shortcutBtn(shortcut.GetWidth(), shortcut.GetHeight());
	shortcutBtn.SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	shortcutBtn.SetPosition(489, 371);
	shortcutBtn.SetImage(&shortcutBtnImg);
	shortcutBtn.SetImageOver(&shortcutBtnImgOver);
	shortcutBtn.SetSoundOver(&btnSoundOver);
	shortcutBtn.SetSoundClick(&btnSoundClick);
	shortcutBtn.SetTrigger(&trigA);
	shortcutBtn.SetEffectGrow();
	
	GuiButton homeBtn(home.GetWidth(), home.GetHeight());
	homeBtn.SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	homeBtn.SetPosition(576, 355);
	homeBtn.SetImage(&homeBtnImg);
	homeBtn.SetImageOver(&homeBtnImgOver);
	homeBtn.SetSoundOver(&btnSoundOver);
	homeBtn.SetSoundClick(&btnSoundClick);
	homeBtn.SetTrigger(&trigA);
	homeBtn.SetTrigger(&trigHome);
	homeBtn.SetEffectGrow();
	
	GuiImage wiiTabImg(&tabImgData);
	GuiImage miiTabImg(&tabImgData);
	GuiImage sdTabImg(&tabImgData);
	GuiImage usbTabImg(&tabImgData);
	
	GuiText wiiTxt(tr("Wii"), 20, (GXColor){0, 0, 0, 200});
	wiiTxt.SetMaxWidth(tabImgData.GetWidth()-10, GuiText::SCROLL);
	GuiText miiTxt(tr("Mii"), 20, (GXColor){0, 0, 0, 200});
	miiTxt.SetMaxWidth(tabImgData.GetWidth()-10, GuiText::SCROLL);
	GuiText sdTxt(tr("SD"), 20, (GXColor){0, 0, 0, 200});
	sdTxt.SetMaxWidth(tabImgData.GetWidth()-10, GuiText::SCROLL);
	GuiText usbTxt(tr("USB"), 20, (GXColor){0, 0, 0, 200});
	usbTxt.SetMaxWidth(tabImgData.GetWidth()-10, GuiText::SCROLL);
	
	GuiButton wiiTab(tabImgData.GetWidth(), tabImgData.GetHeight());
	wiiTab.SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	wiiTab.SetPosition(35, 304);
	wiiTab.SetImage(&wiiTabImg);
	wiiTab.SetLabel(&wiiTxt);
	wiiTab.SetSoundOver(&btnSoundOver);
	wiiTab.SetSoundClick(&btnSoundClick);
	wiiTab.SetTrigger(&trigA);
	
	GuiButton miiTab(tabImgData.GetWidth(), tabImgData.GetHeight());
	miiTab.SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	miiTab.SetPosition(95, 304);
	miiTab.SetImage(&miiTabImg);
	miiTab.SetLabel(&miiTxt);
	miiTab.SetSoundOver(&btnSoundOver);
	miiTab.SetSoundClick(&btnSoundClick);
	miiTab.SetTrigger(&trigA);
	
	GuiButton sdTab(tabImgData.GetWidth(), tabImgData.GetHeight());
	sdTab.SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	sdTab.SetPosition(335, 304);
	sdTab.SetImage(&sdTabImg);
	sdTab.SetLabel(&sdTxt);
	sdTab.SetSoundOver(&btnSoundOver);
	sdTab.SetSoundClick(&btnSoundClick);
	sdTab.SetTrigger(&trigA);
	
	GuiButton usbTab(tabImgData.GetWidth(), tabImgData.GetHeight());
	usbTab.SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	usbTab.SetPosition(395, 304);
	usbTab.SetImage(&usbTabImg);
	usbTab.SetLabel(&usbTxt);
	usbTab.SetSoundOver(&btnSoundOver);
	usbTab.SetSoundClick(&btnSoundClick);
	usbTab.SetTrigger(&trigA);
	
	if (cfg.Tooltips == 1)
	{
		AllManageBtn.SetToolTip(&AllManageBtnTT, 24, -30, 0, 5);
		settingsBtn.SetToolTip(&settingsBtnTT, 65, -30, 0, 5);
		shortcutBtn.SetToolTip(&shortcutBtnTT, -65, -30, 1, 5);
		homeBtn.SetToolTip(&homeBtnTT, -24, -30, 1, 5);
		wiiTab.SetToolTip(&wiiTabTT, -20, -30, 0, 3);
		miiTab.SetToolTip(&miiTabTT, -20, -30, 0, 3);
		sdTab.SetToolTip(&sdTabTT, 20, -30, 1, 3);
		usbTab.SetToolTip(&usbTabTT, 20, -30, 1, 3);
	}	
	
	GuiButton switchBtn(1,1);
	switchBtn.SetTrigger(&trigB);
	
	GuiWindow buttonWindow(screenwidth, screenheight);
	buttonWindow.Append(&AllManageBtn);
	buttonWindow.Append(&settingsBtn);
	buttonWindow.Append(&shortcutBtn);
	buttonWindow.Append(&homeBtn);
	buttonWindow.Append(&switchBtn);
	buttonWindow.Append(&wiiTab);
	buttonWindow.Append(&miiTab);
	if(SDCard_Inserted())
		buttonWindow.Append(&sdTab);
	if(USBDevice_Inserted())
		buttonWindow.Append(&usbTab);
	
	GuiText clockTimeBg("88:88", 40, (GXColor) {138, 138, 138, 40});
    clockTimeBg.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
    clockTimeBg.SetPosition(0, 340);
    clockTimeBg.SetFont(fontClock);
    GuiText clockTime(theTime, 40, (GXColor) {138, 138, 138, 240});
    clockTime.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
    clockTime.SetPosition(0, 340);
    clockTime.SetFont(fontClock);

	GuiWindow clock(screenwidth, screenheight);
	clock.Append(&clockTimeBg);
	clock.Append(&clockTime);
	
	MainWindow::Instance()->HaltGui();
	
	MainWindow::Instance()->Append(&deviceBrowser);
	if(device.BrowsedDevice == 3 || (device.LastBrowsedWii == 3 && device.BrowsedDevice < 3))
		MainWindow::Instance()->Append(&saveBrowser);
	if(device.BrowsedDevice == 4 || (device.LastBrowsedWii == 4 && device.BrowsedDevice < 3))
		MainWindow::Instance()->Append(&miiBrowser);
	MainWindow::Instance()->Append(&InfoWindow);
	MainWindow::Instance()->Append(&buttonWindow);
	if(cfg.Clock != 2)
		MainWindow::Instance()->Append(&clock);
	
	MainWindow::Instance()->ResumeGui();	
	
	while(menu == MENU_NONE)
	{
		usleep(THREAD_SLEEP);
		
		if(MainWindow::Instance()->GetBootHomebrew())
			menu = MENU_EXIT;
		
		//Clock
        time_t rawtime = time(0);
        if ((cfg.Clock != 2) && rawtime != lastrawtime) {
            lastrawtime = rawtime;
            timeinfo = localtime (&rawtime);
            if (dataed < 1) {
                if (cfg.Clock == 0) {
                    if (rawtime & 1)
                        strftime(theTime, sizeof(theTime), "%I:%M", timeinfo);
                    else
                        strftime(theTime, sizeof(theTime), "%I %M", timeinfo);
                }
                if (cfg.Clock == 1) {
                    if (rawtime & 1)
                        strftime(theTime, sizeof(theTime), "%H:%M", timeinfo);
                    else
                        strftime(theTime, sizeof(theTime), "%H %M", timeinfo);
                }
                clockTime.SetText(theTime);
            } else if (dataed > 0) {
                clockTime.SetTextf("%i", (dataed-1));
            }
        }
		
		if(refreshMainMenu)
		{
			menu = MENU_SAVEGAME;
		}
		
		if(device.BrowsedDevice < 3)
		{
			for(i=0; i < DEVICE_PAGESIZE; i++)
			{
				if(deviceBrowser.deviceList[i]->GetState() == STATE_CLICKED)
				{
					deviceBrowser.deviceList[i]->ResetState();
					if(browserDevice[browserD.selIndex].isdir)
					{
						if(BrowserDeviceChangeFolder())
						{
							deviceBrowser.ResetState();
							deviceBrowser.deviceList[0]->SetState(STATE_SELECTED);
							deviceBrowser.TriggerUpdate();
						}
						else
						{
							menu = MENU_SAVEGAME;
							break;
						}
					}
					else if(browserDevice[browserD.selIndex].isSave)
					{
						Manage(browserD.selIndex,
							   ManageWindow::DEVICE);
						
						menu = MENU_SAVEGAME;
						
					}
					else if(browserDevice[browserD.selIndex].isLog)
					{
						ShutoffRumble();
						MainWindow::Instance()->SetState(STATE_DISABLED);
						
						char logPath[200];
						snprintf(logPath, sizeof(logPath), "%s%s", navinfo.LastBrowsedPath, browserDevice[browserD.selIndex].filename);
						
						TextViewer(logPath);
						
						menu = MENU_SAVEGAME;
					}
					else if(browserDevice[browserD.selIndex].isMii)
					{
						ManageMii(browserD.selIndex,
								  ManageWindow::DEVICE);
						
						menu = MENU_SAVEGAME;
					}
				}
			}
		}
		else if(device.BrowsedDevice == 3)
		{
			for(i=0; i < SAVE_PAGESIZE && i<wiiSaveInfo.Cnt; i++)
			{
				if(saveBrowser.saveBtn[i]->GetState() == STATE_SELECTED && cfg.InfoWindow && oldsel != wiiSaveInfo.selIndex)
				{
					oldsel = wiiSaveInfo.selIndex;
					InfoWindow.SetName(wiiSaveList[wiiSaveInfo.selIndex].name);
				}
				
				if(saveBrowser.saveBtn[i]->GetState() == STATE_CLICKED)
				{
					saveBrowser.saveBtn[i]->ResetState();
					
					Manage(wiiSaveInfo.selIndex,
						   ManageWindow::WII);
						
					menu = MENU_SAVEGAME;
				}
			}
		}
		else if(device.BrowsedDevice == 4)
		{
			for(i=0; i < MII_PAGESIZE; i++)
			{
				if(miiBrowser.miiBtn[i]->GetState() == STATE_CLICKED)
				{
					ManageMii(miiListInfo.selIndex,
							  ManageWindow::WII);
					
					menu = MENU_SAVEGAME;
				}
			}
		}
	
		if(homeBtn.GetState() == STATE_CLICKED)
		{
			homeBtn.ResetState();
			MainWindow::Instance()->SetState(STATE_DISABLED);
			ShowHomeMenu();
			MainWindow::Instance()->SetState(STATE_DEFAULT);
			menu = MENU_SAVEGAME;
		}
		
		if (shortcutBtn.GetState() == STATE_CLICKED)
		{
			ShowShortcuts();
			
			shortcutBtn.ResetState();
			menu = MENU_SAVEGAME;
        }
		
		if (wiiTab.GetState() == STATE_SELECTED || device.BrowsedDevice == 3)
			wiiTxt.SetColor((GXColor) {0, 0, 0, 255});
		else
			wiiTxt.SetColor((GXColor) {0, 0, 0, 180});
		if (miiTab.GetState() == STATE_SELECTED || device.BrowsedDevice == 4)
			miiTxt.SetColor((GXColor) {0, 0, 0, 255});
		else
			miiTxt.SetColor((GXColor) {0, 0, 0, 180});
		if (sdTab.GetState() == STATE_SELECTED || device.BrowsedDevice == 1)
			sdTxt.SetColor((GXColor) {0, 0, 0, 255});
		else
			sdTxt.SetColor((GXColor) {0, 0, 0, 180});
		if (usbTab.GetState() == STATE_SELECTED || device.BrowsedDevice == 2)
			usbTxt.SetColor((GXColor) {0, 0, 0, 255});
		else
			usbTxt.SetColor((GXColor) {0, 0, 0, 180});
		
		if (switchBtn.GetState() == STATE_CLICKED)
		{
			switchBtn.ResetState();
			
			if (device.BrowsedDevice <= 2) {
				device.LastBrowsedDevice = device.BrowsedDevice;
				device.BrowsedDevice = device.LastBrowsedWii;
			}
			else if (device.BrowsedDevice > 2) {
				device.LastBrowsedWii = device.BrowsedDevice;
				device.BrowsedDevice = device.LastBrowsedDevice;
			}
			
			menu = MENU_SAVEGAME;
		}
		
		if (wiiTab.GetState() == STATE_CLICKED)
		{
			wiiTab.ResetState();
			
			if (device.BrowsedDevice != 3) {
				if(device.BrowsedDevice == 4)
					device.LastBrowsedWii = device.BrowsedDevice;
				else
					device.LastBrowsedDevice = device.BrowsedDevice;
					
				if (device.BrowsedDevice == 1)
					snprintf(navinfo.LastBrowsedSdPath, sizeof(navinfo.LastBrowsedSdPath), "%s%s", navinfo.CurrentDevice, browserD.dir);
				if (device.BrowsedDevice == 2)
					snprintf(navinfo.LastBrowsedUsbPath, sizeof(navinfo.LastBrowsedUsbPath), "%s%s", navinfo.CurrentDevice, browserD.dir);
				
				device.BrowsedDevice = 3;
				
				menu = MENU_SAVEGAME;
			}
		}
		else if (miiTab.GetState() == STATE_CLICKED)
		{
			miiTab.ResetState();
			
			if (device.BrowsedDevice != 4) {
				if(device.BrowsedDevice == 3)
					device.LastBrowsedWii = device.BrowsedDevice;
				else
					device.LastBrowsedDevice = device.BrowsedDevice;
					
				if (device.BrowsedDevice == 1)
					snprintf(navinfo.LastBrowsedSdPath, sizeof(navinfo.LastBrowsedSdPath), "%s%s", navinfo.CurrentDevice, browserD.dir);
				if (device.BrowsedDevice == 2)
					snprintf(navinfo.LastBrowsedUsbPath, sizeof(navinfo.LastBrowsedUsbPath), "%s%s", navinfo.CurrentDevice, browserD.dir);
				
				device.BrowsedDevice = 4;
				
				menu = MENU_SAVEGAME;
			}
		}
		
		else if (sdTab.GetState() == STATE_CLICKED)
		{
			sdTab.ResetState();
			
			if (device.BrowsedDevice != 1) {
				if(device.BrowsedDevice >= 3)
					device.LastBrowsedWii = device.BrowsedDevice;
				else
					device.LastBrowsedDevice = device.BrowsedDevice;
					
				if (device.BrowsedDevice == 2)
					snprintf(navinfo.LastBrowsedUsbPath, sizeof(navinfo.LastBrowsedUsbPath), "%s%s", navinfo.CurrentDevice, browserD.dir);
				device.BrowsedDevice = 1;
				navinfo.switchdevice = true;
				
				menu = MENU_SAVEGAME;
			}
		}
		else if (usbTab.GetState() == STATE_CLICKED)
		{
			usbTab.ResetState();
			
			if (device.BrowsedDevice != 2) {
				if(device.BrowsedDevice >= 3)
					device.LastBrowsedWii = device.BrowsedDevice;
				else
					device.LastBrowsedDevice = device.BrowsedDevice;
					
				if (device.BrowsedDevice == 1)
					snprintf(navinfo.LastBrowsedSdPath, sizeof(navinfo.LastBrowsedSdPath), "%s%s", navinfo.CurrentDevice, browserD.dir);
				device.BrowsedDevice = 2;
				navinfo.switchdevice = true;
				
				menu = MENU_SAVEGAME;
			}
		}
		
		else if(AllManageBtn.GetState() == STATE_CLICKED)
		{
			AllManageBtn.ResetState();
			
			ManageAll();
			
			navinfo.refresh = true;
			menu = MENU_SAVEGAME;
			
		}
		
		else if(settingsBtn.GetState() == STATE_CLICKED)
		{
			menu = MENU_SETTINGS;
			settingsBtn.ResetState();
		}
		
		// to skip the first call of windowScreensaver at startup when wiimote is not connected
        if (IsWpadConnected()) {
            check = 1;
        }

        // screensaver is called when wiimote shuts down, depending on the wiimotet idletime
        if (!IsWpadConnected() && check !=0 && cfg.Screensaver!=0) {
            check++;
            int screensaverIsOn=0;
            if (check==11500) { //to allow time for the wii to turn off and not show the screensaver
                screensaverIsOn=WindowScreensaver();
            }
            if (screensaverIsOn==1)check=0;
        }
	}
	MainWindow::Instance()->HaltGui();
	MainWindow::Instance()->Remove(&InfoWindow);
	MainWindow::Instance()->Remove(&buttonWindow);
	MainWindow::Instance()->Remove(&deviceBrowser);
	MainWindow::Instance()->Remove(&saveBrowser);
	MainWindow::Instance()->Remove(&miiBrowser);
	MainWindow::Instance()->Remove(&clock);
	return menu;
}
