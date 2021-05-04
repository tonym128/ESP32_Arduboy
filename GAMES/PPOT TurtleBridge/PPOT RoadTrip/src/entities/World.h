#pragma once

#define ROAD_SEGMENTS 16
#define DRAW_DISTANCE 10
#include <stdint.h>

struct World {

    private:

        VecMovement vecHeight = { MovementType::Vertical, Direction::None, static_cast<uint8_t>(random(5, 20)), 0 };
        VecMovement vecTurn = { MovementType::Horizontal, Direction::None, static_cast<uint8_t>(random(5, 20)), 0 };

        Vec3 roadSegments[ROAD_SEGMENTS] = {

            {0, 0, 0 * UPM},
            {0, 0, 1 * UPM},
            {0, 0, 2 * UPM},
            {0, 0, 3 * UPM},
            {0, 0, 4 * UPM},
            {0, 0, 5 * UPM},
            {0, 0, 6 * UPM},
            {0, 0, 7 * UPM},
            {0, 0, 8 * UPM},
            {0, 0, 9 * UPM},
            {0, 0, 10 * UPM},
            {0, 0, 11 * UPM},
            {0, 0, 12 * UPM},
            {0, 0, 13 * UPM},
            {0, 0, 14 * UPM},
            {0, 0, 15 * UPM},

        };

    public:

        void print(uint8_t index) {

            for (uint8_t i = 0; i < ROAD_SEGMENTS; i++) {

                if (i == index) Serial.print("*");

                Serial.print(roadSegments[i].getX());
                Serial.print(",");
                Serial.print(roadSegments[i].getY());
                Serial.print(",");
                Serial.print(roadSegments[i].getZ());
                Serial.print(" ");
                
            }

            Serial.println("");

        }
        

        void decZ() {

            for (uint8_t i = 0; i < ROAD_SEGMENTS; i++) {

                roadSegments[i].setZ(roadSegments[i].getZ() - Constants::ZReduction);
                
            }

        }

        void addRoadSegment(uint8_t index, bool noCurves) {

            uint8_t modOld = index % ROAD_SEGMENTS;
            uint8_t modNew = (index + 1) % ROAD_SEGMENTS;

            Vec3 &segmentOld = roadSegments[modOld];
            Vec3 segmentNew = segmentOld.clone();

            segmentNew.setZ(segmentOld.getZ() + UPM);

            #ifndef DEBUG_LEVEL_ROAD
            segmentNew.setY(vecHeight.getMovement());
            #endif

            #ifndef DEBUG_STRAIGHT_ROAD
            if (!noCurves) {
                segmentNew.setX(vecTurn.getMovement());
            }
            #endif

            vecHeight.update();
            vecTurn.update();

            roadSegments[modNew] = segmentNew;

        }

        Vec3 getRoadSegment(uint16_t index) {
        
            Vec3 result;
            uint8_t mod = index % ROAD_SEGMENTS;

            result = roadSegments[mod];

            return result;

        }

        Vec3 getRoadSegment(int16_t zMin, int16_t zMax) {

            Vec3 result;

            for (uint8_t i = 0; i < ROAD_SEGMENTS; i++) {

                if (roadSegments[i].getZ() > zMin && roadSegments[i].getZ() <= zMax) {

                    result = roadSegments[i];
                    break;

                }
                
            }

            return result;

        }

        int16_t roadHeightAt(int16_t roadPosition) {

            uint16_t index = roadPosition / UPM;
            int16_t fract = roadPosition % UPM;

            int16_t h1 = getRoadSegment(index).getY();
            int16_t h2 = getRoadSegment(index + 1).getY();

            return h1 + ((h2 - h1) * fract) / UPM;

        }

        // Returns: x, y = screen position, z = screen scale
        Vec3 perspective(Vec3 worldPosition, Vec3 cameraPosition) {

            Vec3 result;
            Vec3 relativeWorldPos = worldPosition - cameraPosition;

            // prevent division by zero
            if (relativeWorldPos.getZ() == 0) relativeWorldPos.setZ(1);

            result.setX(DISPLAY_WIDTH / 2 + (relativeWorldPos.getX() * UPM) / relativeWorldPos.getZ());
            result.setY(DISPLAY_HEIGHT / 2 - (relativeWorldPos.getY() * UPM) / relativeWorldPos.getZ());
            result.setZ((64 * DISPLAY_WIDTH) / relativeWorldPos.getZ());

            return result;

        }

        void reset() {

            for (uint8_t i = 0; i < ROAD_SEGMENTS; i++) {

                roadSegments[i].setX(0);
                roadSegments[i].setY(0);
                roadSegments[i].setZ(i * UPM);
                
            }

            this->vecHeight.setDirection(Direction::None);
            this->vecHeight.setCount(static_cast<uint8_t>(random(5, 20)));

            this->vecTurn.setDirection(Direction::None);
            this->vecTurn.setCount(static_cast<uint8_t>(random(5, 20)));

        }

};