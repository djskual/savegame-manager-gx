#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#include "../Language/gettext.h"
#include "../Prompts/PromptWindows.h"

extern "C" void ShowError(const char * format, ...)
{
	char *tmp=0;
	va_list va;
	va_start(va, format);
	if((vasprintf(&tmp, format, va)>=0) && tmp)
	{
		WindowPrompt(tr("Error:"), tmp, tr("OK"));
	}
	va_end(va);

	if(tmp)
		free(tmp);
}
