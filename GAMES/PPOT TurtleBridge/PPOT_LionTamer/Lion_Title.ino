#include <Arduboy2.h>
#include <ArduboyTones.h>

#define NOTE_ROAR 0x7FFF

extern ArduboyTones sound;

void title() {

    uint8_t justPressed = arduboy.justPressedButtons();
    uint8_t pressed = arduboy.pressedButtons();

    if (counter >= 0) counter++;

    #ifdef SOUNDS
        if (soundCounter > 0) soundCounter--;
    #endif

    #ifdef THREE_IMAGES
        marqueeCounter++; 
        if (marqueeCounter == 195) marqueeCounter = 0;
    #else
        marqueeCounter++; 
        if (marqueeCounter == 166) marqueeCounter = 0;
    #endif

    Sprites::drawOverwrite(0, 0, Images::Title_LionTop, arduboy.getFrameCount(255) < 16 ? 1 : 0);

    if ((justPressed & A_BUTTON) || (justPressed & B_BUTTON)) { 

        gameMode = (justPressed & A_BUTTON) ? GameMode::Easy : GameMode::Hard;

        #ifdef SHOW_ROAR
            
            if (counter == -1) {

                #ifdef SOUNDS
                    sound.noTone();
                #endif
                counter = 0;
                
                #ifdef SOUNDS
                    soundVars.reset();
                #endif

            }
            else {

                #ifdef SHOW_INSTUCTIONS
                    gameState = GameState::Instructions_Init;
                #else
                    gameState = GameState::PlayGame_Init;
                #endif

            }

        #else

            #ifdef SHOW_INSTUCTIONS
                gameState = GameState::Instructions_Init;
            #else
                gameState = GameState::PlayGame_Init;
            #endif

            counter = 131;

        #endif

    }

    #ifdef SOUNDS
        if (counter == -1 && (justPressed & UP_BUTTON) || (justPressed & DOWN_BUTTON)) {

            sounds = EEPROM_Utils::toggleSoundSettings(arduboy);
            soundCounter = 64;
            
        }
    #endif

    #ifdef RESET
        if (counter == -1 && (pressed & LEFT_BUTTON) && (pressed & RIGHT_BUTTON)) {

            resetCounter++;
            arduboy.setRGBled(BLUE_LED, resetCounter);

            if (resetCounter == 128) {
                EEPROM_Utils::initEEPROM(true);
                arduboy.setRGBled(RED_LED, resetCounter);
                resetCounter = 0;

            }
            
        }
        else {

            resetCounter = 0;
            arduboy.setRGBled(RED_LED, 0);
            arduboy.setRGBled(BLUE_LED, 0);

        }
    #endif

    #ifdef SHOW_ROAR

        switch (counter) {

            case -1:
                drawElements(true, 0);
                break;

            case 0 ... 9:
                drawElements(false, 0);
                break;

            case 10:
                explosions.setExplosions(35, 35);
                drawElements(true, 1);
                break;

            case 11:

                #ifdef SOUNDS
                    for(uint8_t i=0; i<25; i++){
                      sound.tone(50, 10);
                      delay(50);
                      sound.noTone();
                      delay(5);
                    }
                    //sound.tone(NOTE_ROAR, 750);
                #endif

                drawElements(false, 1);
                Sprites::drawOverwrite(52 + ((counter % 3) - 1), 6 + ((counter % 3) - 1), Images::Roar, 0);
                break;

            case 12 ... 120:
                drawElements(false, 1);
                Sprites::drawOverwrite(52 + ((counter % 3) - 1), 6 + ((counter % 3) - 1), Images::Roar, 0);
                break;

            case 121 ... 130:
                drawElements(false, 1);
                break;

            case 131:
                drawElements(false, 0);

                if ((pressed & UP_BUTTON) || (pressed & DOWN_BUTTON) || (pressed & LEFT_BUTTON) || (pressed & RIGHT_BUTTON)) {

                    counter = 0;

                }
                else {
                    #ifdef SHOW_INSTUCTIONS
                        gameState = GameState::Instructions_Init;
                    #else
                        gameState = GameState::PlayGame_Init;
                    #endif
                }
                break;

        }

    #else
        drawElements(true, 0);
    #endif

    if (counter < 0) {

        #ifdef THREE_IMAGES

            switch (marqueeCounter) {

                case 0 ... 14:
                case 65 ... 79:
                case 130 ... 144:
                    break;

                case 15 ... 64:
                    Sprites::drawOverwrite(73, 54, Images::Mode, 0);
                    break;

                case 80 ... 129:
                    Sprites::drawOverwrite(73, 54, Images::Mode, 1);
                    break;

                case 145 ... 194:
                    {

                        uint8_t digits[3] = {};
                        extractDigits(digits, score);
                        Sprites::drawOverwrite(72, 54, Images::Hard, 0);
                        
                        for (uint8_t i = 3, x = 88; i > 0; i--, x = x + 4) {

                            Sprites::drawErase(x, 54, Images::Font, digits[i - 1]);

                        }

                    }
                    break;

            }

        #else

            switch (marqueeCounter) {

                case 0 ... 14:
                case 85 ... 99:
                    break;

                case 15 ... 84:
                    Sprites::drawOverwrite(59, 54, Images::Mode, 0);
                    Sprites::drawOverwrite(88, 54, Images::Mode, 1);
                    break;

                case 100 ... 165:
                    {

                        uint8_t digits[3] = {};
                        Sprites::drawOverwrite(64, 54, Images::High, 0);
                        
                        extractDigits(digits, EEPROM_Utils::getScore(GameMode::Easy));
                        for (uint8_t i = 3, x = 81; i > 0; i--, x = x + 4) {

                            Sprites::drawErase(x, 54, Images::Font, digits[i - 1]);

                        }
                        
                        extractDigits(digits, EEPROM_Utils::getScore(GameMode::Hard));
                        for (uint8_t i = 3, x = 96; i > 0; i--, x = x + 4) {

                            Sprites::drawErase(x, 54, Images::Font, digits[i - 1]);

                        }

                    }
                    break;

            }

        #endif

    }

    #ifdef SOUNDS

        if (soundCounter > 0) {
            Sprites::drawOverwrite(119, 55, Images::Sound, !sounds);
        }

    #endif


    // Render roar ..

    #ifdef SHOW_ROAR

        for (uint8_t i = 0; i < Constants::Particle_Count; i++) {

            Explosion explosion = explosions.getExplosion(i);

            if (explosion.getCounter() > 0) {
                Sprites::drawExternalMask(explosion.getX() + 1, explosion.getY(), Images::Pixel, Images::Pixel_Mask, 0, 0);
            }

        }

        explosions.update(arduboy.frameCount);

    #endif

}

void drawElements(bool title, uint8_t lionFrame) {

    if (title) Sprites::drawOverwrite(43, 10, Images::Title, 0);
    Sprites::drawOverwrite(0, 32, Images::Title_Lion_Bottom, lionFrame);

}
