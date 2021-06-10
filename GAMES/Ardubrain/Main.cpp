#include "Main.hpp"
#include "Associative.hpp"
#include "Bitmaps.hpp"
#include "Breathe.hpp"
#include "Major.hpp"
#include "Maze.hpp"
#include "MemoryGrid.hpp"
#include "Reflex.hpp"
#include "Series.hpp"
#include "SquarePuzzle.hpp"
#include "VerbalMemory.hpp"
#include "Arrows.hpp"

void Main::init() {
  /* initialize the random seed with noise */
//  srand(arduboy.randomSeed());

  /* initiate arduboy instance */
  arduboy.begin();

  /* set the framerate */
  arduboy.setFrameRate(frame_rate);

  /* start the music */
  arduboy.audio.begin();

  /* load the high score */
  loadHighScore();
}

void Main::saveHighScore() {
  const uint16_t loc = EEPROM_STORAGE_SPACE_START;
  EEPROM.write(loc + 0, 'A');
  EEPROM.write(loc + 1, 'R');
  EEPROM.write(loc + 2, 'B');
  EEPROM.write(loc + 3, '0');
  EEPROM.write(loc + 4, '1');
  uint8_t* scores = reinterpret_cast<uint8_t*>(hiScores);
  for (uint16_t l = 0; l < sizeof(hiScores); l++) {
    EEPROM.write(loc + 10 + l, scores[l]);
  }
 EEPROM.commit();
}

void Main::loadHighScore() {
  const uint16_t loc = EEPROM_STORAGE_SPACE_START;
  uint8_t h0 = EEPROM.read(loc + 0);
  uint8_t h1 = EEPROM.read(loc + 1);
  uint8_t h2 = EEPROM.read(loc + 2);
  uint8_t h3 = EEPROM.read(loc + 3);
  uint8_t h4 = EEPROM.read(loc + 4);
  if (h0 == 'A' && h1 == 'R' && h2 == 'B' && h3 == '0' && h4 == '1') {
    uint8_t* scores = reinterpret_cast<uint8_t*>(hiScores);
    for (uint16_t l = 0; l < sizeof(hiScores); l++) {
      scores[l] = EEPROM.read(loc + 10 + l);
    }
  }
}

void Main::loop() {
  /* pause render until it's time for the next frame */
  if (!(arduboy.nextFrame())) {
    return;
  }
  /* update the scene */
  update();
  /* render the scene */
  render();
}

void Main::update() {
  arduboy.pollButtons();

  switch (currentMenu) {
    case Menus::main:
      if (arduboy.justPressed(UP_BUTTON)) {
        uint8_t sel = static_cast<uint8_t>(currentMainSelection);
        sel = (sel + selectionsCount - 1) % selectionsCount;
        currentMainSelection = static_cast<MainSelections>(sel);
      }
      if (arduboy.justPressed(DOWN_BUTTON)) {
        uint8_t sel = static_cast<uint8_t>(currentMainSelection);
        sel = (sel + selectionsCount + 1) % selectionsCount;
        currentMainSelection = static_cast<MainSelections>(sel);
      }
      if (arduboy.justPressed(RIGHT_BUTTON)) {
      }
      if (arduboy.justPressed(LEFT_BUTTON)) {
      }
      if (arduboy.justPressed(A_BUTTON)) {
        if (test != nullptr) {
          delete test;
        }
        switch (currentMainSelection) {
          case MainSelections::reflex:
            test = new Reflex(arduboy, tinyfont);
            currentMenu = Menus::test;
            break;
          case MainSelections::planning:
            test = new SquarePuzzle(arduboy, tinyfont);
            currentMenu = Menus::test;
            break;
          case MainSelections::focus:
            test = new Breathe(arduboy, tinyfont);
            currentMenu = Menus::test;
            break;
          case MainSelections::memory:
            test = new MemoryGrid(arduboy, tinyfont);
            currentMenu = Menus::test;
            break;
          case MainSelections::verbal:
            test = new VerbalMemory(arduboy, tinyfont);
            currentMenu = Menus::test;
            break;
          case MainSelections::major:
            test = new Major(arduboy, tinyfont);
            currentMenu = Menus::test;
            break;
          case MainSelections::associative:
            test = new Associative(arduboy, tinyfont);
            currentMenu = Menus::test;
            break;
          case MainSelections::maze:
            test = new Maze(arduboy, tinyfont);
            currentMenu = Menus::test;
            break;
          case MainSelections::series:
            test = new Series(arduboy, tinyfont);
            currentMenu = Menus::test;
            break;
          case MainSelections::arrows:
            test = new Arrows(arduboy, tinyfont);
            currentMenu = Menus::test;
            break;
          default:
            break;
        }
      }
      if (arduboy.justPressed(B_BUTTON)) {
        currentMenu = Menus::credits;
      }
      break;
    case Menus::test:
      if (test != nullptr) {
        test->update();
        uint16_t score = 0;
        if (test->complete(score)) {
          delete test;
          test = nullptr;
          currentMenu = Menus::main;
          uint8_t sel = static_cast<uint8_t>(currentMainSelection);
          if (hiScores[sel] < score) {
            hiScores[sel] = score;
          }
          saveHighScore();
        }
      } else {
        currentMenu = Menus::main;
      }
      break;
    case Menus::credits:
      if (arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON)) {
        currentMenu = Menus::main;
      }
      break;
  }
}

void Main::render() {
  /* clear the whole screen to black */
  arduboy.clear();

  /* render the menus */
  switch (currentMenu) {
    case Menus::main: {
      tinyfont.setCursor(1, 1);
      tinyfont.print(F("ARDU\n BRAIN"));
      arduboy.drawBitmap(1, 12, bmp_logo, 32, 32);

      tinyfont.setCursor(20, 64 - 6);
      tinyfont.print(F("TOTAL SCORE:"));
      tinyfont.setCursor(38 + 1 + 5 * 9, 64 - 6);
      uint16_t totalScore = 0;
      for (uint8_t s = 0; s < selectionsCount; s++) {
        totalScore += hiScores[s];
      }
      tinyfont.print(totalScore);

      tinyfont.setCursor(38 + 1, 0 + 3);
      tinyfont.print(F("TEST:    HI-SCORE:"));

      uint8_t sel = static_cast<uint8_t>(currentMainSelection);

      for (uint8_t e = 0; e < 5; e++) {
        uint8_t s = (sel < 3) ? e
                              : ((sel < selectionsCount - 2) ? (e + sel - 2)
                                                             : (e + selectionsCount - 5));
        uint8_t x = 38;
        uint8_t y = e * 8 + 14;
        tinyfont.setTextColor(sel == s ? BLACK : WHITE);
        arduboy.fillRect(x, y, WIDTH, 6, sel == s ? WHITE : BLACK);
        tinyfont.setCursor(x + 1 + 5 * 9, y + 1);
        tinyfont.print(hiScores[s]);
        tinyfont.setCursor(x + 1, y + 1);
        tinyfont.print(Selections::name(s));
      }

      tinyfont.setTextColor(WHITE);

      arduboy.drawFastHLine(38, 10, WIDTH);
      arduboy.drawFastHLine(0, HEIGHT - 10, WIDTH);
      arduboy.drawFastVLine(37, 0, HEIGHT - 10);

      break;
    }
    case Menus::test:
      test->render();
      break;
    case Menus::credits: {
      tinyfont.setCursor(2, 0);
      tinyfont.print(
          F("\nARDUBRAIN WAS MADE BY\n" /**/
            "\tRENATO GROTTESI\n"       /**/
            ));
      break;
      default:
        currentMenu = Menus::main;
    } break;
  }

  /* tell the arduboy to swap buffers */
  arduboy.display();
}

const Main::Selections selections[Main::selectionsCount] PROGMEM = {
    {"REFLEX"}, /* reflex      */
    {"PLAN"},   /* planning    */
    {"FOCUS"},  /* focus       */
    {"VISUAL"}, /* memory      */
    {"VERBAL"}, /* verbal      */
    {"MAJOR"},  /* major       */
    {"PAIRS"},  /* associative */
    {"MAZE"},   /* maze        */
    {"SERIES"}, /* series      */
    {"ARROWS"}, /* arrows      */
};

const __FlashStringHelper* Main::Selections::name() const {
  return reinterpret_cast<const __FlashStringHelper*>(_name);
}

const __FlashStringHelper* Main::Selections::name(const uint8_t id) {
  return selections[id].name();
}
