#include "Arrows.hpp"

void Arrows::update() {
  millis += 31; // 32 frames per second, so 31.25 ms are rounded to 31
  switch (status) {
    case Status::instructions:
      if (arduboy.justPressed(A_BUTTON)) {
        status = Status::testing;
        millis = 0; // Reset the counter
      }
      break;
    case Status::testing:
      if (arduboy.justPressed(A_BUTTON)) {
        status = Status::score;
      }
      break;
    case Status::score:
      if (arduboy.justPressed(A_BUTTON)) {
        status = Status::done;
      }
      break;
    case Status::done:
      break;
  }
}

void Arrows::render() {
  switch (status) {
    case Status::instructions:
      tinyfont.setCursor(16, 16);
      tinyfont.print(F(                       /**/
                       "PRESS THE ARROWS'\n"   /**/
                       "DIRECTION WHEN THE\n"   /**/
                       "SCREEN IS BLACK.\n"   /**/
                       "PRESS THE ARROWS\n"   /**/
                       "MOVEMENT'S DIRECTION\n"   /**/
                       "WHEN THE SCREEN IS\n"   /**/
                       "WHITE.\n"   /**/
                       ));
      break;
    case Status::testing:
      break;
    case Status::score:
      tinyfont.setCursor(1, 1);
      tinyfont.print(F("FINAL SCORE:"));
      tinyfont.setCursor(1, 32);
      tinyfont.print(mScore);
      break;
    case Status::done:
      break;
  }
}

bool Arrows::complete(uint16_t& score) {
  score = mScore;
  return status == Status::done;
}
