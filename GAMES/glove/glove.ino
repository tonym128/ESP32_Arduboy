#include "Arduino.h"
#include "globals.h"

#include "save.h"
#include "title.h"
#include "mainmenu.h"
#include "game.h"

void inogamesetup() {
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

void inogameloop() {
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
void gameLogicLoop(void *)
{
  for (;;) {
    inogameloop(); 
    // ArduinoOTA.handle();
  }
}

void setup() {
  inogamesetup();
  xTaskCreatePinnedToCore(gameLogicLoop, "g", 4096, nullptr, 0, nullptr, 0);
}

void loop() {
	delay(60000);
}
