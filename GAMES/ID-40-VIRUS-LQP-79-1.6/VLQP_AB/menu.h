#ifndef MENU_BITMAPS_H
#define MENU_BITMAPS_H

#include "game.h"
#include "globals.h"

// globals ///////////////////////////////////////////////////////////////////

extern int16_t menuSelection;
extern int level;
extern int displayLevel;


// method prototypes /////////////////////////////////////////////////////////

void stateMenuIntro();
void stateMenuMain();
void stateMenuHelp();
void stateMenuPlay();
void stateMenuInfo();
void stateMenuSoundfx();


#endif
