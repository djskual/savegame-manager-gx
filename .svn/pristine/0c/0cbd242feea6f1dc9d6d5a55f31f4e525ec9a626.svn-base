/***************************************************************************
 * Copyright (C) 2009
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
 * ManageThrobber.cpp
 *
 * for SaveGame Manager GX 2010
 ***************************************************************************/
#include <gccore.h>
#include <unistd.h>

#include "manage_all/ManageThrobber.h"
#include "main.h"

/****************************************************************************
* 
* ManageThrobber Class
*
***************************************************************************/
ManageThrobber::ManageThrobber(int i UNUSED)
{
    throbberCount = 1;
	
    throbberBg = Resources::GetImageData(manage_throbber_bg_png, manage_throbber_bg_png_size);
	throbberBgImg = new GuiImage(throbberBg);
	
	throbber = Resources::GetImageData(manage_throbber_png, manage_throbber_png_size);
	
	throbberCentreImg = new GuiImage(throbber);
	throbberCentreImg->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	throbberCentreImg->SetPosition(0, 0);
	
	throbber1Img = new GuiImage(throbber);
	throbber1Img->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	throbber1Img->SetPosition(4, 4);
	throbber1Img->SetVisible(false);
	
	throbber2Img = new GuiImage(throbber);
	throbber2Img->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	throbber2Img->SetPosition(16, 4);
	
	throbber3Img = new GuiImage(throbber);
	throbber3Img->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	throbber3Img->SetPosition(28, 4);
	
	throbber4Img = new GuiImage(throbber);
	throbber4Img->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	throbber4Img->SetPosition(28, 16);
	
	throbber5Img = new GuiImage(throbber);
	throbber5Img->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	throbber5Img->SetPosition(28, 28);
	
	throbber6Img = new GuiImage(throbber);
	throbber6Img->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	throbber6Img->SetPosition(16, 28);
	
	throbber7Img = new GuiImage (throbber);
	throbber7Img->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	throbber7Img->SetPosition(4, 28);
	
	throbber8Img = new GuiImage(throbber);
	throbber8Img->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	throbber8Img->SetPosition(4, 16);
	
	Throbber = new GuiWindow(40, 40);
	Throbber->SetParent(this);
	
	Throbber->Append(throbberBgImg);
    Throbber->Append(throbberCentreImg);
	Throbber->Append(throbber1Img);
	Throbber->Append(throbber2Img);
	Throbber->Append(throbber3Img);
	Throbber->Append(throbber4Img);
	Throbber->Append(throbber5Img);
	Throbber->Append(throbber6Img);
	Throbber->Append(throbber7Img);
	Throbber->Append(throbber8Img);
	
    SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	
	this->SetState(STATE_DEFAULT);
	
}

ManageThrobber::~ManageThrobber()
{
	Throbber->RemoveAll();

    /** Images **/
	delete throbberBgImg;
	delete throbberCentreImg;
	delete throbber1Img;
	delete throbber2Img;
    delete throbber3Img;
    delete throbber4Img;
    delete throbber5Img;
    delete throbber6Img;
    delete throbber7Img;
	delete throbber8Img;
	
	/** ImageDatas **/
	Resources::Remove(throbber);
	Resources::Remove(throbberBg);
	
	/** Window **/
    delete Throbber;
}

void ManageThrobber::SetThrobberCount(int t)
{
	throbberCount = t;
}

void ManageThrobber::SetAlignment(int h, int v)
{
    GuiElement::SetAlignment(h, v);
	Throbber->SetAlignment(h, v);
}

/**
 * Draw the window
 */
void ManageThrobber::Draw()
{
	if(!this->IsVisible())
		return;

	Throbber->Draw();

	this->UpdateEffects();
}

void ManageThrobber::Update(GuiTrigger * t)
{
	if(state == STATE_DISABLED || !t)
		return;
	
	if(throbberCount == 1) {
		throbber1Img->SetVisible(false);
		throbber8Img->SetVisible(true); }
	else if(throbberCount == 2) {
		throbber2Img->SetVisible(false);
		throbber1Img->SetVisible(true); }
	else if(throbberCount == 3) {
		throbber3Img->SetVisible(false);
		throbber2Img->SetVisible(true); }
	else if(throbberCount == 4) {
		throbber4Img->SetVisible(false);
		throbber3Img->SetVisible(true); }
	else if(throbberCount == 5) {
		throbber5Img->SetVisible(false);
		throbber4Img->SetVisible(true); }
	else if(throbberCount == 6) {
		throbber6Img->SetVisible(false);
		throbber5Img->SetVisible(true); }
	else if(throbberCount == 7) {
		throbber7Img->SetVisible(false);
		throbber6Img->SetVisible(true); }
	else if(throbberCount == 8) {
		throbber8Img->SetVisible(false);
		throbber7Img->SetVisible(true); }
    
	if(updateCB)
		updateCB(this);
}
