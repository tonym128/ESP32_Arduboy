#include "Associative.hpp"
#include "Bitmaps.hpp"
#include "Words.hpp"

Associative::Associative(Arduboy2Base& arduboy, Tinyfont& tinyfont)
    : arduboy(arduboy), tinyfont(tinyfont) {
  for (int8_t i = 0; i < pairsCount; i++) {
    pairsL[i] = rand() % Words::wordsCount;
    pairsR[i] = rand() % Words::wordsCount;

    if (pairsL[i] == pairsR[i]) {
      i--;
      continue;
    }

    for (int8_t j = 0; j < pairsCount; j++) {
      if (i != j) {
        if (pairsL[i] == pairsL[j] || pairsL[i] == pairsR[j] || pairsR[i] == pairsL[j] ||
            pairsR[i] == pairsR[j]) {
          i--;
          continue;
        }
      }
    }
  }
}

bool Associative::updateMath() {
  bool res = l == r;
  if (s == 1) {
    res = l < r;
  }
  if (s == 2) {
    res = l > r;
  }

  s = rand() % 3;
  l = rand() % 16;
  r = rand() % 16;

  return res;
}

void Associative::shuffle() {
  do {
    for (uint8_t i = 0; i < 4; i++) {
      options[i] = rand() % pairsCount;
    }
  } while (options[0] == options[1] || options[0] == options[2] || options[0] == options[3] ||
           options[1] == options[2] || options[1] == options[3] || options[2] == options[3]);

  correct = rand() % 4;
  LtoR = (rand() % 2) == 0;
}

void Associative::update() {
  millis += 31;  // 32 frames per second, so 31.25 ms are rounded to 31
  switch (status) {
    case Status::instructionsMemorize:
      if (arduboy.justPressed(A_BUTTON)) {
        status = Status::showing;
        millis = 0;
        correct = 0;
      }
      break;
    case Status::instructionsMath:
      if (arduboy.justPressed(A_BUTTON)) {
        status = Status::math;
        updateMath();
        millis = 0;
        correct = 0;
      }
      break;
    case Status::instructionsTest:
      if (arduboy.justPressed(A_BUTTON)) {
        shuffle();
        status = Status::testing;
        millis = 0;
      }
      break;
    case Status::showing: {
      if (millis > 5000) {
        correct++;
        millis = 0;
      }
      if (correct >= pairsCount) {
        status = Status::instructionsMath;
      }
    } break;
    case Status::mathError: {
      if (arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON)) {
        status = Status::math;
      }
    } break;
    case Status::math: {
      if (millis > 40000) {
        millis = 0;
        status = Status::instructionsTest;
      }
      if (arduboy.justPressed(A_BUTTON)) {
        if (updateMath()) {
          mScore++;
        } else {
          status = Status::mathError;
        }
      } else if (arduboy.justPressed(B_BUTTON)) {
        if (!updateMath()) {
          mScore++;
        } else {
          status = Status::mathError;
        }
      }
    } break;
    case Status::testingError: {
      if (arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON)) {
        status = Status::testing;
      }
    } break;
    case Status::testing: {
      bool done = millis > 90000;
      if (arduboy.justPressed(LEFT_BUTTON)) {
        if (correct == 0) {
          mScore++;
        } else {
          status = Status::testingError;
        }
        shuffle();
      } else if (arduboy.justPressed(DOWN_BUTTON)) {
        if (correct == 1) {
          mScore++;
        } else {
          status = Status::testingError;
        }
        shuffle();
      } else if (arduboy.justPressed(UP_BUTTON)) {
        if (correct == 2) {
          mScore++;
        } else {
          status = Status::testingError;
        }
        shuffle();
      } else if (arduboy.justPressed(RIGHT_BUTTON)) {
        if (correct == 3) {
          mScore++;
        } else {
          status = Status::testingError;
        }
        shuffle();
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

void Associative::render() {
  switch (status) {
    case Status::instructionsMemorize:
      tinyfont.setCursor(16, 16);
      tinyfont.print(F(                           /**/
                       "YOU WILL BE SHOWED\n"     /**/
                       "RANDOM PAIRS OF WORDS\n"  /**/
                       "FOR SOME SECONDS EACH.\n" /**/
                       "TRY MAKING SHORT\n"       /**/
                       "MENTAL STORIES THAT\n"    /**/
                       "ASSOCIATE THE PAIRS.\n"   /**/
                       ));
      break;
    case Status::instructionsMath:
      tinyfont.setCursor(16, 16);
      tinyfont.print(F(                        /**/
                       "IF THE EQUATION IS\n"  /**/
                       "TRUE PRESS A, IF IT\n" /**/
                       "IS FALSE, PRESS B.\n"  /**/
                       ));
      break;
    case Status::instructionsTest:
      tinyfont.setCursor(16, 16);
      tinyfont.print(F(                          /**/
                       "PRESS THE D-PAD ARROW\n" /**/
                       "THAT MAPS THE WORD ON\n" /**/
                       "TOP TO ITS PAIR.\n"      /**/
                       ));
      break;
    case Status::mathError:
    case Status::testingError:
      tinyfont.setCursor(24, 32);
      tinyfont.print(F("ERROR!\n"));
      break;
    case Status::showing:
      tinyfont.setCursor(16, 16);
      tinyfont.print(Words::word(pairsL[correct]));
      tinyfont.setCursor(16, 32);
      tinyfont.print(Words::word(pairsR[correct]));
      break;
    case Status::math:
      tinyfont.setCursor(16, 32);
      tinyfont.print(l);
      tinyfont.setCursor(28, 32);
      tinyfont.print(s == 0 ? F("=") : (s == 1 ? F("<") : F(">")));
      tinyfont.setCursor(32, 32);
      tinyfont.print(r);
      break;
    case Status::testing:
      tinyfont.setCursor(32, 2);
      if (LtoR) {
        tinyfont.print(Words::word(pairsL[options[correct]]));
      } else {
        tinyfont.print(Words::word(pairsR[options[correct]]));
      }

      arduboy.drawBitmap(22, 32, bmp_left, 8, 8);
      arduboy.drawBitmap(22, 40, bmp_down, 8, 8);
      arduboy.drawBitmap(22, 48, bmp_up, 8, 8);
      arduboy.drawBitmap(22, 56, bmp_right, 8, 8);

      for (uint8_t opt = 0; opt < 4; opt++) {
        tinyfont.setCursor(32, 32 + 8 * opt);
        if (LtoR) {
          tinyfont.print(Words::word(pairsR[options[opt]]));
        } else {
          tinyfont.print(Words::word(pairsL[options[opt]]));
        }
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

bool Associative::complete(uint16_t& score) {
  score = mScore;
  return status == Status::done;
}
