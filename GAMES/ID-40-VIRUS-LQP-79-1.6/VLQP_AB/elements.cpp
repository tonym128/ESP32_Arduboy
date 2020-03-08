#include "elements.h"
#include "player.h"
#include "door.h"
#include "menu.h"

// globals ///////////////////////////////////////////////////////////////////

Element survivors[SURVIVOR_MAX];

// animation frame for all survivors
int16_t survivorFrame = 0;
boolean showHelp = true;
int16_t amountActiveSurvivors = 0;
int16_t survivorType[5] = {0, 1, 2, 3, 4};

// method implementations ////////////////////////////////////////////////////

// addSurvivor
// searches the survivor list for an empty slot, adds one if available
void swapSurvivorPool()
{
  for (int16_t i = 0; i < 5; i++)
  {
    int16_t k = random (5);
    int16_t temp = survivorType[i];
    survivorType[i] = survivorType[k];
    survivorType[k] = temp;
  }
}

// updateSurvivors
// updates the survivor states
void updateSurvivors()
{
  // advance the frame
  if (arduboy.everyXFrames(SURVIVOR_FRAME_SKIP)) survivorFrame++;

  // clamp to 4 frames
  if (survivorFrame >= SURVIVOR_FRAME_COUNT ) survivorFrame = 0;
}



// drawSurvivors
// draws every active survivor in the list to the display
void drawSurvivors()
{
  if (arduboy.everyXFrames(30)) showHelp = !showHelp;
  // draw the survivor!
  for (int16_t id = 0; id < SURVIVOR_MAX; id++)
  {
    Element &surv = survivors[id];
    if (!surv.active) continue;
    sprites.drawErase(surv.x - mapPositionX, surv.y - mapPositionY, survivorMask, survivorType[id]);
    sprites.drawSelfMasked(surv.x - mapPositionX, surv.y - mapPositionY, survivor, survivorFrame + (4 * survivorType[id]));
    if (showHelp)sprites.drawPlusMask(surv.x + 16 - mapPositionX, surv.y - 9 - mapPositionY, help_plus_mask, 0);
  }

}

// survivorCollision
// takes a survivor, collision box to test against
// returns true if collision boxes intersect
bool survivorCollision(Element& obj, int x, int y, int8_t w, int8_t h)
{
  return
    ( obj.active ) &&
    ( obj.x < x + w ) &&
    ( obj.x + SURVIVOR_WIDTH > x ) &&
    ( obj.y < y + h ) &&
    ( obj.y + SURVIVOR_HEIGHT > y );
}

// collectSurvivor
// takes a survivor, sets it inactive.
// returns false if no survivors are left on the map, otherwise true
bool collectSurvivor(Element& obj)
{
  int16_t id;
  obj.active = false;
  sound.tone(660, 20);
  rollingScore += 500;

  for (id = 0; id < SURVIVOR_MAX; id++)
  {
    if (survivors[id].active)
      return false;
  }

  return true;
}

// clearSurvivors
// clears the entire list of survivors
void clearSurvivors()
{
  int16_t id;

  for (id = 0; id < SURVIVOR_MAX; id++)
  {
    survivors[id].active = false;
  }
}

void drawAmountSurvivors()
{
  for (int16_t amountSurvivors = 0; amountSurvivors < countAmountActiveSurvivors(); amountSurvivors++) // needs the amount of active survivors
  {
    sprites.drawPlusMask(40 + amountSurvivors * 9, 0, HUD_plus_mask, 2);
  }
  if (!countAmountActiveSurvivors())
  {
    if (showHelp)
    {
      sprites.drawPlusMask(45, 0, HUD_plus_mask, 3);  //55
      sprites.drawPlusMask(54, 0, HUD_plus_mask, 4);  //64
    }
    sprites.drawPlusMask(65, 0, HUD_plus_mask, 5);
    sprites.drawPlusMask(74, 0, HUD_plus_mask, 6);
    drawNumbers(68, 1, FONT_TINY, DATA_TIMER);
  }
}

int16_t countAmountActiveSurvivors()
{
  int16_t id;
  int16_t countAmount = 0;
  for (id = 0; id < SURVIVOR_MAX; id++)
  {
    if (survivors[id].active) countAmount++;
  }
  return countAmount;
};


void survivorCollide(int x, int y)
{
  int16_t id;

  for (id = 0; id < SURVIVOR_MAX; id++)
  {
    if (survivorCollision(survivors[id], x, y, PLAYER_WIDTH, PLAYER_HEIGHT))
    {
      if (collectSurvivor(survivors[id]))
      {
        exitDoor.active = true;
      }
    }
  }
}
