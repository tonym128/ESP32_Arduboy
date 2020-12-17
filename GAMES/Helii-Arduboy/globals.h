#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduboy2.h>

// Global arduboy instance
extern Arduboy2 arduboy;

// Enum for game states
enum GameState {
  TITLE_STATE,
	MAIN_MENU_STATE,
	FLYING_STATE,
	DEAD_STATE
};

extern unsigned int g_ticks;
extern GameState g_currentState;

#endif
