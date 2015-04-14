/****************************************************************************
 *
 * manage_tools.h
 *
 ***************************************************************************/

#ifndef _MANAGE_TOOLS_H_
#define _MANAGE_TOOLS_H_

// typedef returning info about current navigation state
typedef struct {
	bool refresh;
	short move;
	short refreshType;
} WINDOWINFO;

void ResetWindowInfo();
int SaveInstall();
int SaveExtract(const char * name, u64 tid, const char * path);
int SaveDelete();
int SaveCopy();
int SaveMove();

#endif
