#ifndef PICKUP_H
#define PICKUP_H

#include "globals.h"

// constants /////////////////////////////////////////////////////////////////

#define PICKUP_WIDTH 16
#define PICKUP_HEIGHT 16

#define PICKUP_TYPE_INACTIVE 0
#define PICKUP_TYPE_COIN 1
#define PICKUP_TYPE_HEART 2

#define PICKUP_MAX 3


// structures ////////////////////////////////////////////////////////////////

struct Pickup
{
  int x;
  int y;
  int16_t type;
  int16_t frame;
  int16_t counter;
  boolean isVisible;
};



// globals ///////////////////////////////////////////////////////////////////

extern Pickup pickups[PICKUP_MAX];

// method prototypes /////////////////////////////////////////////////////////

bool addPickup(int x, int y);
void drawPickups();
void clearPickups();
void pickupCollision(int x, int y);

#endif
