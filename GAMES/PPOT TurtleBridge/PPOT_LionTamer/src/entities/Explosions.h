#pragma once

#include <Arduboy2.h>
#include "../images/Images.h"
#include "../utils/Consts.h"
#include "Explosion.h"

struct Explosions {

    private:

        Explosion explosion[Constants::Particle_Count];
        int16_t x = 0;
        int16_t y = 0;

    public:

        int16_t getX()                              { return this->x; }
        int16_t getY()                              { return this->y; }

        void setX(int16_t val)                      { this->x = val; }
        void setY(int16_t val)                      { this->y = val; }


    public:

        void setExplosions(int16_t x, int16_t y) {

            this->x = x;
            this->y = y;

            for (uint8_t i = 0; i < Constants::Particle_Count; i++) {

                this->explosion[i].setX(x + (random(0, 14) - 7));
                this->explosion[i].setY(y + (random(0, 14) - 7));
                this->explosion[i].setCounter(-random(1, 32));
                this->explosion[i].setDirection(random(0, 9));
                
            }

        }

        void update(uint16_t frame) {

            for (uint8_t i = 0; i < Constants::Particle_Count; i++) {

                this->explosion[i].update(frame);
                
            }

        }

        Explosion & getExplosion(uint8_t index) {

            return this->explosion[index];

        }

        void reset() {
            
            for (uint8_t i = 0; i < Constants::Particle_Count; i++) {

                this->explosion[i].setCounter(-127);
                
            }

        }

};