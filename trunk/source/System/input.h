/****************************************************************************
 * libwiigui Template
 * Tantric 2009
 *
 * input.h
 * Wii/GameCube controller management
 ***************************************************************************/

#ifndef _INPUT_H_
#define _INPUT_H_

#include <gccore.h>
#include <wiiuse/wpad.h>

#define PI 				3.14159265f
#define PADCAL			50

void UpdatePads();
void SetupPads();
void ShutdownPads();
void ShutoffRumble();
void DoRumble(int i);
void RequestRumble(int chan);
int Wpad_Init(void);
void SetWPADTimeout();
bool ControlActivityTimeout(void);

bool ButtonPressed(void);
bool ButtonReleased(void);

//! Callbacks for shutdown/reset
extern bool shutdown;
extern bool reset;
//! User input
class GuiTrigger;
extern GuiTrigger userInput[4];

#endif
