#pragma once

#include <stdint.h>
#include "Constants.h"

struct TitleVars {

    uint8_t cursor = 0;
    uint8_t offset = 0;
    bool flyIn = true;
    TitleState titleState = TitleState::StartGame;

    void reset() {

        this->offset = 0;
        this->flyIn = true;
        this->titleState = TitleState::StartGame;

    }

};

struct HighScoreVars {

    uint8_t alternate = 0;

};

struct GameOverVars {

    uint8_t offset = 0;
    uint8_t finalSpeed = 0;

    void reset() {

        this->offset = 0;
        this->finalSpeed = 0;
    }

};

struct GamePlayVars {
    
    uint16_t distance = 0;
    uint16_t oldIndexTo = 0;
    uint16_t previousZ = 0;
    uint8_t showDayBannerCount = Constants::NewDayBannerDelay_CountDown;
    uint16_t days = 1;
    uint16_t timeOfDay = 100;
    uint16_t horizonX = 64;
    uint8_t numberOfOtherCars = 0;

    uint8_t carsPassed = 0;
    bool gameOver = false;


    // Dirt cloud ..

    uint8_t brakeCount = 0;
    Direction brakeSide = Direction::None;

    void reset() {

        this->distance = 0;
        this->carsPassed = 0;
        this->oldIndexTo = 0;
        this->previousZ = 0;
        this->showDayBannerCount = Constants::NewDayBannerDelay_CountDown;
        this->days = 1;
        this->brakeCount = 0;
        this->brakeSide = Direction::None;
        this->timeOfDay = 100; //SJH  100
        this->carsPassed = Constants::CarsToPass_Init;
        this->gameOver = false;
        this->numberOfOtherCars = (Constants::NumberOfOtherCars > 2 ? 2 : Constants::NumberOfOtherCars);

    }

    TimeOfDay getTimeOfDay() {

        switch (this->timeOfDay) {

            case 0 ... 1700:
                return TimeOfDay::Day;

            case 1701 ... 2100:
                return TimeOfDay::Dawn;

            case 2101 ... 2800:
                return TimeOfDay::Night;

            case 2801 ... 3200:
                return TimeOfDay::Dawn;
                
        }

    }


    uint8_t getTimeOfDay_Pattern() {

        switch (this->timeOfDay) {

            case    1 ... 1400:     return Constants::Sky_BX______D;
            case 1401 ... 1500:     return Constants::Sky_B_X_____D;
            case 1501 ... 1600:     return Constants::Sky_B__X____D;
            case 1601 ... 1700:     return Constants::Sky_B___X___D;
            case 1701 ... 1800:     return Constants::Sky_B____X__D;
            case 1801 ... 1900:     return Constants::Sky_B_____X_D;
            case 1901 ... 2700:     return Constants::Sky_B______XD;
            case 2701 ... 2800:     return Constants::Sky_B_____X_D;
            case 2801 ... 2900:     return Constants::Sky_B____X__D;
            case 2901 ... 3000:     return Constants::Sky_B___X___D;
            case 3001 ... 3100:     return Constants::Sky_B__X____D;
            case 3101 ... 3200:     return Constants::Sky_B_X_____D;


        }

    }

    bool incTimeOfDay() {

        this->timeOfDay++;

        if (this->timeOfDay > 3200) {
            this->timeOfDay = 0;
        }

        if (this->timeOfDay == 40) {
            
            this->days++;

            if (this->days == 3) {

                this->numberOfOtherCars++;

            }

            if (this->days == 7) {

                this->numberOfOtherCars++;

            }

            return true;

        }

        return false;

    }

    void decShowDayBannerCount() {

        if (this->showDayBannerCount > 0) {

            this->showDayBannerCount--;

        }

    }

};