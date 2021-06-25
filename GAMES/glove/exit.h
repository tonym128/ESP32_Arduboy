#ifndef _EXIT_H
#define _EXIT_H

struct Exit{
	int8_t x;
	int8_t y;
	int8_t dest;
	bool active;
};

void add_exit(int8_t x, int8_t y, int8_t dest);
void draw_exit(Exit &obj);
void activate_exit(Exit& obj);

#endif
