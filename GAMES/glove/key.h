#ifndef _KEY_H
#define _KEY_H

struct Key {
	int8_t x;
	int8_t y;
	uint8_t target;
	bool active;
};

void add_key(int8_t x, int8_t y, uint8_t target);
void draw_key(Key & obj);
void activate_key(Key& obj);

#endif
