
#pragma once

namespace Sounds {

    const uint16_t Success[] PROGMEM = 	            { NOTE_F4, 80, NOTE_G5, 80, TONES_END };    
    const uint16_t Failure[] PROGMEM = 	            { NOTE_G5, 80, NOTE_F4, 80, TONES_END };    
    const uint16_t InvalidChoice[] PROGMEM = 	    { NOTE_G4, 80, NOTE_F3, 80, TONES_END };
    const uint16_t LoadSaveGame[] PROGMEM = 	    { NOTE_G4, 60, NOTE_REST, 60, NOTE_G4, 60, TONES_END };
    const uint16_t ReticleSelect[] PROGMEM = 	    { NOTE_F3, 60, TONES_END };
    const uint16_t MemoryFull[] PROGMEM = 	        { NOTE_F4, 20, TONES_END };
    const uint16_t ReticleShown[] PROGMEM =        { NOTE_F4, 60, TONES_END };

    const uint16_t Score[] PROGMEM = {
        NOTE_A4,   333, NOTE_E4,   666, NOTE_G4,   333, NOTE_D4,  333, NOTE_REST, 333, NOTE_G4,   666, NOTE_E4,  1000, NOTE_E4,   333,
        NOTE_REST, 333, NOTE_A4,   333, NOTE_E4,   666, NOTE_G4,  333, NOTE_D4,   333, NOTE_REST, 333, NOTE_G4,   666, NOTE_E4,  1000,
        NOTE_E4,  1000, NOTE_REST, 333, NOTE_A4,   333, NOTE_E4,  666, NOTE_G4,   333, NOTE_D4,   333, NOTE_REST, 333, NOTE_G4,   666,
        NOTE_E4,  1000, NOTE_E4,  1000, NOTE_REST, 333, NOTE_A4,  333, NOTE_E4,   666, NOTE_G4,   333, NOTE_D4,   333, NOTE_REST, 333,
        NOTE_G4,   666, NOTE_E4,  2333,
        TONES_END
    };

}