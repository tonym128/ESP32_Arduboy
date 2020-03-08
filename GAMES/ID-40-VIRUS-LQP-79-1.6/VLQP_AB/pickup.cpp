#include "pickup.h"
#include "player.h"

// globals ///////////////////////////////////////////////////////////////////

Pickup pickups[PICKUP_MAX];

// method implementations ////////////////////////////////////////////////////

// addPickup
// tries to add a pickup to the world. returns true if succsessful
bool addPickup(int x, int y)
{
  for (int16_t id = 0; id < PICKUP_MAX; id++)
  {
    if (!pickups[id].type)
    {
      pickups[id].isVisible = true;
      pickups[id].counter = 0;
      pickups[id].x = x;
      pickups[id].y = y;
      pickups[id].type = pickupsAvailable[pickupsCounter];
      pickupsCounter++;
      if (pickupsCounter > 9) pickupsCounter = 0;
      return true;
    }
  }
  return false;
}


// drawPickups
// draws the entire list of pickups
void drawPickups()
{
  for (int16_t id = 0; id < PICKUP_MAX; id++)
  {
    if (arduboy.everyXFrames(6))
    {
      pickups[id].counter++;
      pickups[id].frame++;
    }
    if ((arduboy.everyXFrames(2)) && (pickups[id].counter > 25)) pickups[id].isVisible = !pickups[id].isVisible;
    if (pickups[id].counter > 30) pickups[id].type = PICKUP_TYPE_INACTIVE;
    if (pickups[id].frame > 5) pickups[id].frame = 0;
    if ((pickups[id].type > PICKUP_TYPE_INACTIVE) && pickups[id].isVisible) sprites.drawPlusMask(pickups[id].x - mapPositionX, pickups[id].y - mapPositionY, collectables_plus_mask, pickups[id].frame + (6 * (pickups[id].type - 1)));
  }
}


// pickupCollision
// checks for collision against the player, and handles it
void pickupCollision(int x, int y)
{
  for (int16_t id = 0; id < PICKUP_MAX; id++)
  {
    if (
      ( pickups[id].type ) &&
      ( pickups[id].x < x + PLAYER_WIDTH ) &&
      ( pickups[id].x + PICKUP_WIDTH > x ) &&
      ( pickups[id].y < y + PLAYER_HEIGHT ) &&
      ( pickups[id].y + PICKUP_HEIGHT > y ))
    {
      if (pickups[id].type == PICKUP_TYPE_HEART)
      {
        sound.tone(660, 20);
        playerHealthOffset(coolGirl, 2);
      }
      else
      {
        sound.tone(880, 20);
        rollingScore += 100;
      }
      pickups[id].type = PICKUP_TYPE_INACTIVE;
    }
  }
}


// clearPickups
// clears the entire list of pickups
void clearPickups()
{
  for (int16_t id = 0; id < PICKUP_MAX; id++)
  {
    pickups[id].type = PICKUP_TYPE_INACTIVE;
  }
}
