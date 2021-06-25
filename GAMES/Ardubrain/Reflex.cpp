#include "Reflex.hpp"

void Reflex::update() {
  millis += 31;  // 32 frames per second, so 31.25 ms are rounded to 31
  switch (status) {
    case Status::instructions:
      if (arduboy.justPressed(A_BUTTON)) {
        status = Status::black;
        millis = 0;                        // Reset the counter
        waiting = 2000 + (rand() % 5000);  // between 2 and 5 seconds
      }
      break;
    case Status::black:
      if (millis > waiting) {
        status = Status::white;
      }
      if (arduboy.justPressed(A_BUTTON)) {
        status = Status::pressed;
        reactions[attempt] = 2000;  // Penalty for early pressing
      }
      break;
    case Status::white:
      if (arduboy.justPressed(A_BUTTON)) {
        status = Status::pressed;
        reactions[attempt] = millis - waiting;
        if (reactions[attempt] > 2000) {
          reactions[attempt] = 2000;
        }
      }
      break;
    case Status::pressed:
      if (arduboy.justPressed(A_BUTTON)) {
        attempt++;
        if (attempt < 4) {
          status = Status::black;
          millis = 0;                        // Reset the counter
          waiting = 2000 + (rand() % 5000);  // between 2 and 5 seconds
        } else {
          mScore = 2000 - (reactions[0] + reactions[1] + reactions[2] + reactions[3]) / 4;
          status = Status::score;
        }
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

void Reflex::render() {
  switch (status) {
    case Status::instructions:
      tinyfont.setCursor(16, 16);
      tinyfont.print(F(                       /**/
                       "PRESS A WHEN THE\n"   /**/
                       "SQUARE TURNS WHITE\n" /**/
                       ));
      break;
    case Status::black:
      arduboy.drawRoundRect(64 - 16, 32 - 16, 32, 32, WHITE);
      break;
    case Status::white:
      arduboy.fillRoundRect(64 - 16, 32 - 16, 32, 32, WHITE);
      break;
    case Status::pressed:
      tinyfont.setCursor(1, 1);
      tinyfont.print(F("REACTION MS:"));
      tinyfont.setCursor(1, 32);
      tinyfont.print(reactions[attempt]);
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

bool Reflex::complete(uint16_t& score) {
  score = mScore;
  return status == Status::done;
}
