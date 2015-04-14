#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <gctypes.h>

#include "StringTools.h"

char *str_replace(const char *txt, const char *Before, const char *After)
{
	const char *pos;
	char *ReturnTxt;
	size_t PosReturnTxt;
	size_t Long;
	size_t AllocatedSize;
	
	pos = strstr (txt, Before);
	if(pos == NULL)
		return NULL;
	
	Long = (size_t)pos - (size_t)txt;
	AllocatedSize = Long + strlen (After) + 1;
	ReturnTxt = malloc (AllocatedSize);
	PosReturnTxt = 0;

	strncpy (ReturnTxt + PosReturnTxt, txt, Long);
	PosReturnTxt += Long;
	txt = pos + strlen (Before);
	
	Long = strlen (After);
	strncpy (ReturnTxt + PosReturnTxt, After, Long);
	PosReturnTxt += Long;
	
	pos = strstr (txt, Before);
	while (pos != NULL)
	{
		Long = (size_t)pos - (size_t)txt;
		AllocatedSize += Long + strlen (After);
		ReturnTxt = (char *)realloc (ReturnTxt, AllocatedSize);
		
		strncpy (ReturnTxt + PosReturnTxt, txt, Long);
		PosReturnTxt += Long;
		
		txt = pos + strlen (Before);

		Long = strlen (After);
		strncpy (ReturnTxt + PosReturnTxt, After, Long);
		PosReturnTxt += Long;
		
		pos = strstr (txt, Before);
	}
	
	Long = strlen (txt) + 1;
	AllocatedSize += Long;
	ReturnTxt = realloc (ReturnTxt, AllocatedSize);
	strncpy (ReturnTxt + PosReturnTxt, txt, Long);
	return ReturnTxt;
}

void fix_filename(char * filename)
{
	char * temp = NULL;
	
	temp = str_replace(filename, ":", "(COLON)");
	if(temp == NULL)
		temp = str_replace(filename, "(COLON)", ":");
	
	if(temp != NULL)
		strcpy(filename, temp);
	
}

const char * fmt(const char * format, ...)
{
    static char strChar[512];
    strChar[0] = 0;
	char * tmp = NULL;

	va_list va;
	va_start(va, format);
	if((vasprintf(&tmp, format, va) >= 0) && tmp)
	{
        snprintf(strChar, sizeof(strChar), tmp);
		free(tmp);
        va_end(va);
		return (const char *) strChar;
	}
	va_end(va);

    if(tmp)
        free(tmp);

	return NULL;
}
