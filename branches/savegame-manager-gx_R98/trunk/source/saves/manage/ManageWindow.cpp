 /****************************************************************************
 * Copyright (C) 2010
 * by Dj_Skual
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
 *
 * ManageWindow.cpp
 *
 * for SaveGame Manager GX 2010
 ***************************************************************************/

#include <gccore.h>
#include <unistd.h>

#include "saves/title.h"
#include "saves/utils.h"
#include "saves/manage/ManageWindow.h"
#include "saves/manage/ManageButtons.h"
#include "prompts/browsers/browsers.h"
#include "menu/menu.h"
#include "main.h"

/*** Extern functions ***/
extern void ResumeGui();
extern void HaltGui();

/**
 * Constructor for the ManageWindow class.
 */
ManageWindow::ManageWindow (const char *name,
							const char *subname,
							float block,
							const char *ID,
							u64 tid,
							int supported,
							int selected)
{
	notSupportedFormat = supported;
	choice = 0;
	SaveInstalled = true;
	move = windowinfo.move;
	
	if(subname) {
		if(!strncmp(subname, " ", 1))
			subname = NULL;
	}
	
	trigA = new SimpleGuiTrigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);
	trigB = new GuiTrigger;
	trigB->SetButtonOnlyTrigger(-1, WPAD_BUTTON_B | WPAD_CLASSIC_BUTTON_B, PAD_BUTTON_B);
	trigHome = new GuiTrigger;
	trigHome->SetButtonOnlyTrigger(-1, WPAD_BUTTON_HOME | WPAD_CLASSIC_BUTTON_HOME, PAD_BUTTON_START);
	trigMinus = new GuiTrigger;
	trigMinus->SetButtonOnlyTrigger(-1, WPAD_BUTTON_MINUS | WPAD_CLASSIC_BUTTON_MINUS, 0);
	trigPlus = new GuiTrigger;
	trigPlus->SetButtonOnlyTrigger(-1, WPAD_BUTTON_PLUS | WPAD_CLASSIC_BUTTON_PLUS, 0);
	trigLeft = new GuiTrigger;
	trigLeft->SetButtonOnlyTrigger(-1, WPAD_BUTTON_LEFT | WPAD_CLASSIC_BUTTON_LEFT, PAD_BUTTON_LEFT);
	trigRight = new GuiTrigger;
	trigRight->SetButtonOnlyTrigger(-1, WPAD_BUTTON_RIGHT | WPAD_CLASSIC_BUTTON_RIGHT, PAD_BUTTON_RIGHT);
	
	btnSoundOver = Resources::GetSound(button_over_wav, button_over_wav_size, cfg.SFXVolume);
	btnClick = Resources::GetSound(button_click_wav, button_click_wav_size, cfg.SFXVolume);
	
	dialogBox = Resources::GetImageData(save_manage_bg_png, save_manage_bg_png_size);
	dialogBoxImg = new GuiImage(dialogBox);

	iconBox = Resources::GetImageData(save_icon_bg_png, save_icon_bg_png_size);
	iconBoxImg = new GuiImage(iconBox);
	iconBoxImg->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	iconBoxImg->SetPosition(-190,-113);

	// Get The SaveIcon //
	char savepath[128];
	snprintf(savepath, sizeof(savepath),"/title/%08x/%08x/data/banner.bin",TITLE_UPPER(tid),TITLE_LOWER(tid));
	u8* buffer = NULL;
	u32 ret = read_isfs(savepath, &buffer);
	if(!ret){
		SaveInstalled = false;
		
		snprintf(savepath, sizeof(savepath),"%s%s/banner.bin", navinfo.LastBrowsedPath, browserDevice[selected].filename);
		FILE *fp = fopen(savepath,"rb");
		if(fp !=NULL) {
			fseek (fp , 0 , SEEK_END);
			ret = ftell (fp);
			rewind (fp);
			buffer = (u8*)memalign(32, ret);
			if(!buffer)
				fclose(fp);
			fread(buffer, 1, ret, fp);
			fclose(fp);
		}
	}
	icon = new GuiSaveIcon(buffer, ret);
	icon->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	icon->SetPosition(-195,-116);
	free(buffer);
	
	nameTxt = new GuiText(name, 35, (GXColor){255, 255, 255, 255});
    nameTxt->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
    nameTxt->SetPosition(184,10);
    nameTxt->SetMaxWidth(320, GuiText::SCROLL);

	subnameTxt = new GuiText(subname, 22, (GXColor){255, 255, 255, 255});
    subnameTxt->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
    subnameTxt->SetPosition(184,50);
    subnameTxt->SetMaxWidth(350, GuiText::SCROLL);

	char blocktext[10];
	snprintf(blocktext, sizeof(blocktext), "%.0f", ceil(block));
	
	blockTxt = new GuiText(blocktext, 28, (GXColor){255, 255, 255, 255});
    blockTxt->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
    blockTxt->SetPosition(-70,-80);
    blockTxt->SetMaxWidth(55, GuiText::SCROLL);

	blockBox = Resources::GetImageData(save_manage_block_png, save_manage_block_png_size);
	blockBoxImg = new GuiImage(blockBox);
	blockBoxImg->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	blockBoxImg->SetPosition(-70,-82);
	
	idTxt = new GuiText(ID, 28, (GXColor){255, 255, 255, 255});
    idTxt->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
    idTxt->SetPosition(40,-80);
    idTxt->SetMaxWidth(85, GuiText::SCROLL);

	idBox = Resources::GetImageData(save_manage_block_id_png, save_manage_block_id_png_size);
	idBoxImg = new GuiImage(idBox);
	idBoxImg->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	idBoxImg->SetPosition(40,-82);

	close = Resources::GetImageData(close_png, close_png_size);
	closeImg = new GuiImage(close);
    closeOver = Resources::GetImageData(close_over_png, close_over_png_size);
    closeOverImg = new GuiImage(closeOver);
	
	closeBtn = new GuiButton(closeImg->GetWidth(), closeImg->GetHeight());
    closeBtn->SetImage(closeImg);
    closeBtn->SetImageOver(closeOverImg);
    closeBtn->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
    closeBtn->SetPosition(-22, 10);
    closeBtn->SetSoundOver(btnSoundOver);
    closeBtn->SetSoundClick(btnClick);
    closeBtn->SetTrigger(trigA);
    closeBtn->SetTrigger(trigB);
    closeBtn->SetTrigger(trigHome);
    closeBtn->SetEffectGrow();

    left = Resources::GetImageData(arrow_left_png, arrow_left_png_size);
	leftImg = new GuiImage(left);
    
	leftBtn = new GuiButton(leftImg->GetWidth(), leftImg->GetHeight());
    leftBtn->SetImage(leftImg);
    leftBtn->SetImageOver(leftImg);
    leftBtn->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
    leftBtn->SetPosition(-23, 0);
    leftBtn->SetSoundOver(btnSoundOver);
    leftBtn->SetSoundClick(btnClick);
    leftBtn->SetTrigger(trigA);
    leftBtn->SetTrigger(trigLeft);
    leftBtn->SetTrigger(trigMinus);
    leftBtn->SetEffectGrow();

    right = Resources::GetImageData(arrow_right_png, arrow_right_png_size);
	rightImg = new GuiImage(right);
    
	rightBtn = new GuiButton(rightImg->GetWidth(), rightImg->GetHeight());
    rightBtn->SetImage(rightImg);
    rightBtn->SetImageOver(rightImg);
    rightBtn->SetAlignment(ALIGN_RIGHT, ALIGN_MIDDLE);
    rightBtn->SetPosition(10, 0);
    rightBtn->SetSoundOver(btnSoundOver);
    rightBtn->SetSoundClick(btnClick);
    rightBtn->SetTrigger(trigA);
    rightBtn->SetTrigger(trigRight);
    rightBtn->SetTrigger(trigPlus);
    rightBtn->SetEffectGrow();

	warningTxt = new GuiText(tr("WARNING: You need to have a save already installed in the Wii to install this one!!!"), 20, (GXColor){255, 255, 255, 255});
    warningTxt->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
    warningTxt->SetPosition(-6,-20);
    warningTxt->SetMaxWidth(500, GuiText::SCROLLFULL);

	supportTxt = new GuiText(tr("WARNING: This old SaveGame Manager GX Save format is not anymore supported since revision 87!!!"), 20, (GXColor){255, 255, 255, 255});
    supportTxt->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
    supportTxt->SetPosition(-6,-20);
    supportTxt->SetMaxWidth(500, GuiText::SCROLLFULL);

	if(!subname) {
		nameTxt->SetPosition(184,20);    
		blockBoxImg->SetPosition(-70,-92);
		blockTxt->SetPosition(-70,-90);
		idBoxImg->SetPosition(40,-92);
		idTxt->SetPosition(40,-90);
	}
	if(!block) {
		idBoxImg->SetPosition(-50,-82);
		idTxt->SetPosition(-50,-80);
		if(!subname) {
			idBoxImg->SetPosition(-50,-92);
			idTxt->SetPosition(-50,-90);
		}
	}
	
	window = new GuiWindow(dialogBoxImg->GetWidth(), dialogBoxImg->GetHeight());
	window->SetParent(this);
	
	window->Append(dialogBoxImg);
	window->Append(nameTxt);
	window->Append(closeBtn);
	window->Append(leftBtn);
	window->Append(rightBtn);
	window->Append(iconBoxImg);
	window->Append(icon);
	
	if(subname)
		window->Append(subnameTxt);
	if(block && SaveInstalled) {
		window->Append(blockTxt);
		window->Append(blockBoxImg);
	}
	if(tid) {
		window->Append(idTxt);
		window->Append(idBoxImg);
	}
	if(!SaveInstalled && !notSupportedFormat) {
		window->Append(warningTxt);
	}
	if(notSupportedFormat) {
		window->Append(supportTxt);
	}
	
	SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	SetPosition(6, 0);
	
	SetWindowEffect();
	
	this->SetState(STATE_DEFAULT);
}

/**
 * Destructor for the ManageWindow class.
 */
ManageWindow::~ManageWindow()
{
	window->RemoveAll();

    /** Buttons **/
	delete closeBtn;

    /** Images **/
	delete dialogBoxImg;
	delete iconBoxImg;
	delete blockBoxImg;
	delete idBoxImg;
	delete closeImg;
	delete closeOverImg;
	delete leftImg;
	delete rightImg;
	
	/** ImageDatas **/
	Resources::Remove(dialogBox);
	Resources::Remove(iconBox);
	Resources::Remove(blockBox);
	Resources::Remove(idBox);
	Resources::Remove(close);
	Resources::Remove(closeOver);
	Resources::Remove(left);
	Resources::Remove(right);
	
	/** GuiSaveIcon **/
	if(icon)
		delete icon;
	
    /** Sounds **/
	Resources::Remove(btnSoundOver);
	Resources::Remove(btnClick);
	
    /** Triggers **/
	delete trigHome;
	delete trigA;
	delete trigB;
	delete trigMinus;
	delete trigPlus;
	delete trigLeft;
	delete trigRight;
		
    /** Texts **/
    delete nameTxt;
	delete subnameTxt;
	delete blockTxt;
	delete idTxt;
	delete warningTxt;
	delete supportTxt;
	
    /** Window **/
    delete window;
	
}

void ManageWindow::SetAlignment(int h, int v)
{
    GuiElement::SetAlignment(h, v);
	window->SetAlignment(h, v);
}

void ManageWindow::SetWindowEffect()
{
	move = windowinfo.move;
    
	if(windowinfo.move == ManageWindow::MOVE_IN)
		window->SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_IN, 50);
	
	else if(windowinfo.move == ManageWindow::MOVE_OUT)
		window->SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_OUT, 50);
		
	else if(windowinfo.move == ManageWindow::MOVE_PREV_IN)
		window->SetEffect(EFFECT_SLIDE_LEFT | EFFECT_SLIDE_IN, 50);
	
    else if(windowinfo.move == ManageWindow::MOVE_NEXT_IN)
		window->SetEffect(EFFECT_SLIDE_RIGHT | EFFECT_SLIDE_IN, 50);
    
	else if(windowinfo.move == ManageWindow::MOVE_PREV_OUT)
		window->SetEffect(EFFECT_SLIDE_RIGHT | EFFECT_SLIDE_OUT, 50);
	
    else if(windowinfo.move == ManageWindow::MOVE_NEXT_OUT)
		window->SetEffect(EFFECT_SLIDE_LEFT | EFFECT_SLIDE_OUT, 50);
	
	else if(windowinfo.move == ManageWindow::DEFAULT)
		window->StopEffect();
	
}

int ManageWindow::GetChoice()
{
    return choice;
}

int ManageWindow::GetIfInstalled()
{
	if(notSupportedFormat)
		return 0;
	
	return SaveInstalled;
}

void ManageWindow::ResetState()
{
    state = STATE_DEFAULT;
	stateChan = -1;

	closeBtn->ResetState();
	leftBtn->ResetState();
	rightBtn->ResetState();
}

void ManageWindow::Disabled()
{
	closeBtn->SetState(STATE_DISABLED);
	leftBtn->SetState(STATE_DISABLED);
	rightBtn->SetState(STATE_DISABLED);
	leftBtn->SetVisible(false);
	rightBtn->SetVisible(false);
}

void ManageWindow::Enabled()
{
	closeBtn->SetState(STATE_DEFAULT);
	leftBtn->SetState(STATE_DEFAULT);
	rightBtn->SetState(STATE_DEFAULT);
	leftBtn->SetVisible(true);
	rightBtn->SetVisible(true);
}

/**
 * Draw the window
 */
void ManageWindow::Draw()
{
	if(!this->IsVisible())
		return;

	window->Draw();

	this->UpdateEffects();
}

void ManageWindow::Update(GuiTrigger * t)
{
	if(state == STATE_DISABLED || !t)
		return;

	closeBtn->Update(t);
	leftBtn->Update(t);
	rightBtn->Update(t);

    if(closeBtn->GetState() == STATE_CLICKED)
	{
		closeBtn->ResetState();
		choice = 1;
		this->SetState(STATE_CLICKED);
	}
	else if(leftBtn->GetState() == STATE_CLICKED)
	{
		leftBtn->ResetState();
		choice = 2;
		this->SetState(STATE_CLICKED);
	}
	else if(rightBtn->GetState() == STATE_CLICKED)
	{
		rightBtn->ResetState();
		choice = 3;
		this->SetState(STATE_CLICKED);
	}
	
	if(updateCB)
		updateCB(this);
}
