
#pragma once

namespace Sounds {

    const uint16_t Success[]  = 	            { NOTE_F4, 80, NOTE_G5, 80, TONES_END };    
    const uint16_t Failure[]  = 	            { NOTE_G5, 80, NOTE_F4, 80, TONES_END };    
    const uint16_t InvalidChoice[]  = 	    { NOTE_G4, 80, NOTE_F3, 80, TONES_END };
    const uint16_t LoadSaveGame[]  = 	    { NOTE_G4, 60, NOTE_REST, 60, NOTE_G4, 60, TONES_END };
    const uint16_t ReticleSelect[]  = 	    { NOTE_F3, 60, TONES_END };
    const uint16_t MemoryFull[]  = 	        { NOTE_F4, 20, TONES_END };
    const uint16_t ReticleShown[]  =        { NOTE_F4, 60, TONES_END };

    const uint16_t Score[]  = {
        NOTE_A4,   333, NOTE_E4,   666, NOTE_G4,   333, NOTE_D4,  333, NOTE_REST, 333, NOTE_G4,   666, NOTE_E4,  1000, NOTE_E4,   333,
        NOTE_REST, 333, NOTE_A4,   333, NOTE_E4,   666, NOTE_G4,  333, NOTE_D4,   333, NOTE_REST, 333, NOTE_G4,   666, NOTE_E4,  1000,
        NOTE_E4,  1000, NOTE_REST, 333, NOTE_A4,   333, NOTE_E4,  666, NOTE_G4,   333, NOTE_D4,   333, NOTE_REST, 333, NOTE_G4,   666,
        NOTE_E4,  1000, NOTE_E4,  1000, NOTE_REST, 333, NOTE_A4,  333, NOTE_E4,   666, NOTE_G4,   333, NOTE_D4,   333, NOTE_REST, 333,
        NOTE_G4,   666, NOTE_E4,  2333,
        TONES_END
    };

}