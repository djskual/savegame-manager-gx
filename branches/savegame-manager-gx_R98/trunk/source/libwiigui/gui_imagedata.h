/****************************************************************************
 * Copyright (C) 2010
 * by Dimok
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
#ifndef GUI_IMAGEDATA_H_
#define GUI_IMAGEDATA_H_

#include <gctypes.h>
#include <gd.h>
//#include "system/images/GifImage.hpp"

class GuiImageData
{
	public:
		//!Constructor
		//!\param img Image data
		//!\param imgSize The image size
		GuiImageData(const u8 * img, int imgSize);
		//!Destructor
		~GuiImageData();
		//!Gets a pointer to the image data
		//!\return pointer to image data
		u8 * GetImage() { return data ? data /*: AnimGif ? AnimGif->GetFrameImage(0) */: NULL; };
		//!Gets the image width
		//!\return image width
		int GetWidth() { return width; };
		//!Gets the image height
		//!\return image height
		int GetHeight() { return height; };
		//!Gets the texture format
		u8 GetTextureFormat() { return format; };
		/*//!Gets animated gif if present
		GifImage * GetAnimGif() { return AnimGif; };*/
	protected:
		void LoadPNG(const u8 *img, int imgSize);
		void LoadBMP(const u8 *img, int imgSize);
		void LoadJpeg(const u8 *img, int imgSize);
		//void LoadGIF(const u8 *img, int imgSize);
        void LoadGD(const u8 *img, int imgSize);
        void LoadGD2(const u8 *img, int imgSize);
        void LoadTIFF(const u8 *img, int imgSize);
		void LoadTGA(const u8 *img, int imgSize);
		void LoadTPL(const u8 *img, int imgSize);

		u8 * data; //!< Image data
		int height; //!< Height of image
		int width; //!< Width of image
		u8 format; //!< Texture format
		//GifImage * AnimGif;
};

#endif
