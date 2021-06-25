#include <Arduboy2.h>

void title_Init() {

    gameState = GameState::Title;
    arduboy.resetFrameCount();

    clearLEDs();

    #ifdef ARDUBOY_TONES_SOUNDS
        sound.tones(Sounds::Score);
    #endif

}   

void title() {

    Sprites::drawOverwrite(0, 0, Images::Title_00, 0);
    Sprites::drawOverwrite(73, 13, Images::Title_01, 0);
    Sprites::drawOverwrite(51, 46, Images::Title_01, 0);
    Sprites::drawOverwrite(5, 17, Images::Title_02, 0);

    flickering(17);

    arduboy.fillRect(20, 4, WIDTH, 4, WHITE);
    arduboy.drawFastHLine(0, 13, 73);
    arduboy.drawFastHLine(13, 9, WIDTH);
    arduboy.drawFastHLine(55, 46, WIDTH);
    arduboy.drawFastHLine(0, 50, 25);
    arduboy.fillRect(0, 52, 25, 4, WHITE);
    arduboy.drawRect(25, 50, 77, 11);
    font3x5.setCursor(29, 52);

    if (arduboy.getFrameCountHalf(254)) {
        font3x5.print(F("PRESS~'A'~TO~BREACH"));
    }
    else {
        font3x5.print(F("HOLD   ~BOOTLOADER")); 
        Sprites::drawSelfMasked(46, 53, Images::UpDown, 0);
    }

    Sprites::drawSelfMasked(51, 46, Images::Title_01, 0);

    if (arduboy.justPressed(A_BUTTON)) { 

        player.reset();
        mapGameVars.setMessage(Message::Intro_1);

        gameState = GameState::Message_Intro_Init;

        #ifdef ARDUBOY_TONES_SOUNDS
            sound.noTone();
        #endif

        #ifdef BYTE_BEAT_SOUNDS
            enableByteBeat();
        #endif

    }

}
