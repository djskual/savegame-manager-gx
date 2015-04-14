/****************************************************************************
 * libwiigui
 *
 * Tantric 2009
 *
 * gui_devicebrowser.cpp
 *
 * GUI class definitions
 ***************************************************************************/

#include "libwiigui/gui_browsers/gui_miibrowser.h"
#include "mii/mii_list.h"
#include "main.h"

/**
 * Constructor for the GuiMiiBrowser class.
 */
GuiMiiBrowser::GuiMiiBrowser(int w, int h)
{
	if(navinfo.LastMiiSelected > miiListInfo.Cnt-1)
		navinfo.LastMiiSelected = miiListInfo.Cnt-1;
	
	width = w;
	height = h;
	numEntries = 0;
	selectedItem = navinfo.LastMiiSelected;
	selectable = true;
	listChanged = true; // trigger an initial list update
	focus = 0; // allow focus
	
	miiListInfo.pageIndex = navinfo.LastMiiFirstItem;

	trigA = new SimpleGuiTrigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);

	trigHeldA = new GuiTrigger;
	trigHeldA->SetHeldTrigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);

	btnSoundOver = Resources::GetSound(button_over_wav, button_over_wav_size, cfg.SFXVolume);
	btnSoundClick = Resources::GetSound(button_click_wav, button_click_wav_size, cfg.SFXVolume);
	
	bg = Resources::GetImageData(bg_options_png, bg_options_png_size);
	bgImg = new GuiImage(bg);
	bgImg->SetParent(this);
	bgImg->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);

	bgMiiSelection = Resources::GetImageData(bg_device_options_entry_png, bg_device_options_entry_png_size);
	
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


	for(int i=0; i<MII_PAGESIZE; i++)
	{
		miiTxt[i] = new GuiText(NULL, 20, (GXColor){0, 0, 0, 255});
		miiTxt[i]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
		miiTxt[i]->SetPosition(25,0);
		miiTxt[i]->SetMaxWidth(width-28-25, GuiText::SCROLL);

		bgMiiSelectionImg[i] = new GuiImage(bgMiiSelection);
		
		miiBtn[i] = new GuiButton(width-28,30);
		miiBtn[i]->SetParent(this);
		miiBtn[i]->SetLabel(miiTxt[i]);
		miiBtn[i]->SetImageOver(bgMiiSelectionImg[i]);
		miiBtn[i]->SetPosition(3,30*i+4);
		miiBtn[i]->SetRumble(false);
		miiBtn[i]->SetTrigger(trigA);
		miiBtn[i]->SetSoundClick(btnSoundClick);
	}
}

/**
 * Destructor for the GuiMiiBrowser class.
 */
GuiMiiBrowser::~GuiMiiBrowser()
{
	delete arrowUpBtn;
	delete arrowDownBtn;
	delete scrollbarBoxBtn;

	delete bgImg;
	delete scrollbarImg;
	delete arrowDownImg;
	delete arrowDownOverImg;
	delete arrowUpImg;
	delete arrowUpOverImg;
	delete scrollbarBoxImg;
	delete scrollbarBoxOverImg;

	Resources::Remove(bgMiiSelection);
	Resources::Remove(bg);
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

	for(int i=0; i<MII_PAGESIZE; i++)
	{
		delete miiTxt[i];
		delete miiBtn[i];
		delete bgMiiSelectionImg[i];
	}
}

void GuiMiiBrowser::SetFocus(int f)
{
	focus = f;

	for(int i=0; i<MII_PAGESIZE; i++)
		miiBtn[i]->ResetState();

	if(f == 1 && miiListInfo.Cnt>0)
		miiBtn[selectedItem]->SetState(STATE_SELECTED);
}

void GuiMiiBrowser::ResetState()
{
	state = STATE_DEFAULT;
	stateChan = -1;
	selectedItem = 0;

	for(int i=0; i<MII_PAGESIZE; i++)
	{
		miiBtn[i]->ResetState();
	}
}

void GuiMiiBrowser::TriggerUpdate()
{
	listChanged = true;
}

/**
 * Draw on screen
 */
void GuiMiiBrowser::Draw()
{
	if(!this->IsVisible())
		return;

	bgImg->Draw();

	for(int i=0; i<MII_PAGESIZE && miiListInfo.Cnt>0; i++)
	{
		miiBtn[i]->Draw();
	}

	scrollbarImg->Draw();
	arrowUpBtn->Draw();
	arrowDownBtn->Draw();
	scrollbarBoxBtn->Draw();

	this->UpdateEffects();
}

void GuiMiiBrowser::Update(GuiTrigger * t)
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
		miiListInfo.Cnt > MII_PAGESIZE)
	{
		scrollbarBoxBtn->SetPosition(12,0);
		positionWiimote = t->wpad->ir.y - 50 - scrollbarBoxBtn->GetTop();

		if(positionWiimote < scrollbarBoxBtn->GetMinY())
			positionWiimote = scrollbarBoxBtn->GetMinY();
		else if(positionWiimote > scrollbarBoxBtn->GetMaxY())
			positionWiimote = scrollbarBoxBtn->GetMaxY();

		miiListInfo.pageIndex = (positionWiimote * miiListInfo.Cnt)/180 - selectedItem;

		if(miiListInfo.pageIndex <= 0)
		{
			miiListInfo.pageIndex = 0;
		}
		else if(miiListInfo.pageIndex+MII_PAGESIZE >= miiListInfo.Cnt)
		{
			miiListInfo.pageIndex = miiListInfo.Cnt-MII_PAGESIZE;
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
		if(miiListInfo.pageIndex < miiListInfo.Cnt && miiListInfo.Cnt > MII_PAGESIZE)
		{
			miiListInfo.pageIndex += MII_PAGESIZE;
			if(miiListInfo.pageIndex+MII_PAGESIZE >= miiListInfo.Cnt)
				miiListInfo.pageIndex = miiListInfo.Cnt-MII_PAGESIZE;
			listChanged = true;
		}
	}
	else if(t->Left())
	{
		if(miiListInfo.pageIndex > 0)
		{
			miiListInfo.pageIndex -= MII_PAGESIZE;
			if(miiListInfo.pageIndex < 0)
				miiListInfo.pageIndex = 0;
			listChanged = true;
		}
	}
	else if(t->Down())
	{
		if(miiListInfo.pageIndex + selectedItem + 1 < miiListInfo.Cnt)
		{
			if(selectedItem == MII_PAGESIZE-1)
			{
				// move list down by 1
				miiListInfo.pageIndex++;
				listChanged = true;
			}
			else if(miiBtn[selectedItem+1]->IsVisible())
			{
				miiBtn[selectedItem]->ResetState();
				miiBtn[++selectedItem]->SetState(STATE_SELECTED, t->chan);
			}
		}
	}
	else if(t->Up())
	{
		if(selectedItem == 0 &&	miiListInfo.pageIndex + selectedItem > 0)
		{
			// move list up by 1
			miiListInfo.pageIndex--;
			listChanged = true;
		}
		else if(selectedItem > 0)
		{
			miiBtn[selectedItem]->ResetState();
			miiBtn[--selectedItem]->SetState(STATE_SELECTED, t->chan);
		}
	}

  endNavigation:

	for(int i=0; i<MII_PAGESIZE; i++)
	{
		if(listChanged || numEntries != miiListInfo.Cnt)
		{
			if(miiListInfo.pageIndex+i < miiListInfo.Cnt)
			{
				if(miiBtn[i]->GetState() == STATE_DISABLED)
					miiBtn[i]->SetState(STATE_DEFAULT);

				miiBtn[i]->SetVisible(true);

				miiTxt[i]->SetText(miiList[miiListInfo.pageIndex+i].displayname);				
			}
			else
			{
				miiBtn[i]->SetVisible(false);
				miiBtn[i]->SetState(STATE_DISABLED);
			}
		}

		if(i != selectedItem && miiBtn[i]->GetState() == STATE_SELECTED)
			miiBtn[i]->ResetState();
		else if(focus && i == selectedItem && miiBtn[i]->GetState() == STATE_DEFAULT)
			miiBtn[selectedItem]->SetState(STATE_SELECTED, t->chan);

		int currChan = t->chan;

		if(t->wpad->ir.valid && !miiBtn[i]->IsInside(t->wpad->ir.x, t->wpad->ir.y))
			t->chan = -1;

		miiBtn[i]->Update(t);
		t->chan = currChan;

		if(miiBtn[i]->GetState() == STATE_SELECTED)
		{
			selectedItem = i;
			miiListInfo.selIndex = miiListInfo.pageIndex + i;
		}
	}

	// update the location of the scroll box based on the position in the file list
	if(positionWiimote > 0)
	{
		position = positionWiimote; // follow wiimote cursor
	}
	else
	{
		position = 212*((miiListInfo.pageIndex + MII_PAGESIZE)/2.0) / (miiListInfo.Cnt*1.0);

		if(miiListInfo.pageIndex/(MII_PAGESIZE/2.0) < 1)
			position = 0;
		else if((miiListInfo.pageIndex+MII_PAGESIZE)/(MII_PAGESIZE*1.0) >= (miiListInfo.Cnt)/(MII_PAGESIZE*1.0))
			position = 212;
	}

	scrollbarBoxBtn->SetPosition(12,position+8);

	listChanged = false;
	numEntries = miiListInfo.Cnt;
	
	// Update NAVINFO //
	if(navinfo.LastMiiSelected != selectedItem)
		navinfo.LastMiiSelected = selectedItem;
	if(navinfo.LastMiiFirstItem != miiListInfo.pageIndex) {
		navinfo.LastMiiFirstItem = miiListInfo.pageIndex;
	}

	if(updateCB)
		updateCB(this);
}
