/*************************************************************************
 * Shattered Lands: Towers of Perdition                                  *
 * C opyright (C) 2019  Justin (tuxinator2009) Davis                     *
 *                                                                       *
 * This program is free software: you can redistribute it and/or modify  *
 * it under the terms of the GNU General Public License as published by  *
 * the Free Software Foundation, either version 3 of the License, or     *
 * (at your option) any later version.                                   *
 *                                                                       *
 * This program is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 * GNU General Public License for more details.                          *
 *                                                                       *
 * You should have received a copy of the GNU General Public License     *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *************************************************************************/

#ifndef GAME_H
#define GAME_H

#include "globals.h"
#include "text.h"
#ifdef ENABLE_MUSIC
#include "music.h"
#endif // ENABLE_MUSIC
#include "player.h"
#include "maps.h"
#include "battle.h"
#include "mapevents.h"

//ARDUBOY_NO_USB;


void drawYesNo()
{
	uint8_t startX = 91;
	uint8_t stopX = 109;
	if (selection == 1)
	{
		startX = 115;
		stopX = 127;
	}
	for (uint8_t x = 90; x <= 128; ++x)
		arduboy.sBuffer[640 + x] = (x >= startX && x <= stopX) ? 0xFF:0x00;
	cursorX = 92;
	cursorY = 40;
	drawMessageCompressed(wordYesNo);
}

void drawGold()
{
	for (uint8_t x = 0; x <= 37; ++x)
		arduboy.sBuffer[640 + x] = 0x00;
	cursorX = 30;
	cursorY = 40;
	drawChar(CHAR_G);
	cursorX -= 12;
	drawNumber(player.gold);
}

void drawMessageBuffer()
{
	uint8_t justPressed = arduboy.justPressedButtons();
	register uint8_t count = (uint8_t)globalCounter;
	cursorX = 0;
	cursorY = 56;
	drawMessageBox();
	for (uint8_t i = 0; i < count; ++i)
	{
		uint8_t ch = messageBuffer[i];
		if (ch == CHAR_LINE)
		{
			for (uint8_t x = 0; x < 128; ++x)
			{
				arduboy.sBuffer[x + 768] = arduboy.sBuffer[x + 896];
				arduboy.sBuffer[x + 896] = 0x00;
			}
			cursorX = 0;
		}
		else
			drawChar(ch);
	}
	if (messageBuffer[count] != CHAR_NULL)
		++globalCounter;
	else if (((justPressed & A_BUTTON) || (justPressed & B_BUTTON)) && !(flags & FLAG_HOLD_MESSAGE_BUFFER))
		flags &= ~FLAG_SHOW_MESSAGE_BUFFER;
}

void gameLoop_Title()
{
	uint8_t justPressed = arduboy.justPressedButtons();
	uint8_t startX = 0;
	uint8_t stopX = 126;
	arduboy.drawBitmap(1, 2, bmpTitle, 126, 48);
	drawSmallMessageBox();
#ifdef ENABLE_NAME_ENTRY
	if (selection == 0)
	{
		startX = 0;
		stopX = 18;
	}
	else if (selection == 1)
	{
		startX = 30;
		stopX = 78;
	}
	else if (selection == 2)
	{
		startX = 90;
		stopX = 126;
	}
	cursorX = 1;
	cursorY = 56;
#else
	if (selection == 0)
	{
		startX = 15;
		stopX = 63;
	}
	else if (selection == 1)
	{
		startX = 75;
		stopX = 111;
	}
	cursorX = 16;
	cursorY = 56;
#endif //ENABLE_NAME_ENTRY
	for (uint8_t x = startX; x <= stopX; ++x)
		arduboy.sBuffer[x + 896] = 0xFF;
	drawMessageCompressed(messageTitleOptions);
#ifdef ENABLE_NAME_ENTRY
	if (justPressed & LEFT_BUTTON)
		selection = (selection + 2) % 3;
	else if (justPressed & RIGHT_BUTTON)
		selection = (selection + 1) % 3;
#else
	if (justPressed & (LEFT_BUTTON | RIGHT_BUTTON))
		selection ^= 1;
#endif
	else if (justPressed & B_BUTTON)
	{
#ifdef ENABLE_NAME_ENTRY
		if (selection == 0) // NEW GAME
		{
			loadMap();
			gameState = STATE_NAMEENTRY;
			selection = 0;
			globalCounter = 0;
			arduboy.initRandomSeed();
		}
		else if (selection == 1)
		{
			if (saveExists())
			{
				loadGame();
				loadMap();
				#ifdef ENABLE_MUSIC
				song = getMapSong(pMap);
				songT = 0;
				#endif
				gameState = STATE_PLAYING;
				selection = 0;
				globalCounter = 0;
				arduboy.initRandomSeed();
			}
			else
			{
				cursorX = 10;
				drawSmallMessageBox();
				drawMessageCompressed(messageNoSaveData);
				arduboy.display();
				waitButtons(A_BUTTON | B_BUTTON);
			}
		}
		else if (selection == 2)
			gameState = STATE_UPLOAD;
#else
		if (selection == 0)
		{
			if (saveExists())
			{
				loadGame();
				if (player.chunkID == -1) //Game continued from previous game in series
				{
					player.map = START_MAP;
					player.chunkID = START_CHUNK;
					player.x = START_X;
					player.y = START_Y;
					player.dir = DIR_DOWN;
				}
				loadMap();
				#ifdef ENABLE_MUSIC
				song = getMapSong(pMap);
				songT = 0;
				#endif
				gameState = STATE_PLAYING;
				selection = 0;
				globalCounter = 0;
				arduboy.initRandomSeed();
			}
			else
			{
				cursorX = 10;
				drawSmallMessageBox();
				drawMessageCompressed(messageNoSaveData);
				arduboy.display();
				waitButtons(A_BUTTON | B_BUTTON);
			}
		}
		else if (selection == 1)
			gameState = STATE_UPLOAD;
#endif //ENABLE_NAME_ENTRY
		selection = 0;
	}
}

void gameLoop_Playing()
{
	bool check = false;
	uint8_t pressed = arduboy.pressedButtons();
	uint8_t justPressed = arduboy.justPressedButtons();
	
	drawMap();
	
	if (flags & FLAG_SHOW_MESSAGE_BUFFER)
		return;
	else if (playerXOff == 0 && playerYOff == 0)
	{
		if (pressed & LEFT_BUTTON)
		{
			player.dir = DIR_LEFT;
			if (canMove(pChunk, player.x, player.y, DIR_LEFT))
			{
				--player.x;
				playerXOff = -16;
			}
		}
		else if (pressed & RIGHT_BUTTON)
		{
			player.dir = DIR_RIGHT;
			if (canMove(pChunk, player.x, player.y, DIR_RIGHT))
			{
				++player.x;
				playerXOff = 16;
			}
		}
		else if (pressed & UP_BUTTON)
		{
			player.dir = DIR_UP;
			if (canMove(pChunk, player.x, player.y, DIR_UP))
			{
				--player.y;
				playerYOff = -16;
			}
		}
		else if (pressed & DOWN_BUTTON)
		{
			player.dir = DIR_DOWN;
			if (canMove(pChunk, player.x, player.y, DIR_DOWN))
			{
				++player.y;
				playerYOff = 16;
			}
		}
		else if (justPressed & B_BUTTON)
			execEvent(EVENT_BUTTON_ACTIVATED);
		else if (justPressed & A_BUTTON)
		{
			previousState = gameState;
			gameState = STATE_MENU_MAIN;
			globalCounter = 0;
			selection = 0;
		}
	}
	else
		check = true;
	if (playerXOff < 0)
		playerXOff += 2;
	else if (playerXOff > 0)
		playerXOff -= 2;
	if (playerYOff < 0)
		playerYOff += 2;
	else if (playerYOff > 0)
		playerYOff -= 2;
	if (player.x == -1)
	{
		player.x = 7;
		player.chunkID = getNeighborChunk(pChunk, DIR_LEFT);
		pChunk = getChunk(pMap, player.chunkID);
		loadChunkIDs();
	}
	else if (player.x == 8)
	{
		player.x = 0;
		player.chunkID = getNeighborChunk(pChunk, DIR_RIGHT);
		pChunk = getChunk(pMap, player.chunkID);
		loadChunkIDs();
	}
	if (player.y == -1)
	{
		player.y = 7;
		player.chunkID = getNeighborChunk(pChunk, DIR_UP);
		pChunk = getChunk(pMap, player.chunkID);
		loadChunkIDs();
	}
	else if (player.y == 8)
	{
		player.y = 0;
		player.chunkID = getNeighborChunk(pChunk, DIR_DOWN);
		pChunk = getChunk(pMap, player.chunkID);
		loadChunkIDs();
	}
	if (check && playerXOff == 0 && playerYOff == 0)
	{
		++playerSteps;
		if (!execEvent(EVENT_STEP_ACTIVATED))
		{
			if (player.map == 0)
				randomBattle((uint8_t)getTileID(pChunk, player.x, player.y));
			else
				randomBattle(pgm_read_byte(mapMonsters + player.map));
		}
	}
}

void gameLoop_Battle()
{
	for (uint8_t y = 0; y < 6; ++y)
	{
		for (uint8_t x = 0; x < 72; ++x)
			arduboy.sBuffer[y * 128 + x] = 0xFF;
		for (uint8_t x = 72; x < 80; ++x)
			arduboy.sBuffer[y * 128 + x] = (x % 2 == 0) ? 0x55:0xAA;
	}
	if (isBossBattle(currentMonsterGroup))
	{
		if (battleRenderSprite & 64)
			drawBoss();
		if (battleRenderSprite & 8)
			drawMonster(4);
		if (battleRenderSprite & 4)
			drawMonster(5);
	}
	else
	{
		for (uint8_t i = 0; i < 6; ++i)
		{
			if (battleRenderSprite & (128 >> i))
				drawMonster(i);
		}
	}
	drawPlayer(battleRenderSprite & 2);
	if (arduboy.everyXFrames(3))
		battleRenderSprite ^= battleFlashSprite;
	switch (battleState)
	{
		case BATTLE_CHOOSEACTION:
			battleChooseAction();
			break;
		case BATTLE_SELECT_MONSTER:
			battleSelectMonster();
			break;
		case BATTLE_SELECT_ITEM:
			battleSelectItem();
			break;
		case BATTLE_SELECT_SPELL:
			battleSelectSpell();
			break;
		case BATTLE_MONSTER_ACTIONS:
			battleMonsterActions();
			break;
		case BATTLE_RESOLVE_COMBAT:
			battleResolveCombat();
			break;
		case BATTLE_VICTORY:
			battleVictory();
			break;
	}
}

void gameLoop_Menu_Main()
{
	uint8_t startX = 0;
	uint8_t stopX = 0;
	uint8_t justPressed = arduboy.justPressedButtons();
	
	drawMap();
	drawSmallMessageBox();
	if (selection == 0)
	{
		startX = 3;
		stopX = 27;
	}
	else if (selection == 1)
	{
		startX = 33;
		stopX = 63;
	}
	else if (selection == 2)
	{
		startX = 69;
		stopX = 93;
	}
	else if (selection == 3)
	{
		startX = 99;
		stopX = 123;
	}
	for (uint8_t x = startX; x <= stopX; ++x)
		arduboy.sBuffer[x + 896] = 0xFF;
	cursorX = 4;
	cursorY = 56;
	drawMessageCompressed(messageMainMenu);
	if (justPressed & LEFT_BUTTON)
		selection += 3;
	else if (justPressed & RIGHT_BUTTON)
		++selection;
	else if (justPressed & A_BUTTON)
		gameState = STATE_PLAYING;
	else if (justPressed & B_BUTTON)
	{
		if (selection == 0) //ITEM MENU
			gameState = STATE_MENU_ITEMS;
		else if (selection == 1) //STATS MENU
			gameState = STATE_MENU_STATS;
		else if (selection == 2) //SAVE GAME
		{
			saveGame();
			cursorX = 34;
			cursorY = 56;
			drawSmallMessageBox();
			drawMessageCompressed(messageGameSaved);
			arduboy.display();
			waitButtons(A_BUTTON | B_BUTTON);
			gameState = STATE_PLAYING;
		}
		else if (selection == 3) //QUIT GAME
			arduboy.exitToBootloader();
		selection = 0;
		globalCounter = 0;
	}
	selection &= 3;
}

void gameLoop_Menu_Items()
{
	uint8_t justPressed = arduboy.justPressedButtons();
	
	for (int16_t i = 128; i < 896; ++i)
		arduboy.sBuffer[i] = 0xFF;
	cursorX = 22;
	cursorY = -1;
	drawMessageCompressed(messageSelectAnItem);
	cursorX = 0;
	cursorY = 56;
	if (numItems(selection) > 0)
		drawMessageCompressed((uint8_t*)pgm_read_dword(messageItemDescription + selection));
	if (player.items == 0)
	{
		cursorX = 7;
		cursorY = 56;
		drawMessageCompressed(messageNoItems);
	}
	else
	{
		uint8_t startX = (selection & 1) ? 65:0;
		uint8_t stopX = (selection & 1) ? 128:63;
		uint8_t y = (selection >> 1) + 2;
		for (uint8_t x = startX; x < stopX; ++x)
			arduboy.sBuffer[y * 128 + x] = 0x00;
		for (uint8_t i = 0; i < 8; ++i)
		{
			cursorX = (i & 1) ? 66:0;
			cursorY = ((i << 2) & 0xF8) + 16;
			if (numItems(i) > 0)
			{
				drawName(wordItems + i * 8, 8);
				cursorX += 9;
				drawNumber(numItems(i));
			}
		}
	}
	if ((justPressed & LEFT_BUTTON) || (justPressed & RIGHT_BUTTON))
		selection ^= 1;
	else if (justPressed & UP_BUTTON)
		selection = (selection + 6) & 7;
	else if (justPressed & DOWN_BUTTON)
		selection = (selection + 2) & 7;
	else if (justPressed & A_BUTTON)
	{
		selection = -1;
		gameState = previousState;
	}
	else if ((justPressed & B_BUTTON) && player.items[selection] != 0)
	{
		gameState = previousState;
		if (previousState == STATE_PLAYING)
		{
			playerUseItem(selection);
			--player.items[selection];
		}
	}
}

void gameLoop_Menu_Stats()
{
	uint8_t justPressed = arduboy.justPressedButtons();
	uint16_t immunity = IMMUNE_FIRE;
	arduboy.drawBitmap(0, 0, playerBattleSprite, 24, 24);
	cursorX = 25;
	cursorY = 0;
	lineStartX = 25;
	drawPlayerName();
	cursorX = 80;
	drawMessageCompressed(wordLevel);
	drawNumber(player.level);
	drawChar(CHAR_LINE);
	drawChar(CHAR_H);
	drawStat(player.hp, getStat(player.level, PLAYER_HP));
	cursorX = 80;
	drawChar(CHAR_M);
	drawStat(player.mp, getStat(player.level, PLAYER_MP));
	drawChar(CHAR_LINE);
	drawMessageCompressed(wordExp);
	cursorX -= 6;
	drawStat(player.exp, getStat(player.level, PLAYER_NEXTEXP));
	cursorX = 122;
	drawChar(CHAR_G);
	cursorX -= 12;
	drawNumber(player.gold);
	cursorX = 38;
	cursorY = 24;
	drawMessageCompressed(messageStats);
	cursorY = 32;
	for (uint8_t i = 0; i < 4; ++i)
	{
		cursorX = 50 + i * 24;
		drawNumber(getStat(player.level, pgm_read_byte(level1Stats + i), 255) + player.statBonus[i]);
	}
	cursorY = 40;
	for (uint8_t i = 0; i < 4; ++i)
	{
		cursorX = 38 + 24 * i;
		for (uint8_t j = 0; j < ((player.equipment >> (i * 2)) & 3); ++j)
			drawChar(i + CHAR_SWORD);
	}
	drawSpells();
	for (uint8_t i = 0; i < 6; ++i)
	{
		if (player.immunity & immunity)
			arduboy.drawBitmap((i & 1) * 8, (i >> 1) * 8 + 24, spellIcons + i * 8, 8, 8);
		immunity *= 4;
	}
	for (uint8_t i = 0; i < 4; ++i)
	{
		uint8_t x = (i & 1) * 8 + 19;
		uint8_t y = (i >> 1) * 8 + 28;
		if (player.seal & (1 << i))
			arduboy.drawBitmap(x, y, tileBitmaps + i * 8 + 2016, 8, 8);
		else if (player.seal & (16 << i))
			arduboy.drawBitmap(x + 4, y, bmpKey, 4, 8);
	}
	if ((justPressed & A_BUTTON) || (justPressed & B_BUTTON))
		gameState = STATE_PLAYING;
}

void gameLoop_Upload()
{
	uint8_t justPressed = arduboy.justPressedButtons();
	
	cursorX = 0;
	cursorY = 0;
	lineStartX = 0;
	drawMessageCompressed(messageUpload);
	if (justPressed & DOWN_BUTTON)
		arduboy.exitToBootloader();
	else if (justPressed & A_BUTTON)
		gameState = STATE_TITLE;
}

void gameLoop_Story()
{
	uint8_t justPressed = arduboy.justPressedButtons();
	
	cursorX = 0;
	cursorY = 0;
	lineStartX = 0;
	drawMessageCompressed((uint8_t*)pgm_read_dword(messageStory + selection));
	if ((justPressed & A_BUTTON) || (justPressed & B_BUTTON))
		++selection;
	if (selection == 4)
	{
		#ifdef ENABLE_MUSIC
		song = getMapSong(pMap);
		songT = 0;
		#endif
		gameState = STATE_PLAYING;
	}
	else if (selection == 9)
	{
		player.game[2] = GAME_ID3 + 1;
		player.chunkID = -1;
		player.largeChests = 0;
		player.seal = 0;
		player.equipment = 0;
		saveGame();
		arduboy.exitToBootloader();
	}
}

void gameLoop_Purchase()
{
	int8_t count = 1;
	int8_t dir = 1;
	uint8_t justPressed = arduboy.justPressedButtons();
	
	drawMap();
	if ((justPressed & LEFT_BUTTON) || (justPressed & RIGHT_BUTTON))
		selection ^= 1;
	else if (justPressed & A_BUTTON) 
		gameState = STATE_PLAYING;
	else if (justPressed & B_BUTTON)
	{
		if (selection == 0)
		{
			drawSmallMessageBox();
			cursorX = 0;
			cursorY = 56;
			if (player.gold < cost)
			{
				drawMessageCompressed(messageInsufficientFunds);
				arduboy.display();
				waitButtons(A_BUTTON | B_BUTTON);
			}
			else
			{
				player.gold -= cost;
				if (battleFlashSprite == 8)
				{
					player.hp = getStat(player.level, PLAYER_HP);
					player.mp = getStat(player.level, PLAYER_MP);
					do
					{
						drawMap();
						teleportErase(count);
						arduboy.display(CLEAR_BUFFER);
						count += dir;
						if (count == 32)
							dir *= -1;
					} while (count > 0);
				}
				else if (numItems(battleFlashSprite) == 99)
				{
					drawMessageCompressed(messageItemsFull);
					arduboy.display();
					waitButtons(A_BUTTON | B_BUTTON);
				}
				else
					addRemoveItems(battleFlashSprite, 1);
			}
		}
		gameState = STATE_PLAYING;
	}
	if (gameState != STATE_PURCHASE)
	{
		battleFlashSprite = 8;
		globalCounter = 0;
		selection = 0;
		flags &= ~(FLAG_SHOW_GOLD | FLAG_SHOW_YESNO | FLAG_SHOW_MESSAGE_BUFFER | FLAG_HOLD_MESSAGE_BUFFER);
	}
}

#ifdef ENABLE_NAME_ENTRY
void gameLoop_NameEntry()
{
	uint8_t justPressed = arduboy.justPressedButtons();
	
	cursorX = 1;
	cursorY = 0;
	drawMessageCompressed(wordName);
	arduboy.sBuffer[73] = 0xCC;
	arduboy.sBuffer[74] = 0xCC;
	cursorX = 80 + globalCounter * 6;
	cursorY = 4;
	drawChar(CHAR_DASH);
	cursorX = 80;
	cursorY = 0;
	drawPlayerName();
	cursorX = 1;
	cursorY = 10;
	for (uint8_t i = 0; i < 42; ++i)
	{
		if (i == selection)
			arduboy.fillRect(cursorX - 1, cursorY, 7, 9, WHITE);
		drawChar(i + 1);
		drawChar(CHAR_SPACE);
		if (i % 11 == 10)
		{
			cursorX = 1;
			cursorY += 15;
		}
	}
	if (selection == 42)
		arduboy.fillRect(cursorX - 1, cursorY, 7, 9, WHITE);
	drawChar(CHAR_NONE);
	drawChar(CHAR_SPACE);
	if (selection == 43)
		arduboy.fillRect(cursorX - 1, cursorY, 7, 9, WHITE);
	drawChar(CHAR_OK);
	if (justPressed & LEFT_BUTTON)
		--selection;
	else if (justPressed & RIGHT_BUTTON)
		++selection;
	else if (justPressed & UP_BUTTON)
		selection -= 11;
	else if (justPressed & DOWN_BUTTON)
		selection += 11;
	else if (justPressed & A_BUTTON)
	{
		if (globalCounter == 0)
		{
			selection = 0;
			globalCounter = 0;
			gameState = STATE_TITLE;
		}
		else if (player.name[globalCounter] == CHAR_NULL)
		{
			--globalCounter;
			player.name[globalCounter] = CHAR_NULL;
		}
		else
			player.name[globalCounter] = CHAR_NULL;
	}
	else if (justPressed & B_BUTTON)
	{
		if (selection == 42)
			player.name[globalCounter] = CHAR_SPACE;
		else if (selection == 43)
		{
			selection = 0;
			globalCounter = 0;
			gameState = STATE_STORY;
		}
		else
			player.name[globalCounter] = selection + 1;
		if (globalCounter < 7)
			++globalCounter;
	}
	if (selection < 0)
		selection += 44;
	else if (selection >= 44)
		selection -= 44;
}
#endif //ENABLE_NAME_ENTRY

void setup()
{
	//Serial.begin(115200);
	//Space Savings
	//Minimal Boot: 746 bytes
	//No USB: 30 bytes
	//BEGIN: arduboy.begin()
	arduboy.boot();
	arduboy.display();
	arduboy.systemButtons();
	#ifdef ENABLE_MUSIC
		arduboy.audio.begin();
		#ifdef MUSIC_USE_BEEP
			beep1.begin();
			#ifdef MUSIC_2PIN
				beep2.begin();
			#endif
		#endif
	#endif
	//END: arduboy.begin()
	arduboy.setFrameRate(30);
	
}

void loop()
{
	if (!(arduboy.nextFrame()))
		return;
	#ifdef MUSIC_USE_BEEP
		beep1.timer();
		#ifdef MUSIC_2PIN
			beep2.timer();
		#endif
	#endif
	arduboy.pollButtons();
	switch (gameState)
	{
		case STATE_TITLE:
			gameLoop_Title();
			break;
		case STATE_PLAYING:
			gameLoop_Playing();
			break;
		case STATE_BATTLE:
			gameLoop_Battle();
			break;
		case STATE_MENU_MAIN:
			gameLoop_Menu_Main();
			break;
		case STATE_MENU_ITEMS:
			gameLoop_Menu_Items();
			break;
		case STATE_MENU_STATS:
			gameLoop_Menu_Stats();
			break;
		case STATE_UPLOAD:
			gameLoop_Upload();
			break;
		case STATE_STORY:
			gameLoop_Story();
			break;
		case STATE_PURCHASE:
			gameLoop_Purchase();
			break;
#ifdef ENABLE_NAME_ENTRY
		case STATE_NAMEENTRY:
			gameLoop_NameEntry();
			break;
#endif
	}
	if (flags & FLAG_SHOW_GOLD)
		drawGold();
	if (flags & FLAG_SHOW_YESNO)
		drawYesNo();
	if (flags & FLAG_SHOW_MESSAGE_BUFFER)
		drawMessageBuffer();
	arduboy.display(CLEAR_BUFFER);
	#ifdef ENABLE_MUSIC
	if (arduboy.everyXFrames(songTempo[song]))
		nextNote();
	#endif
}

#endif //GAME_H
