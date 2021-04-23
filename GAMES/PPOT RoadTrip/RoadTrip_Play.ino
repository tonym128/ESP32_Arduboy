#include <stdint.h>

#define max(a,b) ((a>b)?a:b)
#define min(a,b) ((a<b)?a:b)


// ----------------------------------------------------------------------------
//  Initialise state ..
//
void game_Init() {

    #ifdef SOUNDS

        sound.noTone();

    #endif

    world.reset();
    car.reset();
    gamePlayVars.reset();
    gameOverVars.reset();

    cameraPos.setX(0);
    cameraPos.setY(UPM);
    cameraPos.setZ(-40);

    for (uint8_t i = 0; i < gamePlayVars.numberOfOtherCars; i++) {

        OtherCar &otherCar = otherCars[i];

        otherCar.setX(0);
        otherCar.setZ(400 + (i * 200));
        otherCar.setSpeed(random(7, 10) * Constants::SpeedDiv);

    }

    #ifdef CACTII
        placeCactii();
    #endif

    gameState = GameState::Game;

    #ifdef DEBUG_COLLISIONS
        gamePlayVars.showDayBannerCount = 1;
    #endif
}


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void game() {

    #ifdef SOUNDS

        if (car.getGear() > 0 && !sound.playing()) {

            sound.tones(Sounds::Drive[car.getTacho()]);
        }

    #endif


    switch (car.getTransmissionType()) {
        
        case TransmissionType::Manual:

            if (arduboy.justPressed(UP_BUTTON)) {

                if (gamePlayVars.showDayBannerCount < 59 || gamePlayVars.days > 1) {
                    car.incGear();
                }
                else {

                    if (gamePlayVars.brakeCount == 0) {

                        gamePlayVars.brakeCount = Constants::BrakeCloud_DownGear;
                        gamePlayVars.brakeSide = Direction::Both;

                    }

                }

            }

            if (arduboy.justPressed(DOWN_BUTTON)) {

                bool brakes = car.decGear();

                if (brakes) {

                    gamePlayVars.brakeCount = Constants::BrakeCloud_DownGear;
                    gamePlayVars.brakeSide = Direction::Both;
                    
                }

            }

            break;

        case TransmissionType::Auto:
        
            if (car.getTacho() == 1 && car.getGear() > 1) {

                car.decGear();

            }
            
            if (!car.getOffroad()) {

                if (car.getTacho() == 8 && car.getGear() > 0) {

                    if (gamePlayVars.showDayBannerCount < 59 || gamePlayVars.days > 1) {
                        car.incGear();
                    }
                    else {

                        if (gamePlayVars.brakeCount == 0) {

                            gamePlayVars.brakeCount = Constants::BrakeCloud_DownGear;
                            gamePlayVars.brakeSide = Direction::Both;

                        }

                    }

                }

            }

            break;

    }


    // Render screen ..

    draw(true, false);
    renderPlayerCar();
    renderHud();

    if (gamePlayVars.days > 1) {
        renderDayBanner();
    }
    else {
       renderLights();
    }
    

    // Do some house keeping ..

    houseKeeping();

    #ifdef CACTII
      moveCactii();
    #endif
    

    // When moving left or right, the speed must be > 0 .. 
 
    uint8_t speed = car.getSpeed_Display();

    if (gamePlayVars.showDayBannerCount > 59 && gamePlayVars.days == 1) {

        speed = 0;


    }
    uint8_t speedForSteering = Constants::SpeedSteering[speed];


    // Steering ..
    
    Vec3 segClosest = world.getRoadSegment(0, car.getZ());
    Vec3 segNext = world.getRoadSegment(car.getZ(), car.getZ() + UPM);

    int16_t carOffsetOnRoad = car.getX() - ((segClosest.getX() + segNext.getX()) / 2);

    if (arduboy.pressed(LEFT_BUTTON) && carOffsetOnRoad > -300) {

        #ifdef DEBUG_COLLISIONS
            Serial.print("Left - ");
        #endif

        if (moveCar(-speedForSteering, 0) == Constants::NoCollision) {
            cameraPos.setX(cameraPos.getX() - speedForSteering);
            car.setX(car.getX() - speedForSteering);
        }

    }

    if (arduboy.pressed(RIGHT_BUTTON) && carOffsetOnRoad < 300) {

        #ifdef DEBUG_COLLISIONS
            Serial.print("Right -");
        #endif

        if (moveCar(speedForSteering, 0) == Constants::NoCollision) {
            cameraPos.setX(cameraPos.getX() + speedForSteering);
            car.setX(car.getX() + speedForSteering);
        }

    }



    if (speed > 0 && gamePlayVars.brakeCount == 0) {

        switch (carOffsetOnRoad) {

            case -1000 ... -191:

                gamePlayVars.brakeCount = Constants::BrakeCloud_OffRoad;
                gamePlayVars.brakeSide = Direction::Both;
                car.setOffroad(true);
                break;

            case -190 ... -130:

                gamePlayVars.brakeCount = Constants::BrakeCloud_OffRoad;
                gamePlayVars.brakeSide = Direction::Left;
                car.setOffroad(true);
                break;

            case 130 ... 190:

                gamePlayVars.brakeCount = Constants::BrakeCloud_OffRoad;
                gamePlayVars.brakeSide = Direction::Right;
                car.setOffroad(true);
                break;

            case 191 ... 1000:

                gamePlayVars.brakeCount = Constants::BrakeCloud_OffRoad;
                gamePlayVars.brakeSide = Direction::Both;
                car.setOffroad(true);
                break;

            default:
                car.setOffroad(false);
                break;

        }

    }


    int8_t collide = Constants::NoCollision;

    if (arduboy.isFrameCount(4)) {

        uint16_t tacho = car.getTacho();

        collide = moveCar(0, speed);

        CarMovement carMovement = CarMovement::NoMovement;

        if (collide == Constants::NoCollision) {

            if (!car.getOffroad() || car.getGear() == 1) {

                if (arduboy.pressed(A_BUTTON) && ((car.getGear() == 1 || tacho > 1) && tacho < 8)) {


                    // If accelerating from a stand still, render the dirt cloud ..

                    if (car.getSpeed().getInteger() == 0) {
                        gamePlayVars.brakeCount = Constants::BrakeCloud_Accelerate;
                        gamePlayVars.brakeSide = Direction::Both;       
                    }

                    carMovement = CarMovement::Accelerate;

                }
                else if (arduboy.pressed(A_BUTTON) && (tacho == 8 || (car.getGear() > 1 && tacho == 1))) {

                    carMovement = CarMovement::NoMovement;

                }
                else {

                    carMovement = CarMovement::Coast;

                }

            }
            else {

                carMovement = CarMovement::Coast;

            }

            if (arduboy.notPressed(A_BUTTON) && arduboy.pressed(B_BUTTON)) {

                carMovement = CarMovement::Decelerate;
                
            }

            car.changeSpeed(carMovement);

        }
        else {

            if (collide >= 0) {

                OtherCar &otherCar = otherCars[collide];

                if (arduboy.pressed(A_BUTTON)) {

                    car.setSpeed(otherCar.getSpeed());
                    otherCar.setZ(car.getZ() + Constants::PlayerCarLengthUnits);

                    #ifdef DEBUG_COLLISIONS
                        Serial.println("set speed 1");
                    #endif
                }
                else {

                    if (!car.getOffroad()) {
                        car.changeSpeed(CarMovement::NoMovement);
                    }
                    else{
                        car.changeSpeed(CarMovement::Coast);
                    }

                }

            }

        }

    }
    else {

        collide = moveCar(0, speed);

        if (collide != Constants::NoCollision) {
                    
                OtherCar &otherCar = otherCars[abs(collide)];

                if (otherCar.getZ() > car.getZ()) {

                    car.setSpeed(otherCar.getSpeed());


                    if (collide >= 0) {

                        otherCar.setZ(car.getZ() + Constants::PlayerCarLengthUnits);
                        #ifdef DEBUG_COLLISIONS
                            Serial.println("set speed 2");
                        #endif

                    }

                }

            

        }

    }
    

    // Now, after adjusting the car's speed for collisions, move the car forward ..

    if ((gamePlayVars.showDayBannerCount < 59 && gamePlayVars.days == 1) || gamePlayVars.days > 1) {

        speed = car.getSpeed_Display();

        car.setZ(car.getZ() + speed);
        cameraPos.setZ(cameraPos.getZ() + speed);
        cameraPos.setY(world.roadHeightAt(cameraPos.getZ() + 2 * UPM) + UPM);

        moveOtherCars();

    }

}




// ----------------------------------------------------------------------------
// Render the screen .. 
//
void draw(bool drawOtherCars, bool noCurves) {
    
    Vec3 startPos, endPos;

    uint16_t indexFrom = cameraPos.getZ() / UPM;
    uint16_t indexTo = indexFrom + DRAW_DISTANCE; 


    // draw road segments starting with the backmost one first ..
    // IndexTo is the last position and is an ever increasing value.

    if (gamePlayVars.oldIndexTo != indexTo + 5) {
        world.addRoadSegment(indexTo + 5, noCurves);
        gamePlayVars.oldIndexTo = indexTo + 5;
    }

    for (uint16_t i = indexTo; i > indexFrom; --i) {

        Vec3 startVec = world.getRoadSegment(i);
        Vec3 endVec = world.getRoadSegment(i + 1);

        startPos = world.perspective(startVec, cameraPos);
        startPos.setY(startPos.getY() - (((i -1) - indexFrom) ));
        endPos   = world.perspective(endVec, cameraPos);
        endPos.setY(endPos.getY() - ((i - indexFrom) ));

        int16_t x1Bottom = startPos.getX() - startPos.getZ() * 1.25;
        int16_t x2Bottom = x1Bottom + startPos.getZ() * 2.5;
        int16_t x1Top = endPos.getX() - endPos.getZ() * 1.25;
        int16_t x2Top = x1Top + endPos.getZ() * 2.5;

        int16_t x1Diff = x1Top - x1Bottom;
        int16_t x2Diff = x2Top - x2Bottom;


        // Draw Horizon ..

        if (i == indexTo) {

            Vec3 frontMost = world.getRoadSegment(indexFrom);
            gamePlayVars.horizonX = 64 + frontMost.getX() / 4;

            uint8_t idx = gamePlayVars.getTimeOfDay() == TimeOfDay::Day ? 0 : 1;

            for (int16_t i = gamePlayVars.horizonX - 256; i < WIDTH; i = i + 128) {

                Sprites::drawExternalMask(i, endPos.getY() - 9, Images::Horizons, Images::Horizons_Mask, idx, idx);

            }

        }


        bool odd = i % 2;

        // if visible (ie leaning towards camera) ..

        if (startPos.getY() > endPos.getY() && startPos.getZ() > 0 && endPos.getZ() > 0) {

            int16_t segmentHeight = startPos.getY() - endPos.getY();
            int16_t y = startPos.getY();

            for (int16_t j = 0; j < segmentHeight; ++j) {

                int16_t pos1 = x1Bottom + (j * x1Diff) / segmentHeight;
                int16_t pos2 = x2Bottom + (j * x2Diff) / segmentHeight;

                if (y >= DISPLAY_HEIGHT) {
                    y--;
                    continue;
                }

                pos1 = max(0, pos1);
                pos2 = min(DISPLAY_WIDTH, pos2);

                bool dither = j % 2;
                bool color = y % 2;

                // Odd line
                if (odd) {


                    int16_t k = 0;

                    switch (gamePlayVars.getTimeOfDay()) {

                        case TimeOfDay::Day:
                            renderRoadEdge_Light(k, pos1, y);
                            renderRoad_Dither_Light(k, pos2, y, color);
                            renderRoadEdge_Light(k, DISPLAY_WIDTH, y);
                            break;

                        case TimeOfDay::Dawn:
                            renderRoadEdge_Black(k, pos1, y, color);
                            renderRoad_Dither_Light(k, pos2, y, color);
                            renderRoadEdge_Black(k, DISPLAY_WIDTH, y, color);
                            break;

                        case TimeOfDay::Night:
                            color = y % 2;
                            renderRoadEdge_Dark(k, pos1, y, color);                        
                            renderRoad_Black(k, pos2, y, color);
                            renderRoadEdge_Dark(k, DISPLAY_WIDTH, y, color);
                            break;

                    }

                }

                // Even line
                else {
                    
                    int16_t k = 0;
                    
                    switch (gamePlayVars.getTimeOfDay()) {

                        case TimeOfDay::Day:
                            color = y % 2;
                            renderRoadEdge_Dark(k, pos1, y, color);
                            dither = j % 2;
                            renderRoad_Dither_Dark(k, pos2, y, dither, color);
                            renderRoadEdge_Dark(k, DISPLAY_WIDTH, y, color);
                            break;

                        case TimeOfDay::Dawn:
                            color = y % 2;
                            renderRoadEdge_Dark(k, pos1, y, color);
                            dither = j % 2;
                            renderRoad_Dither_Dark(k, pos2, y, dither, color);
                            renderRoadEdge_Dark(k, DISPLAY_WIDTH, y, color);
                            break;

                        case TimeOfDay::Night:
                            renderRoadEdge_Black(k, pos1, y, color);
                            dither = j % 2;
                            renderRoad_Dither_Dark(k, pos2, y, dither, color);
                            renderRoadEdge_Black(k, DISPLAY_WIDTH, y, color);
                            break;

                    }

                }

                y--;
            }


            // Draw lines ..

            if (!odd) {

                int16_t pos1Top = x1Bottom + x1Diff;
                int16_t pos2Top = x2Bottom + x2Diff;

                int16_t pBottom = (x2Bottom - x1Bottom) / 6; 
                int16_t pTop = (pos2Top - pos1Top) / 6;

                int16_t centreTop = (x1Top + x2Top) / 2;
                int16_t centreBottom = (x1Bottom + x2Bottom) / 2;

                arduboy.drawLine(centreBottom - pBottom,startPos.getY(),centreTop - pTop,endPos.getY(),WHITE);
                arduboy.drawLine(centreBottom + pBottom,startPos.getY(),centreTop + pTop,endPos.getY(),WHITE);

            }

        }


    
        if (drawOtherCars) {


            // draw other cars ..


            for (uint8_t j = 0; j < gamePlayVars.numberOfOtherCars; j++) {

                OtherCar &otherCar = otherCars[j];


                // If the other car is within the road segment being rendered then render it ..

                if (otherCar.getZ() > startVec.getZ() && otherCar.getZ() <= endVec.getZ()) {


                    // Render cars based on time of day ..
                    
                    uint8_t imageIndex = gamePlayVars.getTimeOfDay() == TimeOfDay::Night ? 1 : 0;


                    
                    // Add the average road segement x value to the car ..

                    otherCar.setXWorld((startVec.getX() + endVec.getX()) / 2);
                    otherCar.setY(world.roadHeightAt(otherCar.getZ()));


                    #ifndef DEBUG_COLLISIONS

                        Vec3 otherCarNonRef = otherCar.clone();
                        otherCarNonRef.setX(otherCar.getX() + otherCar.getXWorld());

                        Vec3 carPerspective = world.perspective(otherCarNonRef, cameraPos);
                        carPerspective.setY(carPerspective.getY() - (i - indexFrom));
                        int16_t distToCar = otherCar.getZ() - cameraPos.getZ();


                        // Only render the cars if they are visible ..

                        uint8_t index = 0;

                        if (distToCar > -50 && distToCar < 650) {
                            
                            switch (distToCar) {

                                case 460 ... 650: // 100
                                    index = 5;
                                    break;

                                case 360 ... 459: // 90
                                    index = 4;
                                    break;

                                case 290 ... 359: // 90
                                    index = 3;
                                    break;

                                case 220 ... 289: // 80
                                    index = 2;
                                    break;

                                case 180 ... 219: // 50
                                    index = 1;
                                    break;

                                default:
                                    index = 0;
                                    break;

                            }
                    
                            Sprites::drawPlusMask(carPerspective.getX() - (pgm_read_byte(&Images::OtherCar_Width[index]) / 2), carPerspective.getY() - pgm_read_byte(&Images::OtherCar_Height[index]) + 1, Images::OtherCar[index], imageIndex);

                        }

                    #else

                        Vec3 leftTopCorner = otherCar.clone();
                        leftTopCorner.setX(leftTopCorner.getX() + otherCar.getXWorld() - (Constants::OtherCarWidthUnits / 2));
                        leftTopCorner.setZ(leftTopCorner.getZ() + Constants::OtherCarLengthUnits);

                        Vec3 rightTopCorner = otherCar.clone();
                        rightTopCorner.setX(rightTopCorner.getX() + otherCar.getXWorld() + (Constants::OtherCarWidthUnits / 2));
                        rightTopCorner.setZ(rightTopCorner.getZ() + Constants::OtherCarLengthUnits);
                        
                        Vec3 leftBottomCorner = otherCar.clone();
                        leftBottomCorner.setX(leftBottomCorner.getX() + otherCar.getXWorld() - (Constants::OtherCarWidthUnits / 2));

                        Vec3 rightBottomCorner = otherCar.clone();
                        rightBottomCorner.setX(rightBottomCorner.getX() + otherCar.getXWorld() + (Constants::OtherCarWidthUnits / 2));
                        
                        Vec3 leftTopCornerPersepective = world.perspective(leftTopCorner, cameraPos);
                        Vec3 rightTopCornerPersepective = world.perspective(rightTopCorner, cameraPos);
                        Vec3 leftBottomCornerPersepective = world.perspective(leftBottomCorner, cameraPos);
                        Vec3 rightBottomCornerPersepective = world.perspective(rightBottomCorner, cameraPos);

                        arduboy.drawLine(leftTopCornerPersepective.getX(), leftTopCornerPersepective.getY(), rightTopCornerPersepective.getX(), rightTopCornerPersepective.getY());
                        arduboy.drawLine(rightTopCornerPersepective.getX(), rightTopCornerPersepective.getY(), rightBottomCornerPersepective.getX(), rightBottomCornerPersepective.getY());
                        arduboy.drawLine(rightBottomCornerPersepective.getX(), rightBottomCornerPersepective.getY(), leftBottomCornerPersepective.getX(), leftBottomCornerPersepective.getY());
                        arduboy.drawLine(leftBottomCornerPersepective.getX(), leftBottomCornerPersepective.getY(), leftTopCornerPersepective.getX(), leftTopCornerPersepective.getY());

                    #endif

                }

            }

        }


        // Draw Cactii

        #ifdef CACTII

            for (uint8_t j = 0; j < Constants::NumberOfCactii; j++) {

                Vec3 &cactiiPos = cactii[j];

                if (cactiiPos.getZ() > startVec.getZ() && cactiiPos.getZ() <= endVec.getZ()) {

                    uint8_t cactiiIdx = 0;

                    if (i > indexFrom + 2 && i < indexFrom + 4) {
                        cactiiIdx = 4;
                    }

                    if (i >= indexFrom + 4 && i < indexFrom + 6) {
                        cactiiIdx = 3;
                    }

                    if (i >= indexFrom + 6 && i < indexFrom + 8) {
                        cactiiIdx = 2;
                    }

                    if (i >= indexFrom + 8 && i < indexFrom + 10) {
                        cactiiIdx = 1;
                    }

                    if (i >= indexFrom + 10) {
                        cactiiIdx = 0;
                    }

                    if (cactiiPos.getX() == 0) {

                        Sprites::drawPlusMask(x1Bottom - 12 - (pgm_read_byte(&Images::Cactii_Width[cactiiIdx]) / 2), startPos.getY() - pgm_read_byte(&Images::Cactii_Height[cactiiIdx]), Images::Cactii[cactiiIdx], 0);

                    }
                    else {

                        Sprites::drawPlusMask(x2Bottom + 13 - (pgm_read_byte(&Images::Cactii_Width[cactiiIdx]) / 2), startPos.getY() - pgm_read_byte(&Images::Cactii_Height[cactiiIdx]), Images::Cactii[cactiiIdx], 0);

                    }

                }

            }

        #endif


        // Sign posts / rocks ..

        if (odd && i > indexFrom + 2) {

            #ifdef ROCKS
                
                uint8_t rockIdx = 0;

                if (i > indexFrom + 2 && i < indexFrom + 4) {
                    rockIdx = 3;
                }

                if (i >= indexFrom + 4 && i < indexFrom + 6) {
                    rockIdx = 2;
                }

                if (i >= indexFrom + 6 && i < indexFrom + 8) {
                    rockIdx = 1;
                }

                if (i >= indexFrom + 8) {
                    rockIdx = 0;
                }

                int16_t spriteSize = Images::Rocks_Width[rockIdx];

                Sprites::drawPlusMask(x1Bottom - spriteSize, startPos.getY() - spriteSize, Images::Rocks[rockIdx], 0);
                Sprites::drawPlusMask(x2Bottom , startPos.getY() - spriteSize, Images::Rocks[rockIdx], 0);
                
            #endif

        }

    }
  
}
