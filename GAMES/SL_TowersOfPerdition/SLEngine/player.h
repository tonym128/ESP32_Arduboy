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

#ifndef PLAYER_H
#define PLAYER_H

#include <Arduino.h>

#define PLAYER_HP				PLAYER_LEVEL1_HP, PLAYER_LEVEL99_HP
#define PLAYER_MP				PLAYER_LEVEL1_MP, PLAYER_LEVEL99_MP
#define PLAYER_NEXTEXP	PLAYER_LEVEL1_NEXTEXP, PLAYER_LEVEL99_NEXTEXP

struct Player //location: xxx yyy ff
{
	uint8_t game[3];
	uint8_t name[8];
	uint8_t map;
	int8_t chunkID;
	int8_t x;
	int8_t y;
	int8_t dir;
	int16_t hp;
	uint8_t mp;
	uint8_t level;
	uint8_t statBonus[4];
	int16_t exp;
	int16_t gold;
	uint32_t largeChests;
	uint16_t immunity; //HY DK AO RK AQ FR NK CR (2-bits per spell)
	uint8_t items[8]; //8 bytes instead of 4 bytes in SRAM but saves 
	//uint32_t items; //POTN XPTN ETHR ELXR ANDT BELL EDRP RMDY (4-bits each indicating quantity)
	uint8_t spells[8]; //HOLY DARK AERO ROCK AQUA FIRE NUKE CURE (4-bits each indicating spell level)
	uint8_t seal;
	uint8_t equipment; //2-bits per type (accessory boots armor sword)
};

static Player player =
{
	//game
	{GAME_ID1, GAME_ID2, GAME_ID3},
	//name
	{CHAR_NULL, CHAR_NULL, CHAR_NULL, CHAR_NULL, CHAR_NULL, CHAR_NULL, CHAR_NULL, CHAR_NULL},
	//map, chunkID, x, y, dir
	START_MAP,START_CHUNK,START_X,START_Y,DIR_DOWN,
	//hp, mp
	PLAYER_LEVEL1_HP,PLAYER_LEVEL1_MP,
	//level, attackBonus, defenseBonus, agilityBonus, magicBonus, exp, gold
	1,{0,0,0,0},0,0,
	//largeChests
	0x00000000,
	//immunity (HOLY DARK AERO ROCK AQUA FIRE NUKE CURE)
	//10 10 10 10 10 10 10 10
	0xAAAA,
	//Items (POTION S-POTION, X-POTION, ETHER, S-ETHER, X-ETHER, ELIXER, X-ELIXIR)
	{0, 0, 0, 0, 0, 0, 0, 0},
	//Spells (CURE NUKE FIRE AQUA ROCK AERO DARK HOLY)
	{0, 0, 0, 0, 0, 0, 0, 0},
	//Seal
	//12
	//48
	0x00,
	//Equipment
	//mmssddaa
	0x00
};

//This takes less than 1 microsecond
//((lvl99 - lvl1) * (level - 1) * (level - 1) / 9604) + lvl1
int16_t getStat(int16_t level, int16_t lvl1, int16_t lvl99)
{
	//exponential curve doesn't work as well
	//reason: first 4 levels player has the same stats
	//return (int16_t)(((long)(lvl99 - lvl1) * (long)((level - 1) * (level - 1))) / 9604l) + lvl1;
	//linear curve works better
	return (int16_t)(((long)(lvl99 - lvl1) * (long)(level - 1)) / 98l) + lvl1;
}

int8_t numItems(int8_t id)
{
	return player.items[id];
}

void addRemoveItems(int8_t id, int8_t amount)
{
	player.items[id] += amount;
	if (player.items[id] > 99)
		player.items[id] = 99;
}

bool saveExists()
{
	if (EEPROM.read(EEPROM_STORAGE_SPACE_START) != GAME_ID1)
		return false;
	if (EEPROM.read(EEPROM_STORAGE_SPACE_START + 1) != GAME_ID2)
		return false;
	if (EEPROM.read(EEPROM_STORAGE_SPACE_START + 2) != GAME_ID3)
		return false;
	return true;
}

void loadGame()
{
	uint8_t *pValue = (uint8_t*)(void*)&player;
	uint16_t address = EEPROM_STORAGE_SPACE_START;
	for (uint8_t i = 0; i < sizeof(player); ++i)
		*pValue++ = EEPROM.read(address++);
}

void saveGame()
{
	const uint8_t *pValue = (const uint8_t*)(const void*)&player;
	uint16_t address = EEPROM_STORAGE_SPACE_START;
	for (uint8_t i = 0; i < sizeof(player); ++i)
		EEPROM.write(address++, *pValue++);
    EEPROM.commit();
}

void fillSpellRect(uint8_t pattern)
{
	uint8_t startX = (selection >> 1) * 32;
	uint8_t stopX = startX + 30;
	uint8_t y = (selection & 1) + 6;
	for (uint8_t x = startX; x <= stopX; ++x)
		arduboy.sBuffer[y * 128 + x] = pattern;
}

void drawSpells()
{
	cursorX = 1;
	cursorY = 48;
	lineStartX = 1;
	for (uint8_t i = 0; i < 8; ++i)
	{
		cursorX = ((i & 6) << 4) + 1;
		cursorY = ((i & 1) << 3) + 48;
		if (player.spells[i] > 0)
		{
			drawName(wordSpells + i * 4, 4);
			drawChar(CHAR_0 + player.spells[i]);
		}
		if (i == 3)
			drawChar(CHAR_LINE);
	}
}

void drawStat(int16_t value, int16_t maxValue)
{
	int8_t x = cursorX;
	cursorX += 12;
	drawNumber(value);
	cursorX = x + 18;
	drawChar(CHAR_SLASH);
	cursorX += 12;
	drawNumber(maxValue);
}

void drawPlayerName()
{
	for (uint8_t i = 0; i < 8; ++i)
		drawChar(player.name[i]);
}

void playerUseItem(int8_t id)
{
	int16_t amountHP = 0;
	uint8_t amountMP = 0;
	int16_t maxHP = getStat(player.level, PLAYER_HP);
	uint8_t maxMP = getStat(player.level, PLAYER_MP);
	if (id < 3)
		amountHP = (int16_t)pgm_read_byte(healItems + id);
	else if (id < 6)
		amountMP = (uint8_t)pgm_read_byte(healItems + id);
	else if (id == 6)
	{
		amountHP = maxHP >> 1;
		amountMP = maxMP >> 1;
	}
	else if (id == 7)
	{
		amountHP = maxHP;
		amountMP = maxMP;
	}
	player.hp += amountHP;
	player.mp += amountMP;
	if (player.hp > maxHP)
		player.hp = maxHP;
	if (player.mp > maxMP)
		player.mp = maxMP;
}

#endif //PLAYER_H
