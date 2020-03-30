#include "Arduino.h"
#include "globals.h"

#include "save.h"
#include "title.h"
#include "mainmenu.h"
#include "game.h"

void setup() {
	// Start arduboy
    arduboy.begin();

    EEPROM.begin(1000);
	
	// Set audio state
	if(!arduboy.audio.enabled())
	{
		arduboy.audio.off();
	}

	// Set framerate
    arduboy.setFrameRate(50);
	
	// Perform boot save operations
	saveSetup();
}

void loop() {
	// Show title screen
    displayTitle();
	
	updateInput();
	
	// If the game is freshly installed, jump right in
	if(!validSave)
	{
		saveCreate();
		displayGame();
	}
	// Otherwise show the main menu
	else
	{
		if(displayMainMenu() == GAME_REBOOT)
		{
			saveSetup();
		}
	}
}
