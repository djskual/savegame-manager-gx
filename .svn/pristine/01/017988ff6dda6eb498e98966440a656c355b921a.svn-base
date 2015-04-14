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
 * InfoWindow.cpp
 *
 * for SaveGame Manager GX 2010
 ***************************************************************************/

#include <gccore.h>
#include <unistd.h>

#include "menu/InfoWindow.h"
#include "menu/menu.h"
#include "main.h"

/**
 * Constructor for the InfoWindow class.
 */
InfoWindow::InfoWindow(const char *savename UNUSED)
{
	infoBox = Resources::GetImageData(info_window_png, info_window_png_size);
	infoBoxImg = new GuiImage(infoBox);
	
	nameTxt = new GuiText(NULL, 20, (GXColor){0, 0, 0, 255});
    nameTxt->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
    nameTxt->SetPosition(0, -3);
	nameTxt->SetMaxWidth(260, GuiText::SCROLL);
	
	infoWindow = new GuiWindow(280, 32);
	this->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	this->SetPosition(-(infoBoxImg->GetWidth())/2, -70);
	infoWindow->SetParent(this);
	
	infoWindow->Append(infoBoxImg);
	infoWindow->Append(nameTxt);
	
	this->SetState(STATE_DEFAULT);
	this->SetVisible(false);
}

/**
 * Destructor for the InfoWindow class.
 */
InfoWindow::~InfoWindow()
{
	infoWindow->RemoveAll();

    /** Images **/
	delete infoBoxImg;
	
	/** ImageDatas **/
	Resources::Remove(infoBox);
	
    /** Texts **/
    delete nameTxt;
	
	/** Window **/
    delete infoWindow;
	
}

void InfoWindow::SetName(const char *savename)
{
    nameTxt->SetText(savename);
}

/**
 * Draw the window
 */
void InfoWindow::Draw()
{
	if(!this->IsVisible())
		return;

	infoWindow->Draw();
	
	this->UpdateEffects();
}

void InfoWindow::Update(GuiTrigger * t UNUSED)
{
	if(updateCB)
		updateCB(this);
}
