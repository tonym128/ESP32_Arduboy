#pragma once

struct Vec3 {

    private: 

        int16_t x; // horizontal
        int16_t y; // vertical
        int16_t z; // forward/backward

    public:

        Vec3() {}

        Vec3(int16_t x, int16_t y, int16_t z) {

            this->x = x;
            this->y = y;
            this->z = z;
            
        }

        int16_t getX()                      { return this->x; }
        int16_t getY()                      { return this->y; }
        int16_t getZ()                      { return this->z; }

        void setX(int16_t val)              { this->x = val; }    
        void setY(int16_t val)              { this->y = val; }    
        void setZ(int16_t val)              { this->z = val; }    

        Vec3 operator-(Vec3 another) {

            Vec3 result;

            result.x = this->x - another.getX();
            result.y = this->y - another.getY();
            result.z = this->z - another.getZ();

            return result;

        }

        Vec3 clone() {

            Vec3 result;

            result.x = this->x;
            result.y = this->y;
            result.z = this->z;

            return result;

        }

        void decZ() {

            this->z = this->z - Constants::ZReduction;
            
        }

};