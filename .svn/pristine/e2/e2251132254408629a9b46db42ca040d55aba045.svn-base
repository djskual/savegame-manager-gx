/****************************************************************************
 * libwiigui
 *
 * Tantric 2009
 *
 * gui_devicebrowser.cpp
 *
 * GUI class definitions
 ***************************************************************************/

#include "libwiigui/gui_browsers/gui_savebrowser.h"
#include "saves/wii_list.h"
#include "saves/utils.h"
#include "main.h"

/**
 * Constructor for the GuiSaveBrowser class.
 */
GuiSaveBrowser::GuiSaveBrowser(int w, int h)
{
	if(navinfo.LastWiiSaveSelected > wiiSaveInfo.Cnt-1)
		navinfo.LastWiiSaveSelected = wiiSaveInfo.Cnt-1;
	
	width = w;
	height = h;
	numEntries = 0;
	selectedItem = navinfo.LastWiiSaveSelected;
	selectable = true;
	listChanged = true; // trigger an initial list update
	focus = 0; // allow focus
	
	wiiSaveInfo.pageIndex = navinfo.LastWiiSaveFirstItem;

	trigA = new SimpleGuiTrigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);

	trigHeldA = new GuiTrigger;
	trigHeldA->SetHeldTrigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);

	btnSoundOver = Resources::GetSound(button_over_wav, button_over_wav_size, cfg.SFXVolume);
	btnSoundClick = Resources::GetSound(button_click_wav, button_click_wav_size, cfg.SFXVolume);
	
	bgSaveSelection = Resources::GetImageData(bg_options_png, bg_options_png_size);
	bgSaveSelectionImg = new GuiImage(bgSaveSelection);
	bgSaveSelectionImg->SetParent(this);
	bgSaveSelectionImg->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);

	bgSaveSelectionEntry = Resources::GetImageData(bg_save_options_entry_png, bg_save_options_entry_png_size);
	bg = new GuiImageData(save_button_bg_png, save_button_bg_png_size);
	
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


	if(wiiSaveInfo.Cnt>0)
		banner = new GuiSaveBanner * [wiiSaveInfo.Cnt];
	
	for(int i=0;i<wiiSaveInfo.Cnt; i++)
	{
	    char savepath[ISFS_MAXPATH];
		u32 high = (u32)(wiiSaveList[i].tid >> 32);
		u32 low  = (u32)(wiiSaveList[i].tid & 0xFFFFFFFF);

	    snprintf(savepath, sizeof(savepath),"/title/%08x/%08x/data/banner.bin", high, low);
	    
		u8* buffer = NULL;
	    read_isfs(savepath, &buffer);

	    banner[i] = new GuiSaveBanner(buffer);
	    banner[i]->SetPosition(58,-2);
	    banner[i]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
		
	    free(buffer);
	}

	for(int i=0; i<SAVE_PAGESIZE && i<wiiSaveInfo.Cnt; i++)
	{
		saveBg[i] = new GuiImage(bgSaveSelectionEntry);

		bgImg[i] = new GuiImage(bg);
		bgImg[i]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
		bgImg[i]->SetPosition(30,0);
		
		saveBtn[i] = new GuiButton(width-28,68);
		saveBtn[i]->SetParent(this);
		saveBtn[i]->SetImage(bgImg[i]);
		saveBtn[i]->SetIcon2(banner[i]);
		saveBtn[i]->SetImageOver(saveBg[i]);
		saveBtn[i]->SetPosition(2,68*i+3);
		saveBtn[i]->SetRumble(false);
		saveBtn[i]->SetTrigger(trigA);
		saveBtn[i]->SetSoundClick(btnSoundClick);
	}
}

/**
 * Destructor for the GuiSaveBrowser class.
 */
GuiSaveBrowser::~GuiSaveBrowser()
{
	delete arrowUpBtn;
	delete arrowDownBtn;
	delete scrollbarBoxBtn;

	delete scrollbarImg;
	delete arrowDownImg;
	delete arrowDownOverImg;
	delete arrowUpImg;
	delete arrowUpOverImg;
	delete scrollbarBoxImg;
	delete scrollbarBoxOverImg;

	Resources::Remove(bg);
	Resources::Remove(scrollbar);
	Resources::Remove(arrowDown);
	Resources::Remove(arrowDownOver);
	Resources::Remove(arrowUp);
	Resources::Remove(arrowUpOver);
	Resources::Remove(scrollbarBox);
	Resources::Remove(scrollbarBoxOver);

	delete bgSaveSelectionImg;

	Resources::Remove(bgSaveSelection);
	Resources::Remove(bgSaveSelectionEntry);

	Resources::Remove(btnSoundOver);
	Resources::Remove(btnSoundClick);
	delete trigHeldA;
	delete trigA;

	for(int i=0; i<SAVE_PAGESIZE && i<wiiSaveInfo.Cnt; i++)
	{
		if(saveBg[i]) delete saveBg[i];
		if(bgImg[i]) delete bgImg[i];
		if(saveBtn[i]) delete saveBtn[i];
		saveBg[i] = NULL;
		bgImg[i] = NULL;
		saveBtn[i] = NULL;
	}

	for(int i=0;i<wiiSaveInfo.Cnt; i++)
	{
	    if(banner[i])delete(banner[i]);
	    banner[i] = NULL;
	}
}

void GuiSaveBrowser::SetFocus(int f)
{
	focus = f;

	for(int i=0; i<SAVE_PAGESIZE && i<wiiSaveInfo.Cnt; i++)
		saveBtn[i]->ResetState();

	if(f == 1 && wiiSaveInfo.Cnt>0)
		saveBtn[selectedItem]->SetState(STATE_SELECTED);
}

void GuiSaveBrowser::ResetState()
{
	state = STATE_DEFAULT;
	stateChan = -1;
	selectedItem = 0;

	for(int i=0; i<SAVE_PAGESIZE; i++)
	{
		saveBtn[i]->ResetState();
	}
}

void GuiSaveBrowser::TriggerUpdate()
{
	listChanged = true;
}

/**
 * Draw on screen
 */
void GuiSaveBrowser::Draw()
{
	if(!this->IsVisible())
		return;

	bgSaveSelectionImg->Draw();

	for(int i=0; i<SAVE_PAGESIZE && i<wiiSaveInfo.Cnt; i++)
	{
		if(saveBtn[i])
			saveBtn[i]->Draw();
	}

	scrollbarImg->Draw();
	arrowUpBtn->Draw();
	arrowDownBtn->Draw();
	scrollbarBoxBtn->Draw();

	this->UpdateEffects();
}

void GuiSaveBrowser::Update(GuiTrigger * t)
{
	if(!t)
		return;
	
	int position = 0;
	int positionWiimote = 0;
	
	if(state == STATE_DISABLED)
		goto endNavigation;

	arrowUpBtn->Update(t);
	arrowDownBtn->Update(t);
	scrollbarBoxBtn->Update(t);

	// move the file listing to respond to wiimote cursor movement
	if(scrollbarBoxBtn->GetState() == STATE_HELD &&
		scrollbarBoxBtn->GetStateChan() == t->chan &&
		t->wpad->ir.valid &&
		wiiSaveInfo.Cnt > SAVE_PAGESIZE)
	{
		scrollbarBoxBtn->SetPosition(12,0);
		positionWiimote = t->wpad->ir.y - 50 - scrollbarBoxBtn->GetTop();

		if(positionWiimote < scrollbarBoxBtn->GetMinY())
			positionWiimote = scrollbarBoxBtn->GetMinY();
		else if(positionWiimote > scrollbarBoxBtn->GetMaxY())
			positionWiimote = scrollbarBoxBtn->GetMaxY();

		wiiSaveInfo.pageIndex = (positionWiimote * wiiSaveInfo.Cnt)/180 - selectedItem;

		if(wiiSaveInfo.pageIndex <= 0)
		{
			wiiSaveInfo.pageIndex = 0;
		}
		else if(wiiSaveInfo.pageIndex+SAVE_PAGESIZE >= wiiSaveInfo.Cnt)
		{
			wiiSaveInfo.pageIndex = wiiSaveInfo.Cnt-SAVE_PAGESIZE;
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
		if(wiiSaveInfo.pageIndex < wiiSaveInfo.Cnt && wiiSaveInfo.Cnt > SAVE_PAGESIZE)
		{
			wiiSaveInfo.pageIndex += SAVE_PAGESIZE;
			if(wiiSaveInfo.pageIndex+SAVE_PAGESIZE >= wiiSaveInfo.Cnt)
				wiiSaveInfo.pageIndex = wiiSaveInfo.Cnt-SAVE_PAGESIZE;
			listChanged = true;
		}
	}
	else if(t->Left())
	{
		if(wiiSaveInfo.pageIndex > 0)
		{
			wiiSaveInfo.pageIndex -= SAVE_PAGESIZE;
			if(wiiSaveInfo.pageIndex < 0)
				wiiSaveInfo.pageIndex = 0;
			listChanged = true;
		}
	}
	else if(t->Down())
	{
		if(wiiSaveInfo.pageIndex + selectedItem + 1 < wiiSaveInfo.Cnt)
		{
			if(selectedItem == SAVE_PAGESIZE-1)
			{
				// move list down by 1
				wiiSaveInfo.pageIndex++;
				listChanged = true;
			}
			else if(saveBtn[selectedItem+1]->IsVisible())
			{
				saveBtn[selectedItem]->ResetState();
				saveBtn[++selectedItem]->SetState(STATE_SELECTED, t->chan);
			}
		}
	}
	else if(t->Up())
	{
		if(selectedItem == 0 &&	wiiSaveInfo.pageIndex + selectedItem > 0)
		{
			// move list up by 1
			wiiSaveInfo.pageIndex--;
			listChanged = true;
		}
		else if(selectedItem > 0)
		{
			saveBtn[selectedItem]->ResetState();
			saveBtn[--selectedItem]->SetState(STATE_SELECTED, t->chan);
		}
	}

  endNavigation:

	for(int i=0; i<SAVE_PAGESIZE && i<wiiSaveInfo.Cnt; i++)
	{
		if(listChanged || numEntries != wiiSaveInfo.Cnt)
		{
			if(wiiSaveInfo.pageIndex+i < wiiSaveInfo.Cnt)
			{
				if(saveBtn[i]->GetState() == STATE_DISABLED)
					saveBtn[i]->SetState(STATE_DEFAULT);

				saveBtn[i]->SetVisible(true);
				saveBtn[i]->SetIcon2(banner[wiiSaveInfo.pageIndex+i]);			
			}
			else if(i<SAVE_PAGESIZE && i<wiiSaveInfo.Cnt && wiiSaveInfo.Cnt>0)
			{
				saveBtn[i]->SetVisible(false);
				saveBtn[i]->SetState(STATE_DISABLED);
			}
		}

		if(i != selectedItem && saveBtn[i]->GetState() == STATE_SELECTED)
			saveBtn[i]->ResetState();
		else if(focus && i == selectedItem && saveBtn[i]->GetState() == STATE_DEFAULT)
			saveBtn[selectedItem]->SetState(STATE_SELECTED, t->chan);

		int currChan = t->chan;

		if(t->wpad->ir.valid && !saveBtn[i]->IsInside(t->wpad->ir.x, t->wpad->ir.y))
			t->chan = -1;

		saveBtn[i]->Update(t);
		t->chan = currChan;

		if(saveBtn[i]->GetState() == STATE_SELECTED)
		{
			selectedItem = i;
			wiiSaveInfo.selIndex = wiiSaveInfo.pageIndex + i;
		}
	}

	// update the location of the scroll box based on the position in the file list
	if(positionWiimote > 0)
	{
		position = positionWiimote; // follow wiimote cursor
	}
	else
	{
		position = 212*((wiiSaveInfo.pageIndex + SAVE_PAGESIZE)/2.0) / (wiiSaveInfo.Cnt*1.0);

		if(wiiSaveInfo.pageIndex/(SAVE_PAGESIZE/2.0) < 1)
			position = 0;
		else if((wiiSaveInfo.pageIndex+SAVE_PAGESIZE)/(SAVE_PAGESIZE*1.0) >= (wiiSaveInfo.Cnt)/(SAVE_PAGESIZE*1.0))
			position = 212;
	}

	scrollbarBoxBtn->SetPosition(12,position+8);

	listChanged = false;
	numEntries = wiiSaveInfo.Cnt;
	
	// Update NAVINFO //
	if(navinfo.LastWiiSaveSelected != selectedItem)
		navinfo.LastWiiSaveSelected = selectedItem;
	if(navinfo.LastWiiSaveFirstItem != wiiSaveInfo.pageIndex) {
		navinfo.LastWiiSaveFirstItem = wiiSaveInfo.pageIndex;
	}

	if(updateCB)
		updateCB(this);
}
