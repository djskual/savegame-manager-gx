/***************************************************************************
 * Copyright (C) 2010
 * by Dimok & r-win
 * Modified by Dj_Skual
 * for SaveGame Manager GX 2011
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
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#include "libwiigui/gui.h"
#include "system/images/TextureConverter.h"
#include "system/images/TplImage.h"
#include "saves/utils.h"

/**
 * Constructor for the GuiImageData class.
 */
GuiImageData::GuiImageData(const u8 * img, int imgSize)
{
	data = NULL;
    //AnimGif = NULL;
	width = 0;
	height = 0;
	format = GX_TF_RGBA8;

	if(img)
	{
		if (imgSize < 8)
		{
			return;
		}
		if (img[0] == 0xFF && img[1] == 0xD8)
		{
		    // IMAGE_JPEG
			LoadJpeg(img, imgSize);
		}
		else if (img[0] == 0x49 && img[1] == 0x49)
		{
		    // IMAGE_TIFF_PC
            LoadTIFF(img, imgSize);
		}
		else if (img[0] == 0x4D && img[1] == 0x4D)
		{
		    // IMAGE_TIFF_MAC
            LoadTIFF(img, imgSize);
		}
		else if (img[0] == 'B' && img[1] == 'M')
		{
		    // IMAGE_BMP
			LoadBMP(img, imgSize);
		}
		/*else if (img[0] == 'G' && img[1] == 'I' && img[2] == 'F')
		{
		    // IMAGE_GIF
			LoadGIF(img, imgSize);
		}*/
		else if (img[0] == 0x89 && img[1] == 'P' && img[2] == 'N' && img[3] == 'G')
		{
		    // IMAGE_PNG
			LoadPNG(img, imgSize);
		}
		else if ((img[0] == 0xFF && img[1] == 0xFF) || (img[0] == 0xFF && img[1] == 0xFE))
		{
		    // IMAGE_GD
			LoadGD(img, imgSize);
		}
		else if (img[0] == 0x67 && img[1] == 0x64 && img[2] == 0x32 && img[3] == 0x00)
		{
		    // IMAGE_GD2
			LoadGD2(img, imgSize);
		}
		else if (img[0] == 0x00 && img[1] == 0x20 && img[2] == 0xAF && img[3] == 0x30)
		{
		    // IMAGE_TPL
			LoadTPL(img, imgSize);
		}
		//!This must be last since it can also intefere with outher formats
		else if(img[0] == 0x00)
		{
		    // Try loading TGA image
			LoadTGA(img, imgSize);
		}
	}
}

/**
 * Destructor for the GuiImageData class.
 */
GuiImageData::~GuiImageData()
{
	if(data)
	{
		free(data);
		data = NULL;
	}

	/*if(AnimGif)
        delete AnimGif;*/
}

void GuiImageData::LoadPNG(const u8 *img, int imgSize)
{
    gdImagePtr gdImg = gdImageCreateFromPngPtr(imgSize, (u8*) img);
    if(gdImg == 0)
        return;

    data = GDImageToRGBA8(&gdImg, &width, &height);
    gdImageDestroy(gdImg);
}

void GuiImageData::LoadJpeg(const u8 *img, int imgSize)
{
    gdImagePtr gdImg = gdImageCreateFromJpegPtr(imgSize, (u8*) img);
    if(gdImg == 0)
        return;

    data = GDImageToRGBA8(&gdImg, &width, &height);
    gdImageDestroy(gdImg);
}

/*void GuiImageData::LoadGIF(const u8 *img, int imgSize)
{
    AnimGif = new GifImage(img, imgSize);
    if(AnimGif->GetFrameCount() > 1)
        return;

    delete AnimGif;
    AnimGif = NULL;

    gdImagePtr gdImg = gdImageCreateFromGifPtr(imgSize, (u8*) img);
    if(gdImg == 0)
        return;

    data = GDImageToRGBA8(&gdImg, &width, &height);
    gdImageDestroy(gdImg);
}*/

void GuiImageData::LoadGD(const u8 *img, int imgSize)
{
    gdImagePtr gdImg = gdImageCreateFromGdPtr(imgSize, (u8*) img);
    if(gdImg == 0)
        return;

    data = GDImageToRGBA8(&gdImg, &width, &height);
    gdImageDestroy(gdImg);
}

void GuiImageData::LoadGD2(const u8 *img, int imgSize)
{
    gdImagePtr gdImg = gdImageCreateFromGd2Ptr(imgSize, (u8*) img);
    if(gdImg == 0)
        return;

    data = GDImageToRGBA8(&gdImg, &width, &height);
    gdImageDestroy(gdImg);
}

void GuiImageData::LoadTIFF(const u8 *img, int imgSize)
{
    gdImagePtr gdImg = gdImageCreateFromTiffPtr(imgSize, (u8*) img);
    if(gdImg == 0)
        return;

    data = GDImageToRGBA8(&gdImg, &width, &height);
    gdImageDestroy(gdImg);
}

void GuiImageData::LoadBMP(const u8 *img, int imgSize)
{
    gdImagePtr gdImg = gdImageCreateFromBmpPtr(imgSize, (u8*) img);
    if(gdImg == 0)
        return;

    data = GDImageToRGBA8(&gdImg, &width, &height);
    gdImageDestroy(gdImg);
}

void GuiImageData::LoadTGA(const u8 *img, int imgSize)
{
    gdImagePtr gdImg = gdImageCreateFromTgaPtr(imgSize, (u8*) img);
    if(gdImg == 0)
        return;

    data = GDImageToRGBA8(&gdImg, &width, &height);
    gdImageDestroy(gdImg);
}

void GuiImageData::LoadTPL(const u8 *img, int imgSize)
{
    TplImage TplFile(img, imgSize);

    width = TplFile.GetWidth(0);
    height = TplFile.GetHeight(0);
    format = (u8) TplFile.GetFormat(0);

    const u8 * ImgPtr = TplFile.GetTextureBuffer(0);

    if(ImgPtr)
    {
        int len =  ALIGN32(TplFile.GetTextureSize(0));

        data = (u8 *) memalign(32, len);
        if(!data)
            return;

        memcpy(data, ImgPtr, len);
        DCFlushRange(data, len);
    }
}
