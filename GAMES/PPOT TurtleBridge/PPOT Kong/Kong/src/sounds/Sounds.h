#pragma once

#include <ArduboyTones.h>

#define NOTE_JUMP 0

namespace Sounds {

  const uint16_t lever_on[]  = 	  { NOTE_F3, 20, NOTE_G4, 20, TONES_END };
  const uint16_t lever_off[]  = 	  { NOTE_G4, 20, NOTE_F3, 20, TONES_END };

  const uint16_t drop_barrel[]  =  { NOTE_F1, 50, NOTE_C1, 50, NOTE_A1, 50, TONES_END };
  const uint16_t jump_barrel[]  =  { NOTE_A4,75, NOTE_B4,75, NOTE_CS5,75, NOTE_G5,150, NOTE_DS5,150, TONES_END };
  const uint16_t jump[]  = 		    { NOTE_A4,75, NOTE_JUMP,2, TONES_END };
  const uint16_t hammer_time[]  =  { NOTE_A3,180, NOTE_A3,90, NOTE_A3,90, NOTE_A3,180, NOTE_A3,180,
                                            NOTE_CS4,180, NOTE_A3,180, NOTE_CS4,180, NOTE_A3,180, TONES_END };

  // https://tcrf.net/Donkey_Kong_%28Arcade%29#Unused_Music
  // https://github.com/furrykef/dkdasm/blob/master/dkong-snd.asm#L700
  const uint16_t unused_mischievous_cutscene_music[]  = { NOTE_F2,360, NOTE_B1,360, NOTE_F2,360, NOTE_B1,360,
    NOTE_F3,180, NOTE_DS3,180, NOTE_F3,180, NOTE_DS3,180, NOTE_REST,25, NOTE_DS3,90, NOTE_REST,90, NOTE_DS3,90,
    NOTE_REST,90, NOTE_DS3,90, NOTE_REST,90, NOTE_DS3,90, NOTE_REST,90, NOTE_G3,360, NOTE_FS3,360, NOTE_G3,360, NOTE_E3,360,
    NOTE_F3,90, NOTE_E3,90, NOTE_F3,90, NOTE_E3,90, NOTE_F3,90, NOTE_E3,90, NOTE_F3,90, NOTE_E3,90, NOTE_F3,750,
    TONES_END };

  const uint16_t falling[]  = {
    NOTE_G5, 50, NOTE_E5, 50, NOTE_C5, 50, NOTE_A5, 50,
    NOTE_F4, 50, NOTE_D4, 50, NOTE_B4, 50,
    NOTE_G3, 50, NOTE_E3, 50, NOTE_C3, 50, NOTE_A3, 50,
    NOTE_F2, 50, NOTE_D2, 50, NOTE_B2, 50,
    NOTE_B2, 50, NOTE_REST, 20, NOTE_B2, 50, NOTE_REST, 20, NOTE_B2, 50,
    TONES_END
  };

  const uint16_t crash_into_barrel[]  = {
    NOTE_G5, 50, NOTE_E5, 50, NOTE_C5, 50, NOTE_A5, 50,
    NOTE_F4, 50, NOTE_D4, 50, NOTE_B4, 50,
    TONES_END
  };

  const uint16_t mission_success[]  = {
    NOTE_FS5,498, NOTE_REST,144, NOTE_FS5,34, NOTE_REST,72, NOTE_FS5,34, NOTE_REST,72, NOTE_FS5,177,
    NOTE_REST,108, NOTE_FS5,105, NOTE_REST,36, NOTE_FS5,70, NOTE_REST,72, NOTE_GS5,105, NOTE_REST,36, NOTE_AS5,105,
    NOTE_REST,36, NOTE_B5,1034, NOTE_REST,251, NOTE_B5,105, NOTE_REST,36, NOTE_CS6,70, NOTE_REST,72, NOTE_DS6,70,
    NOTE_REST,72, NOTE_F6,605, NOTE_REST,36, NOTE_F6,34, NOTE_REST,72, NOTE_F6,34, NOTE_REST,72, NOTE_F6,34,
    NOTE_REST,179, NOTE_CS6,177, NOTE_REST,36, NOTE_F6,70, NOTE_REST,72, NOTE_FS6,117, NOTE_REST,24, NOTE_GS6,105,
    NOTE_REST,36, NOTE_FS6,1641,
    TONES_END
  };

}