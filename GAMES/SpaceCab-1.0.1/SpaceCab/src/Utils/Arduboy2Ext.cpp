#include "Arduboy2Ext.h"

Arduboy2Ext::Arduboy2Ext() : Arduboy2Base() { }

uint8_t Arduboy2Ext::justPressedButtons() const {

  return (~previousButtonState & currentButtonState);

}

uint8_t Arduboy2Ext::pressedButtons() const {

  return currentButtonState;

}

void Arduboy2Ext::clearButtonState() {

  currentButtonState = previousButtonState = 0;

}

uint16_t Arduboy2Ext::getFrameCount() const {

  return frameCount;

}

uint8_t Arduboy2Ext::getFrameCount(uint8_t mod) const {

  return frameCount % mod;

}


/* ----------------------------------------------------------------------------
 *  Draw a horizontal dotted line. 
 */
void Arduboy2Ext::drawHorizontalDottedLine(uint8_t x1, uint8_t x2, uint8_t y, uint8_t spacing) {

  for (uint8_t x3 = x1; x3 <= x2; x3+=spacing) {
    drawPixel(x3, y, WHITE);
  }
  
}


/* ----------------------------------------------------------------------------
 *  Draw a vertical dotted line. 
 */
void Arduboy2Ext::drawVerticalDottedLine(uint8_t y1, uint8_t y2, uint8_t x, uint8_t spacing) {

  for (uint8_t y3 = y1; y3 <= y2; y3+=spacing) {
    drawPixel(x, y3, WHITE);
  }
  
}