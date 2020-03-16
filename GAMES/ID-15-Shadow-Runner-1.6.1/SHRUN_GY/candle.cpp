#include <avr/pgmspace.h>
#include <Gamby.h>
#include "candle.h"
#include "candle_bitmap.h"

extern GambyGraphicsMode gamby;


void candle_draw(char x, char y)
{
  gamby.drawSprite(x,y+34,candle_bitmap);
  gamby.drawSprite(x+3,y+18,flame_bitmap,random(0,24));
}
