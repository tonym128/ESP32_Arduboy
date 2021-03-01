#ifndef DrawHelper_h
#define DrawHelper_h

/*
   this function is needed to print the images
*/
void drawOffsetBitmap(int16_t x, int16_t y, const uint8_t *image, int16_t iW,
                      int16_t w, int16_t h, int16_t oX, uint8_t flip, uint16_t color) {

  // this would cause the for loop to count forever
  if (w + oX < oX)
    return;
                      
  int16_t xi, yi, byteWidth = (iW + 7) / 8;
  for (yi = 0; yi < h; yi++) {
    for (xi = oX; xi < w + oX; xi++ ) {
      if (pgm_read_byte(image + yi * byteWidth + xi / 8) & (128 >> (xi & 7)))
        continue;

      // flip == 0 no Flip
      // flip == 1 horizontal Flip
      // flip == 2 vertical Flip
      // flip == 3 vertical and horizontal Flip

      /* NOTE: calculating the values before checking the flip is not fastest way
      int16_t xPos = x + xi - oX;
      int16_t yPos = y + yi;

      if (flip == 1 || flip == 3 )
        xPos = x + w - xi + oX - 1;

      if (flip == 2 || flip == 3  )
        yPos = y + h - yi - 1;

      // I hope this is not necassary anymore
      if (xPos < 0 || xPos > 128 - 1 || yPos < 0 || yPos > 64 - 1)
        continue;
      */
      
      int16_t xPos, yPos;
      
      // calc xPos
      if (flip == 1 || flip == 3) {
        xPos = x + w - xi + oX - 1;
      } else {
        xPos = x + xi - oX;        
      } 

      // calc yPos
      if (flip == 2 || flip == 3) {
        yPos = y + h - yi - 1;        
      } else {
        yPos = y + yi;
      } 

      arduboy.drawPixel(xPos, yPos, color);
    }
  }
};

/*
   this function draws only every second pixel
*/
void fillRectChess(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color) {
  if (w + x > 128)
    return;

  if (h + y > 64)
    return;

  for (uint8_t i = x; i < w + x; i++) {
    for (uint8_t j = y; j < h + y; j++) {

      if (i >= 128)
        continue;

      if (j >= 64)
        continue;

      if ((i % 2 && j % 2) || (!(i % 2) && !(j % 2)))
        arduboy.drawPixel(i, j, color);
    }
  }
}

/*
   this function is to print the 3 special stages
*/

void drawSpecialStatus(uint8_t xPos, uint8_t yPos, uint8_t s, uint8_t color) {

  uint8_t g1 = uint8_t(s > 1);

  arduboy.drawRoundRect(xPos + 4 - s, yPos + 2 - g1, 2 * s, 2 + g1 * 2, g1, color);
}

/*
   functions to Print numbers without the arduboy print functionality
*/

// 'menuNumbers', 40x6px
const unsigned char menuNumbers [] PROGMEM = {
  0x9d, 0x91, 0x70, 0x90, 0x99, 0x69, 0x6e, 0x57, 0x7e, 0x66, 0x6d, 0xd9, 0x51, 0x0d, 0x96, 0x6d,
  0xbe, 0x0e, 0x6b, 0x68, 0x6d, 0x7e, 0xd6, 0x6b, 0x6e, 0x98, 0x01, 0xd9, 0x9b, 0x99
};

void drawNumber6x4(int8_t x, int8_t y, uint8_t v) {
  drawOffsetBitmap(x, y, menuNumbers, 40, 4, 6, v * 4, false, WHITE);
}

uint8_t drawNumbers(int8_t x, int8_t y, int16_t v) {
  uint8_t stellen;
  if (v < 10) {
    stellen = 1;
  } else if (v < 100) {
    stellen = 2;
  } else if (v < 1000) {
    stellen = 3;
  } else if (v < 10000) {
    stellen = 4;
  } else {
    stellen = 5;
  }

  uint8_t xOffset = x + (stellen - 1) * 5;

  for (uint8_t i = 0; i < stellen; i++) {

    drawNumber6x4(xOffset, y, v % 10);

    v = v / 10;
    xOffset -= 5;
  }

  return stellen;
}

/*
  little helper for simple fire animtations
*/

void drawSmallFire(uint8_t xPos, uint8_t yPos, bool burnsToRight) {
  // draw a fire only every third frame
  if (gameFrames % 3)
    return;

  arduboy.drawPixel(xPos, yPos + 1, BLACK);
  arduboy.drawPixel(xPos, yPos - 1, BLACK);

  arduboy.fillRect(xPos - 2 + uint8_t(burnsToRight) * 3, yPos, 2, 1, BLACK);
}

#endif
