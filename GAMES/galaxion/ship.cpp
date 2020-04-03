//
//
//

#include "game.h"
#include "ArduboyTones.h"

#include "bmp_chara.h"

extern Arduboy2 mArduboy;
extern Game mGame;
extern ArduboyTones sound;

//=============================================================================
void Ship::init()
{
  state = SHIP_ST_READY;
  pos.set((SCREEN_W - 5) / 2, 64 - 7);
  if (missile != NULL) {
    delete missile;
    missile = NULL;
  }
  dont_shoot = FRAMES_IN_SECOND / 4;
}

//=============================================================================
void Ship::explode()
{
  state = SHIP_ST_EXPLODED;
  pat = 0;
}

//=============================================================================
void Ship::check_input(int frame)
{
  switch (state) {
  case SHIP_ST_READY:
    if (dont_shoot <= 0
	&& (mArduboy.pressed(A_BUTTON) || mArduboy.pressed(B_BUTTON))) {
      state = SHIP_ST_SHOOTING;
      if (mArduboy.audio.enabled() && !sound.playing()) {
        sound.tone(50, 30, 100, 30, 200, 30);
      }
      
      if (missile == NULL) {
	missile = new Position(pos.get_x() + 2, pos.get_y());
      }
    }
  case SHIP_ST_SHOOTING:
    if (mArduboy.pressed(LEFT_BUTTON)) {
      if (pos.get_x() > 0) {
	pos.move_x(-0.3f);
      }
    }
    else if (mArduboy.pressed(RIGHT_BUTTON)) {
      if (pos.get_x() < (SCREEN_W - 5)) {
	pos.move_x(0.3f);
      }
    }
    break;
  }
}

//=============================================================================
void Ship::do_state(int frame)
{
  if (missile != NULL) {
    int mis_y = missile->move_y(-1.5f);
    if (mis_y < 0) {
      if (state == SHIP_ST_SHOOTING) {
	state = SHIP_ST_READY;
	dont_shoot = FRAMES_IN_SECOND / 4;
      }
      delete missile;
      missile = NULL;
    }
  }

  if (dont_shoot > 0) {
    dont_shoot --;
  }

  switch (state) {
  case SHIP_ST_READY:
    break;
  case SHIP_ST_SHOOTING:
    break;
  case SHIP_ST_EXPLODED:
    if ((frame & 15) == 0) {
      pat ++;
      if (pat >= 4) {
	state = SHIP_ST_LOST;
      }
    }
//    if (mArduboy.audio.enabled() && !mArduboy.tunes.playing()) {
   if (mArduboy.audio.enabled() && !sound.playing()) {
      sound.tone(100 + (frame & 3) * 10, 20);
    }
    break;
  case SHIP_ST_LOST:
    break;
  }
}

//=============================================================================
void Ship::draw(int frame)
{
  int x = pos.get_x();
  int y = pos.get_y();

  switch (state) {
  case SHIP_ST_READY:
    mArduboy.drawBitmap(x + SCREEN_X, y, ship_bmp[0], 5, 7, WHITE);
    break;
  case SHIP_ST_SHOOTING:
    mArduboy.drawBitmap(x + SCREEN_X, y, ship_bmp[1], 5, 7, WHITE);
    break;
  case SHIP_ST_EXPLODED:
    mArduboy.drawBitmap(x + SCREEN_X - 1, y - 1, shipexp_bmp[pat], 9, 9, WHITE);
    break;
  case SHIP_ST_LOST:
    break;
  }

  if (missile != NULL) {
    mArduboy.drawFastVLine(missile->get_x() + SCREEN_X,
			   missile->get_y(), 3, WHITE);
  }
}

//=============================================================================
void Ship::cancel_missile()
{
  if (missile != NULL) {
    delete missile;
    missile = NULL;
  }

  if (state == SHIP_ST_SHOOTING) {
    state = SHIP_ST_READY;
    dont_shoot = FRAMES_IN_SECOND / 8;
  }
}

//=============================================================================
bool Ship::is_alive()
{
  if (state == SHIP_ST_READY || state == SHIP_ST_SHOOTING) {
    return (true);
  }
  return (false);
}

//================================ END-OF-FILE ================================
