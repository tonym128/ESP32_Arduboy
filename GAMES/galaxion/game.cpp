//
//
//

#include "game.h"
#include "ArduboyTones.h"

extern Arduboy2 mArduboy;
extern Game mGame;
extern ArduboyTones sound;

#include "bmp_title.h"
#include "bmp_chara.h"
#include "sound.h"

//=============================================================================
struct {
  int8_t speed;
  int8_t x;
  int16_t y;
} star[20];

//=============================================================================
Game::Game()
{
  mState = GAME_ST_LOGO;
  mFrame = 0;

  mHighScore = 0;
  mScore = 0;

  mMode = GAME_MODE_NORMAL;
  mCursor = 0;
}

//=============================================================================
Game::~Game()
{
}

//=============================================================================
void Game::init_stage()
{
  // ------------------------------------------------------ Init Parameters ---
  for (int idx = 0; idx < 20; idx ++) {
    star[idx].x = random(0, 19);
    star[idx].y = random(0, 64) * 8;
    star[idx].speed = random(0, 4) + 2;
  }

  mNumShips = 3;
  mStageNo = 1;
  mScore = 0;

  next_stage();

  mShip.init();

#if 0
  // ---------------------------------------------------- Play Credit Sound ---
  if mArduboy.audio.enabled() && !sound.playing()) {
    for (int tone = 400; tone < 1000; tone += 10) {
      sound.tone(tone, 10);
      delay(10);
    }
    sound.tone(1000, 60);
    delay(200);
  }
#endif

  // ---------------------------------------------- Play Game Opening Music ---
  if (mArduboy.audio.enabled() && !sound.playing()) {
    //mArduboy.tunes.playScore(snd_start);
    //sound.tones(snd_start);
  }
}

//=============================================================================
void Game::next_stage()
{
  mConvoy.init(mStageNo);
}

//=============================================================================
void Game::add_score(int score)
{
  uint16_t next_score;

  next_score = mScore + score;
  if (next_score >= mScore) {
    mScore = next_score;
  } else {
    mScore = 65535; // Score Overflows
  }
  if (mScore > mHighScore) {
    mHighScore = mScore;
  }
}

//=============================================================================
void Game::destroy_ship()
{
  mShip.explode();
  if (mNumShips == 0) {
    mNextShip = FRAMES_IN_SECOND;
  } else {
    mNextShip = FRAMES_IN_SECOND * 5;
  }
}

//=============================================================================
void Game::check_input()
{
  switch (mState) {
  case GAME_ST_TITLE:
    if (mArduboy.pressed(LEFT_BUTTON)) {
      switch (mCursor) {
      case 0:
	mMode = GAME_MODE_PRACTICE;
	break;
      case 1:
	if (mArduboy.audio.enabled()) {
	  mArduboy.audio.off();
	}
	break;
      }
    } else if (mArduboy.pressed(RIGHT_BUTTON)) {
      switch (mCursor) {
      case 0:
	mMode = GAME_MODE_NORMAL;
	break;
      case 1:
	if (!mArduboy.audio.enabled()) {
	  mArduboy.audio.on();
	}
	break;
      }
    } else if (mCursor > 0 && mArduboy.pressed(UP_BUTTON)) {
      mCursor --;
    } else if (mCursor < 1 && mArduboy.pressed(DOWN_BUTTON)) {
      mCursor ++;
    }

  case GAME_ST_GAMEOVER:
    if (mArduboy.pressed(A_BUTTON) || mArduboy.pressed(B_BUTTON)) {
      mState = GAME_ST_START;
      mFrame = 0;
      init_stage();
      mNextStage = 0;
      mNextShip = 0;
    }
    break;

  case GAME_ST_START:
    break;

  case GAME_ST_PLAY:
    mShip.check_input(mFrame);
    break;
  }
}

//=============================================================================
void Game::do_state()
{
  mFrame ++;

  switch (mState) {
  case GAME_ST_LOGO:
    if (mFrame >= (FRAMES_IN_SECOND * 2)) {
      mState = GAME_ST_TITLE;
      mFrame = 0;
    }
    break;

  case GAME_ST_TITLE:
    break;

  case GAME_ST_START:
    if (mFrame >= (FRAMES_IN_SECOND * 5)) {
      mNumShips --;
      mState = GAME_ST_PLAY;
    }
    mConvoy.do_state(mFrame, false, 0, 0);
    break;

  case GAME_ST_PLAY:
    mConvoy.do_state(mFrame, mShip.is_alive(),
		     mShip.pos.get_x(), mShip.pos.get_y());
    if (mNextStage == 0) {
      if (mShip.missile != NULL) {
	int hit = mConvoy.defeat_alien(mShip.missile);
	if (hit > 0) {
	  add_score(hit);

	  mShip.cancel_missile();

	  if (mConvoy.num_enemies == 0) {
	    mNextStage = FRAMES_IN_SECOND * 3;
	  }
	}
      }
    } else {
      mNextStage --;
      if (mNextStage == 0) {
	mStageNo ++;
	next_stage();
      }
    }
    if (mShip.is_alive()) {
      if (mConvoy.missile_hit(mShip.pos.get_x(), mShip.pos.get_y())) {
	destroy_ship();
      } else {
	int hit = mConvoy.alien_hit(mShip.pos.get_x(), mShip.pos.get_y());
	if (hit > 0) {
	  add_score(hit);

	  destroy_ship();

	  if (mConvoy.num_enemies == 0) {
	    mNextStage = FRAMES_IN_SECOND * 3;
	  }
	}
      }
    }

    mShip.do_state(mFrame);

    if (mNextShip > 0) {
      mNextShip --;
      if (mNextShip == 0) {
	if (mNumShips == 0) {
	  mState = GAME_ST_GAMEOVER;
	} else {
	  mNumShips --;
	  mShip.init();
	}
      }
    }
    break;

  case GAME_ST_GAMEOVER:
    mConvoy.do_state(mFrame, false, 0, 0);
    break;
  }
}

//=============================================================================
void Game::draw()
{
  switch (mState) {
  case GAME_ST_LOGO:
    draw_logo(mFrame);
    break;

  case GAME_ST_TITLE:
    draw_title(mFrame);
    break;

  case GAME_ST_START:
    mConvoy.draw(mFrame);
    draw_stage(mFrame);
    mArduboy.setCursor((128 - 60) / 2, (64 - 8) / 2 + 6);
    mArduboy.print("PLAYER ONE");
    break;

  case GAME_ST_PLAY:
    mShip.draw(mFrame);
    mConvoy.draw(mFrame);
    draw_stage(mFrame);
    break;

  case GAME_ST_GAMEOVER:
    mConvoy.draw(mFrame);
    draw_stage(mFrame);
    mArduboy.setCursor((128 - 60) / 2, (64 - 8) / 2 + 6);
    mArduboy.print("GAME  OVER");
    mArduboy.setCursor(64 - 6, 48);
    mArduboy.print("0 pts");
    int x = 64 - 12;
    for (int score = mScore; score > 0; score /= 10, x -= 6) {
      mArduboy.setCursor(x, 48);
      mArduboy.write('0' + (score % 10));
    }
    break;
  }

#ifdef CAPTURE_SCREEN
  // ------------------------------------------------------- Capture Screen ---
  if (Serial.read() >= 0) {
    Serial.write(mArduboy.getBuffer(), 128 * 64 / 8);
  }
#endif
}

//=============================================================================
static int draw_number(int x, int y, uint32_t num)
{
  x -= 2;
  if (num == 0) {
    mArduboy.drawBitmap(x, y, number_bmp[0], 2, 5, WHITE);
    return (x);
  }

  while (num > 0) {
    int digit = (num % 10);
    mArduboy.drawBitmap(x, y, number_bmp[digit], 2, 5, WHITE);
    num /= 10;
    x -= 3;
  }

  return (x + 3);
}

//=============================================================================
static void draw_score(int x, int y, uint32_t score)
{
  x -= 2;
  mArduboy.drawBitmap(x, y, number_bmp[0], 2, 5, WHITE);
  x -= 3;

  if (score == 0) {
    mArduboy.drawBitmap(x, y, number_bmp[0], 2, 5, WHITE);
    return;
  }

  while (score > 0) {
    int digit = (score % 10);
    mArduboy.drawBitmap(x, y, number_bmp[digit], 2, 5, WHITE);
    score /= 10;
    x -= 3;
  }
}

//=============================================================================
static void draw_flag(int x, int y)
{
  mArduboy.drawFastVLine(x, y, 4, WHITE);
  mArduboy.drawFastVLine(x+1, y, 2, WHITE);
}

//=============================================================================
static void draw_remain(int x, int y)
{
  mArduboy.drawFastVLine(x,   y+2, 3, WHITE);
  mArduboy.drawFastVLine(x+1, y,   4, WHITE);
  mArduboy.drawFastVLine(x+2, y+2, 3, WHITE);
}

//=============================================================================
void Game::draw_logo(int frame)
{
  int y;
  int x = 44;

  if (frame < 30) {
    y = 30 + (8 * (30 - frame)) / 30;
  } else {
    y = 30;
  }

  mArduboy.setCursor(x, y);
  mArduboy.print("ARDUBOY");

  if (frame < 30) {
    mArduboy.fillRect(0, 37, 128, 8, BLACK);
    mArduboy.drawLine(x-2, 37, x + 6 * 7, 37, WHITE);
  }
}
//=============================================================================
void Game::draw_title(int frame)
{
  int x, y;

  mArduboy.drawBitmap(32, 2, title_bmp, 64, 16, WHITE);

  x = (128-54)/2-8;

  // ----------------------------------------------------- Select Game Mode ---
  y = 26;
  if (mCursor == 0) {
    mArduboy.fillTriangle(x+3, y, x, y+3, x+3, y+6, WHITE);
    mArduboy.fillTriangle(x+65, y, x+65+3, y+3, x+65, y+6, WHITE);
  }
  mArduboy.setCursor(x + 8 + 3, y);
  switch (mMode) {
  case GAME_MODE_PRACTICE:
    mArduboy.print("PRACTICE");
    break;
  case GAME_MODE_NORMAL:
    mArduboy.print(" NORMAL ");
    break;
  }

  // -------------------------------------------------- Select Sound On/Off ---
  y = 38;
#if 0
  if (mArduboy.audio.enabled()) {
    mArduboy.fillTriangle(x+3, y, x, y+3, x+3, y+6, WHITE);
    mArduboy.drawTriangle(x+65, y, x+65+3, y+3, x+65, y+6, WHITE);
    mArduboy.print("SOUND: ON");
  } else {
    mArduboy.drawTriangle(x+3, y, x, y+3, x+3, y+6, WHITE);
    mArduboy.fillTriangle(x+65, y, x+65+3, y+3, x+65, y+6, WHITE);
    mArduboy.print("SOUND:OFF");
  }
#else
  if (mCursor == 1) {
    mArduboy.fillTriangle(x+3, y, x, y+3, x+3, y+6, WHITE);
    mArduboy.fillTriangle(x+65, y, x+65+3, y+3, x+65, y+6, WHITE);
  }
  mArduboy.setCursor(x + 8, y);
  if (mArduboy.audio.enabled()) {
    mArduboy.print("SOUND: ON");
  } else {
    mArduboy.print("SOUND:OFF");
  }
#endif

  x = (128-6*13)/2;
  y = 54;
  mArduboy.setCursor(x, y);
  mArduboy.print("PUSH A BUTTON");
}

//=============================================================================
void Game::draw_stage(int frame)
{
  // ------------------------------------------------------------- Clipping ---
  mArduboy.fillRect(14, 0, 6, 64, BLACK);
  mArduboy.fillRect(108, 0, 6, 64, BLACK);

  // ------------------------------------------------------ Draw High Score ---
  mArduboy.drawBitmap(128-18, 0, label_bmp[1], 18, 8, WHITE);
  draw_score(128, 8, mHighScore);

  // ------------------------------------------------ Draw Player One Score ---
  mArduboy.drawBitmap(128-18, 16, label_bmp[0], 18, 8, WHITE);
  draw_score(128, 16+8, mScore);

  // -------------------------------------------------- Draw Remained Ships ---
  for (int num = 0; num < mNumShips; num ++) {
    draw_remain(num * 4, 64 - 5);
  }

  // ------------------------------------------------------- Draw Stage No. ---
  if (mStageNo <= 25) {
    for (int num = 0; num < mStageNo; num ++) {
      draw_flag(128 - 2 - (num % 5) * 3, 64 - 4 - (num / 5) * 5);
    }
  } else {
    int x = draw_number(128, 64 - 5, mStageNo);
    draw_flag(x - 4, 64 - 4);
  }

  // ----------------------------------------------------------- Draw Stars ---
  for (int idx = 0; idx < 20; idx ++) {
    if (random(0, 8) > 1) {
      mArduboy.drawPixel(star[idx].x, star[idx].y / 8, WHITE);
    }
    if (random(0, 8) > 1) {
      mArduboy.drawPixel(127 - star[idx].x, star[idx].y / 8, WHITE);
    }
    star[idx].y += star[idx].speed;
    if ((star[idx].y / 8) >= 64) {
      star[idx].x = random(0, 19);
      star[idx].y = 0;
      star[idx].speed = random(0, 4) + 2;
    }
  }

  // ------------------------------------------------------- Draw Game Mode ---
  if (mMode == GAME_MODE_PRACTICE) {
    mArduboy.setCursor(0, 0);
    mArduboy.print("P");
  }
}

//================================ END-OF-FILE ================================
