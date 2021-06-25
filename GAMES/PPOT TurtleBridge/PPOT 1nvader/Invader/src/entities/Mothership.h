#pragma once

#include "../utils/Constants.h"
#include "Player.h"

struct Mothership {

    private:

        int16_t pos = 0;
        int8_t rowAdjustment = 0;
        uint8_t height = 0;
        uint8_t explosionCounter = 0;
        uint8_t counter = Constants::MothershipCounterMax;
        uint8_t wave = 0;

        Movement movement = Movement::Up;

    public:

        int16_t getPos()                                    { return this->pos; }
        uint8_t getHeight()                                 { return this->height; }
        uint8_t getCounter()                                { return this->counter; }
        uint8_t getExplosionCounter()                       { return this->explosionCounter; }
        Movement getMovement()                              { return this->movement; }

        void setPos(int16_t val)                            { this->pos = val; }
        void setHeight(uint8_t val)                         { this->height = val; }
        void setCounter(uint8_t val)                        { this->counter = val; }
        void setMovement(Movement val)                      { this->movement = val; }
        void setRowAdjustment(int8_t val)                   { this->rowAdjustment = val; }

    private:
    
        uint16_t getSpeed(GameRotation gameRotation, GameMode gameMode) {

            uint16_t base = 240 + (gameMode == GameMode::TugOfWar ? 30 : 0);

            switch (gameRotation) {

                case GameRotation::Portrait:
                    return base + (wave * Constants::WaveIncrease) - (counter * 3 / 2);

                case GameRotation::Landscape:

                    return base + (wave * Constants::WaveIncrease) - (counter * 3 / 2);

                default: return 0;

            }
            
        }

        void move(GameRotation gameRotation, GameMode gameMode) {

            if (this->explosionCounter > 0) {

                this->explosionCounter--;

                if (this->explosionCounter == 0) {

                    this->die(gameRotation);

                }

            }

            switch (this->getMovement()) {

                case Movement::Up:

                    this->decPos(gameRotation, gameMode);

                    if (this->getPos() <= Constants::Portrait::MothershipMinPos) {
                        this->setMovement(Movement::Down);
                        if (this->getHeight() > 0) {
                            this->decHeight();
                        }
                    }

                    break;

                case Movement::Down:

                    this->incPos(gameRotation, gameMode);

                    if (this->getPos() >= Constants::Portrait::MothershipMaxPos) {
                        this->setMovement(Movement::Up);
                        if (this->getHeight() > 0) {
                            this->decHeight();
                        }
                    }

                    break;

                case Movement::Left:

                    this->decPos(gameRotation, gameMode);

                    if (this->getPos() <= Constants::Landscape::MothershipMinPos) {
                        this->setMovement(Movement::Right);
                        if (this->getHeight() < 64) {
                            this->decHeight();
                        }
                    }

                    break;

                case Movement::Right:

                    this->incPos(gameRotation, gameMode);

                    if (this->getPos() >= Constants::Landscape::MothershipMaxPos) {
                        this->setMovement(Movement::Left);
                        if (this->getHeight() < 64) {
                            this->decHeight();
                        }
                    }

                    break;

            }

        }

        void die(GameRotation gameRotation) {

            switch (gameRotation) {

                case GameRotation::Portrait:        

                    if (this->rowAdjustment == Constants::MothershipRowHeight) {

                        if (random(0, 2) == 0) {
                            this->pos = Constants::Portrait::MothershipMinPos;
                            this->movement = Movement::Down;
                        }
                        else {
                            this->pos = Constants::Portrait::MothershipMaxPos;
                            this->movement = Movement::Up;
                        }

                        switch (this->height) {

                            case Constants::Portrait::MothershipStartHeight:
                                break;

                            case Constants::Portrait::MothershipStartHeight - Constants::MothershipRowHeight:
                                this->height = Constants::Portrait::MothershipStartHeight;
                                break;

                            case Constants::Portrait::MothershipStartHeight - Constants::MothershipRowHeight - Constants::MothershipRowHeight:
                                this->height = Constants::Portrait::MothershipStartHeight;
                                break;

                            default:
                                this->height = this->height + Constants::MothershipRowHeight + Constants::MothershipRowHeight;
                                break;

                        }

                    }
                    else {

                        if (random(0, 2) == 0) {
                            this->pos = Constants::Portrait::MothershipMinPos;
                            this->movement = Movement::Down;
                        }
                        else {
                            this->pos = Constants::Portrait::MothershipMaxPos;
                            this->movement = Movement::Up;
                        }

                        this->height = this->height - this->rowAdjustment;
                        
                    }

                    break;

                case GameRotation::Landscape:        

                    if (random(0, 2) == 0) {
                        this->pos = Constants::Landscape::MothershipMinPos;
                        this->movement = Movement::Right;
                    }
                    else {
                        this->pos = Constants::Landscape::MothershipMaxPos;
                        this->movement = Movement::Left;
                    }

                    switch (this->height) {

                        case Constants::Landscape::MothershipStartHeight:
                            break;

                        case Constants::Landscape::MothershipStartHeight + Constants::MothershipRowHeight:
                            this->height = Constants::Landscape::MothershipStartHeight;
                            break;

                        case Constants::Landscape::MothershipStartHeight + Constants::MothershipRowHeight + Constants::MothershipRowHeight:
                            this->height = Constants::Landscape::MothershipStartHeight;
                            break;

                        default:
                            this->height = this->height - Constants::MothershipRowHeight - Constants::MothershipRowHeight;
                            break;

                    }

                    break;                    

            }

        }

    public:

        void move(GameRotation gameRotation, GameMode gameMode, Player &player1) {

            this->move(gameRotation, gameMode);

            switch (this->getMovement()) {

                case Movement::Down:

                    if (this->height < Constants::PlayerHeight && this->getPosDisplay() + Constants::MothershipHeight >= player1.getPos()) {

                        player1.setPos(this->getPosDisplay() + Constants::MothershipHeight);

                    }

                    break;

                case Movement::Up:

                    if (this->height < Constants::PlayerHeight && this->getPosDisplay() - Constants::PlayerHeight < player1.getPos()) {

                        player1.setPos(this->getPosDisplay() - Constants::PlayerHeight);

                    }

                    break;

                case Movement::Right:

                    if (this->height > 64 - Constants::PlayerHeight - Constants::MothershipHeight && this->getPosDisplay() + Constants::MothershipHeight >= player1.getPos()) {

                        player1.setPos(this->getPosDisplay() + Constants::MothershipHeight);

                    }

                    break;

                case Movement::Left:

                    if (this->height > 64 - Constants::PlayerHeight - Constants::MothershipHeight && this->getPosDisplay() - Constants::PlayerHeight < player1.getPos()) {

                        player1.setPos(this->getPosDisplay() - Constants::PlayerHeight);

                    }

                    break;

            }
        
        }

        void move(GameRotation gameRotation, GameMode gameMode, Player &player1, Player &player2) {

            this->move(gameRotation, gameMode);

            switch (this->getMovement()) {

                case Movement::Up:

                    if (this->height < Constants::PlayerHeight && this->getPosDisplay() - Constants::PlayerHeight <= player2.getPos()) {

                        player2.setPos(this->getPosDisplay() - Constants::PlayerHeight);

                        if (player2.getPos() - Constants::PlayerHeight <= player1.getPos()) {

                            player1.setPos(player2.getPos() - Constants::PlayerHeight);

                        }

                    }

                    break;

                case Movement::Down:

                    if (this->height < Constants::PlayerHeight && this->getPosDisplay() + Constants::MothershipHeight >= player1.getPos()) {

                        player1.setPos(this->getPosDisplay() + Constants::MothershipHeight);

                        if (player1.getPos() + Constants::PlayerHeight >= player2.getPos()) {

                            player2.setPos(player1.getPos() + Constants::PlayerHeight);

                        }

                    }

                    break;

                case Movement::Left:

                    if (this->height > 64 - Constants::PlayerHeight - Constants::MothershipHeight && this->getPosDisplay() - Constants::PlayerHeight <= player2.getPos()) {

                        player2.setPos(this->getPosDisplay() - Constants::PlayerHeight);

                        if (player2.getPos() - Constants::PlayerHeight <= player1.getPos()) {

                            player1.setPos(player2.getPos() - Constants::PlayerHeight);  //SJH << Height???

                        }

                    }

                    break;

                case Movement::Right:

                    if (this->height > 64 - Constants::PlayerHeight - Constants::MothershipHeight && this->getPosDisplay() + Constants::MothershipHeight >= player1.getPos()) {

                        player1.setPos(this->getPosDisplay() + Constants::MothershipHeight);

                        if (player1.getPos() + Constants::PlayerHeight >= player2.getPos()) {

                            player2.setPos(player1.getPos() + Constants::PlayerHeight);

                        }

                    }

                    break;

            }

        }


        void moveTugOfWar(Player &player1, Player &player2) {

            this->move(GameRotation::Portrait, GameMode::TugOfWar);

            switch (this->getMovement()) {

                case Movement::Up:

                    if (this->height < Constants::PlayerHeight && this->getPosDisplay() - Constants::PlayerWidth <= player1.getPos()) {
                        player1.setPos(this->getPosDisplay() - Constants::PlayerWidth);
                    }

                    if (this->height + Constants::MothershipHeight > 128 - Constants::PlayerHeight && this->getPosDisplay() - Constants::PlayerWidth <= player2.getPos()) {
                        player2.setPos(this->getPosDisplay() - Constants::PlayerWidth);
                    }

                    break;

                case Movement::Down:

                    if (this->height < Constants::PlayerHeight && this->getPosDisplay() + Constants::PlayerWidth >= player1.getPos()) {
                        player1.setPos(this->getPosDisplay() + Constants::PlayerWidth);
                    }

                    if (this->height + Constants::MothershipHeight > 128 - Constants::PlayerHeight && this->getPosDisplay() + Constants::PlayerWidth >= player2.getPos()) {
                        player2.setPos(this->getPosDisplay() + Constants::PlayerWidth);
                    }

                    break;

                default: break;

            }
        }

        void explode(int8_t rowAdjustment) {

            this->explosionCounter = Constants::MothershipExplosionMax;
            this->rowAdjustment = rowAdjustment;

        }

        int16_t getPosDisplay() { 

            return this->pos / 100; 

        }

        bool decCounter() {

            this->counter--;

            if (this->counter == 0) { //SJH
                this->counter = 80;
                this->wave++;
                return true;
            }
            
            return false;
            
        }

        void decHeight() {

            this->height = this->height - this->rowAdjustment;
            
        }

        void decPos(GameRotation gameRotation, GameMode gameMode) {

            this->pos = this->pos - this->getSpeed(gameRotation, gameMode);
            
        }

        void incPos(GameRotation gameRotation, GameMode gameMode) {

            this->pos = this->pos + this->getSpeed(gameRotation, gameMode);
            
        }

        void reset(GameRotation gameRotation, int8_t rowAdjustment) {

            this->counter = Constants::MothershipCounterMax;
            this->rowAdjustment = rowAdjustment;
            this->wave = 0;

            switch (gameRotation) {

                case GameRotation::Portrait:     
                    this->movement = Movement::Down;   
                    this->pos = Constants::Portrait::MothershipMinPos;
                    this->height = Constants::Portrait::MothershipStartHeight;
                    break;
                    
                case GameRotation::Landscape:        
                    this->movement = Movement::Right;   
                    this->pos = Constants::Landscape::MothershipMinPos;
                    this->height = Constants::Landscape::MothershipStartHeight;
                    break;

            }

        }

};
