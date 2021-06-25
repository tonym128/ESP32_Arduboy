#pragma once

#include "Vec3.h"
#include <FixedPointsCommon.h>
#include "Base.h"

struct Car : public Base {

    private:

        bool isOffroad = false;
        uint8_t gear = 0;
        TransmissionType transmissionType = TransmissionType::Auto;

    public:

        uint8_t getGear()                                   { return this->gear; }    
        bool getOffroad()                                   { return this->isOffroad; }    
        TransmissionType getTransmissionType()              { return this->transmissionType; }    

        void setGear(uint8_t val)                           { this->gear = val; }    
        void setOffroad(bool val)                           { this->isOffroad = val; }    
        void setTransmissionType(TransmissionType val)      { this->transmissionType = val; }    

        void incGear() {

            if (this->gear < 5) this->gear++;
            
        }

        boolean decGear() {

            if (this->gear > 0) {

                this->gear--;

                if (this->getSpeed() > Constants::GearsMax[this->gear]) {

                    this->setSpeed(static_cast<UQ16x16>(Constants::GearsMax[this->gear]));

                    return true;

                }   
                else {

                    return false;

                }

            }

            return false;
            
        }

        void changeSpeed(CarMovement carMovement) {

            uint16_t speed = this->getSpeed().getInteger();

            switch (carMovement) {

                case CarMovement::Accelerate:

                    switch (speed) {

                        case 0 ... 2047:

                            this->setSpeed(2048);
                            break;

                        case 2048 ... 8192:

                            this->setSpeed(this->getSpeed() * 1.15);
                            break;

                        case 8193 ... 18000: 

                            this->setSpeed(this->getSpeed() * 1.05);
                            break;

                        case 18001 ... 36000: 

                            this->setSpeed(this->getSpeed() * 1.02);
                            break;

                        case 36001 ... 48000: 

                            this->setSpeed(this->getSpeed() * 1.01);
                            break;

                        default: break;

                    }

                    break;

                case CarMovement::Coast:
    
                    switch (speed) {

                        case 1 ... 1024:

                            this->setSpeed(0);
                            break;

                        default:
                            this->setSpeed(this->getSpeed() / 1.1);
                            break;

                    }

                    break;

                case CarMovement::Decelerate:
    
                    switch (speed) {

                        case 1 ... 1024:

                            this->setSpeed(0);
                            break;

                        default:
                            this->setSpeed(this->getSpeed() / 1.4);
                            break;

                    }

                    break;                    

                case CarMovement::NoMovement:

                    break;

            }

        }


        uint8_t getTacho() {

            uint16_t speed = this->getSpeed().getInteger();

            switch (this->gear) {

                case 0:
                    return 8;

                case 1:

                    switch (speed) {

                        case 0 ... 1023:                        return 1;
                        case 1024 ... Constants::Gear1Max:      return 2 + ((speed - 1024) / 800);
                        default:                                return 8;

                    }

                case 2:

                    switch (speed) {

                        case 0 ... 3999:                        return 1;
                        case 4000 ... Constants::Gear2Max:      return 2 + ((speed - 4000) / 1300);
                        default:                                return 8;

                    }

                case 3:

                    switch (speed) {

                        case 0 ... 7999:                        return 1;
                        case 8000 ... Constants::Gear3Max:      return 2 + ((speed - 8000) / 2400);
                        default:                                return 8;

                    }

                case 4:

                    switch (speed) {

                        case 0 ... 13999:                       return 1;
                        // case 14000 ... Constants::Gear4Max:     return 2 + ((speed - 14000) / 2850);
                        case 14000 ... Constants::Gear4Max:     return 2 + ((speed - 14000) / 3000);
                        default:                                return 8;

                    }

                case 5:

                    switch (speed) {

                        case 0 ... 20999:                       return 1;
//                        case 21000 ... Constants::Gear5Max:     return 2 + ((speed - 21000) / 4100);
                        case 21000 ... Constants::Gear5Max:     return 2 + ((speed - 21000) / 3900);
                        default:                                return 8;

                    }

            }

            return 0;

        }

        void reset() {

            UQ16x16 speed = 0;

            this->setGear(this->transmissionType == TransmissionType::Auto ? 1 : 0);
            this->setSpeed(speed);
            this->setZ(95);
            this->setX(0);
            this->setY(UPM);

        }
};