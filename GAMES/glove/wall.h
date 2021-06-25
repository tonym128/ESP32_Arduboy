#ifndef _WALL_H
#define _WALL_H

struct Wall {
	int8_t x;
	int8_t y;
	int8_t w;
	int8_t h;
	bool active;
	int8_t style;
};

void add_wall(int8_t x, int8_t y, int8_t w, int8_t h, int8_t style);
void draw_wall(Wall &obj);

#endif
