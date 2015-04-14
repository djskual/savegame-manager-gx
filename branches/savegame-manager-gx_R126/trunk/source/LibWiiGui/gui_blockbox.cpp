/****************************************************************************
 * SaveGameManager GX
 *
 * Dj_Skual 2011
 *
 * gui_blockbox.cpp
 *
 ***************************************************************************/

#include "gui.h"
#include "../Themes/CTheme.h"

/**
 * Constructor for the GuiBlockBox class.
 */
GuiBlockBox::GuiBlockBox(const char *t, int Alpha)
{
	blockboxSide = Resources::GetImageData("manage_save_block_side.png");
	blockboxTile = Resources::GetImageData("manage_save_block_tile.png");
	leftImage = new GuiImage(blockboxSide);
	tileImage = new GuiImage(blockboxTile);
	rightImage = new GuiImage(blockboxSide);
	text = NULL;
	height = leftImage->GetHeight();
	leftImage->SetParent(this);
	tileImage->SetParent(this);
	rightImage->SetAngle(180);
	rightImage->SetParent(this);
	leftImage->SetParentAngle(false);
	tileImage->SetParentAngle(false);
	rightImage->SetParentAngle(false);
	SetText(t);
	SetAlpha(Alpha);
}

/**
 * Destructor for the GuiBlockBox class.
 */
GuiBlockBox::~GuiBlockBox()
{
	if (text) delete text;

	delete blockboxSide;
	delete blockboxTile;
	delete leftImage;
	delete tileImage;
	delete rightImage;
}

float GuiBlockBox::GetScale()
{
	float s = scale * scaleDyn;

	return s;
}

/**
 * !Sets the text of the GuiBlockBox element
 * !\param t Text
 */
void GuiBlockBox::SetText(const char * t)
{
	LOCK( this );
	if (text)
	{
		delete text;
		text = NULL;
	}
	int tile_cnt = 0;
	if (t && (text = new GuiText(t, 28, thColor("r=255 g=255 b=255 a=255 - manage windows text color"))))
	{
		text->SetParent(this);
		tile_cnt = ((text->GetTextWidth() - 12) / tileImage->GetWidth()) + 5;
		if (tile_cnt < 0) tile_cnt = 0;
	}
	tileImage->SetPosition(leftImage->GetWidth(), 0);
	tileImage->SetTileHorizontal(tile_cnt);
	rightImage->SetPosition(leftImage->GetWidth() + tile_cnt * tileImage->GetWidth(), 0);
	width = leftImage->GetWidth() + tile_cnt * tileImage->GetWidth() + rightImage->GetWidth();
}

void GuiBlockBox::SetWidescreen(bool )
{
}

/**
 * Draw the GuiBlockBox on screen
 */
void GuiBlockBox::Draw()
{
	LOCK( this );
	if (!this->IsVisible()) return;

	leftImage->Draw();
	tileImage->Draw();
	rightImage->Draw();
	if (text) text->Draw();

	this->UpdateEffects();
}
