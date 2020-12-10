#ifndef SONG_H
#define SONG_H

#define Song const uint8_t PROGMEM

Song titleSong[] = {
  0x0B,             // Number of tracks
  0x00, 0x00,       // Address of track 0   0
  0x10, 0x00,       // Address of track 1   16
  0x18, 0x00,       // Address of track 2   24
  0x19, 0x00,       // Address of track 3   25
  0x1D, 0x00,       // Address of track 4   29
  0x23, 0x00,       // Address of track 5   35
  0x2D, 0x00,       // Address of track 6   45
  0x37, 0x00,       // Address of track 7     55
  0x42, 0x00,       // Address of track 8   66
  0x4D, 0x00,       // Address of track 9   77
  0x5E, 0x00,       // Address of track 10    94

  0x00,             // Channel 0 entry track
  0x01,             // Channel 1 entry track
  0x02,             // Channel 2 entry track
  0x03,             // Channel 3 entry track

  //"Track channel 0"
  0x9D, 24,         // FX: SET TEMPO: tempo = 24
  0x9E, 0, 1, 2, 3, // FX: GOTO advanced: ch0 = 0x00 / ch1 = 0x01 / ch2 = 0x01 / ch3 = 0x01
  0x40, 32,         // FX: SET VOLUME: volume = 32
  0x4E, 3, 15,      // FX: SET TREMOLO: 3 15
  0xFD, 7, 7,       // REPEAT: count = 7 + 1 / track = 7
  0x9F,             // FX: STOP CURRENT CHANNEL

  //"Track channel 1"
  0x40, 63,         // FX: SET VOLUME: volume = 63
  0x41, -16,        // FX: SLIDE VOLUME ON: -16
  0xFD, 7, 8,       // REPEAT: count = 7 + 1 / track = 8
  0x9F,             // FX: STOP CURRENT CHANNEL

  //"Track channel 2"
  0x9F,             // FX: STOP CURRENT CHANNEL

  //"Track channel 3"
  0xFD, 47, 6,      // REPEAT: count = 47 + 1 / track = 6
  0x9F,             // FX: STOP CURRENT CHANNEL

  //"Track tick"
  0x40, 32,         // FX: SET VOLUME: volume = 32
  0x9F + 1,         // DELAY: ticks = 1
  0x40, 0,          // FX: SET VOLUME: volume = 0
  0xFE,             // RETURN

  //"Track shake"
  0x49, 4 + 0,      // FX: RETRIG NOISE: point = 1 (*4) / speed = 0 (fastest)
  0x40, 32,         // FX: SET VOLUME: volume = 32
  0x41, -8,         // FX: VOLUME SLIDE ON: steps = -8
  0x9F + 4,         // DELAY: ticks = 4
  0x4A,             // FX: RETRIG: off
  0x43,             // FX: VOLUME SLIDE OFF
  0xFE,             // RETURN

  //"Track 6"
  0xFC, 4,          // GOTO track 4
  0x9F + 3,         // DELAY: ticks = 3
  0xFC, 5,          // GOTO track 5
  0x9F + 8,         // DELAY: ticks = 8
  0xFC, 4,          // GOTO track 4
  0x9F + 15,        // DELAY: ticks = 15
  0xFE,             // RETURN

  //"Track 7"
  0x4C, 0,          // FX: SET TRANSPOSITION: 0
  0xFD, 2, 10,      // REPEAT: count = 2 + 1 / track = 10
  0x4C, -2,         // FX: SET TRANSPOSITION: -2
  0xFD, 2, 10,      // REPEAT: count = 2 + 1 / track = 10
  0xFE,             // RETURN

  //"Track 8"
  0x4C, 0,          // FX: SET TRANSPOSITION: 0
  0xFD, 2, 9,       // REPEAT: count = 2 + 1 / track = 9
  0x4C, -2,         // FX: SET TRANSPOSITION: -2
  0xFD, 2, 9,       // REPEAT: count = 2 + 1 / track = 9
  0xFE,             // RETURN

  //"Track 9"
  0x00 + 20,        // NOTE ON: note = 20
  0x9F + 4,         // DELAY: ticks = 4
  0x00 + 20,        // NOTE ON: note = 20
  0x9F + 4,         // DELAY: ticks = 4
  0x00 + 27,        // NOTE ON: note = 27
  0x9F + 4,         // DELAY: ticks = 4
  0x00 + 20,        // NOTE ON: note = 20
  0x9F + 4,         // DELAY: ticks = 4
  0x00 + 28,        // NOTE ON: note = 28
  0x9F + 4,         // DELAY: ticks = 4
  0x00 + 20,        // NOTE ON: note = 20
  0x9F + 4,         // DELAY: ticks = 4
  0x00 + 27,        // NOTE ON: note = 27
  0x9F + 4,         // DELAY: ticks = 4
  0x00 + 20,        // NOTE ON: note = 20
  0x9F + 4,         // DELAY: ticks = 4
  0xFE,             // RETURN

  //"Track 10"
  0x00 + 20,        // NOTE ON: note = 20
  0x9F + 32,        // DELAY: ticks = 32
  0xFE,             // RETURN
};


Song playing[] = {
  0x09,			// Number of tracks
  0x00, 0x00,		// Address of track 0
  0x0d, 0x00,		// Address of track 1
  0x0e, 0x00,		// Address of track 2
  0x0f, 0x00,		// Address of track 3
  0x13, 0x00,		// Address of track 4
  0x1a, 0x00,		// Address of track 5
  0x41, 0x00,		// Address of track 6
  0x6a, 0x00,		// Address of track 7
  0x9b, 0x00,		// Address of track 8
  0x00,			// Channel 0 entry track
  0x01,			// Channel 1 entry track
  0x02,			// Channel 2 entry track
  0x03,			// Channel 3 entry track
  //"Track channel 0"
  0x40, 30,		// FX: SET VOLUME: volume = 30
  0x9D, 24,		// FX: SET TEMPO: tempo = 24
  0xFC, 5,		// GOTO track 5
  0xFC, 6,		// GOTO track 6
  0xFC, 5,		// GOTO track 5
  0xFC, 7,		// GOTO track 7
  0x9F,			// FX: STOP CURRENT CHANNEL
  //"Track channel 1"
  0x9F,			// FX: STOP CURRENT CHANNEL
  //"Track channel 2"
  0x9F,			// FX: STOP CURRENT CHANNEL
  //"Track channel 3"
  0xFD, 3, 8,		// REPEAT: count = 3 + 1 / track = 8
  0x9F,			// FX: STOP CURRENT CHANNEL
  //"Track snare"
  0x40, 32,		// FX: SET VOLUME: volume = 32
  0x41, -16,		// FX: VOLUME SLIDE ON: steps = -16
  0x9F + 2,		// DELAY: ticks = 2
  0x43,			// FX: VOLUME SLIDE OFF
  0xFE,			// RETURN
  //"Track Track 1"
  0x00 + 18,		// NOTE ON: note = 18
  0x9F + 2,		// DELAY: ticks = 2
  0x00 + 0,		// NOTE ON: note = 0
  0x9F + 2,		// DELAY: ticks = 2
  0x00 + 18,		// NOTE ON: note = 18
  0x9F + 2,		// DELAY: ticks = 2
  0x00 + 0,		// NOTE ON: note = 0
  0x9F + 2,		// DELAY: ticks = 2
  0x00 + 18,		// NOTE ON: note = 18
  0x9F + 2,		// DELAY: ticks = 2
  0x00 + 0,		// NOTE ON: note = 0
  0x9F + 2,		// DELAY: ticks = 2
  0x00 + 18,		// NOTE ON: note = 18
  0x9F + 2,		// DELAY: ticks = 2
  0x00 + 0,		// NOTE ON: note = 0
  0x9F + 2,		// DELAY: ticks = 2
  0x00 + 18,		// NOTE ON: note = 18
  0x9F + 2,		// DELAY: ticks = 2
  0x00 + 0,		// NOTE ON: note = 0
  0x9F + 2,		// DELAY: ticks = 2
  0x00 + 18,		// NOTE ON: note = 18
  0x9F + 2,		// DELAY: ticks = 2
  0x00 + 0,		// NOTE ON: note = 0
  0x9F + 2,		// DELAY: ticks = 2
  0x00 + 25,		// NOTE ON: note = 25
  0x9F + 1,		// DELAY: ticks = 1
  0x00 + 26,		// NOTE ON: note = 26
  0x9F + 1,		// DELAY: ticks = 1
  0x00 + 25,		// NOTE ON: note = 25
  0x9F + 1,		// DELAY: ticks = 1
  0x00 + 26,		// NOTE ON: note = 26
  0x9F + 1,		// DELAY: ticks = 1
  0x00 + 25,		// NOTE ON: note = 25
  0x9F + 1,		// DELAY: ticks = 1
  0x00 + 26,		// NOTE ON: note = 26
  0x9F + 1,		// DELAY: ticks = 1
  0x00 + 0,		// NOTE ON: note = 0
  0x9F + 2,		// DELAY: ticks = 2
  0xFE,			// RETURN
  //"Track Track 2"
  0x00 + 18,		// NOTE ON: note = 18
  0x9F + 2,		// DELAY: ticks = 2
  0x00 + 0,		// NOTE ON: note = 0
  0x9F + 2,		// DELAY: ticks = 2
  0x00 + 18,		// NOTE ON: note = 18
  0x9F + 2,		// DELAY: ticks = 2
  0x00 + 0,		// NOTE ON: note = 0
  0x9F + 2,		// DELAY: ticks = 2
  0x00 + 18,		// NOTE ON: note = 18
  0x9F + 2,		// DELAY: ticks = 2
  0x00 + 0,		// NOTE ON: note = 0
  0x9F + 2,		// DELAY: ticks = 2
  0x00 + 18,		// NOTE ON: note = 18
  0x9F + 2,		// DELAY: ticks = 2
  0x00 + 0,		// NOTE ON: note = 0
  0x9F + 2,		// DELAY: ticks = 2
  0x00 + 18,		// NOTE ON: note = 18
  0x9F + 2,		// DELAY: ticks = 2
  0x00 + 0,		// NOTE ON: note = 0
  0x9F + 2,		// DELAY: ticks = 2
  0x00 + 18,		// NOTE ON: note = 18
  0x9F + 2,		// DELAY: ticks = 2
  0x00 + 0,		// NOTE ON: note = 0
  0x9F + 1,		// DELAY: ticks = 1
  0x00 + 26,		// NOTE ON: note = 26
  0x9F + 1,		// DELAY: ticks = 1
  0x00 + 27,		// NOTE ON: note = 27
  0x9F + 1,		// DELAY: ticks = 1
  0x00 + 26,		// NOTE ON: note = 26
  0x9F + 1,		// DELAY: ticks = 1
  0x00 + 27,		// NOTE ON: note = 27
  0x9F + 1,		// DELAY: ticks = 1
  0x00 + 26,		// NOTE ON: note = 26
  0x9F + 1,		// DELAY: ticks = 1
  0x00 + 27,		// NOTE ON: note = 27
  0x9F + 1,		// DELAY: ticks = 1
  0x00 + 26,		// NOTE ON: note = 26
  0x9F + 1,		// DELAY: ticks = 1
  0x00 + 0,		// NOTE ON: note = 0
  0x9F + 2,		// DELAY: ticks = 2
  0xFE,			// RETURN
  //"Track Track 3"
  0x00 + 18,		// NOTE ON: note = 18
  0x9F + 2,		// DELAY: ticks = 2
  0x00 + 0,		// NOTE ON: note = 0
  0x9F + 2,		// DELAY: ticks = 2
  0x00 + 18,		// NOTE ON: note = 18
  0x9F + 1,		// DELAY: ticks = 1
  0x00 + 0,		// NOTE ON: note = 0
  0x9F + 1,		// DELAY: ticks = 1
  0x00 + 18,		// NOTE ON: note = 18
  0x9F + 1,		// DELAY: ticks = 1
  0x00 + 0,		// NOTE ON: note = 0
  0x9F + 2,		// DELAY: ticks = 2
  0x00 + 18,		// NOTE ON: note = 18
  0x9F + 2,		// DELAY: ticks = 2
  0x00 + 0,		// NOTE ON: note = 0
  0x9F + 2,		// DELAY: ticks = 2
  0x00 + 18,		// NOTE ON: note = 18
  0x9F + 1,		// DELAY: ticks = 1
  0x00 + 0,		// NOTE ON: note = 0
  0x9F + 1,		// DELAY: ticks = 1
  0x00 + 18,		// NOTE ON: note = 18
  0x9F + 1,		// DELAY: ticks = 1
  0x00 + 0,		// NOTE ON: note = 0
  0x9F + 2,		// DELAY: ticks = 2
  0x00 + 18,		// NOTE ON: note = 18
  0x9F + 2,		// DELAY: ticks = 2
  0x00 + 0,		// NOTE ON: note = 0
  0x9F + 2,		// DELAY: ticks = 2
  0x00 + 18,		// NOTE ON: note = 18
  0x9F + 1,		// DELAY: ticks = 1
  0x00 + 0,		// NOTE ON: note = 0
  0x9F + 1,		// DELAY: ticks = 1
  0x00 + 18,		// NOTE ON: note = 18
  0x9F + 1,		// DELAY: ticks = 1
  0x00 + 0,		// NOTE ON: note = 0
  0x9F + 1,		// DELAY: ticks = 1
  0x00 + 18,		// NOTE ON: note = 18
  0x9F + 1,		// DELAY: ticks = 1
  0x00 + 0,		// NOTE ON: note = 0
  0x9F + 1,		// DELAY: ticks = 1
  0x00 + 18,		// NOTE ON: note = 18
  0x9F + 1,		// DELAY: ticks = 1
  0x00 + 0,		// NOTE ON: note = 0
  0x9F + 1,		// DELAY: ticks = 1
  0x00 + 18,		// NOTE ON: note = 18
  0x9F + 1,		// DELAY: ticks = 1
  0x00 + 0,		// NOTE ON: note = 0
  0x9F + 1,		// DELAY: ticks = 1
  0xFE,      // RETURN

  //"Track Track 4"
  0xFC, 4,    // GOTO track 4
  0x9F + 2,   // DELAY: ticks = 2
  0xFC, 4,    // GOTO track 4
  0x9F + 26,    // DELAY: ticks = 26
  0xFE,     // RETURN

};


#endif
