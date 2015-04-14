/****************************************************************************
 * libwiigui
 *
 * Tantric 2009
 *
 * gui_text.cpp
 *
 * GUI class definitions
 ***************************************************************************/

#include "gui.h"

static int presetSize = 0;
static int presetMaxWidth = 0;
static int presetAlignmentHor = 0;
static int presetAlignmentVert = 0;
static int presetWrapMode = GuiText::WRAP;
static u16 presetStyle = FTGX_NULL;
static GXColor presetColor = (GXColor){255, 255, 255, 255};

/**
 * Constructor for the GuiText class.
 */
 
GuiText::GuiText(const char * t, int s, GXColor c)
{
	text = NULL;
	size = s;
	color = c;
	alpha = c.a;
	style = FTGX_JUSTIFY_CENTER | FTGX_ALIGN_MIDDLE;
	maxWidth = 0;
	wrapMode = GuiText::WRAP;
	scrollPos1 = 0;
	scrollPos2 = 0;
	scrollDelay = 0;
	LineBreak = NULL;
	font = NULL;
	widescreen = 0; //added
	firstLine = 1;
	numLines = -1;
	totalLines = 1;
	passChar = 0;

	alignmentHor = ALIGN_CENTRE;
	alignmentVert = ALIGN_MIDDLE;

	for(int i = 0; i < MAX_LINES_TO_DRAW; i++)
        tmptext[i] = NULL;
	
	if(t)
		text = FreeTypeGX::charToWideChar((char *)t);
}

/**
 * Constructor for the GuiText class, uses presets
 */
 
GuiText::GuiText(const char * t)
{
	text = NULL;
	size = presetSize;
	color = presetColor;
	alpha = presetColor.a;
	style = presetStyle;
	maxWidth = presetMaxWidth;
	wrapMode = presetWrapMode;
	scrollPos1 = 0;
	scrollPos2 = 0;
	scrollDelay = 0;
	LineBreak = NULL;
	font = NULL;
	widescreen = 0; //added
	firstLine = 1;
	numLines = -1;
	totalLines = 1;

	alignmentHor = presetAlignmentHor;
	alignmentVert = presetAlignmentVert;

	for(int i = 0; i < MAX_LINES_TO_DRAW; i++)
        tmptext[i] = NULL;
	
	if(t)
		text = FreeTypeGX::charToWideChar((char *)t);
}

/**
 * Destructor for the GuiText class.
 */
GuiText::~GuiText()
{
	if(text)
	{
		delete [] text;
		text = NULL;
	}
	if(LineBreak) {
        free(LineBreak);
        LineBreak = NULL;
	}
	
	for(int i = 0; i < MAX_LINES_TO_DRAW; i++)
	{
        if(tmptext[i])
	    {
            delete [] tmptext[i];
			tmptext[i] = NULL;			
		}
	}
}

void GuiText::SetText(const char * t)
{
	if(text)
		delete [] text;
	text = NULL;

	for(int i = 0; i < MAX_LINES_TO_DRAW; i++)
	{
        if(tmptext[i])
	    {
            delete [] tmptext[i];
			tmptext[i] = NULL;			
		}
	}
	
	if(t) {
		text = FreeTypeGX::charToWideChar((char *)t);
		
		if (passChar != 0) {
			for (u8 i = 0; i < wcslen(text); i++) {
				text[i] = passChar;
			}
		}
	}
	scrollPos2 = 0;
	scrollDelay = 0;
}

void GuiText::SetTextf(const char *format, ...)
{
	char *tmp=0;
	va_list va;
	va_start(va, format);
	if((vasprintf(&tmp, format, va)>=0) && tmp)
	{
		this->SetText(tmp);
		free(tmp);
	}
	va_end(va);
}

void GuiText::SetText(const wchar_t * t)
{
	if(text)
		delete [] text;
	text = NULL;
	
	for(int i = 0; i < MAX_LINES_TO_DRAW; i++)
	{
        if(tmptext[i])
	    {
            delete [] tmptext[i];
			tmptext[i] = NULL;			
		}
	}
	
	if(t)
	{
		int len = wcslen(t);
		text = new wchar_t[len+1];
		if(text) {
			wcscpy(text, t);
		}
		
		if (passChar != 0) {
			for (u8 i = 0; i < wcslen(text); i++) {
				text[i] = passChar;
			}
		}
	}
	scrollPos2 = 0;
	scrollDelay = 0;
}

void GuiText::SetPresets(int sz, GXColor c, int w, int wrap, u16 s, int h, int v)
{
	presetSize = sz;
	presetColor = c;
	presetMaxWidth = w;
	presetWrapMode = wrap;
	presetStyle = s;
	presetAlignmentHor = h;
	presetAlignmentVert = v;
}

/**
 * Set the Font
 */
 
void GuiText::SetFont(FreeTypeGX *f)
{
	font = f;
}

void GuiText::SetFontSize(int s)
{
	size = s;
}

/**
 * Set a password character
 */
void GuiText::SetPassChar(wchar_t p)
{
	passChar = p;
}

void GuiText::SetWidescreen(bool w)
{
	widescreen = w;
}

void GuiText::SetMaxWidth(int w, short m)
{
	maxWidth = w;
	wrapMode = m;
	
	if(wrapMode == GuiText::LONGTEXT)
	{
        int newSize = size*this->GetScale();
		
		(font ? font : fontSystem)->changeSize(newSize, widescreen ? newSize*0.8 : 0);
		
		int strlen = wcslen(text);
        int i = 0;
        int ch = 0;
        int linenum = 0;
        int lastSpace = -1;
        int lastSpaceIndex = -1;
		
        wchar_t *setText = new wchar_t[maxWidth];
		
        LineBreak = (u32 *) malloc(sizeof(u32));
        memset(&(LineBreak[linenum]), 0, sizeof(u32));
		
        LineBreak[linenum] = 0;
        linenum++;
		
        while(ch < strlen)
        {
            setText[i] = text[ch];
            setText[i+1] = 0;
			
            if(ch == strlen-1 || (font ? font : fontSystem)->getWidth(setText) >= maxWidth)
            {
                if((font ? font : fontSystem)->getWidth(setText) >= maxWidth)
                {
                    if(lastSpace >= 0)
                    {
                        setText[lastSpaceIndex] = 0; // discard space, and everything after
                        ch = lastSpace; // go backwards to the last space
                        lastSpace = -1; // we have used this space
                        lastSpaceIndex = -1;
                    }
                    LineBreak = (u32 *) realloc(LineBreak, (linenum+1)* sizeof(u32));
                    memset(&(LineBreak[linenum]), 0, sizeof(u32));
                    LineBreak[linenum] = ch;
                    linenum++;
                    i = -1;
                }
            }
            if(text[ch] == ' ' && i >= 0)
            {
                lastSpace = ch+1;
                lastSpaceIndex = i;
            }
            if(text[ch] == '\n')
            {
                LineBreak = (u32 *) realloc(LineBreak, (linenum+1)* sizeof(u32));
                memset(&(LineBreak[linenum]), 0, sizeof(u32));
                LineBreak[linenum] = ch+1;
                linenum++;
                i = -1;
                lastSpace = -1;
                lastSpaceIndex = -1;
            }
            ch++;
            i++;
			
            if(ch == strlen)
            {
                LineBreak = (u32 *) realloc(LineBreak, (linenum+1)* sizeof(u32));
                memset(&(LineBreak[linenum]), 0, sizeof(u32));
                LineBreak[linenum] = ch;
                if(text[ch-1] != '\n')
					linenum++;
                break;
            }
        }
        delete [] setText;
		setText = NULL;
        totalLines = linenum;
	}
	
	for(int i = 0; i < MAX_LINES_TO_DRAW; i++)
	{
	    if(tmptext[i])
	    {
            delete [] tmptext[i];
            tmptext[i] = NULL;
	    }
	}
}

void GuiText::SetColor(GXColor c)
{
	color = c;
	alpha = c.a;
}

void GuiText::SetStyle(u16 s, u16 m)
{
	style &= ~m;
	style |= s & m;
}

void GuiText::SetAlignment(int hor, int vert)
{
	style = FTGX_NULL;

	switch(hor)
	{
		case ALIGN_LEFT:
			style |= FTGX_JUSTIFY_LEFT;
			break;
		case ALIGN_RIGHT:
			style |= FTGX_JUSTIFY_RIGHT;
			break;
		default:
			style |= FTGX_JUSTIFY_CENTER;
			break;
	}
	switch(vert)
	{
		case ALIGN_TOP:
			style |= FTGX_ALIGN_TOP;
			break;
		case ALIGN_BOTTOM:
			style |= FTGX_ALIGN_BOTTOM;
			break;
		default:
			style |= FTGX_ALIGN_MIDDLE;
			break;
	}

	alignmentHor = hor;
	alignmentVert = vert;
}

void GuiText::SetLinesToDraw(int l)
{
    linestodraw = l;
}

void GuiText::SetNumLines(int n)
{	
	numLines = n;
}

void GuiText::SetFirstLine(int n)
{
	firstLine = n;
	longListChanged = true;
}

int GuiText::GetNumLines()
{	
	return numLines;
}

int GuiText::GetFirstLine()
{
	return firstLine;
}

int GuiText::GetTotalLines()
{
	return totalLines;
}

int GuiText::GetTextWidth()
{
	if(!text)
		return 0;
	
	int newSize = size*this->GetScale();

	(font ? font : fontSystem)->changeSize(newSize, widescreen ? newSize*0.8 : 0);
	return (font ? font : fontSystem)->getWidth(text);
}

int GuiText::GetLineHeight(int n UNUSED)
{	
	int newSize = size*this->GetScale();
	int lineheight = newSize + 6;
	
	if (numLines <0)
	return totalLines*lineheight+newSize;
	
	else return numLines*lineheight+newSize;
}

/**
 * Draw the text on screen
 */
void GuiText::Draw()
{
	if(!text)
		return;

	if(!this->IsVisible())
		return;

	GXColor c = color;
	c.a = this->GetAlpha();

	int newSize = size*this->GetScale();

	(font ? font : fontSystem)->changeSize(newSize, widescreen ? newSize*0.8 : 0);
	int voffset = 0;

	if(wrapMode == LONGTEXT) // text wrapping
	{
		int lineheight = newSize + 6;
		if(longListChanged == true)
		{
			for(int i = 0; i < MAX_LINES_TO_DRAW; i++)
			{
				if(tmptext[i])
				{
					delete [] tmptext[i];
					tmptext[i] = NULL;
				}
			}
			
			if(!tmptext[0])
			{
				int index = 0;
				u32 strlen = (u32) wcslen(text);
				int linenum = firstLine;
				int lineIndex = 0;
				u32 ch = LineBreak[linenum];

				u32 lastch = LineBreak[linenum+linestodraw]+1;

				tmptext[lineIndex] = new wchar_t[maxWidth];

				tmptext[lineIndex][index] = 0;

				while((ch < lastch) && (ch < strlen+1))
				{
					if(ch == LineBreak[linenum+1])
					{
						(font ? font : fontSystem)->drawText(this->GetLeft(), this->GetTop()+lineIndex*lineheight, tmptext[lineIndex], c, style);
						linenum++;
						lineIndex++;
						index = 0;
						if(!tmptext[lineIndex])
							tmptext[lineIndex] = new wchar_t[maxWidth];
						tmptext[lineIndex][index] = 0;
					}

					tmptext[lineIndex][index] = text[ch];
					tmptext[lineIndex][index+1] = 0;

					index++;
					ch++;
				}
			}
			longListChanged = false;
		}
		
		for(int i = 0; i < linestodraw; i++){
			if(tmptext[i]){
				(font ? font : fontSystem)->drawText(this->GetLeft(), this->GetTop()+i*lineheight, tmptext[i], c, style);
			}
		}
	}
	else if(wrapMode == GuiText::SCROLLFULL) // text full scroller
	{
		wchar_t save;
	
		if(scrollPos2 == 0 || FrameTimer > scrollDelay+5)
		{
			scrollPos1 = 0;
			scrollOffset = maxWidth;
			
			scrollDelay = FrameTimer+50; // wait 50 Frames before beginning with scrolling
			scrollPos2 = 1;
		}
		else if(scrollPos2 > 0 && FrameTimer >= scrollDelay)
		{
			
			if(--scrollOffset <= 0)
			{
				wchar_t tmp[] = { text[scrollPos1], text[scrollPos1+1], 0 };
				scrollOffset += (font ? font : fontSystem)->getWidth(tmp) - (font ? font : fontSystem)->getWidth(tmp+1);
				scrollPos1++;
			}
			
			int strlen = wcslen(text);
			for(; scrollPos2 < strlen; scrollPos2++)
			{
				save = text[scrollPos2+1]; // save Pos2
				text[scrollPos2+1] = 0;
				int textWidth = (font ? font : fontSystem)->getWidth(&text[scrollPos1]);
				text[scrollPos2+1] = save; // restore Pos2
				if(textWidth+scrollOffset > maxWidth)
					break;
			}
			if(scrollPos1 == strlen)
			{
				scrollPos2 = 0;
			}
			else
				scrollDelay = FrameTimer+1; // wait 1 Frames
		}
		
		uint16_t drawStyle = style;
		uint16_t drawX = this->GetLeft() + scrollOffset;
		
		if((drawStyle & FTGX_JUSTIFY_MASK) == FTGX_JUSTIFY_CENTER)
		{
			drawStyle = (drawStyle & ~FTGX_JUSTIFY_MASK) | FTGX_JUSTIFY_LEFT;
			drawX -= maxWidth >> 1;
		}
		else if((drawStyle & FTGX_JUSTIFY_MASK) == FTGX_JUSTIFY_RIGHT)
		{
			drawStyle = (drawStyle & ~FTGX_JUSTIFY_MASK) | FTGX_JUSTIFY_LEFT;
			drawX -= maxWidth;
		}
		
		save = text[abs(scrollPos2)]; // save Pos2
		text[abs(scrollPos2)] = 0;
		if (scrollPos2 > 1){
			(font ? font : fontSystem)->drawText(drawX, this->GetTop()+voffset, &text[scrollPos1], c, drawStyle);
		}
		text[abs(scrollPos2)] = save; // restore Pos2
	}
	
	else if(maxWidth > 0 && (font ? font : fontSystem)->getWidth(text) > maxWidth)
	{
		if(wrapMode == GuiText::WRAP) // text wrapping
		{
			int lineheight = newSize + 6;
			int strlen = wcslen(text);
			int i = 0;
			int ch = 0;
			int linenum = 0;
			int linemax = 200;
			int lastSpace = -1;
			int lastSpaceIndex = -1;
			wchar_t * wraptext[linemax];
			
			totalLines=0;
			while(ch < strlen)
			{
				if(i == 0)
				{
					if (linenum <= linemax)
					{
						wraptext[linenum] = new wchar_t[strlen+1];
					} 
					else 
					{
						break;
					}
				}
					
				wraptext[linenum][i] = text[ch];
				wraptext[linenum][i+1] = 0;

				if((font ? font : fontSystem)->getWidth(wraptext[linenum]) >= maxWidth)
				{
					if(lastSpace >= 0)
					{
						wraptext[linenum][lastSpaceIndex] = 0; // discard space, and everything after
						ch = lastSpace; // go backwards to the last space
						lastSpace = -1; // we have used this space
						lastSpaceIndex = -1;
					}
					linenum++;
					
					if(/*numLines>0 && */linenum == numLines && ch+1<strlen)
					{
						wraptext[linenum-1][i-3] = '.';
						wraptext[linenum-1][i-2] = '.';
						wraptext[linenum-1][i-1] = '.';
						wraptext[linenum-1][i] = 0;
						break;
					}
					
					i = -1;
				}
				else if(ch == strlen-1)
				{
					linenum++;
				}
				
				if(text[ch] == ' ' && i >= 0)
				{
					lastSpace = ch;
					lastSpaceIndex = i;
				}
				if(text[ch] == '\n' && ch != strlen-1 && i >= 0)
				{
					linenum++;
					i = -1;
				}
				ch++;
				i++;
			}
			totalLines = linenum;
			
			if(alignmentVert == ALIGN_MIDDLE)
				voffset = voffset - (lineheight*linenum)/2 + lineheight/2;

			if (numLines<0){
				for(i=0; i < linenum; i++)
				{
					(font ? font : fontSystem)->drawText(this->GetLeft(), this->GetTop()+voffset+i*lineheight, wraptext[i], c, style);
					delete wraptext[i]; 
				}
			}
			
			//put in for txt vertical txt scrolling
			else {
				
				for(i=0; i < numLines; i++)
				{
					if (i < linenum)
					(font ? font : fontSystem)->drawText(this->GetLeft(), this->GetTop()+voffset+i*lineheight, wraptext[i], c, style);
				}
				for(i=0; i < linenum; i++) 
				{
					delete wraptext[i];
				}
			}
		}
		
		else if(wrapMode == GuiText::DOTTED) // text dotted
		{
			wchar_t save[4];
			int strlen = wcslen(text);
			int dotPos=strlen-3;
			int i;
			bool drawed = false;
			while(dotPos > 0 && drawed == false)
			{
				for(i=0; i<4; i++) // save Text for "..."
				{
					save[i] = text[dotPos+i];
					text[dotPos+i] = (i != 3 ? _TEXT('.') : 0);
				}
				if(((font ? font : fontSystem)->getWidth(text)) <= maxWidth)
				{
					(font ? font : fontSystem)->drawText(this->GetLeft(), this->GetTop()+voffset, text, c, style);
					drawed = true;
				}

				for(i=0; i<4; i++) // write saved Text back
					text[dotPos+i] = save[i];
				dotPos--;
			}
			if(!drawed)
				(font ? font : fontSystem)->drawText(this->GetLeft(), this->GetTop()+voffset, text, c, style);
		}
		else if(wrapMode == GuiText::SCROLL) // text scroller
		{
			wchar_t save;
		
			if(scrollPos2 == 0 || FrameTimer > scrollDelay+5)
			{
				scrollPos1 = 0;
				scrollOffset = 0;
				for(scrollPos2 = wcslen(text); scrollPos2 > 1; scrollPos2--)
				{
					save = text[scrollPos2]; // save Pos2
					text[scrollPos2] = 0;
					int textWidth = (font ? font : fontSystem)->getWidth(text);
					text[scrollPos2] = save; // restore Pos2
					if(textWidth <= maxWidth)
						break;
				}
				scrollDelay = FrameTimer+50; // wait 50 Frames before beginning with scrolling
			}
			else if(scrollPos2 > 0 && FrameTimer >= scrollDelay)
			{
				
				if(--scrollOffset < 0)
				{
					wchar_t tmp[] = { text[scrollPos1], text[scrollPos1+1], 0 };
					scrollOffset += (font ? font : fontSystem)->getWidth(tmp) - (font ? font : fontSystem)->getWidth(tmp+1);
					scrollPos1++;
				}
				
				int strlen = wcslen(text);
				for(; scrollPos2 < strlen; scrollPos2++)
				{
					save = text[scrollPos2+1]; // save Pos2
					text[scrollPos2+1] = 0;
					int textWidth = (font ? font : fontSystem)->getWidth(&text[scrollPos1]);
					text[scrollPos2+1] = save; // restore Pos2
					if(textWidth+scrollOffset > maxWidth)
						break;
				}
				if(scrollPos2 == strlen)
				{
					scrollPos2 = -scrollPos2;
					scrollDelay = FrameTimer+25; // when dir-change wait 25 Frames
				}
				else
					scrollDelay = FrameTimer+1; // wait 1 Frames
			}
			else if(FrameTimer >= scrollDelay)
			{
				scrollPos2 = -scrollPos2;
				
				scrollOffset++;
				wchar_t tmp[] = { text[scrollPos1-1], text[scrollPos1], 0 };
				int tmpOffset = (font ? font : fontSystem)->getWidth(tmp) - (font ? font : fontSystem)->getWidth(tmp+1);
				if(scrollOffset >= tmpOffset)
				{
					scrollOffset -= tmpOffset;
					scrollPos1--;
				}

				for(; scrollPos2 > scrollPos1; scrollPos2--)
				{
					save = text[scrollPos2]; // save Pos2
					text[scrollPos2] = 0;
					int textWidth = (font ? font : fontSystem)->getWidth(&text[scrollPos1]);
					text[scrollPos2] = save; // restore Pos2
					if(textWidth+scrollOffset <= maxWidth)
						break;
				}
				if(scrollPos1 == 0)
				{
					scrollPos2 = -scrollPos2;
					scrollDelay = FrameTimer+25; // when dir-change wait 25 Frames
				}
				else
					scrollDelay = FrameTimer+1; // wait 10 Frames

				scrollPos2 = -scrollPos2;
			}
			
			uint16_t drawStyle = style;
			uint16_t drawX = this->GetLeft() + scrollOffset;
			
			if((drawStyle & FTGX_JUSTIFY_MASK) == FTGX_JUSTIFY_CENTER)
			{
				drawStyle = (drawStyle & ~FTGX_JUSTIFY_MASK) | FTGX_JUSTIFY_LEFT;
				drawX -= maxWidth >> 1;
			}
			else if((drawStyle & FTGX_JUSTIFY_MASK) == FTGX_JUSTIFY_RIGHT)
			{
				drawStyle = (drawStyle & ~FTGX_JUSTIFY_MASK) | FTGX_JUSTIFY_LEFT;
				drawX -= maxWidth;
			}
			
			save = text[abs(scrollPos2)]; // save Pos2
			text[abs(scrollPos2)] = 0;
			(font ? font : fontSystem)->drawText(drawX, this->GetTop()+voffset, &text[scrollPos1], c, drawStyle);
			text[abs(scrollPos2)] = save; // restore Pos2
		}
		
		else
		{
			(font ? font : fontSystem)->drawText(this->GetLeft(), this->GetTop()+voffset, text, c, style);
		}
	}
	else
	{
		(font ? font : fontSystem)->drawText(this->GetLeft(), this->GetTop()+voffset, text, c, style);
	}
	this->UpdateEffects();
}
