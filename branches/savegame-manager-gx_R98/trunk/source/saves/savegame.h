#ifndef _SAVEGAME_H_
#define _SAVEGAME_H_

#include "saves/manage/ManageWindow.h"

/* Prototypes */

int Savegame_Install(const char *devpath, const char *nandpath);
int Savegame_Extract(const char *nandpath, const char *devpath, const char *indexpath);
int Savegame_GetNandPath(u64, char *);
int Savegame_CheckTitle(const char *, short source = ManageWindow::DEVICE);

#endif
