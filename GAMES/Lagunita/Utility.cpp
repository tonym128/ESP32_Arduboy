#include "Utility.hpp"

/* This code comes from the arduboy2 library. */
void Drawing::drawBitmapPlain(int16_t x,
                              int16_t y,
                              const uint8_t* bitmap,
                              uint8_t w,
                              uint8_t h) {
  if (outOfBounds(x, y, w, h)) {
    return;
  }

  int yOffset = abs(y) % 8;
  int sRow = y / 8;
  if (y < 0) {
    sRow--;
    yOffset = 8 - yOffset;
  }
  const int rows = (h % 8 != 0) ? (h / 8) + 1 : (h / 8);

  for (int a = 0; a < rows; a++) {
    int bRow = sRow + a;
    if (bRow > (HEIGHT / 8) - 1) {
      break;
    }
    if (bRow > -2) {
      for (int iCol = 0; iCol < w; iCol++) {
        if (iCol + x > (WIDTH - 1)) {
          break;
        }
        if (iCol + x >= 0) {
          if (bRow >= 0) {
            sBuffer[(bRow * WIDTH) + x + iCol] |= pgm_read_byte(bitmap + (a * w) + iCol)
                                                  << yOffset;
          }
          if (yOffset && bRow < (HEIGHT / 8) - 1 && bRow > -2) {
            sBuffer[((bRow + 1) * WIDTH) + x + iCol] |=
                pgm_read_byte(bitmap + (a * w) + iCol) >> (8 - yOffset);
          }
        }
      }
    }
  }
}

void Drawing::drawBitmapMirrored(int16_t x,
                                 int16_t y,
                                 const uint8_t* bitmap,
                                 uint8_t w,
                                 uint8_t h) {
  if (outOfBounds(x, y, w, h)) {
    return;
  }

  int yOffset = abs(y) % 8;
  int sRow = y / 8;
  if (y < 0) {
    sRow--;
    yOffset = 8 - yOffset;
  }
  const int rows = (h % 8 != 0) ? (h / 8) + 1 : (h / 8);

  for (int a = 0; a < rows; a++) {
    int bRow = sRow + a;
    if (bRow > (HEIGHT / 8) - 1) {
      break;
    }
    if (bRow > -2) {
      for (int iCol = 0; iCol < w; iCol++) {
        if (iCol + x > (WIDTH - 1)) {
          break;
        }
        if (iCol + x >= 0) {
          if (bRow >= 0) {
            /* Reverse the column read in the source image. */
            sBuffer[(bRow * WIDTH) + x + iCol] |= pgm_read_byte(bitmap + (a * w) + w - 1 - iCol)
                                                  << yOffset;
          }
          if (yOffset && bRow < (HEIGHT / 8) - 1 && bRow > -2) {
            /* Reverse the column read in the source image. */
            sBuffer[((bRow + 1) * WIDTH) + x + iCol] |=
                pgm_read_byte(bitmap + (a * w) + w - 1 - iCol) >> (8 - yOffset);
          }
        }
      }
    }
  }
}

void Drawing::drawBitmapAlpha(int16_t x,
                              int16_t y,
                              const uint8_t* bitmap,
                              uint8_t w,
                              uint8_t h,
                              uint8_t skipsCount) {
  if (outOfBounds(x, y, w, h)) {
    return;
  }

  int yOffset = abs(y) % 8;
  int sRow = y / 8;
  if (y < 0) {
    sRow--;
    yOffset = 8 - yOffset;
  }
  const int rows = (h % 8 != 0) ? (h / 8) + 1 : (h / 8);

  /* This booleans array track if we found the first white
     pixel on top of the bitmap.
   */
  bool bits[w];/* = {
      false,
  };*/
  memset (bits,0,sizeof(bits));

  for (int a = 0; a < rows; a++) {
    int bRow = sRow + a;
    if (bRow > (HEIGHT / 8) - 1) {
      break;
    }
    if (bRow > -2) {
      for (int iCol = 0; iCol < w; iCol++) {
        if (iCol + x > (WIDTH - 1)) {
          break;
        }
        uint8_t skips = a * 8;
        if (iCol + x >= 0) {
          if (bRow >= 0) {
            uint8_t src_byte = pgm_read_byte(bitmap + (a * w) + iCol) << yOffset;
            if (bits[iCol] && 0 == skipsCount) {
              /* full overwrite of the background */
              sBuffer[(bRow * WIDTH) + x + iCol] &= ~(0xff << yOffset);
              sBuffer[(bRow * WIDTH) + x + iCol] |= src_byte;
            } else {
              /* proceed bit by bit vertically */
              for (uint8_t b = 0; b < (8 - yOffset); b++) {
                skips++;
                if (skips < skipsCount) {
                  continue;
                }
                uint8_t bitmask = (0x1 << (b + yOffset));
                uint8_t bitmap_bit = src_byte & bitmask;
                /* keep or delete the destination's pixel */
                sBuffer[(bRow * WIDTH) + x + iCol] &= bits[iCol] ? ~bitmask : 0xff;
                /* update the bits array */
                bits[iCol] = bits[iCol] || (bitmap_bit != 0);
                /* copy source bit to destination */
                sBuffer[(bRow * WIDTH) + x + iCol] |= bitmap_bit;
              }
            }
          }
          if (yOffset && bRow < (HEIGHT / 8) - 1 && bRow > -2) {
            uint8_t src_byte = pgm_read_byte(bitmap + (a * w) + iCol) >> (8 - yOffset);
            if (bits[iCol] && 0 == skipsCount) {
              /* full overwrite of the background */
              sBuffer[((bRow + 1) * WIDTH) + x + iCol] &= ~(0xff >> (8 - yOffset));
              sBuffer[((bRow + 1) * WIDTH) + x + iCol] |= src_byte;
            } else {
              /* proceed bit by bit vertically */
              for (uint8_t b = 0; b < yOffset; b++) {
                skips++;
                if (skips < skipsCount) {
                  continue;
                }
                uint8_t bitmask = (0x1 << b);
                /* keep or delete the destination's pixel */
                uint8_t bitmap_bit = src_byte & bitmask;
                sBuffer[((bRow + 1) * WIDTH) + x + iCol] &= bits[iCol] ? ~bitmask : 0xff;
                /* update the bits array */
                bits[iCol] = bits[iCol] || (bitmap_bit != 0);
                /* copy source bit to destination */
                sBuffer[((bRow + 1) * WIDTH) + x + iCol] |= bitmap_bit;
              }
            }
          }
        }
      }
    }
  }
}

/* Lake reflection effect. */
void Drawing::waterReflection(uint8_t frame, uint16_t camera) {
  for (uint8_t w = 0; w < waves_cnt; w++) {
    if ((frame % 32) == (w * 4)) {
      waves_x[w] = (rand() & 0x7f) + camera;
      waves_y[w] = rand() & 0x0f;
    }
  }

  for (uint8_t w = 0; w < waves_cnt; w++) {
    if ((frame % 8) == 0) {
      waves_y[w] -= waves_y[w] > 0 ? 1 : 0;
    }
  }

  uint8_t frame2 = frame / 2;

  /* Do the bottom 16 rows of pixels. */
  for (uint16_t y = 0; y < 2; y++) {
    /* Read all 128 columns. */
    for (uint16_t x = 0; x < WIDTH; x++) {
      if (x >= ex_l && x < ex_r) {
        continue;
      }
      /* Init destination color as black. */
      uint8_t dst_col = 0x00;

      /* Read 3 columns of 8 pixels each. */
      uint8_t mirror[3] = {sBuffer[(5 - y) * WIDTH + x - (x == 0 ? 0 : 1)],
                           sBuffer[(5 - y) * WIDTH + x],
                           sBuffer[(5 - y) * WIDTH + x + (x == (WIDTH - 1) ? 0 : 1)]};

      /* Shuffle the 3 columns and OR them.*/
      uint8_t src_col = 0x00;

      src_col = mirror[((frame2 >> 3) + 0) % 3];
      dst_col |= (src_col & 0x80) >> 7;
      src_col = mirror[((frame2 >> 3) + 1) % 3];
      dst_col |= (src_col & 0x40) >> 5;
      src_col = mirror[((frame2 >> 3) + 2) % 3];
      dst_col |= (src_col & 0x20) >> 3;
      src_col = mirror[((frame2 >> 3) + 1) % 3];
      dst_col |= (src_col & 0x10) >> 1;
      src_col = mirror[((frame2 >> 3) + 0) % 3];
      dst_col |= (src_col & 0x08) << 1;
      src_col = mirror[((frame2 >> 3) + 1) % 3];
      dst_col |= (src_col & 0x04) << 3;
      src_col = mirror[((frame2 >> 3) + 2) % 3];
      dst_col |= (src_col & 0x02) << 5;
      src_col = mirror[((frame2 >> 3) + 1) % 3];
      dst_col |= (src_col & 0x01) << 7;

      /* Horizontal like at the top. */
      if (!y) {
        dst_col |= 0x01;
      }

      for (uint8_t w = 0; w < waves_cnt; w++) {
        uint16_t x_off = waves_x[w] - camera;
        if ((x > x_off && x < x_off + 3) && (y == static_cast<uint16_t>(waves_y[w] >> 3))) {
          dst_col |= 0x1 << (waves_y[w] & 0b111);
        }
      }

      /* Write to the frame buffer. */
      sBuffer[(6 + y) * WIDTH + x] = dst_col;
    }
  }
}
