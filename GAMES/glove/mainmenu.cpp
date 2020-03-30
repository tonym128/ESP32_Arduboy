#include <Arduino.h>
#include "save.h"
#include "globals.h"
#include "prompt.h"
#include "mainmenu.h"
#include "records.h"
#include "options.h"

// The Strings used in the game-in-progress menu options
const char mainMenuItems_0[] PROGMEM = "[     Main Menu     ]";
const char mainMenuItems_1[] PROGMEM = "Continue:";
const char mainMenuItems_2[] PROGMEM = "New Game";
const char mainMenuItems_3[] PROGMEM = "Records";
const char mainMenuItems_4[] PROGMEM = "Options";

// The order of menu item strings
const char* const mainMenuItems[] PROGMEM = {
	mainMenuItems_0,
	mainMenuItems_1,
	mainMenuItems_2,
	mainMenuItems_3,
	mainMenuItems_4
};
const int8_t mainMenuItemsCount = 4;


// The Strings used in no-game-in-progress menu options
const char noContinueMenuItems_0[] PROGMEM = "[     Main Menu     ]";
const char noContinueMenuItems_1[] PROGMEM = "New Game";
const char noContinueMenuItems_2[] PROGMEM = "Records";
const char noContinueMenuItems_3[] PROGMEM = "Options";

// The order of menu item strings
const char* const noContinueMenuItems[] PROGMEM = {
	noContinueMenuItems_0,
	noContinueMenuItems_1,
	noContinueMenuItems_2,
	noContinueMenuItems_3
};
const int8_t noContinueMenuItemsCount = 3;

// Display the main menu (captive function)
uint8_t displayMainMenu()
{
    // Read whether the save file has a game in progress
	int16_t address = GameSaveOffset;
	char choice;
	uint8_t result;
    
    // If there exists a game in progress
	if(EEPROM.read(10+address+FILE_CONTINUE) == 1) {
        /* A GAME IS IN PROGRESS */
        
        // Repeat while a choice has not been made
		while(choice != 255) {
        
			// Prompt the user for a choice
			arduboy.clear();
			
			// Display room clear progress
			arduboy.setCursor(6*11 + 2, 8);
			arduboy.print(getCurrentLevelProgress());
			arduboy.print(F(" Rms"));

			choice = prompt_start(mainMenuItems, mainMenuItemsCount);
            
            // Choice One: Continue Game
			if(choice == 0) {
				displayLoadGame(GAME_SAVE_FILE);
				break;
			}
            
            // Choice Two: New Game
			else if(choice == 1) {
				displayGame();
				break;
			}
            
            // Choice Three: View High Scores
			else if(choice == 2)
				displayRecords();
            
            // Choice Four: Display Options Menu
            else if(choice == 3) {
                
                // Get the result
                result = displayOptionsMenu();
                
                // If a setting has been changed, refresh the menu
                while(result == SETTING_CHANGED) {
                    result = displayOptionsMenu();
                }
                
                // Return to the title screen
                if(result == SETTING_REBOOT) {
					return GAME_REBOOT;
                    break;
                }
            }
		}
		
		return GAME_NOTHING;
	} else {
        /* A GAME IS NOT IN PROGRESS */
        
        // Repeat while a choice has not been made
		while(choice != 255) {
            
            // Ask the user for a choice
			arduboy.clear();
			choice = prompt_start(noContinueMenuItems, noContinueMenuItemsCount);
             
            // Choice One: New Game
			if(choice == 0) {
				displayGame();
				break;
			}
            
            // Choice Two: View High Scores
			else if(choice == 1)
				displayRecords();
            
            // Choice Three: Display Options Menu
            else if(choice == 2) {
				
                // Get the result
                result = displayOptionsMenu();
                
                // If a setting has been changed, refresh the menu
                while(result == SETTING_CHANGED) {
                    result = displayOptionsMenu();
                }
                
                // Return to the title screen
                if(result == SETTING_REBOOT) {
					return GAME_REBOOT;
                }
			}
		}
		
		return GAME_NOTHING;
	}
}
