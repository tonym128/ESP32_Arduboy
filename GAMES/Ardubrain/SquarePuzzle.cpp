#include "SquarePuzzle.hpp"

void SquarePuzzle::hN() {
  if (hole_y > 0) {
    matrix[hole_y * cells + hole_x] = matrix[(hole_y - 1) * cells + hole_x];
    matrix[(hole_y - 1) * cells + hole_x] = 0;
    hole_y--;
  }
}

void SquarePuzzle::hS() {
  if (hole_y < cells - 1) {
    matrix[hole_y * cells + hole_x] = matrix[(hole_y + 1) * cells + hole_x];
    matrix[(hole_y + 1) * cells + hole_x] = 0;
    hole_y++;
  }
}

void SquarePuzzle::hW() {
  if (hole_x > 0) {
    matrix[hole_y * cells + hole_x] = matrix[hole_y * cells + hole_x - 1];
    matrix[hole_y * cells + hole_x - 1] = 0;
    hole_x--;
  }
}

void SquarePuzzle::hE() {
  if (hole_x < cells - 1) {
    matrix[hole_y * cells + hole_x] = matrix[hole_y * cells + hole_x + 1];
    matrix[hole_y * cells + hole_x + 1] = 0;
    hole_x++;
  }
}

void SquarePuzzle::update() {
  millis += 31;  // 32 frames per second, so 31.25 ms are rounded to 31
  switch (status) {
    case Status::instructions:
      if (arduboy.justPressed(A_BUTTON)) {
        status = Status::solving;
        for (uint8_t i = 0; i < 128; i++) {
          switch (rand() % cells) {
            case 0:
              hN();
              break;
            case 1:
              hS();
              break;
            case 2:
              hW();
              break;
            case 3:
              hE();
              break;
          }
        }
        uint8_t hole_idx;
        for (hole_idx = 0; matrix[hole_idx] != 0; hole_idx++) {
        }
        hole_x = hole_idx % cells;
        hole_y = hole_idx / cells;
        millis = 0;
        moves = 0;
      }
      break;
    case Status::solving: {
      if (arduboy.justPressed(UP_BUTTON)) {
        hN();
        moves++;
      } else if (arduboy.justPressed(DOWN_BUTTON)) {
        hS();
        moves++;
      } else if (arduboy.justPressed(LEFT_BUTTON)) {
        hW();
        moves++;
      } else if (arduboy.justPressed(RIGHT_BUTTON)) {
        hE();
        moves++;
      } else if (arduboy.justPressed(B_BUTTON)) {
        mScore = 0;
        status = Status::done;
      }

      bool solved = true;
      for (uint8_t i = 0; i < cells * cells; i++) {
        if (matrix[i] != (i + 1) % (cells * cells)) {
          solved = false;
          break;
        }
      }

      if (solved) {
        mScore = (moves > 256 ? 0 : 256 - moves) * 8;
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

void SquarePuzzle::render() {
  tinyfont.setTextColor(WHITE);
  switch (status) {
    case Status::instructions:
      tinyfont.setCursor(16, 16);
      tinyfont.print(F(                          /**/
                       "USE THE DIRECTIONAL\n"   /**/
                       "PAD TO MOVE THE HOLE\n"  /**/
                       "AND SORT THE NUMBERS.\n" /**/
                       ));
      break;
    case Status::solving: {
      const uint8_t size = 14;
      const uint8_t top = (HEIGHT - size * cells) / 2;
      const uint8_t left = (WIDTH - size * cells) / 2;
      for (uint8_t l = 0; l < (cells + 1); l++) {
        arduboy.drawFastHLine(left, top + size * l, size * cells);
        arduboy.drawFastVLine(left + size * l, top, size * cells);
      }
      for (uint8_t r = 0; r < cells; r++) {
        for (uint8_t c = 0; c < cells; c++) {
          uint8_t val = matrix[r * cells + c];
          uint8_t off = (val) < 10 ? 6 : 3;
          tinyfont.setCursor(left + c * size + off, top + r * size + 5);
          if (val > 0) {
            tinyfont.print(val);
          }
        }
      }
    } break;
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

bool SquarePuzzle::complete(uint16_t& score) {
  score = mScore;
  return status == Status::done;
}
