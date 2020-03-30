#include "globals.h"
#include "prompt.h"
#include "save.h"

#define TONE_BACK 400

void displayRecords()
{
	int16_t address = GameSaveOffset;
	
	arduboy.clear();
	arduboy.setCursor(6*5, 0);
	arduboy.print(F("- Records -\n\nName Score Time  #Rms"));
	
	uint8_t hi;
	uint8_t lo;
	int16_t val;
	// Loop through scores
	int16_t scoreAddress;
	
	for(int8_t i=0; i<3; ++i) {
		scoreAddress = 20 + (i*10) + address;
		arduboy.setCursor(0, 8*(3+i));
		arduboy.print(spsn(scoreAddress+FILE_NAME, 4));
		

		hi = EEPROM.read(scoreAddress+FILE_SCORE);
		lo = EEPROM.read(scoreAddress+FILE_SCORE+1);
		val = (hi << 8) | lo;
		arduboy.setCursor(6*5, 8*(3+i));
		arduboy.print(val);
		if(val>0) arduboy.print("00");
		
		hi = EEPROM.read(scoreAddress+FILE_TIME);
		lo = EEPROM.read(scoreAddress+FILE_TIME+1);
		val = (hi << 8) | lo;
		arduboy.setCursor(6*11, 8*(3+i));
		//arduboy.print(val);
		printTime(val);
		
		hi = EEPROM.read(scoreAddress+FILE_LEVEL);
		arduboy.print(' ');
		arduboy.print(hi);
	}
	arduboy.setCursor(0, 8*7);
	
	if(GameMode == GAME_MODE_GLOVE) {
		arduboy.print(F("ROOMS CLEARED: "));
		arduboy.print(getRoomClearPercentage());
		arduboy.print('%');
	} else {
		arduboy.print(F("   YOU ARE GREAT!!"));
	}
	
	arduboy.display();
	
	tautInput();
	while(true) {
		delay(10);
		updateInput();
		if(B_PRESSED || A_PRESSED) {
			arduboy.tunes.tone(TONE_BACK, 20);
			break;
		}
	}
}
