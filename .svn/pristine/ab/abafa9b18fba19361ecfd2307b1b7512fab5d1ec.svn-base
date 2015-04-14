/****************************************************************************
 * USB Loader GX Team
 * gui_banner.h
 *
 * Shows TPL Banner images
 ***************************************************************************/

#ifndef _Gui_BANNER_H_
#define _Gui_BANNER_H_

#include "gui.h"

#define FRAMEDELAY 6

class GuiSaveBanner : public GuiImage
{
public:
    //!Constructor
    //!\param tplfilepath Path of the tpl file
	GuiSaveBanner(const char *tplfilepath);
    //!Constructor
    //!\param mem Memory of the loaded tpl
    //!\param len Filesize of the tpl
    //!\param w Width of the tpl
    //!\param h Height of the tpl
	GuiSaveBanner(u8 *mem, int len, int w, int h);

	//!\ create a TPL of the banner of a "banner.bin"
	//!\ this function expects a buffer containing the entire banner.bin and it takes care of getting the data out of it
	//!\ and adding the propper TPL header
	GuiSaveBanner(u8 *mem);
    //!Destructor
	~GuiSaveBanner();
	void Draw();
protected:
    void * memory;
	bool filecheck;
	int tplfilesize;
    GXTexObj texObj;
};

////////////// GuiSaveIcon functions/////////////////////////////////////////////////////
// by giantpune
//
// give this function a memory buffer containing a "banner.bin" from a save and it will
// create images for each of the icons and when it is drawn, it will cycle them based
// on the value of daley.  since this is a GuiImage, it can be used as icons for buttons
// and rotated just like other images.
//
//////////////////////////////////////////////////////////////////////////////////////////
#define MAXFRAMES 8

class GuiSaveIcon  : public GuiImage
{
public:
    // create the icon from the memory buffer filled with a full banner.bin
    // mem is the buffer containing the banner.bin
    // len is the size of the buffer
    GuiSaveIcon(u8 *mem, u32 len);
    ~GuiSaveIcon();

    //draw the current frame.  this function also keeps count and is responsible for triggering the advance to the next frame.
    void Draw();

    //get the number of frames making up this icon
    //everything after and including the first empty image is dropped and not counted or used
    u8 GetFrameCnt();

    //get the currently displayed frame
    u8 GetFrame();

    //set the icon to a certain frame
    void SetFrame(int i);

    //move to the next frame
    void AdvanceFrame();

    //check if this has at least one frame to display
    bool IsValid();

    //change the size of the banner
    //this function just stretches the texture to fill a rectangle with the size, so it loses detail when you go too big.
    void SetSize(int w, int h);

    // set the delay used for cycling frames
    void SetDelay(int i);
protected:
    void * memory[MAXFRAMES];
    bool filecheck;
    u32 tplfilesize;
    u8 frameCnt;
    u8 currentFrame;
    u8 counter;
    u8 delay;
    GXTexObj texObj[MAXFRAMES];
};

#endif /* _GuiTpl_H_ */
