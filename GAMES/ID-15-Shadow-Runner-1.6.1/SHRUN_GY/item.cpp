#include <avr/pgmspace.h>
#include <Gamby.h>
#include "item.h"
#include "item_bitmap.h"

extern GambyGraphicsMode gamby;
extern char birdframe;
extern char heartframe;
extern byte showitems;

int itemX[]={96,96,96,96,96,96};
int itemY[]={36,36,16,4,16,4};

void stone1_draw()
{
  gamby.drawSprite(itemX[0],itemY[0], stone_bitmap,stonemask_bitmap);
  if ((showitems & B00000001) == B00000001)
  {
    itemX[0] -=6;
    if (itemX[0] < -24)
    {
      showitems = showitems ^ B00000001;
      itemX[0] = 96;
    }
  }
}

void stone2_draw()
{
gamby.drawSprite(itemX[1],itemY[1], stone_bitmap,stonemask_bitmap);
if ((showitems & B00000010) == B00000010)
  {
    itemX[1] -=6;
    if (itemX[1] < -24)
    {
      showitems = showitems ^ B00000010;
      itemX[1] = 96;
    }
  }
}

void bird1_draw()
{
gamby.drawSprite(itemX[2],itemY[2], bird_bitmap,birdframe,birdmask_bitmap,birdframe);
if ((showitems & B00000100) == B00000100)
  {
    itemX[2] -=6;
    if (itemX[2] < -24)
    {
      showitems = showitems ^ B00000100;
      itemX[2] = 96;
    }
  }
}

void bird2_draw()
{
gamby.drawSprite(itemX[3],itemY[3], bird_bitmap,birdframe,birdmask_bitmap,birdframe);
if ((showitems & B00001000) == B00001000)
  {
    itemX[3] -=6;
    if (itemX[3] < -24)
    {
      showitems = showitems ^ B00001000;
      itemX[3] = 96;
    }
  }
}


void bird3_draw()
{
gamby.drawSprite(itemX[4],itemY[4], bird_bitmap,birdframe,birdmask_bitmap,birdframe);
if ((showitems & B00010000) == B00010000)
  {
    itemX[4] -=6;
    if (itemX[4] < -24)
    {
      showitems = showitems ^ B00010000;
      itemX[4] = 96;
    }
  }
}


void heart_draw()
{
gamby.drawSprite(itemX[5],itemY[5], heart_bitmap,heartframe,heartmask_bitmap,heartframe);
if ((showitems & B00100000) == B00100000)
  {
    itemX[5] -=6;
    if (itemX[5] < -24)
    {
      showitems = showitems ^ B00100000;
      itemX[5] = 96;
    }
  }
}

