#ifndef GAME_H
#define GAME_H

// #define DEBUG 1

#include <Arduboy2.h>
#include <ESPboyPlaytune.h>

#define WALL_FREQ   220
#define PADDLE_FREQ 440
#define WALL_DUR    80
#define PADDLE_DUR    80

extern Arduboy2 arduboy; 
extern ESPboyPlaytune sound;

extern Sprites sprites;

void tick();

unsigned int rng();
void drawOverwriteMaskDynamic(int x0, int y0, u8 *diamondsR, int flag);
void menuCredits();
void playSound(int i);
int GetAction(int x, int y);
int TestMove(int x, int y);
void handleBall(void);
void drawTile(u8 x0, u8 y0, u8 z);
void drawTilePixel(u8 x0, u8 y0, u8 z);

#endif // GAME_HPP
