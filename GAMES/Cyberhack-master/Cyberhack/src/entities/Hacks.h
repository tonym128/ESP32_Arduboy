#pragma once

namespace Hacks {
    /*                                      G  B   T   C   M   M |         Clue 1            |          Clue 2           |          Clue 3           |          Clue 4           |          Clue 5           |          Clue 6           |
                                            R  U   I   L   A   I |                           |                           |                           |                           |                           |                           | 
                                            I  F   M   U   X   N |   L   M     O I      O S  |   L   M     O I      O S  |   L   M     O I      O S  |   L   M     O I      O S  |   L   M     O I      O S  |   L   M     O I      O S  |
                                            D  F   E   E         |   E   A     V N      V T  |   E   A     V N      V T  |   E   A     V N      V T  |   E   A     V N      V T  |   E   A     V N      V T  |   E   A     V N      V T  |
                                               E   R   S   V   T |   N   X     E D      E A  |   N   X     E D      E A  |   N   X     E D      E A  |   N   X     E D      E A  |   N   X     E D      E A  |   N   X     E D      E A  |
                                               R           A   R |   G         R E      R R  |   G         R E      R R  |   G         R E      R R  |   G         R E      R R  |   G         R E      R R  |   G         R E      R R  |
                                                           L   G |   T         L X      L T  |   T         L X      L T  |   T         L X      L T  |   T         L X      L T  |   T         L X      L T  |   T         L X      L T  |
                                                           U   T |   H         A        A    |   H         A        A    |   H         A        A    |   H         A        A    |   H         A        A    |   H         A        A    |
                                                           E     |             P        P    |             P        P    |             P        P    |             P        P    |             P        P    |             P        P    |
    */
    const uint8_t PROGMEM hacks_01_00[] = { 4, 5,  15, 3, 32,  7,    3, 16, NO_HACK_ID,   0,     2, 16,     0,        2,      2, 16,    1,        1 }; // 14
    const uint8_t PROGMEM hacks_01_01[] = { 4, 5,  15, 3, 24, 10,    4, 16, NO_HACK_ID,   0,     3, 16,     0,        2,      2, 16,    1,        1 }; // 26
    const uint8_t PROGMEM hacks_01_02[] = { 4, 5,  15, 4, 32, 16,    2, 16, NO_HACK_ID,   0,     3, 16,     0,        1,      4, 16,    1,        1,     3, 16, NO_HACK_ID,   0, }; // 34
    const uint8_t PROGMEM hacks_01_03[] = { 4, 5,  15, 5, 32, 16,    2, 16, NO_HACK_ID,   0,     3, 16,     0,        1,      4, 16,    1,        1,     3, 16, NO_HACK_ID,   0,     3, 16, NO_HACK_ID,   0, }; //26
    const uint8_t PROGMEM hacks_01_04[] = { 4, 5,  15, 4, 32, 23,    4, 16, NO_HACK_ID,   0,     4, 16,     0,        1,      3, 16, NO_HACK_ID,  0,     3, 16, NO_HACK_ID,   0, };  // 46
    //                                                 Total: 72, Average: 14.4, Attempts: 8 = 115.2
    const uint8_t PROGMEM hacks_02_00[] = { 5, 5,  25, 3, 32, 11,    3, 16, NO_HACK_ID,   0,     3, 16,     0,        2,      2, 16,    1,        1 }; // 19
    const uint8_t PROGMEM hacks_02_01[] = { 5, 5,  25, 3, 32, 15,    5, 16, NO_HACK_ID,   0,     3, 16,     0,        2,      2, 16,    1,        1 }; // 41
    const uint8_t PROGMEM hacks_02_02[] = { 5, 6,  25, 3, 32, 22,    4, 16, NO_HACK_ID,   0,     3, 16,     0,        1,      4, 16,    1,        1 }; // 38
    const uint8_t PROGMEM hacks_02_03[] = { 5, 6,  25, 4, 32, 16,    3, 16, NO_HACK_ID,   0,     4, 16,     0,        2,      3, 16,    1,        1,     2, 16,     2,        1 }; // 31
    const uint8_t PROGMEM hacks_02_04[] = { 5, 6,  25, 4, 32, 16,    4, 16, NO_HACK_ID,   0,     3, 16,     0,        1,      3, 16,    1,        1,     2, 16,     2,        1,     3, 16, NO_HACK_ID,   0, }; // 31
    //                                                 Total: 80, Average: 16, Attempts: 10 = 160
    const uint8_t PROGMEM hacks_03_00[] = { 6, 7,  40, 4, 32, 16,    3, 12, NO_HACK_ID,   0,     3, 12,     0,        1,      3, 12,    1,        2,     2, 16,      2,      2, }; // 26
    const uint8_t PROGMEM hacks_03_01[] = { 6, 6,  40, 4, 32, 17,    3, 16, NO_HACK_ID,   0,     3, 16,     0,        1,      3, 16,    1,        2,     2, 16,      2,      1, }; // 26
    const uint8_t PROGMEM hacks_03_02[] = { 6, 7,  40, 4, 32, 15,    3, 16, NO_HACK_ID,   0,     3, 16,     0,        2,      3, 16,    1,        1,     2, 16,      2,      2, }; // 26
    const uint8_t PROGMEM hacks_03_03[] = { 6, 7,  40, 5, 32, 16,    4, 16, NO_HACK_ID,   0,     3, 16,     0,        1,      3, 16,    1,        2,     2, 16,      2,      2,      3, 16, NO_HACK_ID,   0, }; // 34
    const uint8_t PROGMEM hacks_03_04[] = { 6, 7,  40, 6, 32, 18,    3, 16, NO_HACK_ID,   0,     3, 16,     0,        1,      3, 16,    1,        2,     2, 16,      2,      2,      3, 16, NO_HACK_ID,   0,    4, 20, NO_HACK_ID,   0, }; // 26
    //                                                 Total: 82, Average: 16.4, Attempts: 12 = 196
    const uint8_t PROGMEM hacks_04_00[] = { 6, 7,  50, 4, 24, 14,    3, 12, NO_HACK_ID,   0,     3, 12,     0,        1,      3, 12,    1,        2,     2, 16,      2,      2, }; // 26
    const uint8_t PROGMEM hacks_04_01[] = { 6, 6,  50, 4, 24, 16,    3, 16, NO_HACK_ID,   0,     3, 16,     0,        1,      3, 16,    1,        1,     2, 16,      2,      1, }; // 26
    const uint8_t PROGMEM hacks_04_02[] = { 6, 7,  50, 4, 24, 17,    5, 16, NO_HACK_ID,   0,     3, 16,     0,        2,      3, 16,    1,        2,     2, 16,      2,      2, }; // 49
    const uint8_t PROGMEM hacks_04_03[] = { 6, 7,  60, 5, 24, 18,    4, 16, NO_HACK_ID,   0,     3, 16,     0,        1,      3, 16,    1,        2,     2, 16,      2,      2,      3, 16, NO_HACK_ID,   0, }; // 34
    const uint8_t PROGMEM hacks_04_04[] = { 6, 7,  60, 6, 24, 17,    4, 16, NO_HACK_ID,   0,     3, 16,     0,        1,      3, 16,    1,        2,     2, 16,      2,      2,      3, 16, NO_HACK_ID,   0,    4, 20, NO_HACK_ID,   0, }; // 34
    //                                                 Total: 82, Average: 16.4, Attempts: 14 = 229


    //const uint8_t PROGMEM hacks_test[] =  { 4, 5,  20, 4, 32,     4, 16, NO_HACK_ID,   0,     4, 16,     0,        1,      3, 16, NO_HACK_ID,   0,     3, 16, NO_HACK_ID,   0, };

    const uint8_t * const hacks[] PROGMEM = { 

        hacks_01_00, hacks_01_01, hacks_01_02, hacks_01_03, hacks_01_04, 
        hacks_02_00, hacks_02_01, hacks_02_02, hacks_02_03, hacks_02_04, 
        hacks_03_00, hacks_03_01, hacks_03_02, hacks_03_03, hacks_03_04, 
        hacks_04_00, hacks_04_01, hacks_04_02, hacks_04_03, hacks_04_04, 

    };

};