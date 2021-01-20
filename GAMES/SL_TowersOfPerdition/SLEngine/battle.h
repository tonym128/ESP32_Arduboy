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

#ifndef BATTLE_H
#define BATTLE_H

#define MONSTER_SIZE				21
#define BOSS_SIZE						22
#define MONSTER_GROUP_SIZE	4
#define BOSS_GROUP_SIZE			2

#define ACTION_CHOOSING		-1
#define ACTION_ATTACK			0
#define ACTION_DEFEND			1
#define ACTION_USEITEM		2
#define ACTION_CASTSPELL	3
#define ACTION_FOCUS			4
#define ACTION_ESCAPE			5

#define BATTLE_ENCOUNTER_CHANCE	5000

struct BattleAction
{
	int8_t action; //action to be performed
	int8_t target; //target of action
	int8_t id; //item/spell id
	int8_t turn; //turn # based on speed stat
};

struct BattleStats
{
	const uint8_t *name;
	int8_t id;
	int16_t hp;
	int16_t maxHP;
	int16_t mp;
	int16_t maxMP;
	int16_t stats[4];
	uint16_t immunity;
	uint8_t spells[8];
	uint8_t experience;
	uint8_t gold;
};

BattleAction battleActions[7];
BattleStats battleStats[7];
int16_t battleExpGained;
int16_t battleGoldGained;

uint8_t getMonsterID(const uint8_t *group, uint8_t monster)
{
	uint8_t id = pgm_read_byte(group + (monster / 2));
	if (monster % 2 == 0)
		id >>= 4;
	return id & 15;
}

bool isBossBattle(const uint8_t *group)
{
	return (pgm_read_byte(group) & 0xF0) == 0xF0;
	//return getMonsterID(group, 0) == 15;
}

bool isMonsterGroupAvailable(const uint8_t *group, uint8_t info)
{
	uint8_t tiles = pgm_read_byte(group + 3);
	uint8_t monsterID;
	if (player.map == 0)
		return (tiles & (0x80 >> info));
	else
	{
		for (uint8_t i = 0; i < 6; ++i)
		{
			monsterID = getMonsterID(group, i);
			if (monsterID != 15 && (info & (0x80 >> monsterID)) == 0)
				return false;
		}
	}
	return true;
}

bool monsterHasAttackSpells(int8_t id)
{
	for (uint8_t i = 2; i < 8; ++i)
	{
		if (battleStats[id].spells[i] > 0)
			return true;
	}
	return false;
}

void clearBattleActions()
{
	for (uint8_t i = 0; i < 7; ++i)
	{
		battleActions[i].action = ACTION_CHOOSING;
		battleActions[i].target = -1;
		battleActions[i].id = -1;
		battleActions[i].turn = -1;
	}
}

void battleDetermineTurnOrder()
{
	int16_t maxAgility = 0;
	int8_t turn = 0;
	//start at 255 (max speed stat) and count down
	//when the value is equal to a monster/boss/party speed stat
	//set their turn # to the current turn #
	//increment the current turn #
	//in case of ties the order is (boss>monster>player)
	for (uint8_t j = 0; j < 7; ++j)
	{
		if (battleStats[j].stats[2] > maxAgility)
			maxAgility = battleStats[j].stats[2];
	}
	for (int16_t i = maxAgility; i > 0; --i)
	{
		for (uint8_t j = 0; j < 7; ++j)
		{
			if (battleStats[j].id != 15 && battleStats[j].hp > 0 && battleStats[j].stats[2] == i)
				battleActions[j].turn = turn++;
		}
	}
}

void drawMonster(int8_t num)
{
	if (battleStats[num].id != 15 && battleStats[num].hp > 0)
		arduboy.drawBitmap((num / 2) * 24, (num % 2) * 24, monsterSprites + battleStats[num].id * 72, 24, 24, BLACK);
}

void drawBoss()
{
	if (battleStats[1].hp == 0)
		return;
	if (battleStats[1].id == 4)
		arduboy.drawBitmap(0, 0, finalBoss, 72, 48, BLACK);
	else
		arduboy.drawBitmap(0, 0, bossSprites + battleStats[1].id * 288, 48, 48, BLACK);
}

void drawPlayer(bool drawSprite)
{
	if (drawSprite && player.hp > 0)
		arduboy.drawBitmap(80, 0, playerBattleSprite, 21, 24);
	cursorX = 105;
	cursorY = 0;
	drawChar(CHAR_L);
	cursorX += 12;
	drawNumber(player.level);
	cursorX = 81;
	cursorY = 24;
	lineStartX = 81;
	drawPlayerName();
	drawChar(CHAR_LINE);
	drawChar(CHAR_H);
	drawStat(player.hp, battleStats[6].maxHP);
	drawChar(CHAR_LINE);
	drawChar(CHAR_M);
	drawStat(player.mp, battleStats[6].maxMP);
}

void battleDrawName(int8_t id)
{
	if (id == 6)
		drawPlayerName();
	else
		drawName(battleStats[id].name, 8);
}

void loadMonster(uint8_t num, int8_t id)
{
	battleStats[num].id = id;
	if (id == 15)
		return;
	bitReaderInit(monsters + id * MONSTER_SIZE + 8, 0);
	battleStats[num].name = monsters + id * MONSTER_SIZE;
	battleStats[num].mp = bitReaderRead8(6);
	battleStats[num].hp = battleStats[num].maxHP = bitReaderRead16(10);
	for (uint8_t i = 0; i < 4; ++i)
		battleStats[num].stats[i] = bitReaderRead8(8);
	battleStats[num].immunity = bitReaderRead16(16);
	for (uint8_t i = 0; i < 8; ++i)
		battleStats[num].spells[i] = bitReaderRead8(3);
	battleStats[num].experience = bitReaderRead8(8);
	battleStats[num].gold = bitReaderRead8(8);
}

void loadBoss(int8_t id)
{
	bitReaderInit(bosses + id * BOSS_SIZE + 8, 0);
	battleStats[1].name = bosses + id * BOSS_SIZE;
	battleStats[1].id = id;
	battleStats[1].hp = battleStats[1].maxHP = bitReaderRead16(16);
	battleStats[1].mp = bitReaderRead8(8);
	for (uint8_t i = 0; i < 4; ++i)
		battleStats[1].stats[i] = bitReaderRead8(8);
	battleStats[1].immunity = bitReaderRead16(16);
	for (uint8_t i = 0; i < 8; ++i)
		battleStats[1].spells[i] = bitReaderRead8(3);
	battleStats[1].experience = bitReaderRead8(8);
	battleStats[1].gold = bitReaderRead8(8);
}

void loadMonsterGroup()
{
	if (isBossBattle(currentMonsterGroup))
	{
		battleStats[0].id = 15;
		battleStats[2].id = 15;
		battleStats[3].id = 15;
		loadBoss(getMonsterID(currentMonsterGroup, 1));
		loadMonster(4, getMonsterID(currentMonsterGroup, 2));
		loadMonster(5, getMonsterID(currentMonsterGroup, 3));
	}
	else
	{
		for (uint8_t i = 0; i < 6; ++i)
			loadMonster(i, getMonsterID(currentMonsterGroup, i));
	}
	battleStats[6].id = 16;
	//battleStats[6].level = player.level;
	battleStats[6].hp = player.hp;
	battleStats[6].maxHP = getStat(player.level, PLAYER_LEVEL1_HP, PLAYER_LEVEL99_HP);
	battleStats[6].mp = player.mp;
	battleStats[6].maxMP = getStat(player.level, PLAYER_LEVEL1_MP, PLAYER_LEVEL99_MP);
	for (uint8_t i = 0; i < 4; ++i)
		battleStats[6].stats[i] = getStat(player.level, pgm_read_byte(level1Stats + i), 255) + player.statBonus[i];
	battleStats[6].immunity = player.immunity;
	for (uint8_t i = 0; i < 8; ++i)
		battleStats[6].spells[i] = player.spells[i];
	battleStats[6].experience = 0;
	battleStats[6].gold = 0;
	battleState = BATTLE_CHOOSEACTION;
	battleExpGained = 0;
	battleGoldGained = 0;
	clearBattleActions();
}

void randomBattle(uint8_t info)
{
	int16_t groups = 0;
	int16_t groupID = 0;
	if (playerSteps >= 10 && RANDOM_CHANCE(25000))
	{
		for (uint8_t i = 0; i < NUM_MONSTER_GROUPS; ++i)
		{
			if (isMonsterGroupAvailable(monsterGroups + i * MONSTER_GROUP_SIZE, info))
				++groups;
		}
		if (groups == 0)
			return;
		groupID = random(groups);
		for (uint8_t i = 0; i < NUM_MONSTER_GROUPS; ++i)
		{
			if (isMonsterGroupAvailable(monsterGroups + i * MONSTER_GROUP_SIZE, info))
				--groupID;
			if (groupID == -1)
			{
				groupID = i;
				break;
			}
		}
		for (uint8_t i = 0; i <= 32; ++i)
		{
			drawMap();
			for (int16_t j = 0; j < 1024; ++j)
				arduboy.sBuffer[j] = ~arduboy.sBuffer[j];
			teleportErase(i);
			arduboy.display(CLEAR_BUFFER);
			if (i == 0)
				delay(500);
		}
		currentMonsterGroup = monsterGroups + groupID * MONSTER_GROUP_SIZE;
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
	}
}

void setFlashingSprite(int8_t id)
{
	if (battleFlashSprite & (128 >> id))
		return;
	battleRenderSprite = 0xFF;
	battleFlashSprite = 128 >> id;
}

void battleChooseAction()
{
	uint8_t justPressed = arduboy.justPressedButtons();

	uint8_t startX, stopX;
	uint8_t y = (selection & 1) + 6;
	if (selection <= 1)
	{
		startX = 6;
		stopX = 42;
	}
	else if (selection <= 3)
	{
		startX = 48;
		stopX = 78;
	}
	else
	{
		startX = 84;
		stopX = 120;
	}
	for (uint8_t x = startX; x <= stopX; ++x)
		arduboy.sBuffer[y * 128 + x] = 0xFF;
	cursorX = 7;
	cursorY = 48;
	lineStartX = 7;
	drawMessageCompressed(messageBattleChooseAction);
	if (justPressed & B_BUTTON)
	{
		clearBattleActions();
		battleActions[6].action = selection;
		battleActions[6].target = 6;
		setFlashingSprite(8);
		if (selection == ACTION_ATTACK)
			battleState = BATTLE_SELECT_MONSTER;
		else if (selection == ACTION_USEITEM)
		{
			battleState = BATTLE_SELECT_ITEM;
			previousState = STATE_BATTLE;
			gameState = STATE_MENU_ITEMS;
		}
		else if (selection == ACTION_CASTSPELL)
			battleState = BATTLE_SELECT_SPELL;
		else if (selection == ACTION_ESCAPE && isBossBattle(currentMonsterGroup))
		{
			drawMessageBox();
			cursorX = 0;
			cursorY = 48;
			lineStartX = 0;
			drawMessageCompressed(messageBattleNoEscape);
			arduboy.display();
			waitButtons(A_BUTTON | B_BUTTON);
			battleActions[6].action = ACTION_CHOOSING;
			battleState = BATTLE_CHOOSEACTION;
		}
		else
			battleState = BATTLE_MONSTER_ACTIONS;
		selection = 0;
	}
	else if ((justPressed & UP_BUTTON) | (justPressed & DOWN_BUTTON))
		selection ^= 1;
	else if (justPressed & LEFT_BUTTON)
		selection += 4;
	else if (justPressed & RIGHT_BUTTON)
		selection += 2;
	selection %= 6;
}

void battleSelectMonster()
{
	uint8_t justPressed = arduboy.justPressedButtons();
	int8_t count = 0;
	cursorX = 1;
	cursorY = 48;
	lineStartX = 1;
	drawMessageCompressed(messageBattleSelectMonster);
	for (uint8_t y = 0; y < 6; ++y)
	{
		for (uint8_t x = 79; x < 128; ++x)
			arduboy.sBuffer[y * 128 + x] = ((y == selection) ? 0xFF:0x00);
	}
	cursorX = 80;
	cursorY = 0;
	lineStartX = 80;
	for (uint8_t i = 0; i < 6; ++i)
	{
		if (battleStats[i].id != 15 && battleStats[i].hp > 0)
		{
			battleDrawName(i);
			drawChar(CHAR_LINE);
			if (selection == count)
				setFlashingSprite(i);
			++count;
		}
	}
	if (justPressed & A_BUTTON)
	{
		selection = 0;
		battleState = BATTLE_CHOOSEACTION;
		setFlashingSprite(8);
	}
	else if (justPressed & B_BUTTON)
	{
		count = 0;
		for (uint8_t i = 0; i < 6; ++i)
		{
			if (battleStats[i].id != 15 && battleStats[i].hp > 0)
			{
				if (selection == count)
					battleActions[6].target = i;
				++count;
			}
		}
		setFlashingSprite(8);
		battleState = BATTLE_MONSTER_ACTIONS;
	}
	else if (justPressed & UP_BUTTON)
		selection += count - 1;
	else if (justPressed & DOWN_BUTTON)
		++selection;
	selection %= count;
}

void battleSelectItem()
{
	if (selection == -1)
	{
		selection = 0;
		battleState = BATTLE_CHOOSEACTION;
	}
	else
	{
		addRemoveItems(selection, -1);
		battleActions[6].id = selection;
		battleActions[6].target = 6;
		selection = 0;
		setFlashingSprite(8);
		battleState = BATTLE_MONSTER_ACTIONS;
	}
}

void battleSelectSpell()
{
	uint8_t justPressed = arduboy.justPressedButtons();
	fillSpellRect(0xFF);
	drawSpells();
	if (justPressed & A_BUTTON)
	{
		selection = 0;
		battleState = BATTLE_CHOOSEACTION;
	}
	else if ((justPressed & B_BUTTON) && player.spells[selection] > 0)
	{
		if (player.mp < player.spells[selection])
		{
			drawMessageBox();
			cursorX = 19;
			cursorY = 52;
			drawMessageCompressed(messageBattleNotEnoughMana);
			arduboy.display();
			waitButtons(A_BUTTON | B_BUTTON);
		}
		else
		{
			battleActions[6].id = selection;
			battleActions[6].target = 6;
			setFlashingSprite(8);
			if (selection >= 2)
				battleState = BATTLE_SELECT_MONSTER;
			else
				battleState = BATTLE_MONSTER_ACTIONS;
			selection = 0;
		}
	}
	else if ((justPressed & UP_BUTTON) | (justPressed & DOWN_BUTTON))
		selection ^= 1;
	else if (justPressed & LEFT_BUTTON)
		selection += 6;
	else if (justPressed & RIGHT_BUTTON)
		selection += 2;
	selection &= 7;
}

void battleMonsterActions()
{
	battleDetermineTurnOrder();
	for (int8_t i = 0; i < 6; ++i)
	{
		if (battleActions[i].turn != -1)
		{
			//If monster knows cure
				//50% chance to cast if self is below 50% HP
				//25% chance to cast if other monster is below 50% HP
			//else if monster knows life and action == choose
				//life is 10% chance to cast if other monster is dead
			//else
				//10% chance to defend
			//else if monster knows other spells
				//100% chance to cast a random spell
			//attack
			//target player is always 50:50 chance for each character with > 0 health
			if (battleStats[i].mp >= battleStats[i].spells[0] && battleStats[i].spells[0] != 0)
			{
				if (battleStats[i].hp <= (battleStats[i].maxHP >> 1) && RANDOM_CHANCE(50000))
				{
					battleActions[i].action = ACTION_CASTSPELL;
					battleActions[i].target = i;
					battleActions[i].id = 0;
					continue;
				}
				for (int8_t j = 0; j < 6; ++j)
				{
					if (j != i && battleStats[j].hp > 0 && battleStats[j].hp <= (battleStats[j].maxHP >> 1) && RANDOM_CHANCE(25000))
					{
						battleActions[i].action = ACTION_CASTSPELL;
						battleActions[i].target = j;
						battleActions[i].id = 0;
						break;
					}
				}
			}
			if (battleActions[i].action == ACTION_CHOOSING)
			{
				if (RANDOM_CHANCE(10000))
				{
					battleActions[i].action = ACTION_DEFEND;
					battleActions[i].target = i;
				}
				else if (RANDOM_CHANCE(1000))
				{
					battleActions[i].action = ACTION_FOCUS;
					battleActions[i].target = i;
				}
			}
			if (battleActions[i].action == ACTION_CHOOSING)
			{
				battleActions[i].target = 6;
				if (monsterHasAttackSpells(i))
				{
					battleActions[i].action = ACTION_CASTSPELL;
					do
					{
						selection = random(6) + 2;
						if (battleStats[i].spells[selection] > 0)
							battleActions[i].id = selection;
					} while (battleActions[i].id == -1);
					if (battleStats[i].mp < battleStats[i].spells[selection])
						battleActions[i].action = ACTION_CHOOSING;
				}
			}
			if (battleActions[i].action == ACTION_CHOOSING)
				battleActions[i].action = ACTION_ATTACK;
		}
	}
	battleState = BATTLE_RESOLVE_COMBAT;
	globalCounter = 0;
	selection = 0;
}

int16_t battleAttackDamage(BattleStats *source, BattleStats *target, bool defending)
{
	int16_t damage = max((source->stats[0] - (target->stats[1] >> 1)) >> 1, 1);
	if (RANDOM_CHANCE(3000))
		damage *= 2;
	if (defending)
		damage /= 2;
	return damage;
}

int16_t battleSpellDamage(BattleStats *source, BattleStats *target, uint8_t spellID, uint8_t spellLevel)
{
	//(ATK / 2) + (MAG * SPELL / 3) - (DEF / 4) - (MAG / 3)
	int16_t damage = max((source->stats[0] / 2) + (source->stats[3] * spellLevel / 3) - (target->stats[1] / 4) - (target->stats[3] / 3), 1);
	//int16_t damage = max((source->stats[0] + source->stats[3] * spellLevel * source->level - ((target->stats[1] + target->stats[3] * target->level) >> 1)) >> 1, 1);
	int8_t immunity = (target->immunity >> (spellID * 2)) & 3;
	return damage * immunity / 2;
}

void battleInflictDamage(int16_t &hp, int16_t maxHP, int16_t damage)
{
	hp -= damage;
	if (hp < 0)
		hp = 0;
	else if (hp > maxHP)
		hp = maxHP;
}

void battleDrawDamage(int16_t damage)
{
	if (damage < 0)
		damage *= -1;
	if (damage >= 100)
		drawChar(((damage / 100) % 10) + CHAR_0);
	if (damage >= 10)
		drawChar(((damage / 10) % 10) + CHAR_0);
	drawChar((damage % 10) + CHAR_0);
}

void battleResolveCombat()
{
	BattleStats *target;
	uint8_t spellLevel;
	int16_t damage = 0;
	uint8_t count = 0;
	bool targetDefending;
	cursorX = 0;
	cursorY = 48;
	lineStartX = 0;
	if (selection == 0)
	{
		setFlashingSprite(8);
		battleState = BATTLE_CHOOSEACTION;
		for (int8_t i = 0; i < 7; ++i)
		{
			target = &battleStats[battleActions[i].target];
			if (battleActions[i].turn == 0 && battleStats[i].hp == 0)
			{
				for (int8_t j = i; j < 7; ++j)
					--battleActions[j].turn;
				battleState = BATTLE_RESOLVE_COMBAT;
				return;
			}
			else if (battleActions[i].turn == 0 && battleStats[i].hp > 0 && target->hp > 0)
			{
				setFlashingSprite(battleActions[i].target);
				targetDefending = (battleActions[battleActions[i].target].action == ACTION_DEFEND && battleActions[battleActions[i].target].turn < 0);
				battleState = BATTLE_RESOLVE_COMBAT;
				flags |= FLAG_USE_MESSAGE_BUFFER | FLAG_SHOW_MESSAGE_BUFFER;
				battleDrawName(i);
				drawChar(CHAR_SPACE);
				drawMessageCompressed((uint8_t*)pgm_read_dword(battleActionText + battleActions[i].action));
				if (battleActions[i].action == ACTION_ATTACK)
				{
					battleDrawName(battleActions[i].target);
					damage = battleAttackDamage(&battleStats[i], target, targetDefending);
					battleInflictDamage(target->hp, target->maxHP, damage);
					if (target->hp == 0)
					{
						battleExpGained += target->experience;
						battleGoldGained += target->gold;
					}
					drawMessageCompressed(wordFor);
					drawChar(CHAR_SPACE);
					battleDrawDamage(damage);
					drawMessageCompressed(wordDmg);
				}
				else if (battleActions[i].action == ACTION_USEITEM)
				{
					drawName(wordItems + battleActions[i].id * 8, 8);
					playerUseItem(battleActions[i].id);
					target->hp = player.hp;
					target->mp = player.mp;
				}
				else if (battleActions[i].action == ACTION_CASTSPELL)
				{
					spellLevel = battleStats[i].spells[battleActions[i].id];
					battleStats[i].mp -= spellLevel;
					drawName(wordSpells + (battleActions[i].id * 4), 4);
					drawChar(spellLevel + CHAR_0);
					if (battleActions[i].id != 1)
						drawChar(CHAR_LINE);
					if (battleActions[i].id >= 2)
					{
						battleDrawName(battleActions[i].target);
						drawChar(CHAR_SPACE);
					}
					if (battleActions[i].id == 0) //CURE Spell
					{
						damage = 16 * spellLevel + (battleStats[i].stats[3] / 3) * spellLevel;
						drawChar(CHAR_PLUS);
						battleDrawDamage(damage);
						drawChar(CHAR_H);
						battleInflictDamage(target->hp, target->maxHP, -damage);
					}
					else if (battleActions[i].id == 1) //NUKE SPELL
					{
						for (int8_t i = 0; i < 6; ++i)
						{
							if (battleStats[i].id != 15 && battleStats[i].hp > 0)
								++count;
						}
						damage = max(((battleStats[6].stats[1] / 2) + (battleStats[6].stats[3] / 3) * spellLevel) / count, 1);
						for (int8_t i = 0; i < 6; ++i)
						{
							if (battleStats[i].id != 15 && battleStats[i].hp > 0)
							{
								battleInflictDamage(battleStats[i].hp, battleStats[i].maxHP, damage);
								drawChar(CHAR_LINE);
								battleDrawName(i);
								drawChar(CHAR_SPACE);
								battleDrawDamage(damage);
								drawMessageCompressed(wordDmg);
								if (battleStats[i].hp == 0)
								{
									battleExpGained += battleStats[i].experience;
									battleGoldGained += battleStats[i].gold;
								}
							}
						}
						battleRenderSprite = 0xFF;
						battleFlashSprite = 0xFC;
					}
					else
					{
						damage = battleSpellDamage(&battleStats[i], target, battleActions[i].id, spellLevel);
						battleInflictDamage(target->hp, target->maxHP, damage);
						if (target->hp == 0)
						{
							battleExpGained += target->experience;
							battleGoldGained += target->gold;
						}
						battleDrawDamage(damage);
						drawMessageCompressed(wordDmg);
					}
				}
				else if (battleActions[i].action == ACTION_FOCUS)
				{
					for (int8_t i = 0; i < 4; ++i)
						target->stats[i] += target->stats[i] >> 1;
				}
				else if (battleActions[i].action == ACTION_ESCAPE)
					flags |= FLAG_BATTLE_ESCAPE;
			}
			--battleActions[i].turn;
		}
		player.hp = battleStats[6].hp;
		player.mp = battleStats[6].mp;
		fillMessageBuffer();
		if (battleState == BATTLE_RESOLVE_COMBAT)
			++selection;
		else
			selection = battleActions[6].action;
	}
	else if (!(flags & FLAG_SHOW_MESSAGE_BUFFER))
	{
		selection = 0;
		globalCounter = 0;
		if (player.hp == 0)
		{
			do
			{
				teleportErase(count);
				arduboy.display();
				++count;
			} while (count <= 32);
			cursorX = 52;
			cursorY = 24;
			lineStartX = 52;
			drawMessageCompressed(messageGameOver);
			arduboy.display();
			waitButtons(A_BUTTON | B_BUTTON);
			arduboy.exitToBootloader();
		}
		else if (flags & FLAG_BATTLE_ESCAPE)
		{
			flags &= ~FLAG_BATTLE_ESCAPE;
			for (count = 1; count <= 32; ++count)
			{
				teleportErase(count);
				arduboy.display();
			}
			for (count = 32; count > 0; --count)
			{
				drawMap();
				teleportErase(count);
				arduboy.display(CLEAR_BUFFER);
			}
			drawMap();
#ifdef ENABLE_MUSIC
			song = getMapSong(pMap);
			songT = 0;
#endif
			gameState = STATE_PLAYING;
		}
		else
		{
			targetDefending = true;
			for (uint8_t i = 0; i < 6 && targetDefending; ++i)
			{
				if (battleStats[i].id != 15 && battleStats[i].hp > 0)
					targetDefending = false;
			}
			if (targetDefending)
			{
				setFlashingSprite(8);
				battleState = BATTLE_VICTORY;
			}
		}
	}
}

void battleVictory()
{
	int16_t nextExp;
	cursorX = 0;
	cursorY = 48;
	lineStartX = 0;
	if (selection == 0)
	{
		flags |= FLAG_USE_MESSAGE_BUFFER | FLAG_SHOW_MESSAGE_BUFFER;
		drawChar(CHAR_PLUS);
		if (battleExpGained >= 100)
			drawChar(((battleExpGained / 100) % 10) + CHAR_0);
		if (battleExpGained >= 10)
			drawChar(((battleExpGained / 10) % 10) + CHAR_0);
		drawChar((battleExpGained % 10) + CHAR_0);
		drawMessageCompressed(wordExp);
		nextExp = getStat(player.level, PLAYER_NEXTEXP);
		if (battleExpGained + player.exp >= nextExp)
			drawMessageCompressed(wordLevelUp);
		drawChar(CHAR_LINE);
		drawChar(CHAR_PLUS);
		if (battleGoldGained >= 100)
			drawChar(((battleGoldGained / 100) % 10) + CHAR_0);
		if (battleGoldGained >= 10)
			drawChar(((battleGoldGained / 10) % 10) + CHAR_0);
		drawChar((battleGoldGained % 10) + CHAR_0);
		drawChar(CHAR_G);
		fillMessageBuffer();
		player.exp += battleExpGained;
		player.gold += battleGoldGained;
		if (player.gold > 9999)
			player.gold = 9999;
		selection = player.level;
		while (player.exp >= nextExp)
		{
			player.exp -= nextExp;
			++player.level;
			nextExp = getStat(player.level, PLAYER_NEXTEXP);
		}
		if (player.level != selection)
		{
			player.hp = battleStats[6].maxHP = getStat(player.level, PLAYER_HP);
			player.mp = battleStats[6].maxMP = getStat(player.level, PLAYER_MP);
		}
		selection = 1;
	}
	if (!(flags & FLAG_SHOW_MESSAGE_BUFFER))
	{
		if (isBossBattle(currentMonsterGroup) && battleStats[1].id == 4)
		{
#ifdef ENABLE_MUSIC
			song = SONG_TITLE;
			songT = 0;
#endif
			gameState = STATE_STORY;
		}
		else
		{
#ifdef ENABLE_MUSIC
			song = getMapSong(pMap);
			songT = 0;
#endif
			gameState = STATE_PLAYING;
		}
		battleFlashSprite = 0x00;
		selection = 5;
		globalCounter = 0;
		previousState = STATE_PLAYING;
	}
}

#endif //BATTLE_H
