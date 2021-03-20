#pragma once

namespace Sounds {

    const uint16_t catchOil[] PROGMEM = 	    { NOTE_F3, 20, NOTE_G4, 20, TONES_END };
    const uint16_t fireStarted[] PROGMEM =      { NOTE_F1, 50, NOTE_C1, 50, NOTE_A1, 50, TONES_END };
    const uint16_t scoreAdded[] PROGMEM =       { NOTE_A3,180, NOTE_D3,180, NOTE_E4,180, TONES_END };

    const uint16_t score[] PROGMEM = {
    // Untitled
    // NOTE_G4,312, NOTE_D5,156, NOTE_G4,312, NOTE_D5,468, NOTE_G5,156, NOTE_FS4,156, NOTE_G4,156, NOTE_B4,156,
    // NOTE_E5,312, NOTE_FS4,156, NOTE_G4,312, NOTE_B4,468, NOTE_G5,156, NOTE_G5,156, NOTE_G4,156, NOTE_B4,156,
    // NOTE_G4,312, NOTE_D5,156, NOTE_G4,312, NOTE_D5,468, NOTE_G5,156, NOTE_FS4,156, NOTE_G4,156, NOTE_B4,156,
    // NOTE_E5,312, NOTE_FS4,156, NOTE_G4,312, NOTE_B4,468, NOTE_G5,156, NOTE_G5,156, NOTE_G4,156, NOTE_B4,156,
    // NOTE_C5,156, NOTE_E5,156, NOTE_G5,156, NOTE_D5,312, NOTE_G5,156, NOTE_B4,156, NOTE_D5,156, NOTE_G5,156,
    // NOTE_D5,468, NOTE_D5,1875,
    // TONES_END
    NOTE_G4,416, NOTE_D5,208, NOTE_G4,416, NOTE_D5,623, NOTE_G5,208, NOTE_FS4,208, NOTE_G4,208, NOTE_B4,208,
    NOTE_E5,416, NOTE_FS4,208, NOTE_G4,416, NOTE_B4,623, NOTE_G5,208, NOTE_G5,208, NOTE_G4,208, NOTE_B4,208,
    NOTE_G4,416, NOTE_D5,208, NOTE_G4,416, NOTE_D5,623, NOTE_G5,208, NOTE_FS4,208, NOTE_G4,208, NOTE_B4,208,
    NOTE_E5,416, NOTE_FS4,208, NOTE_G4,416, NOTE_B4,623, NOTE_G5,208, NOTE_G5,208, NOTE_G4,208, NOTE_B4,208,
    NOTE_C5,208, NOTE_E5,208, NOTE_G5,208, NOTE_D5,416, NOTE_G5,208, NOTE_B4,208, NOTE_D5,208, NOTE_G5,208,
    NOTE_D5,623, NOTE_D5,1875,
    TONES_END
    };
//312 = 416, 156 = 208, 468 =623

    const uint16_t falling[] PROGMEM = {
    NOTE_G5, 50, NOTE_E5, 50, NOTE_C5, 50, NOTE_A5, 50,
    NOTE_F4, 50, NOTE_D4, 50, NOTE_B4, 50,
    NOTE_G3, 50, NOTE_E3, 50, NOTE_C3, 50, NOTE_A3, 50,
    NOTE_F2, 50, NOTE_D2, 50, NOTE_B2, 50,
    NOTE_B2, 50, NOTE_REST, 20, NOTE_B2, 50, NOTE_REST, 20, NOTE_B2, 50,
    TONES_END
    };

}

