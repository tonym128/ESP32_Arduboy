#ifndef _BULLET_H
#define _BULLET_H
	
struct Bullet {
	int16_t x;
	int16_t y;
	int8_t vx;
	int8_t vy;
	int8_t lifetime;
	bool active;
};

void add_bullet(int16_t x, int16_t y, int16_t vx, int16_t vy);
void update_bullet(Bullet &obj);
void draw_bullet(Bullet& obj);

#endif
