/****************************************************************************
 * libwiigui
 *
 * gui_customoptionbrowser.cpp
 *
 * GUI class definitions
 ***************************************************************************/

#include "libwiigui/gui_browsers/gui_browsers.h"
#include "libwiigui/gui_browsers/gui_customoptionbrowser.h"
#include "libwiigui/gui_browsers/CustomOptionList.hpp"
#include "settings/settings.h"
#include "menu/menu.h"
#include "main.h"

#include <unistd.h>

#define GAMESELECTSIZE      30

/**
 * Constructor for the GuiCustomOptionBrowser class.
 */
GuiCustomOptionBrowser::GuiCustomOptionBrowser(int w, int h, CustomOptionList * l)
{
    width = w;
    height = h;
    options = l;
    selectable = true;
    selectedItem = 0;
    focus = 1; // allow focus
    coL2 = 50;
    scrollbaron = false;
    listOffset = 0;

    trigA = new SimpleGuiTrigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);
    trigHeldA = new GuiTrigger;
    trigHeldA->SetHeldTrigger(-1, WPAD_BUTTON_A, PAD_BUTTON_A);

    btnSoundClick = Resources::GetSound(button_click_wav, button_click_wav_size, cfg.SFXVolume);
	
	bgOptions = Resources::GetImageData(bg_options_settings_png, bg_options_settings_png_size);
	bgOptionsImg = new GuiImage(bgOptions);
    bgOptionsImg->SetParent(this);
    bgOptionsImg->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);

    bgOptionsEntry = Resources::GetImageData(bg_options_entry_png, bg_options_entry_png_size);

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
    arrowUpBtn->SetImageHold(arrowUpOverImg);
    arrowUpBtn->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
    arrowUpBtn->SetPosition(width / 2 - 18 + 7, -18);
    arrowUpBtn->SetSelectable(false);
    arrowUpBtn->SetTrigger(trigA);
    arrowUpBtn->SetEffectOnOver(EFFECT_SCALE, 50, 130);
    arrowUpBtn->SetSoundClick(btnSoundClick);

    arrowDownBtn = new GuiButton(arrowDownImg->GetWidth(), arrowDownImg->GetHeight());
    arrowDownBtn->SetParent(this);
    arrowDownBtn->SetImage(arrowDownImg);
    arrowDownBtn->SetImageOver(arrowDownOverImg);
    arrowDownBtn->SetImageHold(arrowDownOverImg);
    arrowDownBtn->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
    arrowDownBtn->SetPosition(width / 2 - 18 + 7, 18);
    arrowDownBtn->SetSelectable(false);
    arrowDownBtn->SetTrigger(trigA);
    arrowDownBtn->SetEffectOnOver(EFFECT_SCALE, 50, 130);
    arrowDownBtn->SetSoundClick(btnSoundClick);

    scrollbarBoxBtn = new GuiButton(scrollbarBoxImg->GetWidth(), scrollbarBoxImg->GetHeight());
    scrollbarBoxBtn->SetParent(this);
    scrollbarBoxBtn->SetImage(scrollbarBoxImg);
    scrollbarBoxBtn->SetImageOver(scrollbarBoxOverImg);
    scrollbarBoxBtn->SetImageHold(scrollbarBoxOverImg);
    scrollbarBoxBtn->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
    scrollbarBoxBtn->SetSelectable(false);
    scrollbarBoxBtn->SetEffectOnOver(EFFECT_SCALE, 50, 120);
    scrollbarBoxBtn->SetMinY(0);
    scrollbarBoxBtn->SetMaxY(height - 30);
    scrollbarBoxBtn->SetHoldable(true);
    scrollbarBoxBtn->SetTrigger(trigHeldA);

    for (int i = 0; i < PAGESIZE; i++)
    {
        optionTxt[i] = new GuiText(NULL, 20, (GXColor) {0, 0, 0, 255});
        optionTxt[i]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
        optionTxt[i]->SetPosition(24, 0);
        optionTxt[i]->SetMaxWidth(bgOptionsImg->GetWidth() - (coL2 + 24), GuiText::SCROLL);

        optionBg[i] = new GuiImage(bgOptionsEntry);

        optionVal[i] = new GuiText(NULL, 20, (GXColor) {0, 0, 0, 255});
        optionVal[i]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);

        optionValOver[i] = new GuiText(NULL, 20, (GXColor) {0, 0, 0, 255});
        optionValOver[i]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);

        optionBtn[i] = new GuiButton(width - 28, GAMESELECTSIZE);
        optionBtn[i]->SetParent(this);
        optionBtn[i]->SetLabel(optionTxt[i], 0);
        optionBtn[i]->SetLabel(optionVal[i], 1);
        optionBtn[i]->SetLabelOver(optionValOver[i], 1);
        optionBtn[i]->SetImageOver(optionBg[i]);
        optionBtn[i]->SetPosition(10, GAMESELECTSIZE * i + 4);
        optionBtn[i]->SetRumble(false);
        optionBtn[i]->SetTrigger(trigA);
        optionBtn[i]->SetSoundClick(btnSoundClick);
    }
}

/**
 * Destructor for the GuiCustomOptionBrowser class.
 */
GuiCustomOptionBrowser::~GuiCustomOptionBrowser()
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

    delete bgOptionsImg;
    Resources::Remove(bgOptions);
    Resources::Remove(bgOptionsEntry);

    Resources::Remove(btnSoundClick);
	
	delete trigA;
    delete trigHeldA;

    for (int i = 0; i < PAGESIZE; i++)
    {
        delete optionTxt[i];
        delete optionVal[i];
        delete optionValOver[i];
        delete optionBg[i];
        delete optionBtn[i];
    }
}

void GuiCustomOptionBrowser::SetFocus(int f)
{
    focus = f;

    for (int i = 0; i < PAGESIZE; i++)
        optionBtn[i]->ResetState();

    if (f == 1) optionBtn[selectedItem]->SetState(STATE_SELECTED);
}

void GuiCustomOptionBrowser::ResetState()
{
    if (state != STATE_DISABLED)
    {
        state = STATE_DEFAULT;
        stateChan = -1;
    }

    for (int i = 0; i < PAGESIZE; i++)
    {
        optionBtn[i]->ResetState();
    }
}

int GuiCustomOptionBrowser::GetClickedOption()
{
    for (int i = 0; i < PAGESIZE; i++)
    {
        if (optionBtn[i]->GetState() == STATE_CLICKED)
        {
            optionBtn[i]->SetState(STATE_SELECTED);
            return optionIndex[i];
        }
    }

    return -1;
}

int GuiCustomOptionBrowser::GetSelectedOption()
{
    for (int i = 0; i < PAGESIZE; i++)
    {
        if (optionBtn[i]->GetState() == STATE_SELECTED)
        {
            return optionIndex[i];
        }
    }
    return -1;
}

void GuiCustomOptionBrowser::SetClickable(bool enable)
{
    for (int i = 0; i < PAGESIZE; i++)
    {
        optionBtn[i]->SetClickable(enable);
    }
}

void GuiCustomOptionBrowser::SetOffset(int optionnumber)
{
    listOffset = optionnumber;
    selectedItem = optionnumber;
}

/****************************************************************************
 * FindMenuItem
 *
 * Help function to find the next visible menu item on the list
 ***************************************************************************/

int GuiCustomOptionBrowser::FindMenuItem(int currentItem, int direction)
{
    int nextItem = currentItem + direction;

    if (nextItem < 0 || nextItem >= options->GetLength()) return -1;

    if (strlen(options->GetName(nextItem)) > 0)
        return nextItem;

    return FindMenuItem(nextItem, direction);
}

/**
 * Draw the button on screen
 */
void GuiCustomOptionBrowser::Draw()
{
    if (!this->IsVisible()) return;

    bgOptionsImg->Draw();

    int next = listOffset;

    for (int i = 0; i < PAGESIZE; i++)
    {
        if (next >= 0)
        {
            optionBtn[i]->Draw();
            next = this->FindMenuItem(next, 1);
        }
        else break;
    }

    if (PAGESIZE < options->GetLength())
    {
        scrollbarImg->Draw();
        arrowUpBtn->Draw();
        arrowDownBtn->Draw();
        scrollbarBoxBtn->Draw();
    }
    this->UpdateEffects();
}

void GuiCustomOptionBrowser::UpdateListEntries()
{
    //LOCK(this);
    scrollbaron = options->GetLength() > PAGESIZE;
    if (listOffset < 0) listOffset = this->FindMenuItem(-1, 1);
    int next = listOffset;

    int maxNameWidth = 0;
    for (int i = 0; i < PAGESIZE; i++)
    {
        if (next >= 0)
        {
            if (optionBtn[i]->GetState() == STATE_DISABLED)
            {
                optionBtn[i]->SetVisible(true);
                optionBtn[i]->SetState(STATE_DEFAULT);
            }

            optionTxt[i]->SetText(options->GetName(next));
            if (maxNameWidth < optionTxt[i]->GetTextWidth()) maxNameWidth = optionTxt[i]->GetTextWidth();
            optionVal[i]->SetText(options->GetValue(next));
            optionValOver[i]->SetText(options->GetValue(next));

            optionIndex[i] = next;
            next = this->FindMenuItem(next, 1);
        }
        else
        {
            optionBtn[i]->SetVisible(false);
            optionBtn[i]->SetState(STATE_DISABLED);
        }
    }

    if (coL2 < (24 + maxNameWidth + 16))
        coL2 = 24 + maxNameWidth + 16;

    for (int i = 0; i < PAGESIZE; i++)
    {
        if (optionBtn[i]->GetState() != STATE_DISABLED)
        {
            optionVal[i]->SetPosition(coL2, 0);
            optionVal[i]->SetMaxWidth(bgOptionsImg->GetWidth() - (coL2 + 24), GuiText::SCROLL);

            optionValOver[i]->SetPosition(coL2, 0);
            optionValOver[i]->SetMaxWidth(bgOptionsImg->GetWidth() - (coL2 + 24), GuiText::SCROLL);
        }
    }
}

void GuiCustomOptionBrowser::Update(GuiTrigger * t)
{
    if (state == STATE_DISABLED || !t) return;

    int next, prev, length = options->GetLength();
    int old_listOffset = listOffset;

    btnSoundClick->SetVolume(cfg.SFXVolume);
	
	if (length < PAGESIZE)
    {
        // update the location of the scroll box based on the position in the option list
        arrowUpBtn->Update(t);
        arrowDownBtn->Update(t);
        scrollbarBoxBtn->Update(t);
    }

    if(options->IsChanged())
        UpdateListEntries();

    next = listOffset;

    if (!(t->Up() ||
	arrowUpBtn->GetState() == STATE_CLICKED || ////////////////////////////////////////////up
	arrowUpBtn->GetState() == STATE_HELD ||
	t->Down() ||
	arrowDownBtn->GetState() == STATE_CLICKED || //down
	arrowDownBtn->GetState() == STATE_HELD))
    {
        for (int i = 0; i < PAGESIZE; i++)
        {
            if (next >= 0) next = this->FindMenuItem(next, 1);

            if (focus)
            {
                if (i != selectedItem && optionBtn[i]->GetState() == STATE_SELECTED)
                {
                    optionBtn[i]->ResetState();
                }
                else if (i == selectedItem && optionBtn[i]->GetState() == STATE_DEFAULT)
                {
                    optionBtn[selectedItem]->SetState(STATE_SELECTED);
                }
            }

            optionBtn[i]->Update(t);

            if (optionBtn[i]->GetState() == STATE_SELECTED)
            {
                selectedItem = i;
            }
        }
    }

    // pad/joystick navigation
    if (!focus) return; // skip navigation

    if (t->Down())
    {
        next = this->FindMenuItem(optionIndex[selectedItem], 1);

        if (next >= 0)
        {
            if (selectedItem == PAGESIZE - 1)
            {
                // move list down by 1
                listOffset = this->FindMenuItem(listOffset, 1);
            }
            else if (optionBtn[selectedItem + 1]->IsVisible())
            {
                optionBtn[selectedItem]->ResetState();
                optionBtn[selectedItem + 1]->SetState(STATE_SELECTED, t->chan);
                selectedItem++;
            }
        }
    }
    else if (t->Up())
    {
        prev = this->FindMenuItem(optionIndex[selectedItem], -1);

        if (prev >= 0)
        {
            if (selectedItem == 0)
            {
                // move list up by 1
                listOffset = prev;
            }
            else
            {
                optionBtn[selectedItem]->ResetState();
                optionBtn[selectedItem - 1]->SetState(STATE_SELECTED, t->chan);
                selectedItem--;
            }
        }
    }

    if (!scrollbaron)
        return;

    if (arrowDownBtn->GetState() == STATE_CLICKED || arrowDownBtn->GetState() == STATE_HELD)
    {
        next = this->FindMenuItem(optionIndex[selectedItem], 1);

        if (next >= 0)
        {
            if (selectedItem == PAGESIZE - 1)
            {
                // move list down by 1
                listOffset = this->FindMenuItem(listOffset, 1);
            }
            else if (optionBtn[selectedItem + 1]->IsVisible())
            {
                optionBtn[selectedItem]->ResetState();
                optionBtn[selectedItem + 1]->SetState(STATE_SELECTED, t->chan);
                selectedItem++;
            }
            scrollbarBoxBtn->Draw();
            usleep(35000);
        }
        WPAD_ScanPads();
        u8 cnt, buttons = 0;
        /* Get pressed buttons */
        for (cnt = 0; cnt < 4; cnt++)
            buttons |= WPAD_ButtonsHeld(cnt);
        if (buttons == WPAD_BUTTON_A)
		{
            arrowDownBtn->ResetState();
        }
    }
    else if (arrowUpBtn->GetState() == STATE_CLICKED || arrowUpBtn->GetState() == STATE_HELD)
    {
        prev = this->FindMenuItem(optionIndex[selectedItem], -1);

        if (prev >= 0)
        {
            if (selectedItem == 0)
            {
                // move list up by 1
                listOffset = prev;
            }
            else
            {
                optionBtn[selectedItem]->ResetState();
                optionBtn[selectedItem - 1]->SetState(STATE_SELECTED, t->chan);
                selectedItem--;
            }
            scrollbarBoxBtn->Draw();
            usleep(35000);
        }
        WPAD_ScanPads();
        u8 cnt, buttons = 0;
        /* Get pressed buttons */
        for (cnt = 0; cnt < 4; cnt++)
            buttons |= WPAD_ButtonsHeld(cnt);
        if (buttons == WPAD_BUTTON_A)
        {
            arrowUpBtn->ResetState();
        }
    }

    if (scrollbarBoxBtn->GetState() == STATE_HELD && scrollbarBoxBtn->GetStateChan() == t->chan && t->wpad->ir.valid
            && options->GetLength() > PAGESIZE)
    {
        scrollbarBoxBtn->SetPosition(width / 2 - 18 + 7, 0);

        int position = t->wpad->ir.y - 50 - scrollbarBoxBtn->GetTop();

        listOffset = (position * length) / 180 - selectedItem;

        if (listOffset <= 0)
        {
            listOffset = 0;
            selectedItem = 0;
        }
        else if (listOffset + PAGESIZE >= length)
        {
            listOffset = length - PAGESIZE;
            selectedItem = PAGESIZE - 1;
        }
    }
    int positionbar = 237 * (listOffset + selectedItem) / length;

    if (positionbar > 216) positionbar = 216;
    scrollbarBoxBtn->SetPosition(width / 2 - 18 + 7, positionbar + 8);

    if (t->Right())
    {
        if (listOffset < length && length > PAGESIZE)
        {
            listOffset = listOffset + PAGESIZE;
            if (listOffset + PAGESIZE >= length) listOffset = length - PAGESIZE;
        }
    }
    else if (t->Left())
    {
        if (listOffset > 0)
        {
            listOffset = listOffset - PAGESIZE;
            if (listOffset < 0) listOffset = 0;
        }
    }

    if (old_listOffset != listOffset)
        UpdateListEntries();

    if (updateCB) updateCB(this);
}
