#ifndef _SPAWNER_H
#define _SPAWNER_H

struct Spawner {
	int8_t x;
	int8_t y;
	bool active;
	int8_t spawnDelay;
	int8_t health;
};

void add_spawner(int8_t x, int8_t y);
void update_spawner(Spawner& obj);
void draw_spawner(Spawner &obj);
void destroy_spawner(Spawner& obj);
void damage_spawner(Spawner& obj, int16_t dmg);

#endif
