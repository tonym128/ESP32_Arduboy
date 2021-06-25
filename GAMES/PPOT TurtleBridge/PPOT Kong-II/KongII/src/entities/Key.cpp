#include "Key.h"

#include "../utils/Enums.h"

Key::Key() { }

uint8_t Key::getPosition() {

    return this->position;

}

int8_t Key::getXPosition() {

    if (this->position == 0) {

        switch (this->keyLocation) {

            case KeyLocation::LowerPosition:
                return 11;

            case KeyLocation::UpperPosition:
                return 12;

            case KeyLocation::Position_00:
                return 34;

            case KeyLocation::Position_01:
                return 53;

            case KeyLocation::Position_02:
                return 72;

            case KeyLocation::Position_03:
                return 91;

            default:
                return 0;

        }

    }
    else {

        return pgm_read_byte(&this->path[(this->position * 3)]);

    }

}

int8_t Key::getYPosition(uint8_t yOffset) {

    if (this->position == 0) {

        switch (this->keyLocation) {

            case KeyLocation::LowerPosition:
                return 90 - yOffset;

            case KeyLocation::UpperPosition:
                return 35 - yOffset;

            case KeyLocation::Position_00:
            case KeyLocation::Position_01:
            case KeyLocation::Position_02:
            case KeyLocation::Position_03:
                return 12 - yOffset;

            default:
                return 0;

        }

    }
    else {

        uint8_t y = pgm_read_byte(&this->path[(this->position * 3) + 1]);
        return y - yOffset;

    }

}

uint8_t Key::getImage() {

    if (this->position == 0) {

        switch (this->keyLocation) {

            case KeyLocation::LowerPosition:
            case KeyLocation::UpperPosition:
                return NO_IMAGE;

            case KeyLocation::Position_00:
            case KeyLocation::Position_01:
            case KeyLocation::Position_02:
            case KeyLocation::Position_03:
                return static_cast<uint8_t>(KeyImage::Position_04);

            default:    
                return 0;

        }

    }
    else {

        return pgm_read_byte(&this->path[(this->position * 3) + 2]);

    }

}

KeyLocation Key::getKeyLocation() {

    return this->keyLocation;

}

bool Key::isEnabled() {

    return this->enabled;

}

void Key::setEnabled(bool enabled) {

    this->enabled = enabled;
    this->position = 0;

}

void Key::setFlash(bool value) {

    this->flash = value;
    if (this->flash) this->counter = 96;

}

void Key::setPath(uint8_t const *path) {

    this->path = path;
    this->enabled = true;
    this->position = 0;
}

void Key::setKeyLocation(KeyLocation keyLocation) {

    this->keyLocation = keyLocation;

}

void Key::updatePosition() {

    if (this->counter > 0) {
        
        this->counter--;

        if (this->counter == 0) {

            this->flash = false;
            this->keyLocation = KeyLocation::LowerPosition;
            this->position = 0;
            return;

        }

    }

    if (!this->enabled) return;

    this->position++;

    uint8_t x = pgm_read_byte(&this->path[(this->position * 3)]);
    uint8_t y = pgm_read_byte(&this->path[(this->position * 3) + 1]);

    if (x == 0 && y == 0) {

        switch (this->keyLocation) {

            case KeyLocation::LowerPosition:  
                this->keyLocation = KeyLocation::UpperPosition;
                break;

            case KeyLocation::UpperPosition:  
                {
                  KeyLocation keyLocation = static_cast<KeyLocation>(pgm_read_byte(&this->path[(this->position * 3) + 2]));
                  this->keyLocation = keyLocation;
                }
                break;

            default: break;

        }

        this->enabled = false;
        this->position = 0;

    }

}

Rect Key::getRect(uint8_t yOffset) {

    int8_t x = pgm_read_byte(&this->path[(this->position * 3)]);
    uint8_t y = pgm_read_byte(&this->path[(this->position * 3) + 1]) - yOffset;

    return Rect{x, y, 8, 8 };

}


bool Key::getDisplay() {

    return !this->flash || ((this->counter / 16) % 2);

}