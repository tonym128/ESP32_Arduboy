#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduboy2.h>
#include "bmp.h"

extern Arduboy2 arduboy;
extern Sprites sprite;
typedef void (*FunctionPointer) ();
extern FunctionPointer gameState, retState;
extern uint16_t dialoguePosition;
extern uint8_t stateVars[4], stateInitialized;
extern uint16_t frame, camera;
extern uint8_t _justPressed;
extern uint8_t level, msgDelivered;
extern uint16_t nextMap, prevMap;
#define HIGHBYTE(x) ((uint8_t *) &x)[1]
#define LOWBYTE(x) ((uint8_t *) &x)[0]

#define justPressed( x ) (_justPressed&(x))

typedef const __FlashStringHelper *PGMSTR;

extern const int8_t PROGMEM sincos_[];

inline int8_t SIN( int16_t x){
    if( x < 0 )
        x = 256-x;
    return pgm_read_byte(sincos_+(x&0xFF));
}

inline int8_t COS( int16_t x ){
    return SIN(x+180);
}

void waitXFrames( uint8_t t, bool redraw=true );

uint32_t isqrt( uint32_t a_nInput );

extern PGMSTR printstr;
extern uint8_t printstrX, printstrY;

#define MAX_ENEMY_COUNT 10
#define MAX_PARTY_SIZE 4

extern const char PROGMEM TXT_A[];
extern const char PROGMEM TXT_AP[];
extern const char PROGMEM TXT_COST[];
extern const char PROGMEM TXT_NOAP[];

void stateReset();
void stateGameStart();
void stateBossTalk();
void statePartyManagement();
void statePlayerTurn();
void stateRoundStart();
void stateElfTurn();

#endif
