/****************************************************************************
 * libwiigui
 *
 * Tantric 2009
 *
 * gui_optionbrowser.cpp
 *
 * GUI class definitions
 ***************************************************************************/

#include <unistd.h>

#include "libwiigui/gui_browsers/gui_browsers.h"
#include "main.h"

/**
 * Constructor for the GuiOptionBrowser class.
 */
GuiOptionBrowser::GuiOptionBrowser(int w, int h, OptionList * l, int s)
{
	width = w;
	height = h;
	options = l;
	scrollable = s;
	selectable = true;
	listOffset = this->FindMenuItem(-1, 1);
	listChanged = true; // trigger an initial list update
	selectedItem = 0;
	focus = 0; // allow focus
	

	trigA = new SimpleGuiTrigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);

	trigHeldA = new GuiTrigger;
	trigHeldA->SetHeldTrigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);

	btnSoundOver = Resources::GetSound(button_over_wav, button_over_wav_size, cfg.SFXVolume);
	btnSoundClick = Resources::GetSound(button_click_wav, button_click_wav_size, cfg.SFXVolume);
	
	bgOptions = Resources::GetImageData(bg_options_settings_png, bg_options_settings_png_size);
	bgOptionsImg = new GuiImage(bgOptions);
	bgOptionsImg->SetParent(this);
	bgOptionsImg->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);

	bgOptionsEntry = Resources::GetImageData(bg_options_entry_png, bg_options_entry_png_size);

	if (scrollable != 0)
	{
		scrollbar = Resources::GetImageData(scrollbar_png, scrollbar_png_size);
		scrollbarImg = new GuiImage(scrollbar);
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
		arrowDownBtn = new GuiButton(arrowDownImg->GetWidth(), arrowDownImg->GetHeight());
		scrollbarBoxBtn = new GuiButton(scrollbarBoxImg->GetWidth(), scrollbarBoxImg->GetHeight());
		
		scrollbarImg->SetParent(this);
		scrollbarImg->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
		scrollbarImg->SetPosition(0, 4);
		
		arrowUpBtn->SetParent(this);
		arrowUpBtn->SetImage(arrowUpImg);
		arrowUpBtn->SetImageOver(arrowUpOverImg);
		arrowUpBtn->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
		arrowUpBtn->SetPosition(11, -19);
		arrowUpBtn->SetSelectable(false);
		arrowUpBtn->SetClickable(false);
		arrowUpBtn->SetHoldable(true);
		arrowUpBtn->SetTrigger(trigA);
		arrowUpBtn->SetSoundOver(btnSoundOver);
		arrowUpBtn->SetSoundClick(btnSoundClick);
		arrowUpBtn->SetEffectGrow();
		
		arrowDownBtn->SetParent(this);
		arrowDownBtn->SetImage(arrowDownImg);
		arrowDownBtn->SetImageOver(arrowDownOverImg);
		arrowDownBtn->SetAlignment(ALIGN_RIGHT, ALIGN_BOTTOM);
		arrowDownBtn->SetPosition(12, 22);
		arrowDownBtn->SetSelectable(false);
		arrowDownBtn->SetClickable(false);
		arrowDownBtn->SetHoldable(true);
		arrowDownBtn->SetTrigger(trigA);
		arrowDownBtn->SetSoundOver(btnSoundOver);
		arrowDownBtn->SetSoundClick(btnSoundClick);
		arrowDownBtn->SetEffectGrow();
		
		scrollbarBoxBtn->SetParent(this);
		scrollbarBoxBtn->SetImage(scrollbarBoxImg);
		scrollbarBoxBtn->SetImageOver(scrollbarBoxOverImg);
		scrollbarBoxBtn->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
		scrollbarBoxBtn->SetPosition(12, 7);
		scrollbarBoxBtn->SetMinY(0);
		scrollbarBoxBtn->SetMaxY(height);
		scrollbarBoxBtn->SetSelectable(false);
		scrollbarBoxBtn->SetClickable(false);
		scrollbarBoxBtn->SetHoldable(true);
		scrollbarBoxBtn->SetTrigger(trigHeldA);
		scrollbarBoxBtn->SetEffectGrow();
	}
	
	for(int i=0; i<PAGESIZE; i++)
	{
		optionTxt[i] = new GuiText(NULL, 20, (GXColor){0, 0, 0, 0xff});
		optionTxt[i]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
		optionTxt[i]->SetPosition(25,0);
		optionTxt[i]->SetMaxWidth(150-25, 2);

		optionVal[i] = new GuiText(NULL, 20, (GXColor){0, 0, 0, 0xff});
		optionVal[i]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
		optionVal[i]->SetPosition(0,0);
		if (scrollable != 0) {
			optionVal[i]->SetMaxWidth(width-160-40, 2);
		} else {
			optionVal[i]->SetMaxWidth(width-160-25, 2);
		}
		
		optionBg[i] = new GuiImage(bgOptionsEntry);

		optionBtn[i] = new GuiButton(396,30);
		optionBtn[i]->SetParent(this);
		optionBtn[i]->SetLabel(optionTxt[i], 0);
		optionBtn[i]->SetLabel(optionVal[i], 1);
		optionBtn[i]->SetImageOver(optionBg[i]);
		optionBtn[i]->SetPosition(10,32*i+12);
		optionBtn[i]->SetTrigger(trigA);
		optionBtn[i]->SetSoundClick(btnSoundClick);
		optionBtn[i]->SetRumble(false);
	}
}

/**
 * Destructor for the GuiOptionBrowser class.
 */
GuiOptionBrowser::~GuiOptionBrowser()
{
	if (scrollable != 0)
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
		
		Resources::Remove(scrollbar);
		Resources::Remove(arrowDown);
		Resources::Remove(arrowDownOver);
		Resources::Remove(arrowUp);
		Resources::Remove(arrowUpOver);
		Resources::Remove(scrollbarBox);
		Resources::Remove(scrollbarBoxOver);
	}
	
	delete bgOptionsImg;
	
	Resources::Remove(bgOptions);
	Resources::Remove(bgOptionsEntry);
	
	delete trigA;
	delete trigHeldA;
	Resources::Remove(btnSoundOver);
	Resources::Remove(btnSoundClick);

	for(int i=0; i<PAGESIZE; i++)
	{
		delete optionTxt[i];
		delete optionVal[i];
		delete optionBg[i];
		delete optionBtn[i];
	}
}

void GuiOptionBrowser::SetCol2Position(int x)
{
	for(int i=0; i<PAGESIZE; i++)
		optionVal[i]->SetPosition(x,0);
}

void GuiOptionBrowser::SetFocus(int f)
{
	focus = f;

	for(int i=0; i<PAGESIZE; i++)
		optionBtn[i]->ResetState();

	if(f == 1)
		optionBtn[selectedItem]->SetState(STATE_SELECTED);
}

void GuiOptionBrowser::ResetState()
{
	if(state != STATE_DISABLED)
	{
		state = STATE_DEFAULT;
		stateChan = -1;
	}

	for(int i=0; i<PAGESIZE; i++)
	{
		optionBtn[i]->ResetState();
	}
}

int GuiOptionBrowser::GetClickedOption()
{
	int found = -1;
	for(int i=0; i<PAGESIZE; i++)
	{
		if(optionBtn[i]->GetState() == STATE_CLICKED)
		{
			optionBtn[i]->SetState(STATE_SELECTED);
			found = optionIndex[i];
			break;
		}
	}
	return found;
}

int GuiOptionBrowser::GetSelectedOption()
{
	int found = -1;
	for(int i=0; i<PAGESIZE; i++)
	{
		if(optionBtn[i]->GetState() == STATE_SELECTED)
		{
			optionBtn[i]->SetState(STATE_SELECTED);
			found = optionIndex[i];
			break;
		}
	}
	return found;
}

/****************************************************************************
 * FindMenuItem
 *
 * Help function to find the next visible menu item on the list
 ***************************************************************************/

int GuiOptionBrowser::FindMenuItem(int currentItem, int direction)
{
	int nextItem = currentItem + direction;

	if(nextItem < 0 || nextItem >= options->length)
		return -1;

	if(strlen(options->name[nextItem]) > 0)
		return nextItem;
	else
		return FindMenuItem(nextItem, direction);
}

/**
 * Draw the button on screen
 */
void GuiOptionBrowser::Draw()
{
	if(!this->IsVisible())
		return;

	bgOptionsImg->Draw();

	int next = listOffset;

	for(int i=0; i<PAGESIZE; i++)
	{
		if(next >= 0)
		{
			optionBtn[i]->Draw();
			next = this->FindMenuItem(next, 1);
		}
		else
			break;
	}

	if (scrollable != 0)
	{
		scrollbarImg->Draw();
		scrollbarBoxBtn->Draw();
		arrowUpBtn->Draw();
		arrowDownBtn->Draw();
	}
	
	this->UpdateEffects();
}

void GuiOptionBrowser::TriggerUpdate()
{
	listChanged = true;
}

void GuiOptionBrowser::Update(GuiTrigger * t)
{
	int next, prev, lang = options->length;
	int startat = 0, loaded = 0;
	
	if ((loaded == 0) && (startat>0))
	{
		if (startat > (lang-8)){
			listOffset= (lang-8);
			selectedItem=startat;
			optionBtn[selectedItem]->SetState(STATE_SELECTED, t->chan);
			}
		else if (startat < 8){
			selectedItem=startat;
			optionBtn[selectedItem]->SetState(STATE_SELECTED, t->chan);
			}
		else {
			listOffset = (startat-4);
			selectedItem=startat;
			optionBtn[selectedItem]->SetState(STATE_SELECTED, t->chan);
			}
		this->SetFocus(1);
		loaded = 1;
	}
	
	if(state == STATE_DISABLED || !t)
		return;
	
	// scrolldelay affects how fast the list scrolls
	// when the arrows are clicked
	float scrolldelay = 3.5;

	if (scrollable != 0) // update the location of the scroll box based on the position in the option list
	{
		arrowUpBtn->Update(t);
		arrowDownBtn->Update(t);
		scrollbarBoxBtn->Update(t);
    }
	
	next = listOffset;

	if(listChanged)
	{
		listChanged = false;
		for(int i=0; i<PAGESIZE; i++)
		{
			if(next >= 0)
			{
				if(optionBtn[i]->GetState() == STATE_DISABLED)
				{
					optionBtn[i]->SetVisible(true);
					optionBtn[i]->SetState(STATE_DEFAULT);
				}

				optionTxt[i]->SetText(options->name[next]);
				optionVal[i]->SetText(options->value[next]);
				optionIndex[i] = next;
				next = this->FindMenuItem(next, 1);
			}
			else
			{
				optionBtn[i]->SetVisible(false);
				optionBtn[i]->SetState(STATE_DISABLED);
			}
		}
	}

	for(int i=0; i<PAGESIZE; i++)
	{
		if(focus)
		{
			if(i != selectedItem && optionBtn[i]->GetState() == STATE_SELECTED)
				optionBtn[i]->ResetState();
			else if(i == selectedItem && optionBtn[i]->GetState() == STATE_DEFAULT)
				optionBtn[selectedItem]->SetState(STATE_SELECTED, t->chan);
		}

		optionBtn[i]->Update(t);
		
		if(optionBtn[i]->GetState() == STATE_SELECTED)
			selectedItem = i;
	}

	// pad/joystick navigation
	if(!focus)
		return; // skip navigation

	if (scrollable != 0) {

	if (t->Down() ||
	arrowDownBtn->GetState() == STATE_CLICKED || //down
	arrowDownBtn->GetState() == STATE_HELD)
	{

		next = this->FindMenuItem(optionIndex[selectedItem], 1);

		if(next >= 0)
		{
			if(selectedItem == PAGESIZE-1)
			{
				// move list down by 1
				listOffset = this->FindMenuItem(listOffset, 1);
			}
			else if(optionBtn[selectedItem+1]->IsVisible())
			{
				optionBtn[selectedItem]->ResetState();
				optionBtn[selectedItem+1]->SetState(STATE_SELECTED, t->chan);
				selectedItem++;
			}
			scrollbarBoxBtn->Draw();
			usleep(10000 * scrolldelay);


		}WPAD_ScanPads();
        u8 cnt, buttons = 0;
        /* Get pressed buttons */
        for (cnt = 0; cnt < 4; cnt++)
            buttons |= WPAD_ButtonsHeld(cnt);
        if (buttons == WPAD_BUTTON_A) {

        } else {
            arrowDownBtn->ResetState();

        }

	}
	else if(t->Up() ||
	arrowUpBtn->GetState() == STATE_CLICKED || ////////////////////////////////////////////up
	arrowUpBtn->GetState() == STATE_HELD)
	{
		prev = this->FindMenuItem(optionIndex[selectedItem], -1);

		if(prev >= 0)
		{
			if(selectedItem == 0)
			{
				// move list up by 1
				listOffset = prev;
			}
			else
			{
				optionBtn[selectedItem]->ResetState();
				optionBtn[selectedItem-1]->SetState(STATE_SELECTED, t->chan);
				selectedItem--;
			}
			scrollbarBoxBtn->Draw();
			usleep(10000 * scrolldelay);


		}WPAD_ScanPads();
        u8 cnt, buttons = 0;
        /* Get pressed buttons */
        for (cnt = 0; cnt < 4; cnt++)
            buttons |= WPAD_ButtonsHeld(cnt);
        if (buttons == WPAD_BUTTON_A) {

        } else {
            arrowUpBtn->ResetState();

        }
	}

    if(scrollbarBoxBtn->GetState() == STATE_HELD &&
		scrollbarBoxBtn->GetStateChan() == t->chan &&
		t->wpad->ir.valid && options->length > PAGESIZE)
    {
		scrollbarBoxBtn->SetPosition(12,0);
		int position = t->wpad->ir.y - 50 - scrollbarBoxBtn->GetTop();

		listOffset = (position * lang)/180 - selectedItem;

		if(listOffset <= 0)
		{
			listOffset = 0;
			selectedItem = 0;
		}
		else if(listOffset+PAGESIZE >= lang)
		{
			listOffset = lang-PAGESIZE;
			selectedItem = PAGESIZE-1;
		}

	}
        int positionbar = 237*(listOffset + selectedItem) / lang;

        if(positionbar > 216)
		positionbar = 216;
		scrollbarBoxBtn->SetPosition(12,positionbar+36);


    if(t->Right())
	{
		if(listOffset < lang && lang > PAGESIZE)
		{
			listOffset =listOffset+ PAGESIZE;
			if(listOffset+PAGESIZE >= lang)
            listOffset = lang-PAGESIZE;
		}
	}
	else if(t->Left())
	{
		if(listOffset > 0)
		{
			listOffset =listOffset- PAGESIZE;
			if(listOffset < 0)
				listOffset = 0;
		}
	}

    } else {


	if(t->Down())
	{
		next = this->FindMenuItem(optionIndex[selectedItem], 1);

		if(next >= 0)
		{
			if(selectedItem == PAGESIZE-1)
			{
				// move list down by 1
				listOffset = this->FindMenuItem(listOffset, 1);
				listChanged = true;
			}
			else if(optionBtn[selectedItem+1]->IsVisible())
			{
				optionBtn[selectedItem]->ResetState();
				optionBtn[selectedItem+1]->SetState(STATE_SELECTED, t->chan);
				selectedItem++;
			}
		}
	}
	else if(t->Up())
	{
		prev = this->FindMenuItem(optionIndex[selectedItem], -1);

		if(prev >= 0)
		{
			if(selectedItem == 0)
			{
				// move list up by 1
				listOffset = prev;
				listChanged = true;
			}
			else
			{
				optionBtn[selectedItem]->ResetState();
				optionBtn[selectedItem-1]->SetState(STATE_SELECTED, t->chan);
				selectedItem--;
			}
		}
	}
    }
	if(updateCB)
		updateCB(this);
}
