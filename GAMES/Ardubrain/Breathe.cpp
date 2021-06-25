#include "Breathe.hpp"

void Breathe::update() {
  millis += 31;  // 32 frames per second, so 31.25 ms are rounded to 31
  switch (status) {
    case Status::instructions:
      if (arduboy.justPressed(A_BUTTON)) {
        arduboy.waitNoButtons();
        arduboy.pollButtons();
        status = Status::exhale;
        millis = 0;
      }
      break;
    case Status::inhale:
      level = (24 * millis) / 4096;
      if (level > 24) {
        level = 24;
        error++;
      }
      if (arduboy.justReleased(A_BUTTON)) {
        count++;
        if (count > breathsCount) {
          mScore = error < 2000 ? 2000 - error : 0;
          status = Status::score;
        } else {
          if (millis < 4096) {
            error += (4096 - millis) / 32;
          }
          status = Status::exhale;
          millis = 0;
        }
      }
      break;
    case Status::exhale:
      level = (24 * millis) / 4096;
      if (level > 24) {
        level = 24;
        error++;
      }
      level = 24 - level;
      if (arduboy.justPressed(A_BUTTON)) {
        if (millis < 4096) {
          error += (4096 - millis) / 32;
        }
        status = Status::inhale;
        millis = 0;
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

void Breathe::render() {
  switch (status) {
    case Status::instructions:
      tinyfont.setCursor(16, 16);
      tinyfont.print(F(                           /**/
                       "KEEP A PRESSED WHILE\n"   /**/
                       "YOU INHALE AND RELEASE\n" /**/
                       "IT WHILE YOU EXHALE.\n"   /**/
                       "TRY TO KEEP IN SYNCH\n"   /**/
                       "WITH THE SCREEN.\n"       /**/
                       ));
      break;
    case Status::inhale:
      arduboy.drawCircle(WIDTH / 2, HEIGHT / 2, 24);
      arduboy.fillCircle(WIDTH / 2, HEIGHT / 2, level);
      break;
    case Status::exhale:
      arduboy.drawCircle(WIDTH / 2, HEIGHT / 2, 24);
      arduboy.fillCircle(WIDTH / 2, HEIGHT / 2, level);
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

bool Breathe::complete(uint16_t& score) {
  score = mScore;
  return status == Status::done;
}
