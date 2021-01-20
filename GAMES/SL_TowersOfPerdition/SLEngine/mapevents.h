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

#ifndef MAPEVENTS_H
#define MAPEVENTS_H

#define LARGE_CHEST_ITEM		0x80
#define LARGE_CHEST_IMMUNE	0x40
#define LARGE_CHEST_SWORD		0
#define LARGE_CHEST_ARMOR		1
#define LARGE_CHEST_BOOTS		2
#define LARGE_CHEST_ACCESS	3

void drawCost()
{
	if (cost >= 100)
		drawChar(((cost / 100) % 10) + CHAR_0);
	if (cost >= 10)
		drawChar(((cost / 10) % 10) + CHAR_0);
	drawChar((cost % 10) + CHAR_0);
	drawChar(CHAR_G);
}

void eventTeleport(uint16_t param)
{
	//mmmm cccccc xxx yyy
	int8_t count = 0;
	int8_t map = (param >> 12) & 15;
	uint8_t keys = pgm_read_byte(mapKeys + map);
	bool erase = player.map != map;
	if ((keys & player.seal) != keys)
	{
		cursorX = 1;
		cursorY = 56;
		drawSmallMessageBox();
		drawMessageCompressed(messageMissingKey);
		arduboy.display();
		waitButtons(A_BUTTON | B_BUTTON);
		return;
	}
	if (erase)
	{
#ifdef ENABLE_MUSIC
		sound.noTone();
#endif
		do
		{
			++count;
			drawMap();
			teleportErase(count);
			arduboy.display(CLEAR_BUFFER);
		} while (count <= 32);
	}
	player.map = ((param >> 12) & 15);
	player.chunkID = (param >> 6) & 63;
	player.x = (param >> 3) & 7;
	player.y = param & 7;
	loadMap();
	if (erase)
	{
#ifdef ENABLE_MUSIC
		song = getMapSong(pMap);
		songT = 0;
#endif
		do
		{
			--count;
			drawMap();
			teleportErase(count);
			arduboy.display(CLEAR_BUFFER);
		} while (count > 0);
		drawMap();
		playerSteps = 0;
	}
}

void eventInn()
{
	cost = (getStat(player.level, PLAYER_HP) - player.hp) + (getStat(player.level, PLAYER_MP) - player.mp) * 4;
	flags |= FLAG_USE_MESSAGE_BUFFER | FLAG_SHOW_MESSAGE_BUFFER | FLAG_SHOW_GOLD | FLAG_SHOW_YESNO | FLAG_HOLD_MESSAGE_BUFFER;
	drawMessageCompressed(messageInn1);
	drawCost();
	drawChar(CHAR_LINE);
	drawMessageCompressed(messageInn2);
	fillMessageBuffer();
	globalCounter = 0;
	selection = 0;
	battleFlashSprite = 8;
	gameState = STATE_PURCHASE;
}

void eventShop(uint8_t param)
{
	battleFlashSprite = param;
	cost = pgm_read_byte(itemCosts + param);
	flags |= FLAG_USE_MESSAGE_BUFFER | FLAG_SHOW_MESSAGE_BUFFER | FLAG_SHOW_GOLD | FLAG_SHOW_YESNO | FLAG_HOLD_MESSAGE_BUFFER;
	drawMessageCompressed(wordShopBuy);
	drawName(wordItems + param * 8, 8);
	while (messageBuffer[bufferCursor - 1] == CHAR_SPACE)
		messageBuffer[--bufferCursor] = CHAR_NULL;
	drawMessageCompressed(wordFor);
	drawChar(CHAR_SPACE);
	drawCost();
	drawChar(CHAR_LINE);
	drawMessageCompressed((uint8_t*)pgm_read_dword(messageItemDescription + param));
	fillMessageBuffer();
	globalCounter = 0;
	selection = 0;
	gameState = STATE_PURCHASE;
}

void eventLargeChest(uint8_t param1, uint8_t param2)
{
	uint32_t chestBit = (uint32_t)1 << param1;
	flags |= FLAG_USE_MESSAGE_BUFFER | FLAG_SHOW_MESSAGE_BUFFER;
	if (player.largeChests & chestBit)
		drawMessageCompressed(messageChestEmpty);
	else if (param2 & LARGE_CHEST_ITEM)
	{
		uint8_t itemType = param2 & 7;
		uint8_t itemID = ((player.equipment >> (itemType * 2)) & 3) + 1;
		drawMessageCompressed(messageItemFound);
		if (param2 & LARGE_CHEST_IMMUNE)
		{
			drawName(wordSpells + itemType * 4, 4);
			drawMessageCompressed(messageImmunity);
			player.immunity ^= (3 << (itemType * 2));
		}
		else
		{
			drawMessageCompressed((uint8_t*)pgm_read_dword(messageLargeChestItem + itemType));
			player.statBonus[itemType] += 5 * itemID;
			player.equipment += 1 << (itemType * 2);
		}
	}
	else
	{
		++player.spells[param2];
		drawMessageCompressed(messageSpellLearned);
		drawName(wordSpells + param2 * 4, 4);
		drawChar(player.spells[param2] + CHAR_0);
	}
	player.largeChests |= chestBit;
	fillMessageBuffer();
	globalCounter = 0;
}

void eventMessage(uint8_t param1, uint8_t param2)
{
	flags |= FLAG_USE_MESSAGE_BUFFER | FLAG_SHOW_MESSAGE_BUFFER;
	player.seal |= param2;
	drawMessageCompressed((uint8_t*)pgm_read_dword(messageEvents + param1));
	fillMessageBuffer();
	globalCounter = 0;
}

void eventBossBattle(uint8_t param1, uint8_t param2)
{
	int8_t count = 0;
	if (player.seal & param2)
		return;
	do
	{
		drawMap();
		for (int16_t j = 0; j < 1024; ++j)
			arduboy.sBuffer[j] = ~arduboy.sBuffer[j];
		teleportErase(count);
		arduboy.display(CLEAR_BUFFER);
		++count;
		if (count == 0)
			delay(500);
	} while (count <= 32);
	currentMonsterGroup = bossGroups + param1 * BOSS_GROUP_SIZE;
	loadMonsterGroup();
	previousState = gameState;
	gameState = STATE_BATTLE;
#ifdef ENABLE_MUSIC
	song = SONG_BATTLE;
	songT = 0;
#endif
	selection = 0;
	globalCounter = 0;
	playerSteps = 0;
	player.seal |= param2;
}

bool execEvent(uint8_t buttonActivated)
{
	uint8_t num = pgm_read_byte(pMapEvents);
	uint16_t loc;
	uint8_t eventID;
	uint8_t chunkID = player.chunkID;
	int8_t x = player.x;
	int8_t y = player.y;
	const uint8_t *event = pMapEvents + 1;
	if (buttonActivated)
	{
		if (player.dir == DIR_UP)
			--y;
		else if (player.dir == DIR_DOWN)
			++y;
		else if (player.dir == DIR_LEFT)
			--x;
		else if (player.dir == DIR_RIGHT)
			++x;
		if (x < 0)
		{
			x = 7;
			chunkID = chunkIDs[3];
		}
		else if (x == 8)
		{
			x = 0;
			chunkID = chunkIDs[5];
		}
		else if (y < 0)
		{
			y = 7;
			chunkID = chunkIDs[1];
		}
		else if (y == 8)
		{
			y = 0;
			chunkID = chunkIDs[7];
		}
	}
	for (uint8_t i = 0; i < num; ++i)
	{
		loc = READ_WORD(event);
		if ((pgm_read_byte(event) & EVENT_BUTTON_ACTIVATED) == buttonActivated)
		{
			if (((loc >> 9) & 0x3F) == chunkID && ((loc >> 6) & 0x7) == (uint8_t)x && ((loc >> 3) & 0x7) == (uint8_t)y)
			{
				eventID = (uint8_t)(loc & 0x7);
				event += 2;
				switch (eventID)
				{
					case EVENT_TELEPORT:
						eventTeleport(READ_WORD(event));
						break;
					case EVENT_INN:
						eventInn();
						break;
					case EVENT_SHOP:
						eventShop(pgm_read_byte(event));
						break;
					case EVENT_LARGECHEST:
						eventLargeChest(pgm_read_byte(event), pgm_read_byte(event + 1));
						break;
					case EVENT_MESSAGE:
						eventMessage(pgm_read_byte(event), pgm_read_byte(event + 1));
						break;
					case EVENT_BOSSBATTLE:
						eventBossBattle(pgm_read_byte(event), pgm_read_byte(event + 1));
						break;
				}
				return true;
			}
		}
		event += 4;
	}
	return false;
}

#endif //MAPEVENTS_H
