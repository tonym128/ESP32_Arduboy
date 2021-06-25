#include "world.h"

Bullet bullets[3];
BadGuy badguys[20];
Explorer p1;
Wall walls[16];
Key keys[4];
Treasure treasures[9];
Spawner spawners[4];
Exit exits[4];

int16_t spawnx;
int16_t spawny;
int8_t autoFireTime;
int8_t whiteScreenTime;
uint16_t score;
int16_t rollingScore;
uint16_t rollingHealth;
uint8_t currentLevel;
uint8_t levelsCompleted;
uint16_t gameTime;
int16_t gameTimeTick;
bool quitGame;
int16_t scrollx;
int16_t scrolly;
int8_t BadGuy_UpdateDelay;

void worldLoadLevel()
{
	uint8_t level = currentLevel;
	int8_t i;
	uint8_t x, y, w, h, dest, target;
	uint16_t dataItem;
	int8_t style;
	bool direction;
    uint16_t* levelOffset = (uint16_t*) levelData + (level*32);
	
	// Clear the level
	worldClearLevel();
	
	// Read int16_ts from the memory
	for(int8_t i=0; i<32; ++i) {
		dataItem = pgm_read_word(levelOffset + i);
		
		if(i < 16) { // Read a wall
			x = (dataItem >> 11) & 31;
			y = (dataItem >> 7) & 15;
			dest = (dataItem >> 2) & 31;
			style = (dataItem >> 1) & 1;
			direction = (dataItem) & 1;
			w = direction ? 1 : dest+1;
			h = direction ? dest+1 : 1;
			add_wall(x, y, w, h, style);
		} else if(i >= 16 && i < 20) { // Read a spawner
			x = (dataItem >> 11) & 31;
			y = (dataItem >> 7) & 15;
			add_spawner(x, y);
		} else if(i >= 20 && i < 24) { // Read a key
			x = (dataItem >> 11) & 31;
			y = (dataItem >> 7) & 15;
			target = (dataItem >> 3) & 15;
			add_key(x, y, target);
		} else if(i >= 24 && i < 25) { // Read a start
			spawnx = (dataItem >> 11) & 31;
			spawny = (dataItem >> 7) & 15;
		} else if(i >= 25 && i < 29) { // Read an exit
			x = (dataItem >> 11) & 31;
			y = (dataItem >> 7) & 15;
			dest = (dataItem) & 127;
			add_exit(x, y, dest);
		} else if(i >= 29 && i < 32) { // Read a treasure
			x = (dataItem >> 11) & 31;
			y = (dataItem >> 7) & 15;
			target = (dataItem >> 4) & 7; // type
			dest = (dataItem) & 15; // layout
			
			if(target == 0) continue; // If the type is zero, don't do squat with it
			
			switch(dest) { case 0: case 1: case 2: case 3: case 5: case 6: case 9: case 10: case 11: case 12: case 14:
					add_treasure(x, y, target);
			break; }
			switch(dest) { case 2: case 4: case 7: case 9: case 13: case 15:
					add_treasure(x+1, y, target);
			break; }
			switch(dest) { case 8: case 14:
					add_treasure(x+2, y, target);
			break; }
			
			switch(dest) { case 1: case 4: case 8: case 11: case 15:
					add_treasure(x, y+1, target);
			break; }
			switch(dest) { case 3: case 10:
					add_treasure(x+1, y+1, target);
			break; }
			switch(dest) { case 5: case 9:
					add_treasure(x+2, y+1, target);
			break; }
			
			switch(dest) { case 7: case 12: case 13:
					add_treasure(x, y+2, target);
			break; }
			switch(dest) { case 6: case 11: case 14: case 15:
					add_treasure(x+1, y+2, target);
			break; }
			switch(dest) { case 10: case 12: case 13:
					add_treasure(x+2, y+2, target);
			break; }
		}
	}
	p1.x = spawnx*8;
	p1.y = spawny*8;
}

void worldClearLevel()
{
	int8_t i;
	for(i=0; i<numBadguys; ++i) {
		badguys[i].active = false;
	}
	for(i=0; i<numSpawners; ++i) {
		spawners[i].active = false;
	}
	for(i=0; i<numKeys; ++i) {
		keys[i].active = false;
	}
	for(i=0; i<numExits; ++i) {
		exits[i].active = false;
	}
	for(i=0; i<numWalls; ++i) {
		walls[i].active = false;
	}
	for(i=0; i<numBullets; ++i) {
		bullets[i].active = false;
	}
	for(i=0; i<numTreasures; ++i) {
		treasures[i].active = false;
	}
}
