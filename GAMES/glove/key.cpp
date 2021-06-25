#include "world.h"


void add_key(int8_t x, int8_t y, uint8_t target)
{
	for(int16_t i=0; i<numKeys; i++) {
		if(!keys[i].active) {
			keys[i].active = true;
			keys[i].x = x;
			keys[i].y = y;
			keys[i].target = target;
			return;
		}
	}
}
void draw_key(Key & obj)
{
    const uint8_t* drawSpr = (uint8_t*) sprites+SPRITE_KEY;
    
	if(obj.active)
		arduboy.drawBitmap(obj.x*8+scrollx, obj.y*8+scrolly, drawSpr, 8, 8, 1);
}
void activate_key(Key& obj)
{
	if(obj.active)
	{
		walls[obj.target].active = false;
		obj.active = false;
		arduboy.tunes.tone(TONE_KEY_ACTIVATE, 20);
	}
}
