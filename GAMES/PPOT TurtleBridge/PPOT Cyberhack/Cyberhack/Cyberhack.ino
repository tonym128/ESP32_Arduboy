#include "src/utils/Arduboy2Ext.h"
#include "src/entities/Entities.h"
#include "src/images/Images.h"
#include "src/fonts/Font3x5.h"
#include "src/utils/Constants.h"
#include "src/utils/Structs.h"
#include "src/utils/EEPROM_Utils.h"

#ifdef ARDUBOY_TONES_SOUNDS
#include <ArduboyTones.h>
#include "src/sounds/Sounds.h"
#endif

using FlashStringHelper = const __FlashStringHelper *;

#ifndef DEBUG
//ARDUBOY_NO_USB
#endif 
Arduboy2Ext arduboy;

#ifdef ARDUBOY_TONES_SOUNDS
ArduboyTones sound(arduboy.audio.enabled);
#endif

#ifdef BYTE_BEAT_SOUNDS

// interrupt service routines
ISR(TIMER1_OVF_vect) {
    byteBeatStep();
}

ISR(TIMER1_CAPT_vect) {
    byteBeatStep();
}

long t;
#ifdef BYTE_BEAT_SOUNDS_1
uint8_t hpISR = 46;
#endif
#ifdef BYTE_BEAT_SOUNDS_3
uint8_t byteBeatIdx = ByteBeatIndex::Map;
#endif

void byteBeatStep()
{
    t++;
    #ifdef BYTE_BEAT_SOUNDS_1
    OCR4A = ((t*(t>>8|t>>18)&hpISR&t>>3))^(t&t>>8|t>>20);
    #endif
    #ifdef BYTE_BEAT_SOUNDS_3
    switch (byteBeatIdx) {

        case ByteBeatIndex::Hack:
            OCR4A = ((t*((t>>8)|(t>>18))&33&t>>3))^((t&t>>8)|(t>>20));
            break;

        case ByteBeatIndex::Map:
            OCR4A = (((t>>7)|((t&t)>1))|(t&t>>8)|(t>>7))|(t+(t>>8))|((t>>7));
            break;

        case ByteBeatIndex::Street:
            OCR4A = ((t>>6)^(t&0x25))|((t+(t^(t>>11))-t* ((t%24?2:6)&t>>11))^(t<<1&(t&0x256?t>>4:t>>10)));
            break;
            
    }
    #endif

}
#endif

Font3x5 font3x5 = Font3x5(6);
GameState gameState = GameState::Splash_Init;
MapGameVars mapGameVars;
StealthGameVars stealthGameVars;
HackGameVars hackGameVars;
TVSequenceVars tvSequenceVars;
Player player;

#ifdef TV_TRANSITION
    GameState nextState = GameState::Splash_Init;
#endif

void setup() {
  
	arduboy.boot();
	arduboy.systemButtons();
    arduboy.setFrameRate(50);
    arduboy.initRandomSeed();
    arduboy.audio.begin();

    #ifdef ARDUBOY_TONES_SOUNDS
    arduboy.audio.begin();
    #endif

    EEPROM_Utils::initEEPROM(false);
    initByteBeat();

}   


void loop() {

    if ( !arduboy.nextFrame() ) return;    
	arduboy.pollButtons();

    switch (gameState) {

        case GameState::Splash_Init:

            splashScreen_Init();
            splashScreen();
            break;

        case GameState::Splash:

            splashScreen();
            // arduboy.display();
            break;

        case GameState::Title_Init:

            title_Init();
            title();
            break;

        case GameState::Title:

            title();
            break;

        case GameState::HackGame_Init:

            hackGame_Init();
            hackGame();
            break;

        case GameState::HackGame:

            hackGame();
            break;

        case GameState::HackOver_Disconnected:
        case GameState::HackOver_BufferFull:
        case GameState::HackOver_CannotMove:
        case GameState::HackOver_AllHacksPlayed:

            hackGame_HackOver();
            break;

        case GameState::HackScore:

            hackGame_HackScore();
            break;

        case GameState::ShowMap_Init:

            showMap_Init();
            showMap();
            break;

        case GameState::ShowMap:

            showMap();
            break;

        case GameState::Messages_Start_Init ... GameState::Messages_End_Init:

            showMap_Message_Init();
            break;

        case GameState::Messages_Start ... GameState::Messages_End:

            showMap_Message();
            break;

        case GameState::StealthGame_Init:

            stealthGame_Init();
            stealthGame();
            break;

        case GameState::StealthGame:

            stealthGame();
            break;

        #ifdef TV_TRANSITION

            case GameState::TV_Init:

                tv_Init();
                tv();
                break;

            case GameState::TV:

                tv();
                break;
            
        #endif

        #ifdef CREDITS

            case GameState::Credits_Init:

                credits_Init();
                credits();
                break;

            case GameState::Credits:

                credits();
                break;
            
        #endif

    }

    arduboy.display(true);

}
void gameLogicLoop(void *)
{
  for (;;) {
    inogameloop(); 
    // ArduinoOTA.handle();
  }
}

void setup() {
  inogamesetup();
  xTaskCreatePinnedToCore(gameLogicLoop, "g", 4096, nullptr, 0, nullptr, 0);
}

void loop() {
	delay(60000);
}
