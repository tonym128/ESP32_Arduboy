#include "VerbalMemory.hpp"
#include "Words.hpp"

void VerbalMemory::update() {
  millis += 31;  // 32 frames per second, so 31.25 ms are rounded to 31
  switch (status) {
    case Status::instructions:
      if (arduboy.justPressed(A_BUTTON)) {
        status = Status::running;
        millis = 0;  // Reset the counter

        for (uint8_t i = 0; i < wordsCount; i++) {
          words[i] = rand() % Words::wordsCount;
          wordsHits[i] = 0;
        }
        currentWord = 0;
      }
      break;
    case Status::running: {
      bool done = false;
      bool next = false;
      if (arduboy.justPressed(A_BUTTON)) {
        // If this is an error of if the user hit the same word enough times
        if (wordsHits[currentWord] != 0 || wordsHits[currentWord] > 4) {
          done = true;
        } else {
          next = true;
        }
      }
      if (arduboy.justPressed(B_BUTTON)) {
        // If this is an error of if the user hit the same word enough times
        if (wordsHits[currentWord] == 0 || wordsHits[currentWord] > 4) {
          done = true;
        } else {
          next = true;
        }
      }

      if (done) {
        mScore = 0;
        for (uint8_t i = 0; i < wordsCount; i++) {
          mScore += wordsHits[i] * 32;
        }
        status = Status::score;
      }

      if (next) {
        uint8_t oldWord = currentWord;
        wordsHits[currentWord] = wordsHits[currentWord] + 1;
        while (currentWord == oldWord) {
          currentWord = rand() % wordsCount;
        }
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

void VerbalMemory::render() {
  switch (status) {
    case Status::instructions:
      tinyfont.setCursor(16, 16);
      tinyfont.print(F(                          /**/
                       "PRESS A IF THE WORD\n"   /**/
                       "YOU READ IS NEW, OR\n"   /**/
                       "PRESS B IF YOU SAW IT\n" /**/
                       "BEFORE.\n"               /**/
                       ));
      break;
    case Status::running:
      tinyfont.setCursor(1, 1);
      tinyfont.print(Words::word(words[currentWord]));
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

bool VerbalMemory::complete(uint16_t& score) {
  score = mScore;
  return status == Status::done;
}
