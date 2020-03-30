#include "options.h"
#include "prompt.h"
#include "save.h"

// The Strings used for the Options Menu
const char optionsMenuItems_0[] PROGMEM = "[   Options Menu    ]";
const char optionsMenuItems_1[] PROGMEM = "Sound:";
const char optionsMenuItems_2[] PROGMEM = "Play";
const char optionsMenuItems_3[] PROGMEM = "Reset Game Data";

const char* const optionsMenuItems[] PROGMEM = {
    optionsMenuItems_0,
    optionsMenuItems_1,
    optionsMenuItems_2,
    optionsMenuItems_3
};

const char clearSaveConfirmItems_0[] PROGMEM = "[REALLY CLEAR SAVE??]";
const char clearSaveConfirmItems_1[] PROGMEM = "No, whoops!";
const char clearSaveConfirmItems_2[] PROGMEM = "Yes, I'm sure.";

const int8_t optionsMenuItemsCount = 3;

const char* const clearSaveMenuItems[] PROGMEM = {
	clearSaveConfirmItems_0,
	clearSaveConfirmItems_1,
	clearSaveConfirmItems_2
};

const int8_t clearSaveMenuItemsCount = 2;

uint8_t displayOptionsMenu()
{
    uint8_t choice;
	bool unlockedRandom = getRoomClearPercentage() >= 100;
    
    // Repeat while a choice has not been made
    while(choice != 255) {
        // Check if the audio is currently enabled
        bool audioEnabled = arduboy.audio.enabled();
        
        // Prompt the user for a choice
		arduboy.clear();
		
		// Draw whether sound is disabled
		arduboy.setCursor(6*7 + 2, 8);
		if (audioEnabled) {
			arduboy.print(F("On"));
		} else {
			arduboy.print(F("Off"));
		}
		
		// Draw Game Mode selection
		arduboy.setCursor(6*6 + 2, 16);
		if(GameMode == GAME_MODE_RANDOM) {
			arduboy.print(F("Glove"));
		} else if(unlockedRandom) {
			arduboy.print(F("Random"));
		} else {
			arduboy.print(F("??????"));
		}
        choice = prompt_start(optionsMenuItems, optionsMenuItemsCount);
        
        // Choice: Toggle Sound
		
        if(choice == 0) {
			if(audioEnabled) {
				arduboy.audio.off();
			} else {
				arduboy.audio.on();
			}
			arduboy.audio.saveOnOff();
            return SETTING_CHANGED;
        }
		
		// Choice: Change Mode
		else if(choice == 1) {
			if(GameMode == GAME_MODE_RANDOM) {
				GameSaveOffset = GAME_GLOVE_OFFSET;
				GameMode = GAME_MODE_GLOVE;
				return SETTING_REBOOT;
			} else if(unlockedRandom) {
				GameSaveOffset = GAME_RANDOM_OFFSET;
				GameMode = GAME_MODE_RANDOM;
				return SETTING_REBOOT;
			}
			continue;
		}
        
        // Choice: Clear Data
        else if(choice == 2) {
			
			// If it's time to reboot, reboot!
			if(displayClearMenu() == SETTING_REBOOT)
				return SETTING_REBOOT;
			
			// Otherwise return to settings
			continue;
        }
        
        // Back: Go to previous screen
        return SETTING_FINISHED;
    }   
}

uint8_t displayClearMenu()
{
	char choice;
	
	// Prompt the user for a choice
	arduboy.clear();
	choice = prompt_start(clearSaveMenuItems, clearSaveMenuItemsCount);
	
    // Repeat while a choice has not been made
    while(choice != 255) {
		
		// Choice One: Do nothing
		if(choice == 0)
			return SETTING_FINISHED;
		
		// Choice Two: Delete the save file
		if(choice == 1)
		{
			// Delete the save file
			breakSave(GAME_SAVE_FILE);
			
			// Reboot the game
			return SETTING_REBOOT;
		}
		
		// Back: Go to the previous screen
		return SETTING_FINISHED;
	}
}
