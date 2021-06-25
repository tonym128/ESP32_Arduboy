#include "world.h"

void add_wall(int8_t x, int8_t y, int8_t w, int8_t h, int8_t style)
{
	for(int16_t i=0; i<numWalls; ++i) {
		if(!walls[i].active) {
			walls[i].active = true;
			walls[i].x = x;
			walls[i].y = y;
			walls[i].w = w;
			walls[i].h = h;
			walls[i].style = style;
			return;
		}
	}
}

void draw_wall(Wall &obj)
{
    const uint8_t* drawSpr;
    int16_t x, y;
    
	if(!obj.active) return;
    
    drawSpr = (uint8_t*) sprites+SPRITE_WALL + (8*obj.style);
    
    for(y = obj.y; y < obj.y+obj.h; ++y) {
        for(x = obj.x; x < obj.x+obj.w; ++x) {
            arduboy.drawBitmap(x*8+scrollx, y*8+scrolly, drawSpr, 8, 8, 1);
        }
    }
}
