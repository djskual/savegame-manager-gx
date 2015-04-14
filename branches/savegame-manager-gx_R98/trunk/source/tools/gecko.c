#include <gccore.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

/* init-globals */
bool geckoinit = false;
bool textVideoInit = false;

#ifndef NO_DEBUG
#include <stdarg.h>

void gprintf( const char * format, ... )
{
	if (!geckoinit)
        return;

	char *tmp=0;
	
	va_list va;
	va_start(va, format);
	
	if((vasprintf(&tmp, format, va)>=0) && tmp)
	{
        usb_sendbuffer( 1, tmp, strlen(tmp) );
		free(tmp);
	}
	
	va_end(va);
}
 
bool InitGecko()
{
	u32 geckoattached = usb_isgeckoalive(EXI_CHANNEL_1);
	if (geckoattached)
	{
		usb_flush(EXI_CHANNEL_1);
		return true;
	}
	else return false;
}


#endif /* NO_DEBUG */
