#ifndef _EXPLORER_H
#define _EXPLORER_H

struct Explorer {
	int16_t x;
	int16_t y;
	int8_t direction;
	bool frame;
	int8_t frameTime;
	bool active;
	int16_t health;
	int8_t nextHealthDecrease;
};
void initialize_explorer(Explorer& obj);
void update_explorer(Explorer& obj);
void draw_explorer(Explorer& obj);
void damage_explorer(Explorer& obj, int16_t dmg);

#endif
