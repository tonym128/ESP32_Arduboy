#include "Maze.hpp"

bool Maze::solveable() {
  uint8_t temp[8][16];

  for (uint8_t r = 0; r < 8; r++) {
    for (uint8_t c = 0; c < 16; c++) {
      temp[r][c] = 0;
    }
  }

  temp[posY][posX] = 1;
  temp[goalY][goalX] = 2;

  uint8_t oldOnes = 0;

  do {
    uint8_t ones = 0;
    for (uint8_t r = 0; r < 8; r++) {
      for (uint8_t c = 0; c < 16; c++) {
        if (temp[r][c] == 1) {
          ones++;
          if (r > 0 && maze[r - 1][c] == 0) {
            if (temp[r - 1][c] == 2) {
              return true;
            }
            temp[r - 1][c] = 1;
          }
          if (r < 7 && maze[r + 1][c] == 0) {
            if (temp[r + 1][c] == 2) {
              return true;
            }
            temp[r + 1][c] = 1;
          }
          if (c > 0 && maze[r][c - 1] == 0) {
            if (temp[r][c - 1] == 2) {
              return true;
            }
            temp[r][c - 1] = 1;
          }
          if (c < 15 && maze[r][c + 1] == 0) {
            if (temp[r][c + 1] == 2) {
              return true;
            }
            temp[r][c + 1] = 1;
          }
        }
      }
    }
    if (ones == oldOnes) {
      break;
    }
    oldOnes = ones;
  } while (true);

  return false;
}

void Maze::update() {
  millis += 31;  // 32 frames per second, so 31.25 ms are rounded to 31
  switch (status) {
    case Status::instructions:
      if (arduboy.justPressed(A_BUTTON)) {
        status = Status::showing;
        millis = 0;  // Reset the counter
        moves = 0;

        posX = 0;
        posY = 1 + rand() % 6;
        goalX = 15;
        goalY = 1 + rand() % 7;

        for (uint8_t r = 0; r < 8; r++) {
          if (r != posY) {
            maze[r][0] = (rand() % 4) == 1;
          }
          if (r != goalY) {
            maze[r][15] = (rand() % 4) == 1;
          }
        }

        for (uint8_t c = 1; c < 15; c++) {
          maze[0][c] = (rand() % 4) == 1;
          maze[7][c] = (rand() % 4) == 1;
        }

        for (uint8_t a = 0; a < 16; a++) {
          for (uint8_t r = 1; r < 7; r++) {
            for (uint8_t c = 1; c < 15; c++) {
              if (!maze[r][c]) {
                uint8_t borders = (maze[r - 1][c] ? 1 : 0) + (maze[r + 1][c] ? 1 : 0) +
                                  (maze[r][c - 1] ? 1 : 0) + (maze[r][c + 1] ? 1 : 0);
                if (borders == 1) {
                  maze[r][c] = (rand() % 2) == 1;
                  if (!solveable()) {
                    maze[r][c] = false;
                  }
                }
              }
            }
          }
        }
      }
      break;
    case Status::showing:
      if (millis > 5000) {
        moves = 0;
        millis = 0;
        status = Status::navigating;
      }
      break;
    case Status::navigating:
      if (arduboy.justPressed(UP_BUTTON)) {
        if (posY > 0 && (!maze[posY - 1][posX])) {
          posY--;
        }
        moves++;
      } else if (arduboy.justPressed(DOWN_BUTTON)) {
        if (posY < 7 && (!maze[posY + 1][posX])) {
          posY++;
        }
        moves++;
      } else if (arduboy.justPressed(LEFT_BUTTON)) {
        if (posX > 0 && (!maze[posY][posX - 1])) {
          posX--;
        }
        moves++;
      } else if (arduboy.justPressed(RIGHT_BUTTON)) {
        if (posX < 15 && (!maze[posY][posX + 1])) {
          posX++;
        }
        moves++;
      } else if (arduboy.justPressed(B_BUTTON)) {
        mScore = 0;
        status = Status::score;
      }
      if (posX == goalX && posY == goalY) {
        mScore = (100 - moves) * 8;  // TODO considerate the millis as well?
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

void Maze::render() {
  switch (status) {
    case Status::instructions:
      tinyfont.setCursor(16, 16);
      tinyfont.print(F(                           /**/
                       "WAIT FOR THE FIELD OF\n"  /**/
                       "VIEW TO REDUCE, THEN\n"   /**/
                       "NAVIGATE THE MAZE AND\n"  /**/
                       "REACH THE EXIT WITH AS\n" /**/
                       "FEW MOVEMENTS AS\n"       /**/
                       "POSSIBLE.\n"              /**/
                       ));
      break;
    case Status::showing:
      for (uint8_t r = 0; r < 8; r++) {
        for (uint8_t c = 0; c < 16; c++) {
          if (maze[r][c]) {
            arduboy.fillRect(c * 8, r * 8, 8, 8, WHITE);
          }
        }
      }
      break;
    case Status::navigating:
      for (int8_t r = posY - 1; r < posY + 2; r++) {
        for (int8_t c = posX - 1; c < posX + 2; c++) {
          if (r >= 0 && c >= 0 && r < 8 && c < 16 && maze[r][c]) {
            arduboy.fillRect(c * 8, r * 8, 8, 8, WHITE);
          }
        }
      }
      /* Draw a bunch of black circles to simulate a field of view. */
      arduboy.drawCircle(4 + posX * 8, 4 + posY * 8, 10, BLACK);
      arduboy.drawCircle(4 + posX * 8, 4 + posY * 8, 11, BLACK);
      arduboy.drawCircle(4 + posX * 8, 4 + posY * 8, 12, BLACK);
      arduboy.drawCircle(4 + posX * 8, 4 + posY * 8, 13, BLACK);
      arduboy.drawCircle(4 + posX * 8, 4 + posY * 8, 14, BLACK);
      arduboy.drawCircle(4 + posX * 8, 4 + posY * 8, 15, BLACK);
      arduboy.drawCircle(4 + posX * 8, 4 + posY * 8, 16, BLACK);
      arduboy.drawCircle(3 + posX * 8, 4 + posY * 8, 10, BLACK);
      arduboy.drawCircle(3 + posX * 8, 4 + posY * 8, 11, BLACK);
      arduboy.drawCircle(3 + posX * 8, 4 + posY * 8, 12, BLACK);
      arduboy.drawCircle(3 + posX * 8, 4 + posY * 8, 13, BLACK);
      arduboy.drawCircle(3 + posX * 8, 4 + posY * 8, 14, BLACK);
      arduboy.drawCircle(3 + posX * 8, 4 + posY * 8, 15, BLACK);
      arduboy.drawCircle(3 + posX * 8, 4 + posY * 8, 16, BLACK);
      arduboy.drawCircle(5 + posX * 8, 5 + posY * 8, 10, BLACK);
      arduboy.drawCircle(5 + posX * 8, 5 + posY * 8, 11, BLACK);
      arduboy.drawCircle(5 + posX * 8, 5 + posY * 8, 12, BLACK);
      arduboy.drawCircle(5 + posX * 8, 5 + posY * 8, 13, BLACK);
      arduboy.drawCircle(5 + posX * 8, 5 + posY * 8, 14, BLACK);
      arduboy.drawCircle(5 + posX * 8, 5 + posY * 8, 15, BLACK);
      arduboy.drawCircle(5 + posX * 8, 5 + posY * 8, 16, BLACK);
      /* Draw current positionand goal. */
      arduboy.fillCircle(4 + goalX * 8, 4 + goalY * 8, 3);
      arduboy.fillCircle(4 + posX * 8, 4 + posY * 8, 3);
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

bool Maze::complete(uint16_t& score) {
  score = mScore;
  return status == Status::done;
}
