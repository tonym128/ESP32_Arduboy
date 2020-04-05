#include "renderer.h"
#include "strings.h"

void Renderer::pushTranslate(int16_t newTranslateX, int16_t newTranslateY) {
    prevTranslateX = translateX;
    prevTranslateY = translateY;

    translateX = newTranslateX;
    translateY = newTranslateY;
}

void Renderer::popTranslate() {
    translateX = prevTranslateX;
    translateY = prevTranslateY;
}

void Renderer::fillRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t color) {
    arduboy.fillRect(x + translateX, y + translateY, w, h, color);
}

void Renderer::drawRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t color) {
    arduboy.drawRect(x + translateX, y + translateY, w, h, color);
}

void Renderer::drawOverwrite(int16_t x, int16_t y, const uint8_t* bitmap, uint8_t frame, MirrorMode mirror, DrawMode drawMode) {
    drawBitmap(x + translateX, y + translateY, bitmap, NULL, false, frame, mirror, drawMode);
}

void Renderer::drawPlusMask(int16_t x, int16_t y, const uint8_t* bitmap, uint8_t frame, MirrorMode mirror, DrawMode drawMode) {
    drawBitmap(x + translateX, y + translateY, bitmap, NULL, true, frame, mirror, drawMode);
}

void Renderer::drawString(int16_t x, int16_t y, const uint8_t* str) {
    int16_t ogx = x;
    uint8_t ch = pgm_read_byte(str++);

    while (ch != 0xFF) {
        if (ch == NEW_LINE) {
            y += 6;
            x = ogx;
        } else {
            const uint8_t frame = ch >> 1;
            const int8_t offset = ch & 1 ? -4 : 0; 
            const uint8_t* mask = offset ? font_tiles_lower_mask : font_tiles_upper_mask;

            drawBitmap(x + translateX, y + offset + translateY, font_tiles, mask, false, frame, 0, Invert, 0);

            x += 5;
        }

        ch = pgm_read_byte(str++);
    }
}

const uint8_t* const PROGMEM number_strings[] = {
    digit0_string,
    digit1_string,
    digit2_string,
    digit3_string,
    digit4_string,
    digit5_string,
    digit6_string,
    digit7_string,
    digit8_string,
    digit9_string
};

void Renderer::drawNumber(int16_t x, int16_t y, uint16_t number) {
    if (number == 0) {
        Renderer::drawString(x, y, digit0_string);
        return;
    }

    int8_t numDigits = 0;
    int16_t originalNumber = number;


    while (number > 0) {
        numDigits++;
        number /= 10;
    }

    // we are going to draw the least significant digit first. 
    // so move x into position, then march back left after each digit
    // it is numDigits - 1 because we need to get to the left side of the digit
    x += (numDigits - 1) * 5;

    number = originalNumber;
    while (number > 0) {
        int8_t digit = number % 10;
        number /= 10;

        Renderer::drawString(x, y, (const uint8_t*)pgm_read_ptr(number_strings + digit));
        x -= 5;
    }
}
