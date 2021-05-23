#pragma once

/* List of headers included by all the other headers. */
#include <Arduboy2.h>
//#include <avr/io.h>
#include <avr/pgmspace.h>
#include "Tinyfont.hpp"

extern "C" {
#include <stdint.h>
}

/* Enum class for the 6 input buttons. */
enum class Input : uint8_t { left, right, up, down, a, b };

/* A class with utilities for special drawing. */
class Drawing {
 public:
  Drawing(uint8_t* buff) : sBuffer(buff){};

  /* Draw a bitmap. */
  void drawBitmap(int16_t x,
                  int16_t y,
                  const uint8_t* bitmap,
                  uint8_t w,
                  uint8_t h,
                  bool mirrored = false) {
    if (mirrored) {
      drawBitmapMirrored(x, y, bitmap, w, h);
    } else {
      drawBitmapPlain(x, y, bitmap, w, h);
    }
  }

  /* Draw a plain bitmap. */
  void drawBitmapPlain(int16_t x, int16_t y, const uint8_t* bitmap, uint8_t w, uint8_t h);

  /* Draw a mirrored bitmap. */
  void drawBitmapMirrored(int16_t x, int16_t y, const uint8_t* bitmap, uint8_t w, uint8_t h);

  /* Draw a bitmap that is transparent from the top
     down to the first white pixel.
   */
  void drawBitmapAlpha(int16_t x,
                       int16_t y,
                       const uint8_t* bitmap,
                       uint8_t w,
                       uint8_t h,
                       uint8_t skipsCount = 0);

  /* Lake reflection effect. */
  void waterReflection(uint8_t frame, uint16_t camera = 0);

  /* Set a vertical bar to exclude from drawing. */
  void excludeVerticalBar(uint8_t l, uint8_t r) {
    ex_l = l;
    ex_r = r;
  }

  /* Remove any exclusion. */
  void excludeNothing() {
    ex_l = 0xff;
    ex_r = 0xff;
  }

 private:
  bool outOfBounds(int16_t x, int16_t y, uint8_t w, uint8_t h) {
    if (x + w < 0 || x > WIDTH - 1 || y + h < 0 || y > HEIGHT - 1) {
      return true;
    }

    if (x >= ex_l && (x + w) < ex_r) {
      return true;
    }
    return false;
  }

  uint8_t* sBuffer;
  uint8_t ex_l = 0xff;
  uint8_t ex_r = 0xff;

  static const uint8_t waves_cnt = 16;
  int16_t waves_x[waves_cnt] = {}; /* global x for the waves */
  uint8_t waves_y[waves_cnt] = {}; /* local y for the waves  */
};
