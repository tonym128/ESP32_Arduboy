#include "globals.h"

Arduboy2 arduboy;

bool old_a = false;
bool old_b = false;
bool old_up = false;
bool old_down = false;
bool old_left = false;
bool old_right = false;

bool new_a = false;
bool new_b = false;
bool new_up = false;
bool new_down = false;
bool new_left = false;
bool new_right = false;

char progmemString[] = "";

uint8_t GameMode = GAME_MODE_GLOVE;
int16_t GameSaveOffset = GAME_GLOVE_OFFSET;

void tautInput()
{
	new_a = true;
	new_b = true;
	new_up = true;
	new_down = true;
	new_left = true;
	new_right = true;
}

void updateInput()
{
	old_a = new_a;
	old_b = new_b;
	old_up = new_up;
	old_down = new_down;
	old_left = new_left;
	old_right = new_right;
	
	new_a = arduboy.pressed(B_BUTTON);
	new_b = arduboy.pressed(A_BUTTON);
	new_up = arduboy.pressed(UP_BUTTON);
	new_down = arduboy.pressed(DOWN_BUTTON);
	new_left = arduboy.pressed(LEFT_BUTTON);
	new_right = arduboy.pressed(RIGHT_BUTTON);
  delay(1);
}

char* rps(const char* const* str)
{
	strcpy_P(progmemString, (char*)pgm_read_dword(str));
	return progmemString;
}

char* dps(const char* str)
{
	strcpy_P(progmemString, (char*)pgm_read_dword(str));
	return progmemString;
}

char* eps(char* str)
{
	strcpy_P(progmemString, str);
    return progmemString;
}

char* spsn(int16_t addr, uint8_t len)
{
	int16_t offset;
	
	progmemString[0] = '\0';
	
	for(offset = 0; offset < len; offset++)
	{
		progmemString[offset] = EEPROM.read(addr+offset);	
	}
	progmemString[offset] = '\0';
	
    return progmemString;
}


void printTime(uint16_t time)
{
	int16_t mins = time/60;
	int16_t secs = time%60;
	
	if(mins < 10) arduboy.print('0');
	arduboy.print(mins);
	arduboy.print(':');
	if(secs < 10) arduboy.print('0');
	arduboy.print(secs);
		
}

int16_t EEPROMReadint16_t(int16_t addr)
{
	uint8_t hi = EEPROM.read(addr+FILE_SCORE);
	uint8_t lo = EEPROM.read(addr+FILE_SCORE+1);
	return (hi << 8) | lo;
}
