#include "../Prompts/PromptWindows.h"
#include "../Controls/Application.h"

int PasswordCheck(const char * password)
{
	if(!password || strcmp(password, "") == 0 || strcmp(password, "not set") == 0)
		return 2;

	char entered[100];
	memset(entered, 0, sizeof(entered));
	
	int ret = OnScreenNumpad(entered, 9, true);
	if(ret)
	{
		if (!strcmp(entered, password)) //! password correct
			return 1;
		else
			return -1;
	}
	
	return 0;
}
