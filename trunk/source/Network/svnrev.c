#include <stdlib.h>

#define SVN_REV "127"

const char *GetRev()
{
	return SVN_REV;
}

int GetIntRev()
{
	return atoi(SVN_REV);
}
