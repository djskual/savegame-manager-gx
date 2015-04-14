 /****************************************************************************
 * Copyright (C) 2009
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
 *
 * TextEditor.cpp
 * for SaveGame Manager GX 2010
 ***************************************************************************/

#include "text/TextEditor.h"
#include "main.h"

typedef struct _ReplaceStruct
{
    const char * orig;
    char replace;
    short size;
} ReplaceStruct;

//! More replacements can be added if needed
static const ReplaceStruct Replacements[] =
{
    { "&gt;", '>', 4 },
    { "&lt;", '<', 4 },
    { "&quot;", '\"', 6 },
    { "&apos;", '\'', 6 },
    { "&amp;", '&', 5 },
    { NULL, '\0', 0 }
};

static inline char * CleanText(char * in_text)
{
    if(!in_text)
        return NULL;

    const char * ptr = in_text;
    char * text = in_text;

    while(*ptr != '\0')
    {
        for(int i = 0; Replacements[i].orig != 0; ++i)
        {
            if(strncmp(ptr, Replacements[i].orig, Replacements[i].size) == 0)
            {
                ptr += Replacements[i].size;
                *text = Replacements[i].replace;
                ++text;
                i = 0;
                continue;
            }
        }

        if(*ptr == '\r')
        {
            ++ptr;
            continue;
        }

        *text = *ptr;
        ++ptr;
        ++text;
    }

    *text = '\0';

    return in_text;
}

/**
 * Constructor for the TextEditor class.
 */
TextEditor::TextEditor(char *intext, int LinesToDraw, char *filename)
{
	focus = true; // allow focus
	triggerdisabled = false;
	linestodraw = LinesToDraw;
	currentLine = 0;

	trigA = new SimpleGuiTrigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);
	trigHeldA = new GuiTrigger;
	trigHeldA->SetHeldTrigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);
	trigB = new GuiTrigger;
	trigB->SetButtonOnlyTrigger(-1, WPAD_BUTTON_B | WPAD_CLASSIC_BUTTON_B, PAD_BUTTON_B);
	trigHome = new GuiTrigger;
	trigHome->SetButtonOnlyTrigger(-1, WPAD_BUTTON_HOME | WPAD_CLASSIC_BUTTON_HOME, 0);
	
	btnSoundOver = Resources::GetSound(button_over_wav, button_over_wav_size, cfg.SFXVolume);
	btnSoundClick = Resources::GetSound(button_click_wav, button_click_wav_size, cfg.SFXVolume);
	
	bgTexteditorData = Resources::GetImageData(textreader_box_png, textreader_box_png_size);
	bgTexteditorImg = new GuiImage(bgTexteditorData);

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
	closeImgData = Resources::GetImageData(close_png, close_png_size);
	closeImgOverData = Resources::GetImageData(close_over_png, close_over_png_size);
    closeImg = new GuiImage(closeImgData);
    closeImgOver = new GuiImage(closeImgOverData);
	
	arrowUpBtn = new GuiButton(arrowUpImg->GetWidth(), arrowUpImg->GetHeight());
	arrowUpBtn->SetImage(arrowUpImg);
	arrowUpBtn->SetImageOver(arrowUpOverImg);
	arrowUpBtn->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	arrowUpBtn->SetPosition(-33, 55);
	arrowUpBtn->SetSelectable(false);
	arrowUpBtn->SetClickable(false);
	arrowUpBtn->SetHoldable(true);
	arrowUpBtn->SetTrigger(trigHeldA);
	arrowUpBtn->SetSoundOver(btnSoundOver);
	arrowUpBtn->SetSoundClick(btnSoundClick);
	arrowUpBtn->SetEffectGrow();

	arrowDownBtn = new GuiButton(arrowDownImg->GetWidth(), arrowDownImg->GetHeight());
	arrowDownBtn->SetImage(arrowDownImg);
	arrowDownBtn->SetImageOver(arrowDownOverImg);
	arrowDownBtn->SetAlignment(ALIGN_RIGHT, ALIGN_BOTTOM);
	arrowDownBtn->SetPosition(-32, -46);
	arrowDownBtn->SetSelectable(false);
	arrowDownBtn->SetClickable(false);
	arrowDownBtn->SetHoldable(true);
	arrowDownBtn->SetTrigger(trigHeldA);
	arrowDownBtn->SetSoundOver(btnSoundOver);
	arrowDownBtn->SetSoundClick(btnSoundClick);
	arrowDownBtn->SetEffectGrow();

	scrollbarBoxBtn = new GuiButton(scrollbarBoxImg->GetWidth(), scrollbarBoxImg->GetHeight());
	scrollbarBoxBtn->SetImage(scrollbarBoxImg);
	scrollbarBoxBtn->SetImageOver(scrollbarBoxOverImg);
	scrollbarBoxBtn->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	scrollbarBoxBtn->SetPosition(-32, 50+36);
	scrollbarBoxBtn->SetMinY(0);
	scrollbarBoxBtn->SetMaxY(112);
	scrollbarBoxBtn->SetSelectable(false);
	scrollbarBoxBtn->SetClickable(false);
	scrollbarBoxBtn->SetHoldable(true);
	scrollbarBoxBtn->SetTrigger(trigHeldA);
	scrollbarBoxBtn->SetEffectGrow();

    closeBtn = new GuiButton(closeImg->GetWidth(), closeImg->GetHeight());
    closeBtn->SetImage(closeImg);
    closeBtn->SetImageOver(closeImgOver);
    closeBtn->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
    closeBtn->SetPosition(-22, 25);
    closeBtn->SetSoundOver(btnSoundOver);
    closeBtn->SetSoundClick(btnSoundClick);
    closeBtn->SetTrigger(trigA);
    closeBtn->SetTrigger(trigB);
    closeBtn->SetTrigger(trigHome);
    closeBtn->SetEffectGrow();

    filenameTxt = new GuiText(filename, 20, (GXColor){0, 0, 0, 255});
    filenameTxt->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
    filenameTxt->SetPosition(-20,25);
    filenameTxt->SetMaxWidth(340, GuiText::SCROLL);

    char * in_text = CleanText(intext);
	MainFileTxt = new GuiText(in_text, 17, (GXColor){0, 0, 0, 255});
    MainFileTxt->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
    MainFileTxt->SetPosition(43, 65);
    MainFileTxt->SetMaxWidth(350, GuiText::LONGTEXT);
    MainFileTxt->SetFirstLine(0);
    MainFileTxt->SetLinesToDraw(linestodraw);
	
    TotalLines = MainFileTxt->GetTotalLines();
	if(TotalLines<linestodraw) {
		linestodraw = TotalLines;
		MainFileTxt->SetLinesToDraw(linestodraw);
	}
	
	Window = new GuiWindow(bgTexteditorImg->GetWidth(), bgTexteditorImg->GetHeight());
	Window->SetParent(this);

    Window->Append(bgTexteditorImg);
    Window->Append(filenameTxt);
    Window->Append(MainFileTxt);
    Window->Append(scrollbarBoxBtn);
    Window->Append(arrowUpBtn);
    Window->Append(arrowDownBtn);
    Window->Append(closeBtn);
}

/**
 * Destructor for the TextEditor class.
 */
TextEditor::~TextEditor()
{
    Window->RemoveAll();

    /** Buttons **/
	delete arrowUpBtn;
	delete arrowDownBtn;
	delete scrollbarBoxBtn;
	delete closeBtn;

    /** Images **/
	delete bgTexteditorImg;
	delete arrowDownImg;
	delete arrowDownOverImg;
	delete arrowUpImg;
	delete arrowUpOverImg;
	delete scrollbarBoxImg;
	delete scrollbarBoxOverImg;
	delete closeImg;
	delete closeImgOver;
	
    /** ImageDatas **/
	Resources::Remove(bgTexteditorData);
	Resources::Remove(arrowDown);
	Resources::Remove(arrowDownOver);
	Resources::Remove(arrowUp);
	Resources::Remove(arrowUpOver);
	Resources::Remove(scrollbarBox);
	Resources::Remove(scrollbarBoxOver);
	Resources::Remove(closeImgData);
	Resources::Remove(closeImgOverData);

    /** Sounds **/
	Resources::Remove(btnSoundOver);
	Resources::Remove(btnSoundClick);

    /** Triggers **/
	delete trigHeldA;
	delete trigHome;
	delete trigA;
	delete trigB;

    /** Texts **/
    delete filenameTxt;
    delete MainFileTxt;

    /** Window **/
    delete Window;
}

void TextEditor::SetFocus(bool f)
{
    focus = f;
}

void TextEditor::SetText(char *intext)
{
    if(MainFileTxt)
        delete MainFileTxt;

    char * in_text = CleanText(intext);
	MainFileTxt = new GuiText(in_text, 17, (GXColor){0, 0, 0, 255});
    MainFileTxt->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
    MainFileTxt->SetPosition(43, 65);
    MainFileTxt->SetMaxWidth(350, GuiText::LONGTEXT);
    MainFileTxt->SetFirstLine(0);
    MainFileTxt->SetLinesToDraw(linestodraw);
}

void TextEditor::DisableTriggerUpdate(bool set)
{
    triggerdisabled = set;
}

void TextEditor::SetAlignment(int h, int v)
{
    GuiElement::SetAlignment(h, v);
	Window->SetAlignment(h, v);
}

void TextEditor::ResetState()
{
    state = STATE_DEFAULT;
	stateChan = -1;

	arrowUpBtn->ResetState();
	arrowDownBtn->ResetState();
	scrollbarBoxBtn->ResetState();
	closeBtn->ResetState();
}

/**
 * Draw the window
 */
void TextEditor::Draw()
{
	if(!this->IsVisible())
		return;

	Window->Draw();

	this->UpdateEffects();
}

void TextEditor::Update(GuiTrigger * t)
{
	if(state == STATE_DISABLED || !t || triggerdisabled)
		return;

	int position = 0;
	int positionWiimote = 0;
	int oldCurrentLine = currentLine;

	arrowUpBtn->Update(t);
	arrowDownBtn->Update(t);
	scrollbarBoxBtn->Update(t);
	closeBtn->Update(t);

    //!This is temporary till we do a callback event
	if(closeBtn->GetState() == STATE_CLICKED)
        this->SetState(STATE_CLICKED);

	if(TotalLines <= linestodraw)
        return;

	
	if(scrollbarBoxBtn->GetState() == STATE_HELD &&
		scrollbarBoxBtn->GetStateChan() == t->chan &&
		t->wpad->ir.valid)
	{
		scrollbarBoxBtn->SetPosition(-32, 50);
		positionWiimote = t->wpad->ir.y - 60 - scrollbarBoxBtn->GetTop();

		if(positionWiimote < scrollbarBoxBtn->GetMinY())
			positionWiimote = scrollbarBoxBtn->GetMinY();
		else if(positionWiimote > scrollbarBoxBtn->GetMaxY())
			positionWiimote = scrollbarBoxBtn->GetMaxY();

		currentLine = (positionWiimote * TotalLines)/112.0;

        if(currentLine+linestodraw > TotalLines)
            currentLine = TotalLines-linestodraw;
        if(currentLine < 0)
            currentLine = 0;

		if(oldCurrentLine != currentLine)
			MainFileTxt->SetFirstLine(currentLine);
	}

	if(arrowDownBtn->GetState() == STATE_HELD &&
	   arrowDownBtn->GetStateChan() == t->chan &&
	   currentLine<(TotalLines-linestodraw))
	{
        currentLine++;
        if(currentLine+linestodraw > TotalLines)
            currentLine = TotalLines-linestodraw;
        if(currentLine < 0)
            currentLine = 0;

        MainFileTxt->SetFirstLine(currentLine);
	}
	else if(arrowUpBtn->GetState() == STATE_HELD &&
			arrowUpBtn->GetStateChan() == t->chan &&
			currentLine>0)
	{
		currentLine--;
        if(currentLine < 0)
            currentLine = 0;
        MainFileTxt->SetFirstLine(currentLine);
	}

	if(t->Right() && currentLine<(TotalLines-linestodraw))
	{
		currentLine += linestodraw;
        if(currentLine+linestodraw > TotalLines)
            currentLine = TotalLines-linestodraw;
        if(currentLine < 0)
            currentLine = 0;

        MainFileTxt->SetFirstLine(currentLine);
	}
	else if(t->Left() && currentLine>0)
	{
		currentLine -= linestodraw;
        if(currentLine < 0)
            currentLine = 0;

        MainFileTxt->SetFirstLine(currentLine);
	}
	else if(t->Down() && currentLine<(TotalLines-linestodraw))
	{
		currentLine++;
        if(currentLine+linestodraw > TotalLines)
            currentLine = TotalLines-linestodraw;
        if(currentLine < 0)
            currentLine = 0;

        MainFileTxt->SetFirstLine(currentLine);
	}
	else if(t->Up() && currentLine>0)
	{
		currentLine--;
        if(currentLine < 0)
            currentLine = 0;
		
        MainFileTxt->SetFirstLine(currentLine);
	}

	// update the location of the scroll box based on the position in the file list
	if(positionWiimote > 0)
	{
		position = positionWiimote; // follow wiimote cursor
	}
	else
	{
		position = 112*(currentLine + linestodraw/2.0) / (TotalLines*1.0);

		if(currentLine < 1)
			position = 0;
		else if((currentLine+linestodraw) >= TotalLines)
			position = 112;
	}

	scrollbarBoxBtn->SetPosition(-32,position+36+50);

	if(updateCB)
		updateCB(this);
}
