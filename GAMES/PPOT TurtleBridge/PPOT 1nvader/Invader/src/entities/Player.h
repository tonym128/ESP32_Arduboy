#pragma once

#include "../utils/Constants.h"

struct Player {

    private:

        int8_t pos = 28;
        uint8_t playerIdx = 0;
        uint8_t explodeCounter = 0;
        int16_t bulletX = 0;
        int16_t bulletY = 0;
        uint16_t score = 0;
        bool bulletActive = false;
        Movement movement = Movement::Up;


    public:

        int8_t getPos()                                     { return this->pos; }
        uint8_t getPlayerIdx()                              { return this->playerIdx; }
        int16_t getBulletX()                                { return this->bulletX; }
        int16_t getBulletY()                                { return this->bulletY; }
        uint16_t getScore()                                 { return this->score; }
        uint8_t getExplosionCounter()                       { return this->explodeCounter; }
        bool getBulletActive()                              { return this->bulletActive; }
        Movement getMovement()                              { return this->movement; }

        void setPos(int8_t val)                             { this->pos = val; }
        void setPlayerIdx(uint8_t val)                      { this->playerIdx = val; }
        void setBulletX(int16_t val)                        { this->bulletX = val; }
        void setBulletY(int16_t val)                        { this->bulletY = val; }
        void setScore(uint16_t val)                         { this->score = val; }
        void setBulletActive(bool val)                      { this->bulletActive = val; }
        void setMovement(Movement val)                      { this->movement = val; }

    public:

        bool explode() {

            if (this->score > 0) this->score--;
            if (this->explodeCounter == 0) this->explodeCounter = 6;

            return this->explodeCounter == 6;

        }

        void decExplodeCounter() {

            if (this->explodeCounter > 0) this->explodeCounter--;

        }

        void decPos() {

            this->pos--;

        }

        void incPos() {

            this->pos++;

        }

        void incScore() {

            this->score++;

        }

        bool fire(GameRotation gameRotation, GameMode gameMode, Player* otherPlayer) {

            if (this->bulletActive) return false;

            switch (gameRotation) {

                case GameRotation::Portrait:

                    switch (gameMode) {

                        case GameMode::Single ... GameMode::Double:

                            this->bulletX = 4;
                            this->bulletY = this->pos + 3 + (this->movement == Movement::Up ? 1 : 0);
                            this->bulletActive = true;
                            if (!this->arePlayersTouching(otherPlayer)) changeMovement(gameRotation);
                            break;

                        case GameMode::TugOfWar:

                            switch (this->getPlayerIdx()) {

                                case 0:
                                    this->bulletX = 4;
                                    this->bulletY = this->pos + 3 + (this->movement == Movement::Up ? 1 : 0);
                                    this->bulletActive = true;
                                    changeMovement(gameRotation);
                                    break;

                                case 1:
                                    this->bulletX = 112;
                                    this->bulletY = this->pos + 3 + (this->movement == Movement::Up ? 1 : 0);
                                    this->bulletActive = true;
                                    changeMovement(gameRotation);
                                    break;

                            }

                            break;
 
                    }

                    break;

                case GameRotation::Landscape:

                    this->bulletX = this->pos + 3;
                    this->bulletY = 55;
                    this->bulletActive = true;
                    if (!this->arePlayersTouching(otherPlayer)) changeMovement(gameRotation);

                    break;

            }

            return true;

        }


        bool arePlayersTouching(Player* otherPlayer) {

            if (otherPlayer == nullptr) return false;

            switch (this->getPlayerIdx()) {

                case 0:

                    return (this->getPos() + Constants::PlayerWidthNoMask >= otherPlayer->getPos() - Constants::MaskWidth);

                case 1:

                    return (this->getPos() <= otherPlayer->getPos() - Constants::MaskWidth + Constants::PlayerWidthNoMask);

            }

            return false;

        }


        void changeMovement(GameRotation gameRotation) {

            switch (gameRotation) {

                case GameRotation::Portrait:

                    switch (this->pos) {

                        case Constants::Portrait::PlayerMinPos:
                            this->movement = Movement::Down;
                            break;

                        case Constants::Portrait::PlayerMaxPos:
                            this->movement = Movement::Up;
                            break;

                        default:

                            switch (this->movement) {

                                case Movement::Up:
                                    this->movement = Movement::Down;
                                    break;

                                case Movement::Down:
                                    this->movement = Movement::Up;
                                    break;

                                default: break;

                            }

                            break;

                    }

                    break;

                case GameRotation::Landscape:

                    switch (this->pos) {

                        case Constants::Landscape::PlayerMinPos:
                            this->movement = Movement::Right;
                            break;

                        case Constants::Landscape::PlayerMaxPos:
                            this->movement = Movement::Left;
                            break;

                        default:

                            switch (this->movement) {

                                case Movement::Left:
                                    this->movement = Movement::Right;
                                    break;

                                case Movement::Right:
                                    this->movement = Movement::Left;
                                    break;

                                default: break;

                            }

                            break;

                    }

                    break;            

            }

        }

        void incBulletX() {
            
            this->bulletX = this->bulletX + 4;

            if (this->bulletX > 128) {

                this->bulletActive = false;

            }
            
        }

        void decBulletX() {
            
            this->bulletX = this->bulletX - 4;

            if (this->bulletX < 0) {

                this->bulletActive = false;

            }
            
        }

        void decBulletY() {
            
            this->bulletY = this->bulletY - 4;

            if (this->bulletY < 0) {

                this->bulletActive = false;

            }
            
        }

        void reset(uint8_t playerNumber) {

            this->setPlayerIdx(playerNumber);
            this->score = 0;
            this->bulletActive = false;

        }

};
