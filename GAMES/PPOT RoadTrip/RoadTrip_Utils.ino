#include <stdint.h>

void swap(OtherCar *xp, OtherCar *yp) {  

    OtherCar temp = *xp;  
    *xp = *yp;  
    *yp = temp;  

}  
  
void sort(OtherCar arr[], uint8_t n) {  

    uint8_t i, j;  

    for (i = 0; i < n-1; i++) {     

        for (j = 0; j < n-i-1; j++) {

            if (arr[j].getZ() < arr[j+1].getZ()) {  
                swap(&arr[j], &arr[j+1]);  
            }

        }

    }

}  

int8_t moveCar(int16_t xMovement, int16_t zMovement) { // will return NoCollision constant or car index.

    #ifdef DEBUG_COLLISIONS

        Serial.print("moveCars(");
        Serial.print(xMovement);
        Serial.print(",");
        Serial.print(zMovement);
        Serial.println(")");

    #endif

    uint8_t collide = Constants::NoCollision;

    for (uint8_t i = 0; i < gamePlayVars.numberOfOtherCars; i++) {

        OtherCar &otherCar = otherCars[i];

        #ifdef DEBUG_COLLISIONS

            Serial.print("A P ");
            car.printPos();
            Serial.print(" = O(");
            Serial.print(i);
            Serial.print(") ");
            otherCar.printPos();
            Serial.print(" - ");

        #endif

        int16_t xOverlap = (car.getX() + xMovement) - (otherCar.getX() + otherCar.getXWorld());
        int16_t zOverlap = (car.getZ() + zMovement) - otherCar.getZ();

        if (abs(xOverlap) < Constants::OtherCarWidthUnits && abs(zOverlap) < Constants::OtherCarLengthUnits) {
                    
            #ifdef DEBUG_COLLISIONS
                Serial.print("COL x:");
                Serial.print(xMovement);
                Serial.print(", xO:");
                Serial.print(xOverlap);
                Serial.print(", z:");
                Serial.print(zMovement);
                Serial.print(", zO:");
                Serial.print(zOverlap);

            #endif

            if (xMovement != 0) { // Move left or right ..

                if (abs(zOverlap) < Constants::CollisionTollerance_Z) {

                    collide = i;
                                
                    #ifdef SOUNDS

                        if (!sound.playing()) {

                            sound.tones(Sounds::Crash);
                        }

                    #endif

                    #ifdef DEBUG_COLLISIONS
                        Serial.println("BLOCK LR");
                    #endif

                    break;

                }

            }

            if (zMovement != 0) { // Move front or back ..

                if (abs(xOverlap) < Constants::CollisionTollerance_X) {

                    collide = i;
                                
                    #ifdef SOUNDS

                        if (!sound.playing()) {

                            sound.tones(Sounds::Crash);

                        }

                    #endif

                    #ifdef DEBUG_COLLISIONS
                        Serial.println("BLOCK FB");
                    #endif

                    if (car.getZ() < otherCars[i].getZ() && nudgeCarForward(i)) {

                        collide = collide * -1;

                    }

                    break;
                    
                }

            }

            #ifdef DEBUG_COLLISIONS
                Serial.println("No Col");
            #endif

        }
        else {
            
            #ifdef DEBUG_COLLISIONS
                Serial.println("");
            #endif

        }

        // #ifdef DEBUG_COLLISIONS

        //     Serial.print("B P ");
        //     car.printPos();
        //     Serial.print(" = O(");
        //     Serial.print(i);
        //     Serial.print(") ");
        //     otherCar.printPos();
        //     Serial.println(" ");

        // #endif
    }


    return collide;

}

void moveOtherCars() {

    // Sort the cars ..

    sort(otherCars, gamePlayVars.numberOfOtherCars);

    for (uint8_t i = 0; i < gamePlayVars.numberOfOtherCars; i++) {

        bool collide = false;
        OtherCar &otherCar = otherCars[i];

        int16_t otherCarX = otherCar.getX() + otherCar.getXWorld() + otherCar.getXMovement();


        #ifdef DEBUG_COLLISIONS

            Serial.print("C O(");
            Serial.print(i);
            Serial.print(") ");
            otherCar.printPos();
            Serial.println(" - ");

        #endif        

        for (uint8_t j = i; j < gamePlayVars.numberOfOtherCars; j++) {

            if (i != j) {

                OtherCar &testCar = otherCars[j];

                if (abs((testCar.getX() + testCar.getXWorld()) - otherCarX) < Constants::OtherCarWidthUnits && abs(testCar.getZ() - (otherCar.getZ() + otherCar.getSpeed_Display())) < Constants::OtherCarLengthUnits ) {

                    collide = true;

                }

            }

        }


        // What about into the player?

        if (abs(car.getX() - otherCarX) < Constants::OtherCarWidthUnits && abs((car.getZ() + car.getSpeed_Display()) - (otherCar.getZ() + otherCar.getSpeed_Display())) < Constants::OtherCarLengthUnits ) {

            collide = true;

        }


        // If no collision has occurred then move the car forward ..

        if (!collide) {

            otherCar.setZ(otherCar.getZ() + otherCar.getSpeed_Display());

            if (otherCar.getZ() < cameraPos.getZ() - Constants::DistToRenewCars) {

                if (gamePlayVars.numberOfOtherCars > 1) {

                    for (uint8_t j = 0; j < gamePlayVars.numberOfOtherCars; j++) {

                        if (j != i) {

                            int16_t dist = otherCar.getZ() - cameraPos.getZ();

                            if (dist > 0) {

                                otherCar.setZ(otherCars[j].getZ() + random(700 - dist, 1200 - dist));

                            }
                            else {

                                otherCar.setZ(cameraPos.getZ() + random(700, 1200));

                            }

                            break;

                        }

                    }

                }
                else {

                    otherCar.setZ(cameraPos.getZ() + random(700, 1200));

                }

                otherCar.setSpeed(random(9, 14) * Constants::SpeedDiv);
                
                if (gamePlayVars.carsPassed > 0 && !gamePlayVars.gameOver) {

                    gamePlayVars.carsPassed--;

                    #ifdef SOUNDS

                        if (gamePlayVars.carsPassed > 0) {
                            sound.tones(Sounds::PassCar);
                        }
                        else {
                            sound.tones(Sounds::Success);
                        }

                    #endif

                }

            }

            otherCar.update();

        }

    }

    // Has the game finished?

    if (gamePlayVars.gameOver) {

        if (gameOverVars.offset < Constants::GameOver_MaxOffset) {

            gameOverVars.offset = gameOverVars.offset + 3;
            renderGameOver();

        }
        else {

            renderGameOver();
            gameOverVars.finalSpeed = car.getSpeed_Display();
            gameState = GameState::GameOver_Init;

        }

    }

}

void houseKeeping() {

    if (cameraPos.getZ() > Constants::ZMax) {

        cameraPos.decZ();
        car.decZ();
        world.decZ();

        for (uint8_t i = 0; i < gamePlayVars.numberOfOtherCars; i++) {

            OtherCar &otherCar = otherCars[i];
            otherCar.decZ();

        }

        #ifdef CACTII

            for (uint8_t i = 0; i < Constants::NumberOfCactii; i++) {

                Vec3 &cactusPos = cactii[i];
                cactusPos.decZ();

            }

        #endif

    }
    else {

        // Are cars too far into the distance?

        for (uint8_t i = 0; i < gamePlayVars.numberOfOtherCars; i++) {

            OtherCar &otherCar = otherCars[i];
            if (otherCar.getZ() - otherCar.getZ() > 2500) {
                otherCar.setZ(otherCar.getZ() - 1500);
            }

        }

    }


    // Increase the odometer ..

    if (!gamePlayVars.gameOver && (cameraPos.getZ() - gamePlayVars.previousZ > 64)) {

        gamePlayVars.previousZ = cameraPos.getZ();
        gamePlayVars.distance++;


    }


    // Increase time of day ..

    bool newDay = gamePlayVars.incTimeOfDay();

    if (newDay) {

        if (gamePlayVars.carsPassed > 0) {

            gamePlayVars.gameOver = true;
    
            #ifdef SOUNDS
                sound.tones(Sounds::GameOver);
            #endif

        }
        else {

            gamePlayVars.showDayBannerCount = Constants::NewDayBannerDelay;
            gamePlayVars.carsPassed = Constants::CarsToPass_Init + (gamePlayVars.days * Constants::CarsToPass_Increase);

        }
        
    }


    // Decrease banner counter ..

    //if (gamePlayVars.days > 1) {
        gamePlayVars.decShowDayBannerCount();
    //}

}

#ifdef CACTII

    void moveCactii() {

        // Move cactii

        for (uint8_t i = 0; i < Constants::NumberOfCactii; i++) {

            Vec3 &cactiiPos = cactii[i];

            if (cactiiPos.getZ() < cameraPos.getZ() - Constants::DistToRenewCars) {
                cactiiPos.setZ(cameraPos.getZ() + random(600, 900));
                cactiiPos.setX(random(0,2));
            }

        }

    }

    void placeCactii() {

        for (uint8_t i = 1; i <= Constants::NumberOfCactii; i++) {

            Vec3 &cactiiPos = cactii[i - 1];

            cactiiPos.setX(i % 2);
            cactiiPos.setZ(400 + (i * 245));

        }

    }

#endif

bool nudgeCarForward(uint8_t carIdx) {

    OtherCar &otherCar = otherCars[carIdx];
    bool collide = false;

    int16_t otherCarZ = otherCar.getZ() + otherCar.getSpeed_Display() + Constants::NudgeDistance;
    int16_t otherCarX = otherCar.getX() + otherCar.getXWorld() + otherCar.getXMovement();

    for (uint8_t i = 0; i < gamePlayVars.numberOfOtherCars; i++) {

        if (i != carIdx) {

            OtherCar &testCar = otherCars[i];

            if (abs((testCar.getX() + testCar.getXWorld()) - otherCarX) < Constants::OtherCarWidthUnits && abs(testCar.getZ() - otherCarZ) < Constants::OtherCarLengthUnits ) {

                collide = true;

            }

        }

    }

    if (!collide) {

        otherCar.setZ(otherCar.getZ() + Constants::NudgeDistance);
        return true;

    }

    return false;

}
