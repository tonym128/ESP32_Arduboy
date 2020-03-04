#include "globals.h"
#include "world.h"
#include "player.h"
#include "enemy.h"
#include "person.h"


void stateDirectSoldier();
void statePlayerWalk();
void statePlayerShoot();
void stateDuck();
void stateReload();

bool turn(Person *soldiers, uint8_t count){
    int8_t &soldierId = * (int8_t *) &stateVars[0];

    soldierId--;
    nextSoldier( soldiers, soldierId, count, 1 );

    Person &soldier = soldiers[soldierId];
    return !soldier.points;

}

void statePlayerTurn(){
    int8_t &soldierId = * (int8_t *) &stateVars[0];
    uint8_t &tmp = stateVars[1];
    if( turn(player.soldiers, MAX_PARTY_SIZE) ){
        gameState = stateElfTurn;
        return;
    }
    
    Person &soldier = player.soldiers[soldierId];
    camera = soldier.pos;
    arduboy.setCursor(38, 0);
    arduboy.print(soldier.name);
    
    tmp = 0;
    if( justPressed(B_BUTTON) ){
        gameState = stateDirectSoldier;
        return;
    }
    if( justPressed(A_BUTTON) ){
        retState = gameState;
        gameState = statePlayerWalk;
        return;
    }
    if( justPressed(LEFT_BUTTON) ) tmp = -1;
    if( justPressed(RIGHT_BUTTON) ) tmp = 1;

    if( tmp )
        nextSoldier( player.soldiers, soldierId, MAX_PARTY_SIZE, tmp );

    printstrX = 1;
    printstrY = 56;
    printstr = F("A] Walk  B] Commands ");
}

const char TXT_WALK[] PROGMEM =   "  WALK  ";
const char TXT_SHOOT[] PROGMEM =  "  SHOOT ";
const char TXT_DUCK[] PROGMEM =   "  DUCK  ";
const char TXT_RELOAD[] PROGMEM = " RELOAD ";
const char TXT_ENDT[] PROGMEM =   "END TURN";
const char * const OPTS[] PROGMEM = { TXT_WALK, TXT_SHOOT, TXT_DUCK, TXT_RELOAD, TXT_ENDT };
const FunctionPointer playerStates[] PROGMEM = {statePlayerWalk, statePlayerShoot, stateDuck, stateReload, stateElfTurn};

void stateDirectSoldier(){
    uint8_t &tmp = stateVars[1];
    uint8_t &opt = stateVars[2];
    uint8_t &soldierId = stateVars[0];
    Person &soldier = player.soldiers[soldierId];

    if( !stateInitialized ){
        tmp = 0;
        opt = 0;
        retState = gameState;
        printstr = NULL;
    }

    camera = soldier.pos;

    arduboy.setCursor(38, 0);
    arduboy.print(soldier.role());
    arduboy.print(soldier.name);

    arduboy.setCursor(0, 56);
    if( tmp<10 ){
        tmp++;
        PGMSTR text = NULL;
        if( soldier.id == 0 ) text = F("Y-yes, boss?");
        else if( soldier.flags & HSTL ) text = F("LEMME AT EM!");
        else if( soldier.flags & LAZY ) text = F("What now?");
        else text = F("Yes?");
        arduboy.print(text);
    }else{
        // print stats
        arduboy.print(PGMSTR(TXT_AP));
        arduboy.print(soldier.points);
        arduboy.print(F(" AMMO:"));
        arduboy.print(soldier.ammo);
        soldier.printHP();
    }

    if( justPressed(DOWN_BUTTON) ){
        opt = (opt+1) % 5;
    }else if( justPressed(UP_BUTTON) ){
        opt--;
        if( opt & 0x80 ) opt = 4;
    }else if( justPressed(B_BUTTON) ){
        gameState = statePlayerTurn;
        return;
    }else if( justPressed(A_BUTTON) ){
        gameState = (FunctionPointer)pgm_read_dword(playerStates+opt);
    }

    soldier.render( 0, 0, arduboy.everyXFrames(47) );
    
    for( uint8_t i=0; i<5; ++i ){
        arduboy.setCursor(128-6*8, 8+i*8);
        if( i!=opt || frame&2 )
            arduboy.print(PGMSTR(pgm_read_dword(OPTS+i)));
    }
    
}

void stateWalk(){
    gameState = retState;
}

void statePlayerWalk(){
    uint8_t &soldierId = stateVars[0];
    int8_t &ox = *(int8_t *)&stateVars[1];
    int8_t &oy = *(int8_t *)&stateVars[2];
    
    Person &soldier = player.soldiers[soldierId];
    if( !stateInitialized ){
        ox = 0;
        oy = 0;
        printstr = NULL;
    }

    world.tiles[soldier.tileY+oy][soldier.tileX+ox] &= 0x7F;

    if( (justPressed(LEFT_BUTTON) || (ox == 1 && soldier.tileX == 14)) && ox > -1 && soldier.tileX >= 1 )
        ox--;
    if( (justPressed(RIGHT_BUTTON) || (ox == -1 && soldier.tileX == 1)) && ox < 1 && soldier.tileX < 15 )
        ox++;
    if( (justPressed(UP_BUTTON) || (oy == 1 && soldier.tileY == 30)) && oy > -1 && soldier.tileY >= 1 )
        oy--;
    if( (justPressed(DOWN_BUTTON) || (oy == -1 && soldier.tileY == 1)) && oy < 1 && soldier.tileY < 31 )
        oy++;
    
    unsigned char &tile = world.tiles[soldier.tileY+oy][soldier.tileX+ox];
    tile |= 0x80;
    
    HIGHBYTE(camera) = soldier.tileX+ox;
    LOWBYTE(camera) = soldier.tileY+oy;

    uint8_t cost = world.cost( soldier.tileX, soldier.tileY, ox, oy );
    arduboy.setCursor( 0, oy>0?0:56 );
    // arduboy.print(PGMSTR(TXT_AP) );
    arduboy.print(soldier.points);
    if( cost != 0xFF ){
        arduboy.print(PGMSTR(TXT_COST) );
        arduboy.print(cost);
    }else if( (tile & 0x1C) == 0x10 ){
        arduboy.print(F(" ENTER BUNKER"));
        if( justPressed(A_BUTTON) ){
            tile &= 0x7F;
            world.togglePerson( soldier, 0, 1 );
            soldier.points = 0;
            soldier.tileY = 0xFF;
            gameState = retState;
            msgDelivered++;
            return;
        }
    }else{
        arduboy.print(F(" BLOCKED"));
    }
    if( cost <= soldier.points ){
        if( justPressed(A_BUTTON) ){
            tile &= 0x7F;
            world.togglePerson( soldier, 0, 1 );
            soldier.tileX += ox;
            soldier.tileY += oy;
            soldier.points -= cost;
            world.togglePerson( soldier, 0, 0 );
            if( soldier.points == 0 )
                gameState = retState;
            return;
        }
    }

    if( justPressed(B_BUTTON) ){
        tile &= 0x7F;
        gameState = retState;
        return;
    }
        
}


bool checkNOAP( Person &soldier, uint8_t points ){
    
    if( soldier.points >= points )
        return false;
        
    // arduboy.setCursor(0,56);
    // arduboy.print(PGMSTR(TXT_NOAP));
    printstrX = 0;
    printstrY = 56;
    printstr = PGMSTR(TXT_NOAP);
    
    if( justPressed(B_BUTTON))
        gameState = retState;
        
    return true;
    
}

void statePlayerShoot(){
    uint8_t &soldierId = stateVars[0];
    Person &soldier = player.soldiers[soldierId];

    uint8_t tx=soldier.tileX, ty=soldier.tileY;
    
    PGMSTR msg = NULL;
    
    if( !stateInitialized )
        stateVars[1] = 0;
        
    if( checkNOAP(soldier, 5) ) return;

    if( soldier.ammo == 0 ){
        msg = F("No ammo. Reload!");
    }else{
        tx = 0xFF;
        uint8_t count = soldier.scan( 0x40, tx, ty, stateVars[1] );
        
        if( tx == 0xFF ){
            
            msg = F("No target");
            if( stateVars[1] == 0xFF ){
                stateVars[1] = count-1;
                return;
            }
            if( stateVars[1] ) {
                stateVars[1] = 0;
                return;
            }
            tx = soldier.tileX;
            
        }else{

            world.tiles[ty][tx] &= 0x7F;
            msg = F("A] Shoot  B] Cancel");
            if( justPressed(A_BUTTON) ){
                soldier.shoot( tx, ty, enemy.soldiers, MAX_ENEMY_COUNT );
                gameState = statePlayerTurn;
                return;
            }else if( justPressed(LEFT_BUTTON) )
                stateVars[1]--;
            else if( justPressed(RIGHT_BUTTON) )
                stateVars[1]++;
            else if( !justPressed(B_BUTTON) )
                world.tiles[ty][tx] |= 0x80;
            
        }
    }
    
    HIGHBYTE(camera) = tx;
    LOWBYTE(camera) = ty;
    // world.render( tx, ty );

    // arduboy.setCursor(0,56);
    // arduboy.print(msg);
    printstrX = 0;
    printstrY = 56;
    printstr = msg;
    
    if( justPressed(B_BUTTON) )
        gameState = retState;
}

void stateDuck(){
    
    uint8_t &soldierId = stateVars[0];
    Person &soldier = player.soldiers[soldierId];
    
    if( checkNOAP(soldier, 5) ) return;
    
    // arduboy.setCursor(40, 8);
    // arduboy.print(F("*QUACK*"));
    printstrX = 40;
    printstrY = 56;
    printstr = F("*QUACK*");
    
    soldier.perks |= PERKT_HIDEF;
    soldier.points = 0;
    gameState = statePlayerTurn;
    
    waitXFrames(10);
    
}

void stateReload(){
    uint8_t &soldierId = stateVars[0];
    Person &soldier = player.soldiers[soldierId];

    if( checkNOAP(soldier, 5) ) return;

    soldier.reload();    
    gameState = statePlayerTurn;
}
