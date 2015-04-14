/****************************************************************************
 * language update
 * for USB Loader GX  *giantpune*
 ***************************************************************************/
#ifndef ___UPDATELANGUAGE_H_
#define ___UPDATELANGUAGE_H_

#include <string>

#define MAXLANGUAGEFILES     50

//! Checks the language path for files ending in .lang and updates them (up to MAXLANGUAGEFILES)

int UpdateLanguageFiles(std::string langPath);
int DownloadAllLanguageFiles(std::string langPath);
int SelectLanguageFiles(std::string langPath);

#endif
