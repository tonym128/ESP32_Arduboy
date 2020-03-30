#include "world.h"


void add_treasure(int8_t x, int8_t y, int8_t type)
{
	for(int16_t i=0; i<numTreasures; i++) {
		if(!treasures[i].active) {
			treasures[i].active = true;
			treasures[i].x = x;
			treasures[i].y = y;
			treasures[i].type = type;
			return;
		}
	}
}
void draw_treasure(Treasure& obj)
{
	if(!obj.active) return;
    
    const uint8_t* drawSpr = (uint8_t*) sprites+SPRITE_TREASURE+(obj.type*8);
	arduboy.drawBitmap(obj.x*8+scrollx, obj.y*8+scrolly, drawSpr, 8, 8, 1);
}

void activate_treasure(Treasure& obj)
{
	int8_t i;
	obj.active = false;
	switch(obj.type) {
		case TREASURE_GOLD: // Awards 10 points
			addScore(10);
			arduboy.tunes.tone(TONE_GOLD_GET, 20);
			break;
		case TREASURE_POO: // Kills all baddies on screen
			for(i = 0; i < numBadguys; ++i) {
				badguys[i].active = false;
			}
			flashScreen();
			arduboy.tunes.tone(TONE_POO_GET, 20);
			break;
		case TREASURE_CUP: // Awards 6 points
			addScore(6);
			arduboy.tunes.tone(TONE_CUP_GET, 20);
			break;
		case TREASURE_LEMON: // Awards 300 health
			rollingHealth += 300;
			arduboy.tunes.tone(TONE_LEMON_GET, 20);
			break;
	}
	obj.active = false;
}
