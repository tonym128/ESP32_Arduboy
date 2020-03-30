#ifndef _PHYSICS_H
#define _PHYSICS_H


bool intersectSpriteSprite(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
bool collideSpriteSprite(int16_t &x1, int16_t &y1, bool horiz, int8_t &vel, int16_t x2, int16_t y2);
bool collideSpriteWall(int16_t &x1, int16_t &y1, bool horiz, int8_t &vel, int16_t x2, int16_t y2, int16_t w2, int16_t h2);

#endif
