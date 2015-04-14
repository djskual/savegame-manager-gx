/****************************************************************************
 * USB Loader GX Team
 * gui_banner.cpp
 *
 * Shows TPL Banner images
 ***************************************************************************/
 
#include "gui.h"
#include "gui_banner.h"
#include "saves/utils.h"

u8 bannerTPLheader[] = {
    0x00, 0x20, 0xAF, 0x30, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x14,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x40,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

GuiSaveBanner::GuiSaveBanner(const char *tplfilepath)
{
    memory = NULL;
    tplfilesize = 0;
    width = 0;
    height = 0;

	FILE *tplfp = fopen(tplfilepath,"rb");

	if(tplfp !=NULL) {

		unsigned short heighttemp = 0;
		unsigned short widthtemp = 0;

		fseek(tplfp , 0x14, SEEK_SET);
		fread((void*)&heighttemp,1,2,tplfp);
		fread((void*)&widthtemp,1,2,tplfp);
		fseek (tplfp , 0 , SEEK_END);
        tplfilesize = ftell (tplfp);
        rewind (tplfp);
        memory = memalign(32, tplfilesize);
        if(!memory) {
            fclose(tplfp);
            return;
        }
        fread(memory, 1, tplfilesize, tplfp);
		fclose(tplfp);

        TPLFile tplfile;
        int ret;

        ret = TPL_OpenTPLFromMemory(&tplfile, memory, tplfilesize);
        if(ret < 0) {
            free(memory);
            memory = NULL;
            return;
        }
        ret = TPL_GetTexture(&tplfile,0,&texObj);
        if(ret < 0) {
            free(memory);
            memory = NULL;
            return;
        }
        TPL_CloseTPLFile(&tplfile);

		width = widthtemp;
		height = heighttemp;
		widescreen = 0;
		filecheck = true;

    } else {
		filecheck = false;
		fclose(tplfp);
    }
}

GuiSaveBanner::GuiSaveBanner(u8 *mem, int len, int w, int h)
{
    if(!mem || !len)
	return;
    memory = mem;
    tplfilesize = len;
    width = w;
    height = h;

    TPLFile tplfile;

    int ret;

    ret = TPL_OpenTPLFromMemory(&tplfile, memory, tplfilesize);
    if(ret < 0) {
	free(memory);
	memory = NULL;
	return;
    }
    ret = TPL_GetTexture(&tplfile,0,&texObj);
    if(ret < 0) {
	free(memory);
	memory = NULL;
	return;
    }
    TPL_CloseTPLFile(&tplfile);

    filecheck = true;
}


GuiSaveBanner::GuiSaveBanner(u8 *mem)
{
    if(!mem)
	return;
    memory = memalign(32, 24640);
    if(!memory)
    {
	//gprintf("\nGuiTpl:Cannot malloc memory");
	return;
    }
    mem+=0xa0;
    memset(memory,0,24640);
    memcpy(memory, bannerTPLheader, sizeof(bannerTPLheader));
    memcpy((u8*)memory+sizeof(bannerTPLheader),mem,24576);
    memset((u8*)memory+0x15,0x40,1);
    memset((u8*)memory+0x17,0xc0,1);


    tplfilesize = 24640;
    width = 160;
    height = 50;

    TPLFile tplfile;

    int ret;

    ret = TPL_OpenTPLFromMemory(&tplfile, memory,tplfilesize);
    if(ret < 0) {
	free(memory);
	memory = NULL;
	return;
    }

    ret = TPL_GetTexture(&tplfile,0,&texObj);
    if(ret < 0) {
	free(memory);
	memory = NULL;
	return;
    }
    TPL_CloseTPLFile(&tplfile);


    filecheck = true;
}

GuiSaveBanner::~GuiSaveBanner()
{
    if(memory != NULL) {
        free(memory);
        memory = NULL;
    }
}

void GuiSaveBanner::Draw()
{
	if(!filecheck ||!this->IsVisible())
		return;

	float currScale = this->GetScale();

	Menu_DrawTPLImg(this->GetLeft(), this->GetTop(), 0, width, height, &texObj, imageangle, widescreen ? currScale*0.80 : currScale, currScale, this->GetAlpha());

	this->UpdateEffects();
}

GuiSaveIcon::GuiSaveIcon(u8 *mem, u32 len)
{
    if(!mem or len<8)
	return;

    frameCnt = (int)MIN(((len - 0x60a0)/0x1200),MAXFRAMES);
    //gprintf("\nGuiSaveIcon: cnt = %d",frameCnt);
    currentFrame = 0;
    delay = FRAMEDELAY;

    mem+=0x60a0;

    for (int i=0;i<frameCnt;i++)
    {
	//check that there really is data and not just a bunch of 0s
	//there probably is a faster way to check this, as this could slow shit down when there are a bunch of saves
	//also, it assumes that if there is an empty image, that there will be no other images after it
	if(is_empty((char*)mem, 0x1200))
	{
	    frameCnt = i;
	    //gprintf("\nGuiBanner: is_empty (frameCnt is now %d)",i);
	    break;
	}
	memory[i] = memalign(32, 0x1240);
	if(!memory[i])
	{
	    frameCnt = i;
	    //gprintf("\nGuiSaveIcon:Cannot malloc memory");
	    break;
	}

	memset(memory[i],0,0x1240);
	memcpy(memory[i], bannerTPLheader, sizeof(bannerTPLheader));
	memcpy((u8*)memory[i]+sizeof(bannerTPLheader),mem,0x1200);
	memset((u8*)memory[i]+0x15,0x30,1);
	memset((u8*)memory[i]+0x17,0x30,1);

	mem+=0x1200;

	width = 64;
	height = 68;

	TPLFile tplfile;

	int ret;

	ret = TPL_OpenTPLFromMemory(&tplfile, memory[i],0x1240);
	if(ret < 0) {
	    free(memory[i]);
	    memory[i] = NULL;
	    frameCnt = i;
	    //gprintf("\nGuiSaveIcon: TPL_OpenTPLFromMemory error");
	    break;
	}

	ret = TPL_GetTexture(&tplfile,0,&texObj[i]);
	if(ret < 0) {
	    free(memory[i]);
	    memory[i] = NULL;
	    frameCnt = i;
	    //gprintf("\nGuiSaveIcon: TPL_GetTexture error");
	    break;
	}
	TPL_CloseTPLFile(&tplfile);

    }
    counter = 0;

    if (frameCnt>0)
	filecheck = true;
}

GuiSaveIcon::~GuiSaveIcon()
{
    for (int i=0;i<frameCnt;i++)
    {
	if(memory[i] != NULL) {
	    free(memory[i]);
	    memory[i] = NULL;
	}
    }
}

void GuiSaveIcon::SetFrame(int i)
{
    currentFrame = i;
}

u8 GuiSaveIcon::GetFrameCnt()
{
    return frameCnt;
}

u8 GuiSaveIcon::GetFrame()
{
    return currentFrame;
}

void GuiSaveIcon::SetSize(int w, int h)
{
    width = w;
    height = h;
}

void GuiSaveIcon::SetDelay(int i)
{
    delay = i;
}

void GuiSaveIcon::AdvanceFrame()
{
    if(frameCnt<2)return;
    if(++currentFrame>=frameCnt)currentFrame=0;
    //gprintf("\nGuiSaveIcon: currentFrame %d",currentFrame);
}

bool  GuiSaveIcon::IsValid()
{
    return filecheck;
}

void GuiSaveIcon::Draw()
{
	if(!filecheck ||!this->IsVisible())
		return;

	float currScale = this->GetScale();

	counter++;
	if (counter>=delay)
	{
	    counter=0;
	    this->AdvanceFrame();
	}

	Menu_DrawTPLImg(this->GetLeft(), this->GetTop(), 0, width, height, &texObj[currentFrame], imageangle, widescreen ? currScale*0.80 : currScale, currScale, this->GetAlpha());

	this->UpdateEffects();
}

