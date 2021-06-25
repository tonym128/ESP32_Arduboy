#pragma once

#include <Arduboy2.h>

BeepPin1 beep1;
BeepPin2 beep2;

// Gamebuino LCD size
#define LCDWIDTH 84
#define LCDHEIGHT 48

// for Gamebuino extended bitmap function
#define NOROT 0
#define ROTCCW 1
#define ROT180 2
#define ROTCW 3
#define NOFLIP 0
#define FLIPH 1
#define FLIPV 2
#define FLIPVH 3

#define TONES_END 0x8000 // Frequency value for sequence termination. (No duration follows)

static uint16_t *tonesStart1;
static uint16_t *tonesIndex1;
static uint16_t *tonesStart2;
static uint16_t *tonesIndex2;

class Arduboy2Gamebuino : public Arduboy2
{
public:

  uint16_t duration1;
  bool tonesPlaying1 = false;
  uint16_t toneSequence1[3];

  void tone1(const uint16_t *tones)
  {
    uint16_t freq;

    tonesStart1 = tonesIndex1 = (uint16_t *)tones; // set to start of sequence array

    freq = pgm_read_word(tonesIndex1++); // get tone frequency
    duration1 = pgm_read_word(tonesIndex1++) / 50; // get tone duration

    beep1.tone(beep1.freq(freq));

    tonesPlaying1 = true;
  }

  void timer1()
  {
    uint16_t freq;

    if (duration1 == 0)
    {
      if (tonesPlaying1)
      {
        freq = pgm_read_word(tonesIndex1++); // get tone frequency

        if (freq == TONES_END)
        {
          tonesPlaying1 = false; // if freq is actually an "end of sequence" marker
        }
        else
        {
          duration1 = pgm_read_word(tonesIndex1++) / 50; // get tone duration

          if (freq == NOTE_REST)
          {
            beep1.noTone();
          }
          else
          {
            beep1.tone(beep1.freq(freq));
          }
        }
      }
    }
    else if (--duration1 == 0)
    {
      beep1.noTone();
    }
  }

  uint16_t duration2;
  bool tonesPlaying2 = false;
  uint16_t toneSequence2[3];

  void tone2(const uint16_t *tones)
  {
    uint16_t freq;

    tonesStart2 = tonesIndex2 = (uint16_t *)tones; // set to start of sequence array

    freq = pgm_read_word(tonesIndex2++); // get tone frequency
    duration2 = pgm_read_word(tonesIndex2++) / 50; // get tone duration

    beep2.tone(beep2.freq(freq));

    tonesPlaying2 = true;
  }

  void timer2()
  {
    uint16_t freq;

    if (duration2 == 0)
    {
      if (tonesPlaying2)
      {
        freq = pgm_read_word(tonesIndex2++); // get tone frequency

        if (freq == TONES_END)
        {
          tonesPlaying2 = false; // if freq is actually an "end of sequence" marker
        }
        else
        {
          duration2 = pgm_read_word(tonesIndex2++) / 50; // get tone duration

          if (freq == NOTE_REST)
          {
            beep2.noTone();
          }
          else
          {
            beep2.tone(beep2.freq(freq));
          }
        }
      }
    }
    else if (--duration2 == 0)
    {
      beep2.noTone();
    }
  }

  // Bring the cursor variables into public view
  using Arduboy2::cursor_x;
  using Arduboy2::cursor_y;

  // Prevent the inherited 'drawBitmap' functions from being hidden
  using Arduboy2::drawBitmap;

  static bool getBitmapPixel(const uint8_t* bitmap, uint8_t x, uint8_t y)
  {
    return pgm_read_byte(bitmap + 2 + y * ((pgm_read_byte(bitmap) + 7) / 8) + (x >> 3)) & (B10000000 >> (x % 8));
  }

  static bool collideRectRect(uint16_t x1, uint16_t y1, uint16_t w1, uint16_t h1 ,uint16_t x2 ,uint16_t y2, uint16_t w2, uint16_t h2){
      return !( x2     >=  x1+w1  ||
                x2+w2  <=  x1     ||
                y2     >=  y1+h1  ||
                y2+h2  <=  y1     );
  }

  void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap)
  {
    int8_t w = pgm_read_byte(bitmap);
    int8_t h = pgm_read_byte(bitmap + 1);

    bitmap = bitmap + 2;


    uint8_t * buffer  = getBuffer();
    const uint8_t col = textColor;
    const uint8_t bw  = (w + 7) / 8;

    // clip
    if (x >= WIDTH)  return;
    if (y >= HEIGHT) return;
    if (x + w <= 0)     return;
    if (y + h <= 0)     return;

    if (y < 0)
    {
      h += y;
      bitmap -= bw * y;
      y = 0;
    }

    if (y + h > HEIGHT)
    {
      h = HEIGHT - y;
    }

    uint8_t x1 = maxVal(0, x);
    uint8_t x2 = minVal(WIDTH, x + w);

    // draw
    uint8_t first_bitmap_mask = 0x80 >> ((x1 - x) & 7);
    const uint8_t * bitmap_line = bitmap + (x1 - x) / 8;
    uint8_t screen_mask = 0x01 << (y % 8);
    uint8_t * screen_row = buffer + (y / 8) * WIDTH + x1;

    for (uint8_t dy = 0; dy < h; dy++, bitmap_line += bw)
    {
      const uint8_t * bitmap_ptr = bitmap_line;
      uint8_t bitmap_mask = first_bitmap_mask;
      uint8_t pixels = pgm_read_byte(bitmap_ptr);
      uint8_t * dst = screen_row;

      if (col == BLACK)
      {
        for (uint8_t sx = x1; sx < x2; sx++, dst++)
        {
          if (pixels & bitmap_mask)
          {
            *dst |= screen_mask;
          }
          bitmap_mask >>= 1;

          if (!bitmap_mask)
          {
            bitmap_mask = 0x80;
            pixels = pgm_read_byte(++bitmap_ptr);
          }
        }
      }
      else if (col == WHITE)
      {
        uint8_t inv_screen_mask = ~screen_mask;

        for (uint8_t sx = x1; sx < x2; sx++, dst++)
        {
          if (pixels & bitmap_mask)
          {
            *dst &= inv_screen_mask;
          }
          bitmap_mask >>= 1;

          if (!bitmap_mask)
          {
            bitmap_mask = 0x80;
            pixels = pgm_read_byte(++bitmap_ptr);
          }
        }
      }
      else // invert
      {
        for (uint8_t sx = x1; sx < x2; sx++, dst++)
        {
          if (pixels & bitmap_mask)
          {
            *dst ^= screen_mask;
          }
          bitmap_mask >>= 1;

          if (!bitmap_mask)
          {
            bitmap_mask = 0x80;
            pixels = pgm_read_byte(++bitmap_ptr);
          }
        }
      }

      screen_mask <<= 1;

      if (!screen_mask)
      {
        screen_mask = 1;
        screen_row += WIDTH;
      }
    }
  }

  void drawBitmapF(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t rotation, uint8_t flip)
  {
    if ((rotation == NOROT) && (flip == NOFLIP))
    {
      // use the faster algorithm
      drawBitmap(x, y, bitmap);
      return;
    }

    uint8_t w = pgm_read_byte(bitmap);
    uint8_t h = pgm_read_byte(bitmap + 1);

    bitmap = bitmap + 2;


    int8_t i, j; //coordinates in the raw bitmap
    int8_t k, l; //coordinates in the rotated/flipped bitmap
    int8_t byteNum, bitNum, byteWidth = (w + 7) >> 3;

    rotation %= 4;

    for (i = 0; i < w; i++)
    {
      byteNum = i / 8;
      bitNum  = i % 8;

      for (j = 0; j < h; j++)
      {
        if (pgm_read_byte(bitmap + j * byteWidth + byteNum) & (B10000000 >> bitNum))
        {
          switch (rotation)
          {
            // no rotation
            case NOROT:
              k = i;
              l = j;
              break;

            // 90 counter-clockwise
            case ROTCCW:
              k = j;
              l = w - i - 1;
              break;

            // 180
            case ROT180:
              k = w - i - 1;
              l = h - j - 1;
              break;

            // 90 clockwise
            case ROTCW:
              k = h - j - 1;
              l = i;
              break;
          }

          if (flip)
          {
            flip %= 4;

            // horizontal flip
            if (flip & B00000001)
            {
              k = w - k - 1;
            }

            // vertical flip
            if (flip & B00000010)
            {
              l = h - l;
            }
          }

          // place the bitmap on the screen
          k += x;
          l += y;
          if (textColor == BLACK){
            drawPixel(k, l);
          }
          else if (textColor == WHITE){
            drawPixel(k, l, BLACK);
          }
        }
      }
    }
  }
};
