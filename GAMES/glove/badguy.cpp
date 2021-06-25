#include "world.h"
#include "physics.h"

void add_badguy(int16_t x, int16_t y)
{
	bool found = false;
	int16_t foundIndex;
	for(foundIndex=0; foundIndex<numBadguys; ++foundIndex) {
		if(!badguys[foundIndex].active) {
			badguys[foundIndex].active = true;
			badguys[foundIndex].x = x;
			badguys[foundIndex].y = y;
			found = true;
			break;
		}
	}
	
	if(!found) return;
	
	bool valid = true;
	for(int16_t j = 0; j<numBadguys; ++j) {
		if(!badguys[j].active || (foundIndex == j)) continue;
		int16_t wx = badguys[j].x;
		int16_t wy = badguys[j].y;
		if((badguys[foundIndex].x < wx + 8) &&
			(badguys[foundIndex].x + 8 > wx) &&
			(badguys[foundIndex].y < wy + 8) &&
			(badguys[foundIndex].y + 8 > wy))
		{
			valid = false;
			break;
		}
	}
	
	if(!valid) badguys[foundIndex].active = false;
}
void update_badguy(BadGuy &obj)
{
	int8_t i;
	int8_t vx, vy;
	int16_t wx, wy, ww, wh;
	
	// Skip processing if this object is dead
	if(!obj.active) return;
	
	// Get horizontal velocity
	if(obj.x > p1.x) vx = -1;
	else if(obj.x < p1.x) vx = 1;
	else vx = 0;
	
	// Set horizontal position
	obj.x += vx;
	obj.x = (obj.x + 8 > gamew) ? gamew - 8 : obj.x;
	obj.x = (obj.x < 0) ? 0 : obj.x;
	
	// WALL COLLISION
	for(i = 0; i<numWalls; ++i) {
		if(!walls[i].active) continue;
		wx = walls[i].x*8;
		wy = walls[i].y*8;
		ww = walls[i].w*8;
		wh = walls[i].h*8;
		collideSpriteWall(obj.x, obj.y, true, vx, wx, wy, ww, wh);
	}
	
	// BADGUY COLLISION
	for(i = 0; i<numBadguys; ++i) {
		if(!badguys[i].active || (&obj == &badguys[i])) continue;
		wx = badguys[i].x;
		wy = badguys[i].y;
		collideSpriteSprite(obj.x, obj.y, true, vx, wx, wy);
	}
	
	// PLAYER COLLISION
	if(collideSpriteSprite(obj.x, obj.y, true, vx, p1.x, p1.y))
	{
		damage_explorer(p1, 1);
	}
	
	// Get vertical velocity
	if(obj.y > p1.y) vy = -1;
	else if(obj.y < p1.y) vy = 1;
	else vy = 0;
	
	// Set verical position
	obj.y += vy;
	obj.y = (obj.y + 8 > gameh) ? gameh - 8 : obj.y;
	obj.y = (obj.y < 0) ? 0 : obj.y;
	
	// WALL COLLISION
	for(i = 0; i<numWalls; ++i) {
		if(!walls[i].active) continue;
		wx = walls[i].x*8;
		wy = walls[i].y*8;
		ww = walls[i].w*8;
		wh = walls[i].h*8;
		collideSpriteWall(obj.x, obj.y, false, vy, wx, wy, ww, wh);
	}
	
	// BADGUY COLLISION
	for(i = 0; i<numBadguys; ++i) {
		if(!badguys[i].active || (&obj == &badguys[i])) continue;
		wx = badguys[i].x;
		wy = badguys[i].y;
		collideSpriteSprite(obj.x, obj.y, false, vy, wx, wy);
	}
	
	// PLAYER COLLISION
	if(collideSpriteSprite(obj.x, obj.y, false, vy, p1.x, p1.y))
	{
		damage_explorer(p1, 1);
	}
}

void draw_badguy(BadGuy &b)
{
    const uint8_t* drawSpr = (uint8_t*) sprites+SPRITE_BADGUY;
	if(b.active)
		arduboy.drawBitmap(b.x+scrollx, b.y+scrolly, drawSpr, 8, 8, 1);
}

void destroy_badguy(BadGuy &obj)
{
	if(obj.active) {
		arduboy.tunes.tone(TONE_BADGUY_DESTROY, 20);
		obj.active = false;
	}		
}
