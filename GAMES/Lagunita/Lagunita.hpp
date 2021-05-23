#pragma once

#include "Level.hpp"
#include "Music.hpp"
#include "Utility.hpp"

class Lagunita {
 public:
  void init();
  void loop();

  static const uint16_t frame_rate = 32;

 private:
  void update();
  void render();

  bool canSave();
  bool canLoad();

  void save();
  void load();

 private:
  void updateHighScore();

  enum class Menus : uint8_t { main, game, credits, help, lost, exit, saved };
  enum class MainSelections : uint8_t { play, load, save, credits, help, audio };
  uint8_t help_page = 0;
  int16_t weed = 0;
  uint16_t high_score = 0;

  Arduboy2Base arduboy;
  Drawing drawing = {arduboy.sBuffer};
  Tinyfont tinyfont = {arduboy.sBuffer, Arduboy2Base::width(), Arduboy2Base::height()};
  Music music = {arduboy};
  Level level = {arduboy, tinyfont, drawing};
  Menus currentMenu = Menus::main;
  MainSelections currentMainSelection = MainSelections::play;
};
