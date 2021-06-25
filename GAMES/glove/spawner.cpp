#include "world.h"

void add_spawner(int8_t x, int8_t y)
{
	for(int16_t i=0; i<numSpawners; i++) {
		if(!spawners[i].active) {
			spawners[i].active = true;
			spawners[i].x = x;
			spawners[i].y = y;
			spawners[i].spawnDelay = 100;
			spawners[i].health = 50;
			return;
		}
	}
}
void update_spawner(Spawner& obj)
{
	if(!obj.active) return;
	if(obj.spawnDelay <= 0) {
		add_badguy(obj.x*8 + 4, obj.y*8 + 4);
		obj.spawnDelay = 100;
	}
	obj.spawnDelay--;
}
void draw_spawner(Spawner& obj)
{
    const uint8_t* drawSpr = (uint8_t*) sprites+SPRITE_SPAWNER;
    
	if(obj.active)
		arduboy.drawBitmap(obj.x*8+scrollx, obj.y*8+scrolly, drawSpr, 8, 8, 1);
}
void destroy_spawner(Spawner& obj)
{
	obj.active = false;
}
void damage_spawner(Spawner& obj, int16_t dmg)
{
	obj.health -= dmg;
	if(obj.health <= 0)
	{
		obj.active = false;
		addScore(1);
		arduboy.tunes.tone(TONE_SPAWNER_DESTROY, 20);
	} else {
		arduboy.tunes.tone(TONE_SPAWNER_DAMAGE, 20);
	}
}
