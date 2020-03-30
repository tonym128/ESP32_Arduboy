#ifndef _TREASURE_H
#define _TREASURE_H

struct Treasure {
	int8_t x;
	int8_t y;
	int8_t type;
	bool active;
};
void add_treasure(int8_t x, int8_t y, int8_t type);
void draw_treasure(Treasure& obj);
void activate_treasure(Treasure& obj);

#endif
