//----------------------------------------------------------------------------
// ROOFTOP RESCUE 
// (c) 2019 Bert vt Veer
//----------------------------------------------------------------------------

typedef const uint16_t t_tones;


t_tones sfx_ropedown[] PROGMEM = { 
    220,10, 280,5, 0,80,
    TONES_REPEAT
};

t_tones sfx_ropeup[] PROGMEM = { 
    220,15, 0,120,
    TONES_REPEAT
};

t_tones sfx_score[] PROGMEM = { 
    440,20, 330,20, 550,50,
    TONES_END
};

t_tones sfx_noroom[] PROGMEM = { 
    220,20, 450,20, 220,20, 450,20, 220,20, 450,20, 220,20, 450,20, 220,20, 450,20,
    TONES_END
};

t_tones sfx_spawn[] PROGMEM = {
    440,100, 660,20, 550,20, 660,20, 550,20, 660,50,
    TONES_END
};
t_tones sfx_dudegrab[] PROGMEM = {
    240,10, 260,20, 280,30, 300,40, 280,50,
    TONES_END
};

t_tones sfx_dudeenter[] PROGMEM = {
    320,10, 280,20, 240,20, 260,30,
    TONES_END
};

t_tones sfx_ufo[] PROGMEM = {
    330,20, 340,20, 350,20, 360,20, 370,20, 380,20, 390,20, 400,20, 410,20, 420,20, 430,20, 440,20,
    330,30, 340,30, 350,30, 360,30, 370,30, 380,30, 390,30, 400,30, 410,30, 420,30, 430,30, 440,30, 
    330,30, 340,30, 350,30, 360,30, 370,30, 380,30, 390,30, 400,30, 410,30, 420,30, 430,30, 440,30,
    TONES_END
};

t_tones sfx_gameover[] PROGMEM = {
    NOTE_D3,300, NOTE_G3,150, NOTE_REST,150, NOTE_G4,150, NOTE_REST,150, NOTE_D4,150, NOTE_REST,300,
    NOTE_CS4,150, NOTE_REST,150, NOTE_C4,150, NOTE_REST,150, NOTE_AS3,300, NOTE_REST,100, NOTE_G3,150,
    TONES_END
};

t_tones sfx_startgame[] PROGMEM = {
    NOTE_AS3,75, NOTE_REST,75, NOTE_AS3,75, NOTE_REST,75, NOTE_AS3,150, NOTE_REST,150, NOTE_CS4,150,
    NOTE_C4,150, NOTE_CS4,300, NOTE_F4,400,
    TONES_END 
};

t_tones sfx_menutoggle[] PROGMEM = {
    330,20, 385,15, 330,20, 385,15,330,20, 385,15, 
    TONES_END
};

t_tones sfx_menuselect[] PROGMEM = {
    330,10, 
    TONES_END
};

t_tones sfx_menuclick[] PROGMEM = {
    330,20, 440,20, 385,15, 440,15, 480,10, 490,10, 495,50, 
    TONES_END
};
