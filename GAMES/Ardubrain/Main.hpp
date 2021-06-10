#pragma once

#include <Arduboy2.h>
#include "Test.hpp"
#include "Tinyfont.hpp"

class Main {
 public:
  void init();
  void loop();

  static const uint16_t frame_rate = 32;

 private:
  void update();
  void render();
  void loadHighScore();
  void saveHighScore();

 public:
  enum class Menus : uint8_t { main, test, credits };
  enum class MainSelections : uint8_t {
    reflex,
    planning,
    focus,
    memory,
    verbal,
    major,
    associative,
    maze,
    series,
    arrows,
    count
  };

  static const uint8_t selectionsCount = static_cast<uint8_t>(MainSelections::count);

  class Selections {
   private:
    Selections() = default;
    const __FlashStringHelper* name() const;

   public:
    static const __FlashStringHelper* name(const uint8_t id);
    static const __FlashStringHelper* name(MainSelections id) {
      return name(static_cast<uint8_t>(id));
    }

   public:
    const char _name[10];
  };

 private:
  uint16_t hiScores[selectionsCount] = {};

  Test* test = nullptr;

  Arduboy2Base arduboy;
  Tinyfont tinyfont = {arduboy.sBuffer, Arduboy2Base::width(), Arduboy2Base::height()};
  Menus currentMenu = Menus::main;
  MainSelections currentMainSelection = MainSelections::reflex;
};
