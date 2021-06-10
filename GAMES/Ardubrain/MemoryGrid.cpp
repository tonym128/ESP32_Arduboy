#include "MemoryGrid.hpp"

void MemoryGrid::createTest() {
  for (uint8_t i = 0; i < rows * cols; i++) {
    matrix[i] = 0;
    solution[i] = 0;
  }
  for (uint8_t i = 0; i < difficulty; i++) {
    uint8_t x = rand() % cols;
    uint8_t y = rand() % rows;
    if (solution[y * cols + x] == 0) {
      solution[y * cols + x] = 1;
    } else {
      i--;
    }
  }
  millis = 0;
  moves = 0;
}

void MemoryGrid::update() {
  millis += 31;  // 32 frames per second, so 31.25 ms are rounded to 31
  switch (status) {
    case Status::instructions:
      if (arduboy.justPressed(A_BUTTON)) {
        createTest();
        status = Status::showing;
      }
      break;
    case Status::showing:
      if (arduboy.justPressed(B_BUTTON) || millis > 2000) {
        status = Status::solving;
      }
      break;
    case Status::solving: {
      if (arduboy.justPressed(UP_BUTTON)) {
        if (cursor_y > 0) {
          cursor_y--;
        }
      } else if (arduboy.justPressed(DOWN_BUTTON)) {
        if (cursor_y < rows - 1) {
          cursor_y++;
        }
      } else if (arduboy.justPressed(LEFT_BUTTON)) {
        if (cursor_x > 0) {
          cursor_x--;
        }
      } else if (arduboy.justPressed(RIGHT_BUTTON)) {
        if (cursor_x < cols - 1) {
          cursor_x++;
        }
      } else if (arduboy.justPressed(A_BUTTON)) {
        if (solution[cursor_y * cols + cursor_x]) {
          matrix[cursor_y * cols + cursor_x] = 1;
        }
        moves++;
      } else if (arduboy.justPressed(B_BUTTON)) {
        mScore = 0;
        status = Status::done;
      }

      bool solved = true;
      for (uint8_t i = 0; i < rows * cols; i++) {
        if (matrix[i] != solution[i]) {
          solved = false;
          break;
        }
      }
      if (solved) {
        mScore += difficulty * 8;
        if (moves > difficulty) {
          status = Status::score;
        } else {
          difficulty++;
          const uint8_t max_difficulty = (rows * cols) / 2;
          if (difficulty > max_difficulty) {
            difficulty = max_difficulty;
          }
          createTest();
          status = Status::showing;
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

void MemoryGrid::drawGrid(uint8_t mat[]) {
  const uint8_t size = 14;
  const uint8_t top = (HEIGHT - size * rows) / 2;
  const uint8_t left = (WIDTH - size * cols) / 2;
  for (uint8_t l = 0; l < (rows + 1); l++) {
    arduboy.drawFastHLine(left, top + size * l, size * cols);
  }
  for (uint8_t l = 0; l < (cols + 1); l++) {
    arduboy.drawFastVLine(left + size * l, top, size * rows);
  }
  for (uint8_t r = 0; r < rows; r++) {
    for (uint8_t c = 0; c < cols; c++) {
      if (mat[r * cols + c]) {
        arduboy.fillRect(left + c * size + 2, top + r * size + 2, size - 3, size - 3);
      }
    }
  }
}

void MemoryGrid::render() {
  tinyfont.setTextColor(WHITE);
  switch (status) {
    case Status::instructions:
      tinyfont.setCursor(16, 16);
      tinyfont.print(F(                         /**/
                       "REMEMBER THE WHILE \n"  /**/
                       "SQUARES AND PRESS A\n"  /**/
                       "AND SORT THE NUMBERS\n" /**/
                       "TURN BLACK.\n"          /**/
                       ));
      break;
    case Status::showing:
      drawGrid(solution);
      break;
    case Status::solving: {
      drawGrid(matrix);
      const uint8_t size = 14;
      const uint8_t top = (HEIGHT - size * rows) / 2;
      const uint8_t left = (WIDTH - size * cols) / 2;
      const uint8_t color = matrix[cursor_y * cols + cursor_x] ? BLACK : WHITE;
      arduboy.fillCircle(left + cursor_x * size + 7, top + cursor_y * size + 7, 3, color);
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

bool MemoryGrid::complete(uint16_t& score) {
  score = mScore;
  return status == Status::done;
}
