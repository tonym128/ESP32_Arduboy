#pragma once

#include <ArduboyTones.h>

namespace Sounds {

    const uint16_t Theme[] PROGMEM = {
        0x003E,0x00FA, 0x0000,0x00FA, 0x003E,0x00FA, 0x0000,0x00FA, 0x003E,0x00FA, 0x0049,0x00FA, 0x0093,0x007D, 0x0062,0x007D, 0x003E,0x00FA, 
        0x0000,0x00FA, 0x0049,0x0177, 0x003E,0x0177, 0x0041,0x00FA, 0x0000,0x0177, 0x003E,0x00FA, 0x0000,0x00FA, 0x003E,0x00FA, 
        0x0000,0x00FA, 0x003E,0x00FA, 0x0052,0x0177, 0x007B,0x007D, 0x006E,0x007D, 0x0049,0x00FA, 0x0062,0x00FA, 0x0000,0x00FA, 
        0x0052,0x007D, 0x0041,0x00FA, 0x0000,0x0177, TONES_END 
    };

    const uint16_t Player_Fires_Bullet[] PROGMEM = {
        NOTE_C5,50, NOTE_C3,100, NOTE_C2,200, TONES_END
    };

    const uint16_t Enemy_Drops_Bomb[] PROGMEM = {
        NOTE_F3,22, NOTE_F3,22, NOTE_F3,22, TONES_END
    };

    const uint16_t player_hit_by_alien[] PROGMEM = {
        NOTE_F3,3, NOTE_D4,4, NOTE_A4,4, NOTE_E3,1, TONES_END
    };

    const uint16_t Player_Hit_By_Bomb[] PROGMEM = {
        NOTE_A3,9, NOTE_D3,10, NOTE_C3,6, NOTE_F3,10, TONES_END
    };

    const uint16_t Bomb_Explosion[] PROGMEM = {
        NOTE_A3,9, TONES_END
    };

    const uint16_t Player_Explosion[] PROGMEM = {
        NOTE_A3,9, NOTE_D3,10, NOTE_C3,6, NOTE_F3,10, 
        NOTE_B3, 9, NOTE_E3, 10, NOTE_A3,9, NOTE_D3,10, NOTE_C3,6, NOTE_F3,10, 
        NOTE_B3, 9, NOTE_E3, 10, NOTE_A3,9, NOTE_D3,10, NOTE_C3,6, NOTE_F3,10, 
        NOTE_B3, 9, NOTE_E3, 10, TONES_END
    };

    const uint16_t Enemy_Explosion[] PROGMEM = {
        NOTE_A3,9, NOTE_D3,10, NOTE_C3,6, NOTE_F3,10, 
        NOTE_B3, 9, NOTE_E3, 10, NOTE_A3,9, NOTE_D3,10, NOTE_C3,6, NOTE_F3,10, 
        NOTE_B3, 9, NOTE_E3, 10, NOTE_A3,9, NOTE_D3,10, NOTE_C3,6, NOTE_F3,10, 
        NOTE_B3, 9, NOTE_E3, 10, TONES_END
    };

    const uint16_t Wave_Cleared[] PROGMEM = {
        NOTE_A3,80, NOTE_C3,80, NOTE_E3,80, 
        NOTE_A4,80, NOTE_C4,80, NOTE_E4,80, 
        TONES_END
    };

    const uint16_t Game_Over[] PROGMEM = {
        NOTE_F2,360, NOTE_B1,360, NOTE_F2,360, NOTE_B1,360,
        NOTE_F3,180, NOTE_DS3,180, NOTE_F3,180, NOTE_DS3,180, NOTE_REST,25, NOTE_DS3,90, NOTE_REST,90, NOTE_DS3,90,
        NOTE_REST,90, NOTE_DS3,90, NOTE_REST,90, NOTE_DS3,90, NOTE_REST,90, NOTE_G3,360, NOTE_FS3,360, NOTE_G3,360, NOTE_E3,360,
        NOTE_F3,90, NOTE_E3,90, NOTE_F3,90, NOTE_E3,90, NOTE_F3,90, NOTE_E3,90, NOTE_F3,90, NOTE_E3,90, NOTE_F3,750, TONES_END
    };

}