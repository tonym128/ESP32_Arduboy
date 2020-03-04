#include "globals.h"
#include "player.h"

/* * /
uint8_t dialoguePosition;
void stateBossTalk(){
    bool live = false;
    uint8_t nextState = state+1;
    PGMSTR msg = NULL;
    if( !stateInitialized ){
        camera = 0xFFFF;
        for( uint8_t i=0; i<MAX_PARTY_SIZE; ++i ){
            if( player.soldiers[i].selectable() ){
                live = true;
                break;
            }
        }
    }
    
    switch( dialoguePosition ){
    case 0: msg = F("  Hello, Commander."); break;
    case 1: msg = F("We have a problem."); break;
    }
    
    if( justPressed(A_BUTTON) )
        state = nextState;
    
    if( !live )
        while( pgm_read_byte(dialogue+dialoguePosition++) != 4 );
    else{
        level++;
        if( (level+3)%5 == 0 )
            player.spawn();
    }
}
/*/
const char dialogue[] PROGMEM = 
    "  Hello, Commander.\x00" 
    "We have a problem.\x00"
    "\"Elves\".\n"
    "Little green men.\x00"
    "Taking them out is\n"
    "someone else's job.\x00"
    "You deliver the mail\n"
    "while coms are out.\x00"
    "\x01"
    "Take this message\n"
    "to the next bunker.\x00"
    "\x02"
    "\x00"
    "\x04"
    "You have failed,\n"
    "Commander.\x00"
    "\x03"
    "\x05"
    "No chocolate for\n"
    "you, Commander.\x00"
    "\x00"
    "\x06"
    "  Hello, Commander.\x00"
    "\x00"
    ;


uint16_t dialoguePosition, marker;
uint16_t nextDialoguePosition;
void stateBossTalk(){
    camera = 0xFFFF;
    if( !stateInitialized ){
        bool live = false;
        for( uint8_t i=0; i<MAX_PARTY_SIZE; ++i ){
            Person &p = player.soldiers[i];
            if( p.selectable() ){
                live = true;
                int8_t heal = p.defHealth - p.health;
                if( heal>10 ) heal=10;
                p.health += heal;
            }
        }
        uint8_t jmp = 0;
        if( !live ) jmp = 4;
        else if( !msgDelivered ) jmp = 5;
        else if( level ) jmp = 6;
        if( jmp )
            while( pgm_read_byte(dialogue+dialoguePosition++) != jmp );
    }
    
    if( justPressed(A_BUTTON) )
        dialoguePosition = nextDialoguePosition;
    
    nextDialoguePosition = dialoguePosition;
    
    char b = pgm_read_byte(dialogue+dialoguePosition);
    switch( b ){
    case 0: 
        dialoguePosition = nextDialoguePosition = marker; 
        if( (level+3)%5 == 0 )
            player.spawn();
        break;
    case 1: marker = ++dialoguePosition; break;
    case 2: dialoguePosition++; level++; gameState = statePartyManagement; return;
    case 3: gameState = stateReset; return;
    }
    
    char nl = 1;
    while( (b=pgm_read_byte(dialogue+nextDialoguePosition++) ) ) nl += b == '\n';
    
    sprite.drawOverwrite( 0, 0, boss, 0 );
    
    uint8_t *fbend = Arduboy2::sBuffer+HEIGHT/8*WIDTH;
    for( uint8_t *b=fbend-nl*WIDTH; b<fbend; *b++=0 );
    
    // arduboy.setCursor( 0, 64-nl*8 );
    printstrX = 0;
    printstrY = 64-nl*8;
    printstr = PGMSTR(dialogue+dialoguePosition);
    // arduboy.print( PGMSTR(dialogue+dialoguePosition) );
    
    
    if( (frame>>1) & 1 ){
        arduboy.setCursor( 128-6*3, 0 );
        arduboy.print( PGMSTR(TXT_A) );
    }
    
}
/* */