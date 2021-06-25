#include "Series.hpp"
#include "Bitmaps.hpp"
#include "Words.hpp"

void Series::shuffle() {
  do {
    for (uint8_t i = 0; i < 4; i++) {
      options[i] = rand() % seriesLen;
    }
    correct = rand() % 4;
    options[correct] = series[current];
  } while (options[0] == options[1] || options[0] == options[2] || options[0] == options[3] ||
           options[1] == options[2] || options[1] == options[3] || options[2] == options[3]);

  current++;
  if (current > seriesLen) {
    mScore *= 8;
    status = Status::score;
  }
}

void Series::update() {
  millis += 31;  // 32 frames per second, so 31.25 ms are rounded to 31
  switch (status) {
    case Status::instructions:
      if (arduboy.justPressed(A_BUTTON)) {
        status = Status::showing;
        millis = 0;
        for (uint8_t i = 0; i < seriesLen; i++) {
          series[i] = rand() % Words::wordsCount;
        }
        shuffle();
      }
      break;
    case Status::showing:
      if (millis > 15000) {
        status = Status::testing;
      }
    case Status::testing: {
      bool done = false;
      if (arduboy.justPressed(LEFT_BUTTON)) {
        if (correct == 0) {
          mScore++;
          shuffle();
        } else {
          done = true;
        }
      } else if (arduboy.justPressed(DOWN_BUTTON)) {
        if (correct == 1) {
          mScore++;
          shuffle();
        } else {
          done = true;
        }
      } else if (arduboy.justPressed(UP_BUTTON)) {
        if (correct == 2) {
          mScore++;
          shuffle();
        } else {
          done = true;
        }
      } else if (arduboy.justPressed(RIGHT_BUTTON)) {
        if (correct == 3) {
          mScore++;
          shuffle();
        } else {
          done = true;
        }
      } else if (arduboy.justPressed(B_BUTTON)) {
        done = true;
      }

      if (done) {
        mScore *= 8;
        status = Status::score;
      }
    } break;
    case Status::score:
      if (arduboy.justPressed(A_BUTTON)) {
        status = Status::done;
      }
      break;
    case Status::done:
      break;
  }
}

void Series::renderSeries(uint8_t limit) {
  for (uint8_t r = 0; r < 4; r++) {
    for (uint8_t c = 0; c < 8; c++) {
      uint8_t idx = r * 8 + c;
      if (idx >= limit)
        return;
      char buff[4];
      snprintf_P(buff, 4, PSTR("%02d"), series[idx]);
      tinyfont.setCursor(6 + 15 * c, 1 + 7 * r);
      tinyfont.print(buff);
    }
  }
}

void Series::render() {
  switch (status) {
    case Status::instructions:
      tinyfont.setCursor(16, 16);
      tinyfont.print(F(                          /**/
                       "MEMORIZE THE SERIES\n"   /**/
                       "OF NUMBERS.\n"           /**/
                       "LATER, YOU WILL HAVE\n"  /**/
                       "TO RECONSTRUCT THE\n"    /**/
                       "SERIES PRESSING THE\n"   /**/
                       "D-PAD ARROW THAT\n"      /**/
                       "MAPS TO THE RIGHT\n"     /**/
                       "NUMBER IN THE SERIES.\n" /**/
                       ));
      break;
    case Status::showing:
      renderSeries(seriesLen);
      break;
    case Status::testing:
      renderSeries(current - 1);

      arduboy.drawBitmap(22, 32, bmp_left, 8, 8);
      arduboy.drawBitmap(22, 40, bmp_down, 8, 8);
      arduboy.drawBitmap(22, 48, bmp_up, 8, 8);
      arduboy.drawBitmap(22, 56, bmp_right, 8, 8);

      for (uint8_t opt = 0; opt < 4; opt++) {
        char buff[4];
        snprintf_P(buff, 4, PSTR("%02d"), options[opt]);
        tinyfont.setCursor(32, 32 + 8 * opt);
        tinyfont.print(buff);
      }

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

bool Series::complete(uint16_t& score) {
  score = mScore;
  return status == Status::done;
}
