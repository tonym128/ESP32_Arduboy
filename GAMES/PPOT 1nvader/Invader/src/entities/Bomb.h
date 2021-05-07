#pragma once

#include "../utils/Constants.h"

struct Bomb {

    private:

        int8_t pos = 28;
        int8_t height = 28;
        uint8_t explodingCount = 0;
        bool active = false;
        bool exploding = false;

    public:

        int8_t getPos()                                     { return this->pos; }
        int8_t getHeight()                                  { return this->height; }
        bool getActive()                                    { return this->active; }
        bool getExploding()                                 { return this->exploding; }
        uint8_t getExplosionCounter()                       { return this->explodingCount; }

        void setPos(int8_t val)                             { this->pos = val; }
        void setHeight(int8_t val)                          { this->height = val; }
        void setExploding(uint8_t val)                      { this->exploding = val; }
        void setActive(bool val)                            { this->active = val; this->exploding = false; }

    public:

        void decHeight(GameRotation gameRotation) {

            switch (gameRotation) {

                case GameRotation::Portrait:
                    this->height--;
                    if (this->height < -7) {
                        this->active = false;
                    }
                    break;

                case GameRotation::Landscape:
                    this->height++;
                    if (this->height > 71) {
                        this->active = false;
                    }
                    break;

            }

            if (this->explodingCount > 0) {
                
                this->explodingCount--;

                if (this->explodingCount == 0) {
                    this->active = false;
                }

            }

        }

        void explode() {

            this->exploding = true;
            this->explodingCount = 4;

        }

};
