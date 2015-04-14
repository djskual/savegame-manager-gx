/****************************************************************************
 * SaveGame Manager Gx
 *
 * Dj_Skual 2010
 *
 * gui_customfilepathbrowser.cpp
 *
 ***************************************************************************/

#include "libwiigui/gui_browsers/gui_browsers.h"
#include "language/langBrowser.h"
#include "main.h"

/**
 * Constructor for the GuiLangBrowser class.
 */
GuiLangBrowser::GuiLangBrowser(int w, int h)
{
	width = w;
	height = h;
	numEntries = 0;
	selectedItem = 0;
	selectable = true;
	listChanged = true; // trigger an initial list update
	focus = 0; // allow focus
	
	browserl.pageIndex = 0;

	trigA = new SimpleGuiTrigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);

	trigHeldA = new GuiTrigger;
	trigHeldA->SetHeldTrigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);

	btnSoundOver = Resources::GetSound(button_over_wav, button_over_wav_size, cfg.SFXVolume);
	btnSoundClick = Resources::GetSound(button_click_wav, button_click_wav_size, cfg.SFXVolume);
	
	bgCustomSelectionEntry = Resources::GetImageData(bg_custom_entry_png, bg_custom_entry_png_size);
	customFolder = new GuiImageData(icon_folder_png, icon_folder_png_size);
	
	arrowDown = Resources::GetImageData(scrollbar_arrowdown_png, scrollbar_arrowdown_png_size);
	arrowDownImg = new GuiImage(arrowDown);
	arrowDownOver = Resources::GetImageData(scrollbar_arrowdown_png, scrollbar_arrowdown_png_size);
	arrowDownOverImg = new GuiImage(arrowDownOver);
	arrowUp = Resources::GetImageData(scrollbar_arrowdown_png, scrollbar_arrowdown_png_size);
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
	arrowUpBtn->SetPosition(40, -5);
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
	arrowDownBtn->SetPosition(41, 18);
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
	scrollbarBoxBtn->SetMaxY(180);
	scrollbarBoxBtn->SetSelectable(false);
	scrollbarBoxBtn->SetClickable(false);
	scrollbarBoxBtn->SetHoldable(true);
	scrollbarBoxBtn->SetTrigger(trigHeldA);
	scrollbarBoxBtn->SetEffectOnOver(EFFECT_SCALE, 50, 120);


	for(int i=0; i<CUSTOM_PAGESIZE; i++)
	{
		customListText[i] = new GuiText(NULL, 20, (GXColor){0, 0, 0, 0xff});
		customListText[i]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
		customListText[i]->SetPosition(5,0);
		customListText[i]->SetMaxWidth(width-28-25, 2);

		customListBg[i] = new GuiImage(bgCustomSelectionEntry);
		
		customListFolder[i] = new GuiImage(customFolder);
		customListFolder[i]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
		customListFolder[i]->SetPosition(18,0);
		
		customList[i] = new GuiButton(width-28,30);
		customList[i]->SetParent(this);
		customList[i]->SetLabel(customListText[i]);
		customList[i]->SetImageOver(customListBg[i]);
		customList[i]->SetIcon(customListFolder[i]);
		customList[i]->SetPosition(4,30*i+4);
		customList[i]->SetTrigger(trigA);
		customList[i]->SetSoundClick(btnSoundClick);
	}
}

/**
 * Destructor for the GuiLangBrowser class.
 */
GuiLangBrowser::~GuiLangBrowser()
{
	delete arrowUpBtn;
	delete arrowDownBtn;
	delete scrollbarBoxBtn;

	delete arrowDownImg;
	delete arrowDownOverImg;
	delete arrowUpImg;
	delete arrowUpOverImg;
	delete scrollbarBoxImg;
	delete scrollbarBoxOverImg;

	Resources::Remove(bgCustomSelectionEntry);
	Resources::Remove(customFolder);
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

	for(int i=0; i<CUSTOM_PAGESIZE; i++)
	{
		delete customListText[i];
		delete customList[i];
		delete customListBg[i];
		delete customListFolder[i];
	}
}

void GuiLangBrowser::SetFocus(int f)
{
	focus = f;

	for(int i=0; i<CUSTOM_PAGESIZE; i++)
		customList[i]->ResetState();

	if(f == 1)
		customList[selectedItem]->SetState(STATE_SELECTED);
}

void GuiLangBrowser::ResetState()
{
	state = STATE_DEFAULT;
	stateChan = -1;
	selectedItem = 0;

	for(int i=0; i<CUSTOM_PAGESIZE; i++)
	{
		customList[i]->ResetState();
	}
}

void GuiLangBrowser::TriggerUpdate()
{
	listChanged = true;
}

/**
 * Draw the button on screen
 */
void GuiLangBrowser::Draw()
{
	if(!this->IsVisible())
		return;

	for(int i=0; i<CUSTOM_PAGESIZE; i++)
	{
		customList[i]->Draw();
	}

	arrowUpBtn->Draw();
	arrowDownBtn->Draw();
	scrollbarBoxBtn->Draw();

	this->UpdateEffects();
}

void GuiLangBrowser::Update(GuiTrigger * t)
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
		browserl.numEntries > CUSTOM_PAGESIZE
		)
	{
		scrollbarBoxBtn->SetPosition(41,0);
		positionWiimote = t->wpad->ir.y - 50 - scrollbarBoxBtn->GetTop();

		if(positionWiimote < scrollbarBoxBtn->GetMinY())
			positionWiimote = scrollbarBoxBtn->GetMinY();
		else if(positionWiimote > scrollbarBoxBtn->GetMaxY())
			positionWiimote = scrollbarBoxBtn->GetMaxY();

		browserl.pageIndex = (positionWiimote * browserl.numEntries)/180 - selectedItem;

		if(browserl.pageIndex <= 0)
		{
			browserl.pageIndex = 0;
		}
		else if(browserl.pageIndex+CUSTOM_PAGESIZE >= browserl.numEntries)
		{
			browserl.pageIndex = browserl.numEntries-CUSTOM_PAGESIZE;
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
		if(browserl.pageIndex < browserl.numEntries && browserl.numEntries > CUSTOM_PAGESIZE)
		{
			browserl.pageIndex += CUSTOM_PAGESIZE;
			if(browserl.pageIndex+CUSTOM_PAGESIZE >= browserl.numEntries)
				browserl.pageIndex = browserl.numEntries-CUSTOM_PAGESIZE;
			listChanged = true;
		}
	}
	else if(t->Left())
	{
		if(browserl.pageIndex > 0)
		{
			browserl.pageIndex -= CUSTOM_PAGESIZE;
			if(browserl.pageIndex < 0)
				browserl.pageIndex = 0;
			listChanged = true;
		}
	}
	else if(t->Down())
	{
		if(browserl.pageIndex + selectedItem + 1 < browserl.numEntries)
		{
			if(selectedItem == CUSTOM_PAGESIZE-1)
			{
				// move list down by 1
				browserl.pageIndex++;
				listChanged = true;
			}
			else if(customList[selectedItem+1]->IsVisible())
			{
				customList[selectedItem]->ResetState();
				customList[++selectedItem]->SetState(STATE_SELECTED, t->chan);
			}
		}
	}
	else if(t->Up())
	{
		if(selectedItem == 0 &&	browserl.pageIndex + selectedItem > 0)
		{
			// move list up by 1
			browserl.pageIndex--;
			listChanged = true;
		}
		else if(selectedItem > 0)
		{
			customList[selectedItem]->ResetState();
			customList[--selectedItem]->SetState(STATE_SELECTED, t->chan);
		}
	}

  endNavigation:

	for(int i=0; i<CUSTOM_PAGESIZE; i++)
	{
		if(listChanged || numEntries != browserl.numEntries)
		{
			if(browserl.pageIndex+i < browserl.numEntries)
			{
				if(customList[i]->GetState() == STATE_DISABLED)
					customList[i]->SetState(STATE_DEFAULT);
				
				customList[i]->SetVisible(true);
				
				customListText[i]->SetText(browserLang[browserl.pageIndex+i].displayname);
				
				if(browserLang[browserl.pageIndex+i].isdir) {
					customListFolder[i]->SetVisible(true);
					maxtext = (width-80);
					customListText[i]->SetPosition(50,0);
				}
				else {
					customListFolder[i]->SetVisible(false);
					maxtext = (width-50);
					customListText[i]->SetPosition(25,0);
				}
				customListText[i]->SetMaxWidth(maxtext, 2);
			}
			else
			{
				customList[i]->SetVisible(false);
				customList[i]->SetState(STATE_DISABLED);
			}
		}

		if(i != selectedItem && customList[i]->GetState() == STATE_SELECTED)
			customList[i]->ResetState();
		else if(focus && i == selectedItem && customList[i]->GetState() == STATE_DEFAULT)
			customList[selectedItem]->SetState(STATE_SELECTED, t->chan);

		int currChan = t->chan;

		if(t->wpad->ir.valid && !customList[i]->IsInside(t->wpad->ir.x, t->wpad->ir.y))
			t->chan = -1;

		customList[i]->Update(t);
		t->chan = currChan;

		if(customList[i]->GetState() == STATE_SELECTED)
		{
			selectedItem = i;
			browserl.selIndex = browserl.pageIndex + i;
		}
	}

	// update the location of the scroll box based on the position in the file list
	if(positionWiimote > 0)
	{
		position = positionWiimote; // follow wiimote cursor
	}
	else
	{
		position = 180*((browserl.pageIndex + CUSTOM_PAGESIZE)/2.0) / (browserl.numEntries*1.0);

		if(browserl.pageIndex/(CUSTOM_PAGESIZE/2.0) < 1)
			position = 0;
		else if((browserl.pageIndex+CUSTOM_PAGESIZE)/(CUSTOM_PAGESIZE*1.0) >= (browserl.numEntries)/(CUSTOM_PAGESIZE*1.0))
			position = 180;
	}

	scrollbarBoxBtn->SetPosition(41,position+30);

	listChanged = false;
	numEntries = browserl.numEntries;
	
	if(updateCB)
		updateCB(this);
}
