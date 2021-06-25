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


void Arduboy2Ext::resetFrameCount() {

    frameCount = 0;

}

uint8_t Arduboy2Ext::getFrameCount() const {

    return frameCount;

}

uint8_t Arduboy2Ext::getFrameCount(uint8_t mod) const {

    return frameCount % mod;

}

bool Arduboy2Ext::getFrameCountHalf(uint8_t mod) const {

	return getFrameCount(mod) >= (mod / 2);

}

bool Arduboy2Ext::isFrameCount(uint8_t mod) const {

    return (frameCount % mod) == 0;

}

bool Arduboy2Ext::isFrameCount(uint8_t mod, uint8_t val) const {

    return (frameCount % mod) == val;

}

uint8_t Arduboy2Ext::getLastFrameDuration() const {

    return this->lastFrameDurationMs;

}

void Arduboy2Ext::displayClearToPattern(uint8_t idx) {

    const uint8_t pattern[] = { 32, 2, 136, 34, 85, 170, 119, 221, 223, 253, 0, 0, 255, 255 };
    display();
    
    uint8_t even = pattern[(idx * 2)];
    uint8_t odd  = pattern[(idx * 2)+1];
    
    for (uint16_t x = 0; x < 5 * WIDTH; x += 2) {
        sBuffer[x + 0] = even;
        sBuffer[x + 1] = odd;
    }

}

/* ----------------------------------------------------------------------------
 *  Draw a horizontal dotted line. 
 */
void Arduboy2Ext::drawHorizontalDottedLine(uint8_t x1, uint8_t x2, uint8_t y, uint8_t colour) {

	uint8_t diff = (x2 - x1);

	for (uint8_t x = 0; x <= diff; x += 2) {

		drawPixel(x1 + x, y, colour);

    }

}


/* ----------------------------------------------------------------------------
 *  Draw a vertical dotted line. 
 */
void Arduboy2Ext::drawVerticalDottedLine(uint8_t y1, uint8_t y2, uint8_t x, uint8_t colour) {

	uint8_t diff = (y2 - y1);

	for (uint8_t y = 0; y <= diff; y += 2) {

		drawPixel(x, y1 + y, colour);

    }
  
}