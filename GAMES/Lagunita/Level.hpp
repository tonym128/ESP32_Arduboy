#pragma once

#include "Bitmaps.hpp"
#include "Buildings.hpp"
#include "Events.hpp"
#include "Utility.hpp"

class Level {
 public:
  Level(Arduboy2Base& arduboy, Tinyfont& tinyfont, Drawing& drawing)
      : arduboy(arduboy), tinyfont(tinyfont), drawing(drawing) {}

  void init();

  bool isInProgress() { return inProgress; };
  bool gameOver() { return (earnings <= maintenance) && (money < 100); };
  void quit() { inProgress = false; }

  bool isPaused() { return inPause; };
  void pause() { inPause = true; };
  void resume() { inPause = false; };

  void update();
  void nextDay();
  void onInput(Input dir);
  void render();
  void renderBackground(uint16_t frame);

  void load(uint16_t start);
  void save(uint16_t start);

  uint16_t getPopulation() { return population; }

 private:
  Arduboy2Base& arduboy;
  Tinyfont& tinyfont;
  Drawing& drawing;

  /* For each building we need know if it's enabled and if has been built. */
  Building::Status buildings[static_cast<uint8_t>(Building::IDs::count)] = {};

  /* Camera position and scrolling status. */
  uint16_t camera = 0;
  uint8_t camera_off = 0;
  uint8_t camera_sign = 0;
  uint8_t camera_scrolls = 0;

  /* Number of buildable tiles in the level. Should be a power of two to avoid
   * expensive division operations. */
  static const uint16_t size = 512;

  /* Use a bit field to pack more fields in a byte. */
  struct {
    Building::IDs building : 5;
    uint8_t progress : 3;
  } tiles[size] = {};

  uint16_t river_in = 0;  /* X coordinate where the river goes into the lake */
  uint16_t river_out = 0; /* X coordinate where the river leaves the lake */

  static constexpr uint16_t fps = 32;
  static constexpr uint16_t dayLength = 256 * fps;
  uint16_t frame;
  uint16_t days;
  uint16_t housing;
  uint16_t jobs;
  uint16_t food;
  uint16_t earnings;
  uint16_t maintenance;
  uint16_t exports;

  uint8_t happiness;
  uint8_t safety;
  uint8_t spirituality;
  uint8_t environment;

  /* x coordinates for moving objects */
  static const uint8_t npc_count = 16;
  uint16_t flying[npc_count];
  uint16_t walking[npc_count];

  /* Currently selected object */
  Building::IDs currBuil = Building::IDs::house;

  uint16_t money = 999;
  Input lastPressed = Input::a;
  uint16_t long frameLastInput = 0;
  uint16_t long frameNextEvent = 0;

  EventState tutorials[Events::count] = {
      EventState::untriggered,
  };

  static const uint8_t tutorLen = 128; /* 5% of total RAM! */
  bool tutorVisible = false;
  uint8_t tutorChars = 0;
  char tutor[tutorLen] = {};

  uint16_t population = 0;
  bool inProgress = false;
  bool inPause = false;
  bool inStats = false;

  void findFirstAvailableSpot(int8_t dir);
  bool canBuild();
  Building::IDs buildCollides();
  Building::IDs cursorOverlaps(bool clear = false,
                               Building::IDs replaceWith = Building::IDs::empty);
  void setTileEmpty(uint16_t t) {
    tiles[t].building = Building::IDs::empty;
    tiles[t].progress = 0;
  }
};
