#include "globals.h"
#include "person.h"
#include "player.h"

void statePartyManagement(){
    int8_t &selectionId = * (int8_t *) &stateVars[0];
    int16_t &scrollPos = * (int16_t *) &stateVars[1];
    uint8_t &max = stateVars[3];
    max = sizeof(player.soldiers)/sizeof(player.soldiers[0]);
    
    if( !stateInitialized ){
        
        uint8_t selected = 0;
        selectionId = -1;
        scrollPos = 0;
        arduboy.setFrameRate(30);

        for( uint8_t i=0; i<max; ++i ){
            if( selected<MAX_PARTY_SIZE && player.soldiers[i].selectable() )
                player.soldiers[i].tileY = ++selected;
            else
                player.soldiers[i].tileY = 0xFF;
        }
        
    }
    
    scrollPos = (scrollPos*7 + selectionId*0x80)>>3;
    camera = 0xFFFF;

    if( justPressed(UP_BUTTON) && selectionId > -1 )
        selectionId--;
    else if( justPressed(DOWN_BUTTON) && selectionId < MAX_PARTY_SIZE-1 )
        selectionId++;
     
    uint16_t occupied = 0;
        
    if( selectionId == -1 ){
        // arduboy.setCursor( 5, 5 );
        // arduboy.print( F("[A]   Begin Mission") );
        printstrX = printstrY = 5;
        printstr = F("[A]   Begin Mission");
        arduboy.setCursor( 5, 5+10 );
        arduboy.print( F("[DOWN] Manage Squad") );
        
        if( justPressed(A_BUTTON) ){
            gameState = stateGameStart;
            return;
        }
        
        for( uint8_t i=0; i<max; ++i ){
            Person &p = player.soldiers[i];
            if( p.tileY != 0xFF ){
                arduboy.setCursor( 5, 5+10+(p.tileY*10) );
                arduboy.print( p.name );
                arduboy.print( F(" Lv:") );
                arduboy.print( p.level() );
                p.printHP();
                occupied |= 1 << (p.tileY-1);
            }
        }
        
        for( uint8_t i=0; i<MAX_PARTY_SIZE; ++i ){
            if( occupied >> i ) continue;
            arduboy.setCursor( 5, 5+20+(i*10) );
            arduboy.print( F("<EMPTY>") );
        }
        return;
    }
    
    for( uint8_t i=0; i<max; ++i ){
        Person &p = player.soldiers[i];
        if( p.tileY == 0xFF ) continue;
        int16_t y = ( ( ( (p.tileY)<<4 ) - (scrollPos>>3) ) * 45 >> 4 ) - 35;
        if( y < -50 || y >= HEIGHT ) continue;
        p.render( 0, (int8_t) y + (((frame>>3)+i)&1), arduboy.everyXFrames(47) );
        occupied |= 1 << (p.tileY-1);
    }
    
    for( uint8_t i=0; i<MAX_PARTY_SIZE; ++i ){
        if( occupied >> i ) continue;
        int16_t y = ( ( ( (i+1)<<4 ) - (scrollPos>>3) ) * 45 >> 4 ) - 35;
        if( y < -50 || y >= HEIGHT ) continue;
        // sprite.drawExternalMask( 0, y, base1, NULL, 0, 0 );
        sprite.draw( 0, y, base1, 0, NULL, 0, SPRITE_AUTO_MODE );
    }
    
    Person &person = player.soldiers[selectionId];
    if( person.tileY != 0xFF ){
        arduboy.setCursor( 35, 5 );
        arduboy.print( person.name );
        arduboy.print( F(" Lv:") );
        arduboy.print( person.level() );
        // arduboy.setCursor( 40, 5+8 );
        // arduboy.print( F("Exp: ") );
        printstrX = 40;
        printstrY = 5+8;
        printstr = F("Exp: ");
        arduboy.print( person.experience );
        arduboy.setCursor( 40, 5+8*2 );
        arduboy.print( F("Role: ") );
        arduboy.print( person.role() );
        arduboy.setCursor( 40, 5+8*3 );
        arduboy.print( F("Health: ") );
        arduboy.print( (person.health*100L) / person.defHealth );
        arduboy.print( F("%") );
        
        // for debugging
        // if( justPressed(A_BUTTON) ) person.randomize();
        
    }else{
        // arduboy.setCursor( 40, 8 );
        // arduboy.print( F("<EMPTY>") );
        printstrX = 40;
        printstrY = 8;
        printstr = F("<EMPTY>");
        
    }
    
}
