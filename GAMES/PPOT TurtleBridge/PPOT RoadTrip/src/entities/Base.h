#pragma once

#include "Vec3.h"
 #include <FixedPointsCommon.h>

struct Base : Vec3 {

    private:

        UQ16x16 speed = 0;


    public:

        UQ16x16 getSpeed()                    { return this->speed; }    

        void setSpeed(UQ16x16 val)            { this->speed = val; }    

        uint8_t getSpeed_Display() {

            return static_cast<uint8_t>((this->speed / Constants::SpeedDiv).getInteger());

        }

        void printPos() {

            Serial.print("{");
            Serial.print(this->getX());
            Serial.print(",");
            Serial.print(this->getY());
            Serial.print(",");
            Serial.print(this->getZ());
            Serial.print("}");

        }

};