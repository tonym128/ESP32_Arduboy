#ifndef _BADGUY_H
#define _BADGUY_H

struct BadGuy {
	int16_t x;
	int16_t y;
	bool active;
};

void add_badguy(int16_t x, int16_t y);
void update_badguy(BadGuy &b);
void draw_badguy(BadGuy &b);
void destroy_badguy(BadGuy &obj);

#endif
