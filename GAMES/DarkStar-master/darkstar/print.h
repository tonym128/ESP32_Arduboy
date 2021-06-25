#pragma once
#include "Arduboy2.h"
#include "gfx/font.h"

constexpr uint8_t CHR_ZERO   = 'Z' + 1;
constexpr uint8_t CHR_EX     = 'Z' + 11;
constexpr uint8_t CHR_BTN_A  = 'Z' + 12;
constexpr uint8_t CHR_BTN_B  = 'Z' + 13;
constexpr uint8_t CHR_SPACE  = 'Z' + 14;

#define AT_X(x)    ((x) + 128)
#define AT_Y(y)    ((y) + 1)
#define CHR_NUM(n) ((n) + CHR_ZERO)

uint8_t print_x;
uint8_t print_y;

void print_chr(uint8_t c)
{
  uint8_t x = print_x;
  uint8_t y = print_y;
  if (c < 'A') // Y positioning
    y = c - 1;
  else if (c == CHR_SPACE)
    x += 5;
  else if (c < 128) //printable character
  {
    Sprites::drawOverwrite(x, y, font_, c - 'A');
    if      (c == 'I') x += 5;
    else if (c == 'L' || c == 'T') x += 7;
    else    x += 9;
  }
  else // X positioning
    x = c - 128;
  print_x = x;
  print_y = y;
}

void print_num(uint8_t x, uint8_t y, uint16_t n, uint8_t digits)
{
  print_x = x;
  print_y = y;
  while (digits--)
  {
    print_chr(CHR_ZERO + (n % 10));
     n /= 10;
    print_x -= 2 * (fontWidth);
  }
}

void print_str(const uint8_t* ptr)
{
  while(true)
  {
    uint8_t c = pgm_read_byte(ptr++);
    if (c) print_chr(c);
    else break;
  }
}
