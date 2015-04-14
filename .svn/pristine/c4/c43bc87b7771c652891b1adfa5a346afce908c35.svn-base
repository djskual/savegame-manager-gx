/****************************************************************************
 * libwiigui
 *
 * Tantric 2009
 *
 * gui_devicebrowser.cpp
 *
 * GUI class definitions
 ***************************************************************************/

#include "libwiigui/gui_browsers/gui_browsers.h"
#include "prompts/browsers/browsers.h"
#include "main.h"

/**
 * Constructor for the GuiDeviceBrowser class.
 */
GuiDeviceBrowser::GuiDeviceBrowser(int w, int h)
{
	if(navinfo.LastDeviceItemSelected > browserD.numEntries-1)
		navinfo.LastDeviceItemSelected = browserD.numEntries-1;
	
	width = w;
	height = h;
	numEntries = 0;
	selectedItem = navinfo.LastDeviceItemSelected;
	selectable = true;
	listChanged = true; // trigger an initial list update
	focus = 0; // allow focus
	
	browserD.pageIndex = navinfo.LastDeviceFirstItem;

	trigA = new SimpleGuiTrigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);

	trigHeldA = new GuiTrigger;
	trigHeldA->SetHeldTrigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);

	btnSoundOver = Resources::GetSound(button_over_wav, button_over_wav_size, cfg.SFXVolume);
	btnSoundClick = Resources::GetSound(button_click_wav, button_click_wav_size, cfg.SFXVolume);
	
	bgDeviceSelection = Resources::GetImageData(bg_options_png, bg_options_png_size);
	bgDeviceSelectionImg = new GuiImage(bgDeviceSelection);
	bgDeviceSelectionImg->SetParent(this);
	bgDeviceSelectionImg->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);

	bgDeviceSelectionEntry = Resources::GetImageData(bg_device_options_entry_png, bg_device_options_entry_png_size);
	deviceFolder = Resources::GetImageData(icon_folder_png, icon_folder_png_size);
	deviceLog = Resources::GetImageData(icon_txt_png, icon_txt_png_size);
	deviceSave = Resources::GetImageData(icon_save_png, icon_save_png_size);
	deviceNoSave = Resources::GetImageData(icon_nosave_png, icon_nosave_png_size);
	deviceMii = Resources::GetImageData(icon_mii_png, icon_mii_png_size);

	scrollbar = Resources::GetImageData(scrollbar_png, scrollbar_png_size);
	scrollbarImg = new GuiImage(scrollbar);
	scrollbarImg->SetParent(this);
	scrollbarImg->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	scrollbarImg->SetPosition(0, 4);

	arrowDown = Resources::GetImageData(scrollbar_arrowdown_png, scrollbar_arrowdown_png_size);
	arrowDownImg = new GuiImage(arrowDown);
	arrowDownOver = Resources::GetImageData(scrollbar_arrowdown_png, scrollbar_arrowdown_png_size);
	arrowDownOverImg = new GuiImage(arrowDownOver);
	arrowUp = Resources::GetImageData(scrollbar_arrowup_png, scrollbar_arrowup_png_size);
	arrowUpImg = new GuiImage(arrowUp);
	arrowUpOver = Resources::GetImageData(scrollbar_arrowup_png, scrollbar_arrowup_png_size);
	arrowUpOverImg = new GuiImage(arrowUpOver);
	scrollbarBox = Resources::GetImageData(scrollbar_box_png, scrollbar_box_png_size);
	scrollbarBoxImg = new GuiImage(scrollbarBox);
	scrollbarBoxOver = Resources::GetImageData(scrollbar_box_png, scrollbar_box_png_size);
	scrollbarBoxOverImg = new GuiImage(scrollbarBoxOver);

	arrowUpBtn = new GuiButton(arrowUpImg->GetWidth(), arrowUpImg->GetHeight());
	arrowUpBtn->SetParent(this);
	arrowUpBtn->SetImage(arrowUpImg);
	arrowUpBtn->SetImageOver(arrowUpOverImg);
	arrowUpBtn->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	arrowUpBtn->SetPosition(11, -18);
	arrowUpBtn->SetSelectable(false);
	arrowUpBtn->SetClickable(false);
	arrowUpBtn->SetHoldable(true);
	arrowUpBtn->SetTrigger(trigHeldA);
	arrowUpBtn->SetSoundOver(btnSoundOver);
	arrowUpBtn->SetSoundClick(btnSoundClick);
	arrowUpBtn->SetEffectOnOver(EFFECT_SCALE, 50, 130);

	arrowDownBtn = new GuiButton(arrowDownImg->GetWidth(), arrowDownImg->GetHeight());
	arrowDownBtn->SetParent(this);
	arrowDownBtn->SetImage(arrowDownImg);
	arrowDownBtn->SetImageOver(arrowDownOverImg);
	arrowDownBtn->SetAlignment(ALIGN_RIGHT, ALIGN_BOTTOM);
	arrowDownBtn->SetPosition(12, 18);
	arrowDownBtn->SetSelectable(false);
	arrowDownBtn->SetClickable(false);
	arrowDownBtn->SetHoldable(true);
	arrowDownBtn->SetTrigger(trigHeldA);
	arrowDownBtn->SetSoundOver(btnSoundOver);
	arrowDownBtn->SetSoundClick(btnSoundClick);
	arrowDownBtn->SetEffectOnOver(EFFECT_SCALE, 50, 130);

	scrollbarBoxBtn = new GuiButton(scrollbarBoxImg->GetWidth(), scrollbarBoxImg->GetHeight());
	scrollbarBoxBtn->SetParent(this);
	scrollbarBoxBtn->SetImage(scrollbarBoxImg);
	scrollbarBoxBtn->SetImageOver(scrollbarBoxOverImg);
	scrollbarBoxBtn->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	scrollbarBoxBtn->SetMinY(0);
	scrollbarBoxBtn->SetMaxY(212);
	scrollbarBoxBtn->SetSelectable(false);
	scrollbarBoxBtn->SetClickable(false);
	scrollbarBoxBtn->SetHoldable(true);
	scrollbarBoxBtn->SetTrigger(trigHeldA);
	scrollbarBoxBtn->SetEffectOnOver(EFFECT_SCALE, 50, 120);


	for(int i=0; i<DEVICE_PAGESIZE; i++)
	{
		deviceListText[i] = new GuiText(NULL, 20, (GXColor){0, 0, 0, 0xff});
		deviceListText[i]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
		deviceListText[i]->SetPosition(5,0);
		deviceListText[i]->SetMaxWidth(width-28-25, 2);

		deviceListBg[i] = new GuiImage(bgDeviceSelectionEntry);
		
		deviceListSave[i] = new GuiImage(deviceSave);
		deviceListSave[i]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
		deviceListSave[i]->SetPosition(22,0);

		deviceListNoSave[i] = new GuiImage(deviceNoSave);
		deviceListNoSave[i]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
		deviceListNoSave[i]->SetPosition(22,0);

		deviceListFolder[i] = new GuiImage(deviceFolder);
		deviceListFolder[i]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
		deviceListFolder[i]->SetPosition(18,0);
		
		deviceListLog[i] = new GuiImage(deviceLog);
		deviceListLog[i]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
		deviceListLog[i]->SetPosition(22,0);
		
		deviceListMii[i] = new GuiImage(deviceMii);
		deviceListMii[i]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
		deviceListMii[i]->SetPosition(22,0);

		deviceList[i] = new GuiButton(width-28,30);
		deviceList[i]->SetParent(this);
		deviceList[i]->SetLabel(deviceListText[i]);
		deviceList[i]->SetImageOver(deviceListBg[i]);
		deviceList[i]->SetPosition(3,30*i+4);
		deviceList[i]->SetTrigger(trigA);
		deviceList[i]->SetSoundClick(btnSoundClick);
	}
}

/**
 * Destructor for the GuiFileBrowser class.
 */
GuiDeviceBrowser::~GuiDeviceBrowser()
{
	delete arrowUpBtn;
	delete arrowDownBtn;
	delete scrollbarBoxBtn;

	delete bgDeviceSelectionImg;
	delete scrollbarImg;
	delete arrowDownImg;
	delete arrowDownOverImg;
	delete arrowUpImg;
	delete arrowUpOverImg;
	delete scrollbarBoxImg;
	delete scrollbarBoxOverImg;

	Resources::Remove(bgDeviceSelection);
	Resources::Remove(bgDeviceSelectionEntry);
	Resources::Remove(deviceFolder);
	Resources::Remove(deviceLog);
	Resources::Remove(deviceSave);
	Resources::Remove(deviceNoSave);
	Resources::Remove(deviceMii);
	Resources::Remove(scrollbar);
	Resources::Remove(arrowDown);
	Resources::Remove(arrowDownOver);
	Resources::Remove(arrowUp);
	Resources::Remove(arrowUpOver);
	Resources::Remove(scrollbarBox);
	Resources::Remove(scrollbarBoxOver);

	Resources::Remove(btnSoundOver);
	Resources::Remove(btnSoundClick);
	delete trigHeldA;
	delete trigA;

	for(int i=0; i<DEVICE_PAGESIZE; i++)
	{
		delete deviceListText[i];
		delete deviceList[i];
		delete deviceListBg[i];
		delete deviceListFolder[i];
		delete deviceListLog[i];
		delete deviceListSave[i];
		delete deviceListNoSave[i];
		delete deviceListMii[i];
	}
}

void GuiDeviceBrowser::SetFocus(int f)
{
	focus = f;

	for(int i=0; i<DEVICE_PAGESIZE; i++)
		deviceList[i]->ResetState();

	if(f == 1)
		deviceList[selectedItem]->SetState(STATE_SELECTED);
}

void GuiDeviceBrowser::ResetState()
{
	state = STATE_DEFAULT;
	stateChan = -1;
	selectedItem = 0;

	for(int i=0; i<DEVICE_PAGESIZE; i++)
	{
		deviceList[i]->ResetState();
	}
}

void GuiDeviceBrowser::TriggerUpdate()
{
	listChanged = true;
}

/**
 * Draw the button on screen
 */
void GuiDeviceBrowser::Draw()
{
	if(!this->IsVisible())
		return;

	bgDeviceSelectionImg->Draw();

	for(int i=0; i<DEVICE_PAGESIZE; i++)
	{
		deviceList[i]->Draw();
	}

	scrollbarImg->Draw();
	arrowUpBtn->Draw();
	arrowDownBtn->Draw();
	scrollbarBoxBtn->Draw();

	this->UpdateEffects();
}

void GuiDeviceBrowser::Update(GuiTrigger * t)
{
	if(!t)
		return;
	
	int position = 0;
	int positionWiimote = 0;
	int maxtext = 0;

	if(state == STATE_DISABLED)
		goto endNavigation;

	arrowUpBtn->Update(t);
	arrowDownBtn->Update(t);
	scrollbarBoxBtn->Update(t);

	// move the file listing to respond to wiimote cursor movement
	if(scrollbarBoxBtn->GetState() == STATE_HELD &&
		scrollbarBoxBtn->GetStateChan() == t->chan &&
		t->wpad->ir.valid &&
		browserD.numEntries > DEVICE_PAGESIZE
		)
	{
		scrollbarBoxBtn->SetPosition(12,0);
		positionWiimote = t->wpad->ir.y - 50 - scrollbarBoxBtn->GetTop();

		if(positionWiimote < scrollbarBoxBtn->GetMinY())
			positionWiimote = scrollbarBoxBtn->GetMinY();
		else if(positionWiimote > scrollbarBoxBtn->GetMaxY())
			positionWiimote = scrollbarBoxBtn->GetMaxY();

		browserD.pageIndex = (positionWiimote * browserD.numEntries)/180 - selectedItem;

		if(browserD.pageIndex <= 0)
		{
			browserD.pageIndex = 0;
		}
		else if(browserD.pageIndex+DEVICE_PAGESIZE >= browserD.numEntries)
		{
			browserD.pageIndex = browserD.numEntries-DEVICE_PAGESIZE;
		}
		listChanged = true;
		focus = false;
	}

	if(arrowDownBtn->GetState() == STATE_HELD && arrowDownBtn->GetStateChan() == t->chan)
	{
		t->wpad->btns_h |= WPAD_BUTTON_DOWN;
		if(!this->IsFocused())
			((GuiWindow *)this->GetParent())->ChangeFocus(this);
	}
	else if(arrowUpBtn->GetState() == STATE_HELD && arrowUpBtn->GetStateChan() == t->chan)
	{
		t->wpad->btns_h |= WPAD_BUTTON_UP;
		if(!this->IsFocused())
			((GuiWindow *)this->GetParent())->ChangeFocus(this);
	}

	// pad/joystick navigation
	//if(!focus)
	//{
	//	goto endNavigation; // skip navigation
	//	listChanged = false;
	//}

	if(t->Right())
	{
		if(browserD.pageIndex < browserD.numEntries && browserD.numEntries > DEVICE_PAGESIZE)
		{
			browserD.pageIndex += DEVICE_PAGESIZE;
			if(browserD.pageIndex+DEVICE_PAGESIZE >= browserD.numEntries)
				browserD.pageIndex = browserD.numEntries-DEVICE_PAGESIZE;
			listChanged = true;
		}
	}
	else if(t->Left())
	{
		if(browserD.pageIndex > 0)
		{
			browserD.pageIndex -= DEVICE_PAGESIZE;
			if(browserD.pageIndex < 0)
				browserD.pageIndex = 0;
			listChanged = true;
		}
	}
	else if(t->Down())
	{
		if(browserD.pageIndex + selectedItem + 1 < browserD.numEntries)
		{
			if(selectedItem == DEVICE_PAGESIZE-1)
			{
				// move list down by 1
				browserD.pageIndex++;
				listChanged = true;
			}
			else if(deviceList[selectedItem+1]->IsVisible())
			{
				deviceList[selectedItem]->ResetState();
				deviceList[++selectedItem]->SetState(STATE_SELECTED, t->chan);
			}
		}
	}
	else if(t->Up())
	{
		if(selectedItem == 0 &&	browserD.pageIndex + selectedItem > 0)
		{
			// move list up by 1
			browserD.pageIndex--;
			listChanged = true;
		}
		else if(selectedItem > 0)
		{
			deviceList[selectedItem]->ResetState();
			deviceList[--selectedItem]->SetState(STATE_SELECTED, t->chan);
		}
	}

  endNavigation:

	for(int i=0; i<DEVICE_PAGESIZE; i++)
	{
		if(listChanged || numEntries != browserD.numEntries)
		{
			if(browserD.pageIndex+i < browserD.numEntries)
			{
				if(deviceList[i]->GetState() == STATE_DISABLED)
					deviceList[i]->SetState(STATE_DEFAULT);

				deviceList[i]->SetVisible(true);

				deviceListText[i]->SetText(browserDevice[browserD.pageIndex+i].displayname);

				
				if(browserDevice[browserD.pageIndex+i].isLog) // log file
				{
					deviceList[i]->SetIcon(deviceListLog[i]);
					maxtext = (width-28-55);
					deviceListText[i]->SetPosition(55,0);
					deviceListText[i]->SetMaxWidth(maxtext, 2);
				}
				else if(browserDevice[browserD.pageIndex+i].isMii) // directory
				{
					deviceList[i]->SetIcon(deviceListMii[i]);
					maxtext = (width-28-55);
					deviceListText[i]->SetPosition(55,0);
					deviceListText[i]->SetMaxWidth(maxtext, 2);
				}
				else if(browserDevice[browserD.pageIndex+i].isdir) // directory
				{
					deviceList[i]->SetIcon(deviceListFolder[i]);
					maxtext = (width-28-50);
					deviceListText[i]->SetPosition(50,0);
					deviceListText[i]->SetMaxWidth(maxtext, 2);
				}
				else if(!browserDevice[browserD.pageIndex+i].isdir &&
						!browserDevice[browserD.pageIndex+i].isLog &&
						!browserDevice[browserD.pageIndex+i].isSave &&
						!browserDevice[browserD.pageIndex+i].isNotInstalled &&
						!browserDevice[browserD.pageIndex+i].isNotSupported) // other file
				{
					maxtext = (width-28-25);
					deviceListText[i]->SetPosition(25,0);
					deviceListText[i]->SetMaxWidth(maxtext, 2);
				}
				else // save datas
				{
					if(browserDevice[browserD.pageIndex+i].isNotInstalled || browserDevice[browserD.pageIndex+i].isNotSupported)
						deviceList[i]->SetIcon(deviceListNoSave[i]);
					else
						deviceList[i]->SetIcon(deviceListSave[i]);
					
					maxtext = (width-28-55);
					deviceListText[i]->SetPosition(55,0);
					deviceListText[i]->SetMaxWidth(maxtext, 2);
				}				
			}
			else
			{
				deviceList[i]->SetVisible(false);
				deviceList[i]->SetState(STATE_DISABLED);
			}
		}

		if(i != selectedItem && deviceList[i]->GetState() == STATE_SELECTED)
			deviceList[i]->ResetState();
		else if(focus && i == selectedItem && deviceList[i]->GetState() == STATE_DEFAULT)
			deviceList[selectedItem]->SetState(STATE_SELECTED, t->chan);

		int currChan = t->chan;

		if(t->wpad->ir.valid && !deviceList[i]->IsInside(t->wpad->ir.x, t->wpad->ir.y))
			t->chan = -1;

		deviceList[i]->Update(t);
		t->chan = currChan;

		if(deviceList[i]->GetState() == STATE_SELECTED)
		{
			selectedItem = i;
			browserD.selIndex = browserD.pageIndex + i;
		}
	}

	// update the location of the scroll box based on the position in the file list
	if(positionWiimote > 0)
	{
		position = positionWiimote; // follow wiimote cursor
	}
	else
	{
		position = 212*((browserD.pageIndex + DEVICE_PAGESIZE)/2.0) / (browserD.numEntries*1.0);

		if(browserD.pageIndex/(DEVICE_PAGESIZE/2.0) < 1)
			position = 0;
		else if((browserD.pageIndex+DEVICE_PAGESIZE)/(DEVICE_PAGESIZE*1.0) >= (browserD.numEntries)/(DEVICE_PAGESIZE*1.0))
			position = 212;
	}

	scrollbarBoxBtn->SetPosition(12,position+8);

	listChanged = false;
	numEntries = browserD.numEntries;
	
	// Update NAVINFO //
	if(navinfo.LastDeviceItemSelected != selectedItem)
		navinfo.LastDeviceItemSelected = selectedItem;
	if(navinfo.LastDeviceFirstItem != browserD.pageIndex) {
		navinfo.LastDeviceFirstItem = browserD.pageIndex;
	}

	if(updateCB)
		updateCB(this);
}
