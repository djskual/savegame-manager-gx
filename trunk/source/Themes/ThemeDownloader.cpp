/****************************************************************************
 * Copyright (C) 2011
 * by Dimok
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any
 * damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any
 * purpose, including commercial applications, and to alter it and
 * redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you
 * must not claim that you wrote the original software. If you use
 * this software in a product, an acknowledgment in the product
 * documentation would be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and
 * must not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 ***************************************************************************/
#include "ThemeDownloader.h"
#include "ThemeInfosWindow.h"
#include "CTheme.h"
#include "../Prompts/PromptWindows.h"
#include "../Prompts/ProgressWindow.h"
#include "../Network/networkops.h"
#include "../Network/FileDownloader.h"
#include "../Network/http.h"
#include "../ArchiveOperations/ZipFile.h"
#include "../FileOperations/fileops.h"
#include "../Controls/Application.h"

ThemeDownloader::ThemeDownloader()
	: FlyingButtonsMenu()
{
	ThemeList = NULL;
	
	ThemeListURL = "http://wii.spiffy360.com/themes.php?xml=1&category=8&adult=";
	if(Settings.GodMode)
		ThemeListURL += "1";
	else
		ThemeListURL += "0";

	buttonImgData = Resources::GetImageData("theme_box.png");
	
	urlTxt = new GuiText(tr( "Themes by www.spiffy360.com" ), 22, (GXColor) {255, 255, 255, 255});
	urlTxt->SetAlignment(ALIGN_RIGHT | ALIGN_BOTTOM);
	urlTxt->SetPosition(-30, -17);
	Append(urlTxt);

	for(int i = 0; i < 4; ++i)
		ThemePreviews[i] = NULL;
	
	SetupMainButtons();
}

ThemeDownloader::~ThemeDownloader()
{
	Resources::Remove(buttonImgData);
	Remove(urlTxt);

	delete urlTxt;
	if(ThemeList)
		delete ThemeList;
	
	for(int i = 0; i < 4; ++i)
	{
		if(ThemePreviews[i])
			delete ThemePreviews[i];
	}
}

void ThemeDownloader::SetupMainButtons()
{
	if(!NetworkInitPrompt())
		return;
	
	StartThrobber(tr("Downloading pagelist:"), "www.spiffy360.com");
	
	if(!CheckConnection(ThemeListURL.c_str()))
	{
		StopProgress();
		ShowError(tr("Connection to server timed out."));
		return;
	}

	ThemeList = new Theme_List(ThemeListURL.c_str());

	if (!ThemeList->GetThemeCount())
	{
		StopProgress();
		ShowError(tr("No themes found on the site."));
		return;
	}

	for(int i = 0; i < ThemeList->GetThemeCount(); ++i)
	{
		SetMainButton(i, ThemeList->GetThemeTitle(i), buttonImgData, NULL);
	}
}

void ThemeDownloader::SetMainButton(int position, const char * ButtonText, GuiImageData * imageData, GuiImageData * themeImg)
{
	if(position < (int) mainButton.size())
	{
		delete mainButtonImg[position];
		delete mainButtonImgOver[position];
		delete mainButtonTxt[position];
		delete mainButton[position];
	}
	else
	{
		mainButtonImg.resize(position+1);
		mainButtonImgOver.resize(position+1);
		mainButtonTxt.resize(position+1);
		mainButton.resize(position+1);
	}

	mainButtonImg[position] = new GuiImage(imageData);
	mainButtonImgOver[position] = new GuiImage(themeImg);
	mainButtonImgOver[position]->SetScale(0.4);
	mainButtonImgOver[position]->SetAlignment(ALIGN_CENTER | ALIGN_TOP);
	mainButtonImgOver[position]->SetPosition(0, -45);
	
	mainButtonTxt[position] = new GuiText(ButtonText, 18, ( GXColor ) {0, 0, 0, 255});
	mainButtonTxt[position]->SetAlignment(ALIGN_CENTER | ALIGN_TOP);
	mainButtonTxt[position]->SetPosition(0, 10);
	mainButtonTxt[position]->SetMaxWidth(imageData->GetWidth() - 10, DOTTED);

	mainButton[position] = new GuiButton(imageData->GetWidth(), imageData->GetHeight());
	mainButton[position]->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	mainButton[position]->SetImage(mainButtonImg[position]);
	mainButton[position]->SetImageOver(mainButtonImg[position]);
	mainButton[position]->SetSoundOver(btnSoundOver);
	mainButton[position]->SetSoundClick(btnSoundClick);
	mainButton[position]->SetIcon(mainButtonImgOver[position]);
	mainButton[position]->SetLabel(mainButtonTxt[position]);
	mainButton[position]->SetTrigger(trigA);
	mainButton[position]->SetEffectGrow();
	mainButton[position]->Clicked.connect(this, &ThemeDownloader::OnButtonClick);
	mainButton[position]->StateChanged.connect(this, &ThemeDownloader::OnStateChange);
	
	switch(position % 4)
	{
		case 0:
			mainButton[position]->SetPosition(90, 75);
			break;
		case 1:
			mainButton[position]->SetPosition(340, 75);
			break;
		case 2:
			mainButton[position]->SetPosition(90, 230);
			break;
		case 3:
			mainButton[position]->SetPosition(340, 230);
			break;
		default:
			break;
	}
}

GuiImageData * ThemeDownloader::GetImageData(int theme)
{
	GuiImageData * ImageData = NULL;
	char filepath[300];
	snprintf(filepath, sizeof(filepath), "%s/tmp/%s.jpg", Settings.ThemePath, ThemeList->GetThemeTitle(theme));

	if (!CheckFile(filepath))
	{
		struct block file = downloadfile(ThemeList->GetImageLink(theme));
		char storepath[300];
		snprintf(storepath, sizeof(storepath), "%s/tmp/", Settings.ThemePath);
		CreateSubfolder(storepath);
		if (file.data)
		{
			ImageData = new GuiImageData(file.data, file.size);
			
			FILE *storefile = fopen(filepath, "wb");
			if(storefile)
			{
				fwrite(file.data, 1, file.size, storefile);
				fclose(storefile);
			}
			free(file.data);
		}
	}
	else
		ImageData = new GuiImageData(filepath);

	return ImageData;
}

void ThemeDownloader::AddMainButtons()
{
	for(u32 i = 0; i < mainButton.size(); ++i)
		Remove(mainButton[i]);
	
	int firstItem = currentPage*4;
	int n = 0;
	
	int btnCount = 4;
	if(mainButton.size()-firstItem < 4)
		btnCount = mainButton.size()-firstItem;
	
	StartProgress(tr("Downloading image:"));
	
	for(int i = firstItem; i < (int) mainButton.size() && i < firstItem+4; ++i)
	{
		ShowProgress(n, btnCount, ThemeList->GetThemeTitle(i));
		if(ThemePreviews[n])
			delete ThemePreviews[n];
		ThemePreviews[n] = GetImageData(i);
		mainButtonImgOver[i]->SetImage(ThemePreviews[n]);
		n++;
	}
	FinishProgress();
	StopProgress();
	
	FlyingButtonsMenu::AddMainButtons();
}

void ThemeDownloader::OnStateChange(GuiElement *sender, int state, int stateChan UNUSED)
{
	int FirstItem = currentPage*4;
	for(int i = FirstItem; i < (int) mainButton.size() && i < FirstItem+4; ++i)
	{
		if(sender == mainButton[i])
		{
			if(state == STATE_DEFAULT)
				mainButtonTxt[i]->SetPosition(0, 10);
			else
				mainButtonTxt[i]->SetPosition(0, 3);
			
			break;
		}
	}
}

void ThemeDownloader::OnButtonClick(GuiButton *sender, int pointer UNUSED, const POINT &p UNUSED)
{
	FlyingButtonsMenu::OnButtonClick(sender, pointer, p);
	
	for(u32 i = 0; i < mainButton.size(); ++i)
	{
		if(sender != mainButton[i])
			continue;
		
		ThemeInfosWindow * Infos = new ThemeInfosWindow(ThemeList->GetThemeTitle(i), ThemeList->GetThemeAuthor(i), 0);
		Infos->DimBackground(true);
		Application::Instance()->Append(Infos);
		Application::Instance()->SetUpdateOnly(Infos);
		
		int choice = -1;
		while((choice = Infos->GetChoice()) < 0)
			Application::Instance()->updateEvents();
		
		Infos->SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_OUT, 50);
		while(Infos->IsAnimated())
			Application::Instance()->updateEvents();
		
		Application::Instance()->PushForDelete(Infos);
		Application::Instance()->updateEvents();
		
		if(choice > 0)
		{
			if(WindowPrompt(tr( "Do you want to download this theme?" ), ThemeList->GetThemeTitle(i), tr( "Yes" ), tr( "Cancel" )))
				DownloadTheme(i);
		}
		
		break;
	}
}

void ThemeDownloader::DownloadTheme(int ind)
{
	const char *url = ThemeList->GetDownloadLink(ind);
	if (!url)
		return;
	
	if(!CreateSubfolder(Settings.ThemePath))
	{
		ShowError(tr("Can't create path: %s"), Settings.ThemePath);
		return;
	}

	char filepath[300];
	snprintf(filepath, sizeof(filepath), "%s/TempTheme.zip", Settings.ThemePath);
	
	int ret = DownloadFileToPath(url, filepath, false);
	if(ret < 1024)
	{
		ShowError(tr("Error when downloading file: %i"), ret);
		return;
	}
	
	ZipFile *zipfile = new ZipFile(filepath);

	int result = zipfile->ExtractAll(Settings.ThemePath, true);
	if(result < 0)
	{
		WindowPrompt(tr( "Failed to extract." ), tr( "Unsupported format, try to extract manually TempTheme.zip." ), tr( "OK" ));
		return;
	}

	std::string themeDir;
	std::string Filename;
	zipfile->FindFilePart(".them", Filename);
	zipfile->FindFilePart("/", themeDir);

	if(!zipfile->FindFile("theme_preview.png") && themeDir.size() != 0)
	{
		size_t pos = themeDir.find("/");
		if(pos != std::string::npos)
		{
			themeDir.erase(pos);
			char imgfilepath[255];
			char newfilepath[255];
			snprintf(imgfilepath, sizeof(imgfilepath), "%s/tmp/%s.jpg", Settings.ThemePath, ThemeList->GetThemeTitle(ind));
			snprintf(newfilepath, sizeof(newfilepath), "%s/%s/theme_preview.png", Settings.ThemePath, themeDir.c_str());
			CopyFile(imgfilepath, newfilepath);
		}
	}

	delete zipfile;
	RemoveFile(filepath);
	
	if (!WindowPrompt(tr( "Successfully extracted theme." ), tr( "Do you want to apply it now?" ), tr( "Yes" ), tr( "No" )))
		return;

	if(Filename.size() == 0)
	{
		WindowPrompt(tr( "ERROR: Can't set up theme." ), tr( "The .them file was not found in the zip." ), tr( "OK" ));
		return;
	}

	char real_themepath[1024];
	snprintf(real_themepath, sizeof(real_themepath), "%s/%s", Settings.ThemePath, Filename.c_str());
	
	/*if (Theme::Load(real_themepath))
	{
		snprintf(Settings.Theme, sizeof(Settings.Theme), real_themepath);
		Theme::Reload();
	}*/
}
