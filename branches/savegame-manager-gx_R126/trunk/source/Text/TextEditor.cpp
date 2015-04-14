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
 * for WiiXplorer 2009
 ***************************************************************************/
#include <unistd.h>

#include "TextEditor.h"
#include "../FileOperations/fileops.h"
#include "../Settings/CSettings.h"
#include "../Themes/CTheme.h"
#include "../menu.h"

#define FONTSIZE    17

/**
 * Constructor for the TextEditor class.
 */
TextEditor::TextEditor(const wchar_t *intext, int LinesToDraw, const char *path)
{
	triggerupdate = true;
	linestodraw = LinesToDraw;
	filesize = (u32) FileSize(path);

	filepath = new char[strlen(path)+1];
	snprintf(filepath, strlen(path)+1, "%s", path);

	char * filename = strrchr(filepath, '/')+1;

	trigA = new SimpleGuiTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);
	trigB = new GuiTrigger();
	trigB->SetButtonOnlyTrigger(-1, WiiControls.BackButton | ClassicControls.BackButton << 16, GCControls.BackButton);
	trigHome = new GuiTrigger();
	trigHome->SetButtonOnlyTrigger(-1, WiiControls.HomeButton | ClassicControls.HomeButton << 16, GCControls.HomeButton);

	bgTexteditorData = Resources::GetImageData("textreader_box.png");
	bgTexteditorImg = new GuiImage(bgTexteditorData);

	closeImgData = Resources::GetImageData("button_close.png");
	closeImgOverData = Resources::GetImageData("button_close_over.png");
	closeImg = new GuiImage(closeImgData);
	closeImgOver = new GuiImage(closeImgOverData);
	
	scrollbar = new Scrollbar(210, LISTMODE);
	scrollbar->SetParent(this);
	scrollbar->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	scrollbar->SetPosition(-35, 60);
	scrollbar->SetScrollSpeed(5);

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
	
	filenameTxt = new GuiText(filename, 20, thColor("r=0 g=0 b=0 a=255 - text reader filename text color"));
	filenameTxt->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	filenameTxt->SetPosition(-20,25);
	filenameTxt->SetMaxWidth(340, DOTTED);

	MainFileTxt = new Text(intext, FONTSIZE, thColor("r=0 g=0 b=0 a=255 - text reader file text color"));
	MainFileTxt->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	MainFileTxt->SetPosition(43, 65);
	MainFileTxt->SetLinesToDraw(linestodraw);
	MainFileTxt->SetMaxWidth(350);

	
	width = bgTexteditorImg->GetWidth();
	height = bgTexteditorImg->GetHeight();

	this->Append(bgTexteditorImg);
	this->Append(filenameTxt);
	this->Append(scrollbar);
	this->Append(closeBtn);
	this->Append(MainFileTxt);
	
	SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_IN, 50);
	SetPosition(0,0);
	SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
}

/**
 * Destructor for the TextEditor class.
 */
TextEditor::~TextEditor()
{
	ResumeGui();
	SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_OUT, 50);
	while(this->GetEffect() > 0) usleep(50);

	HaltGui();
	if(parentElement)
		((GuiWindow *) parentElement)->Remove(this);

	this->RemoveAll();

	delete scrollbar;

	/** Buttons **/
	delete closeBtn;
	
	/** Images **/
	delete bgTexteditorImg;
	delete closeImg;
	delete closeImgOver;
	
	/** ImageDatas **/
	delete bgTexteditorData;
	delete closeImgData;
	delete closeImgOverData;
	
	/** Triggers **/
	delete trigA;
	delete trigB;
	delete trigHome;

	/** Texts **/
	delete filenameTxt;
	delete MainFileTxt;
	delete [] filepath;

	ResumeGui();
}

void TextEditor::SetTriggerUpdate(bool set)
{
	LOCK(this);
	triggerupdate = set;
}

void TextEditor::SetText(const wchar_t *intext)
{
	LOCK(this);
	MainFileTxt->SetText(intext);
}

int TextEditor::GetState()
{
	return GuiWindow::GetState();
}

void TextEditor::ResetState()
{
	LOCK(this);
	state = STATE_DEFAULT;
	stateChan = -1;

	closeBtn->ResetState();
}


void TextEditor::Update(GuiTrigger * t)
{
	if(state == STATE_DISABLED || !t || !triggerupdate)
		return;

	scrollbar->Update(t);
	closeBtn->Update(t);
	
	if(scrollbar->ListChanged())
	{
		MainFileTxt->SetTextLine(scrollbar->GetSelectedItem()+scrollbar->GetSelectedIndex());
	}

	if(t->Right())
	{
		for(int i = 0; i < linestodraw; i++)
			MainFileTxt->NextLine();
	}
	else if(t->Left())
	{
		for(int i = 0; i < linestodraw; i++)
			MainFileTxt->PreviousLine();
	}
	else if(t->Down())
	{
		MainFileTxt->NextLine();
	}
	else if(t->Up())
	{
		MainFileTxt->PreviousLine();
	}

	if(closeBtn->GetState() == STATE_CLICKED)
		SetState(STATE_CLOSED);
	
	scrollbar->SetEntrieCount(MainFileTxt->GetTotalLinesCount());
	scrollbar->SetPageSize(linestodraw);
	scrollbar->SetRowSize(0);
	scrollbar->SetSelectedItem(0);
	scrollbar->SetSelectedIndex(MainFileTxt->GetCurrPos());
}
