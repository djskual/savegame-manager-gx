#ifndef _MII_TOOLS_H_
#define _MII_TOOLS_H_

void fix_miiName(char * filename);
int ExtractMii(const char * extractPath);
char * readMii(char * path);
int WriteMii(char *Path);
int InstallMii();
int DeleteNandMii();
int DeleteDeviceMii();
int CopyMii();
int MoveMii();

#endif
