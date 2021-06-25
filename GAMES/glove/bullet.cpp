#include "world.h"

void add_bullet(int16_t x, int16_t y, int16_t vx, int16_t vy)
{
	for(int16_t i=0; i<numBullets; ++i) {
		if(!bullets[i].active) {
			arduboy.tunes.tone(TONE_EXPLORER_FIRE, 20);
			bullets[i].active = true;
			bullets[i].x = x;
			bullets[i].y = y;
			bullets[i].vx = vx;
			bullets[i].vy = vy;
			bullets[i].lifetime = 20;
			return;
		}
	}
}

void update_bullet(Bullet &obj)
{
	int16_t wx, wy, ww, wh;
	int16_t i;

	// Skip processing if bullet is dead
	if(!obj.active) return;
	
	// Update horizontal position
	obj.x += obj.vx;
	
	// Touch game border
	if((obj.x + 4 < 0) || (obj.x > gamew)) {
		obj.active = false;
		return;
	}
	
	// Update vertical position
	obj.y += obj.vy;
	
	// Touch game border
	if((obj.y + 4 < 0) || (obj.y > gameh)) {
		obj.active = false;
		return;
	}
	
	// Reduce active time
	obj.lifetime--;
	obj.active = obj.lifetime > 0;
	
	// BADGUY COLLISION
	for(i=0; i<numBadguys; ++i) {
		if(badguys[i].active && (
				(obj.x < badguys[i].x + 8) &&
				(obj.x + 1 > badguys[i].x) &&
				(obj.y < badguys[i].y + 8) &&
				(obj.y + 1 > badguys[i].y)))
		{
			obj.active = false;
			destroy_badguy(badguys[i]);
			return;
		}
	}	
	
	
	// WALL COLLISION
	for(i = 0; i<numWalls; ++i) {
		if(!walls[i].active) continue;
		wx = walls[i].x*8;
		wy = walls[i].y*8;
		ww = walls[i].w*8;
		wh = walls[i].h*8;
		if((obj.x < wx + ww) &&
				(obj.x + 1 > wx) &&
				(obj.y < wy + wh) &&
				(obj.y + 1 > wy))
		{
			obj.active = false;
		}
	}
	
	// SPAWNER COLLISION
	for(i = 0; i<numSpawners; ++i) {
		if(!spawners[i].active) continue;
		wx = spawners[i].x*8;
		wy = spawners[i].y*8;
		ww = 8;
		wh = 8;
		if((obj.x < wx + ww) &&
				(obj.x + 1 > wx) &&
				(obj.y < wy + wh) &&
				(obj.y + 1 > wy))
		{
			damage_spawner(spawners[i], 10);
			obj.active = false;
		}
	}
}

void draw_bullet(Bullet& obj)
{
	if(obj.active) {
		arduboy.drawPixel(obj.x+scrollx,   obj.y+scrolly,   1);
		arduboy.drawPixel(obj.x+1+scrollx, obj.y+scrolly,   1);
		arduboy.drawPixel(obj.x+scrollx,   obj.y+1+scrolly, 1);
		arduboy.drawPixel(obj.x+1+scrollx, obj.y+1+scrolly, 1);
	}	
}
