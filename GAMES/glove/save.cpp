#include "save.h"
#include "records.h"
#include "prompt.h"
#include "game.h"

bool validSave;

uint8_t readAdvance(int16_t &address)
{
	uint8_t val = EEPROM.read(address);
	address++;
	return val;
}

void saveSetup()
{
    validSave = checkFileValidity(GAME_SAVE_FILE);
}

void saveCreate()
{
	if(!validSave)
		initializeSave(GAME_SAVE_FILE);
	
	validSave = true;
}

bool checkFileValidity(uint8_t file)
{
	int16_t address = GameSaveOffset + FILE_VALID;
	
	/*
	int8_t* checkStr = "LoMeM";
	uint8_t checkIndex;
	
	for(checkIndex = 0; checkIndex <= 5; ++checkIndex)
	{
		if(readAdvance(address) != checkStr[checkIndex])
			return false;
	}
	
	return true;
	*/
	
	
	if((EEPROM.read(address+FILE_VALID) == 'L')
		&& (EEPROM.read(address+FILE_VALID+1) == 'o')
		&& (EEPROM.read(address+FILE_VALID+2) == 'M')
		&& (EEPROM.read(address+FILE_VALID+3) == 'e')
		&& (EEPROM.read(address+FILE_VALID+4) == 'M'))
	{
		return true;
	}
	return false;
	
}


void breakSave(uint8_t file)
{
	int16_t address = GameSaveOffset;
	
	// Write game identity
	EEPROM.write(address+FILE_VALID, 'L');
	EEPROM.write(address+FILE_VALID+1, 'o');
	EEPROM.write(address+FILE_VALID+2, 'M');
	EEPROM.write(address+FILE_VALID+3, 'e');
	EEPROM.write(address+FILE_VALID+4, 'q');
  EEPROM.commit();
}

void initializeSave(uint8_t file)
{
	int16_t address = GameSaveOffset;
	int16_t offset;
	
	// Write game identity
	EEPROM.write(address+FILE_VALID, 'L');
	EEPROM.write(address+FILE_VALID+1, 'o');
	EEPROM.write(address+FILE_VALID+2, 'M');
	EEPROM.write(address+FILE_VALID+3, 'e');
	EEPROM.write(address+FILE_VALID+4, 'M');
	
	// Blank out Room Visited Completion and continue file
	for(offset = 0; offset < 14; ++offset) {
		EEPROM.write(address+FILE_COMPLETION+offset, 0);
	}
	
	// Blank out High Score files
	for(int8_t player = 0; player < 30; player += 1) {
		EEPROM.write(20+address+player, 0);
	}
 EEPROM.commit();
}


int16_t getRoomClearPercentage()
{
	int8_t i;
	int16_t address = GameSaveOffset;
	int8_t block;
	int8_t completed = 0;
	
	if(!validSave) return 0;
	
	for(int8_t blockNum = 0; blockNum < 5; ++blockNum) {
		block = EEPROM.read(address+FILE_COMPLETION+blockNum);
		for(i=0; i<8; ++i) {
			if((block>>i) & 1) completed++;
		}
	}
	
	return (completed*100)/30;
}


bool saveHighScore()
{
	int16_t address = GameSaveOffset;
	
	int8_t myScoreIndex;
	int16_t compareScore;
	
	int16_t myScoreAddress;
	int16_t srcScoreAddress;
	int16_t destScoreAddress;
	
	uint8_t hi;
	uint8_t lo;
	
	for(myScoreIndex=0; myScoreIndex<3; ++myScoreIndex) {
		srcScoreAddress = 20 + (myScoreIndex*10) + address;
		hi = EEPROM.read(srcScoreAddress+FILE_SCORE);
		lo = EEPROM.read(srcScoreAddress+FILE_SCORE+1);
		compareScore = (hi << 8) | lo;
		if(score > compareScore) break;
	}
	
	if(myScoreIndex < 3) {
		// Input name
		int8_t input;
		int8_t nameLength = 0;
		char inputName[5];
		
		for(int8_t i=0; i<4; ++i) {
			inputName[i] = ' ';
		}
		inputName[4] = '\0';
		
		int8_t cursorPos = 0;
		
		while(true) {
			arduboy.clear();
			arduboy.setCursor(0,0);
			arduboy.print(F("You set a high score!"));
			arduboy.setCursor(0,8);
			arduboy.print(F(" Enter name: \""));
			arduboy.print(inputName);
			arduboy.setCursor(6*18, 8);
			arduboy.print("\"");
			input = prompt_keyboard(cursorPos);
			if(input == 0) {
				if(nameLength > 0) nameLength--;
				if(nameLength < 4) inputName[nameLength] = ' ';
			} else {
				if(nameLength >= 4) break;
				inputName[nameLength] = input;
				nameLength++;
			}
		}
		
		
		for(int8_t i=2; i>myScoreIndex; --i) {
			srcScoreAddress = 20 + ((i-1)*10) + address;
			destScoreAddress = 20 + (i*10) + address;
			for(int8_t data=0; data<10; ++data) {
				hi = EEPROM.read(srcScoreAddress+data);
				EEPROM.write(destScoreAddress+data, hi);
			}
		}
		
		myScoreAddress = 20 + (myScoreIndex*10) + address;
		EEPROM.write(myScoreAddress+FILE_SCORE, ((score >> 8) & 0xFF));
		EEPROM.write(myScoreAddress+FILE_SCORE+1, (score & 0xFF));
		EEPROM.write(myScoreAddress+FILE_TIME, ((gameTime >> 8) & 0xFF));
		EEPROM.write(myScoreAddress+FILE_TIME+1, (gameTime & 0xFF));
		EEPROM.write(myScoreAddress+FILE_LEVEL, levelsCompleted);
		EEPROM.write(myScoreAddress+FILE_NAME, inputName[0]);
		EEPROM.write(myScoreAddress+FILE_NAME+1, inputName[1]);
		EEPROM.write(myScoreAddress+FILE_NAME+2, inputName[2]);
		EEPROM.write(myScoreAddress+FILE_NAME+3, inputName[3]);
		
		displayRecords();
		EEPROM.commit();
		return true;
	}
  EEPROM.commit();
	return false;
}


void markRoomAsCleared(uint8_t room)
{
	if(room > numLevels) return;
	int16_t address = GameSaveOffset;
	int16_t pos = address+FILE_COMPLETION+(room/8);
	int8_t data = EEPROM.read(pos);
	data |= 1 << (room%8);
	EEPROM.write(pos, data);
  EEPROM.commit();
}

int16_t getCurrentLevelProgress()
{
	int16_t address = GameSaveOffset;
	return EEPROM.read(10+address+FILE_LEVEL);
}

void displayLoadGame(uint8_t file)
{
	uint8_t level = currentLevel;
	rollingScore = -30;
	quitGame = false;
	int16_t address = GameSaveOffset;

	if(EEPROM.read(10+address+FILE_CONTINUE) == 1) {
		gameSetup();
		
		uint8_t hi = EEPROM.read(10+address+FILE_SCORE);
		uint8_t lo = EEPROM.read(10+address+FILE_SCORE+1);
		score = (hi << 8) | lo;
		
		hi = EEPROM.read(10+address+FILE_HEALTH);
		lo = EEPROM.read(10+address+FILE_HEALTH+1);
		p1.health = (hi << 8) | lo;
		
		levelsCompleted = getCurrentLevelProgress();
		currentLevel = EEPROM.read(10+address+FILE_CURRENT_LEVEL);
		
		hi = EEPROM.read(10+address+FILE_TIME);
		lo = EEPROM.read(10+address+FILE_TIME+1);
		gameTime = (hi << 8) | lo;
		
		if(currentLevel == 0)
			currentLevel = 255;
				
		gameGotoLevel(currentLevel);
		gameLoop();
	}
		
	quitGame = true;
}

void saveGame(uint8_t file)
{
	int16_t address = GameSaveOffset + 10;
	EEPROM.write(address+FILE_SCORE, ((score >> 8) & 0xFF));
	EEPROM.write(address+FILE_SCORE+1, (score & 0xFF));
	EEPROM.write(address+FILE_HEALTH, ((p1.health >> 8) & 0xFF));
	EEPROM.write(address+FILE_HEALTH+1, (p1.health & 0xFF));
	EEPROM.write(address+FILE_TIME, ((gameTime >> 8) & 0xFF));
	EEPROM.write(address+FILE_TIME+1, (gameTime & 0xFF));
	EEPROM.write(address+FILE_LEVEL, levelsCompleted);
	EEPROM.write(address+FILE_CURRENT_LEVEL, currentLevel);
	EEPROM.write(address+FILE_CONTINUE, 1);
 EEPROM.commit();
}

void deleteContinueSave()
{
	EEPROM.write(GameSaveOffset + 10 + FILE_CONTINUE, 0);
  EEPROM.commit();
}

void clearAllRooms()
{
	int16_t roomID;
	
	for(roomID = 0; roomID < numLevels; roomID++) {
		markRoomAsCleared(roomID);
	}
}
