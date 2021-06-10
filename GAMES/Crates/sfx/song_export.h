#ifndef SONG_H
#define SONG_H

#define Song const uint8_t PROGMEM

Song music[] = {
0x05,			// Number of tracks
0x00, 0x00,		// Address of track 0
0x03, 0x00,		// Address of track 1
0x04, 0x00,		// Address of track 2
0x05, 0x00,		// Address of track 3
0x06, 0x00,		// Address of track 4
0x00,			// Channel 0 entry track
0x01,			// Channel 1 entry track
0x02,			// Channel 2 entry track
0x03,			// Channel 3 entry track
//"Track channel 0"
0x9D, 24,		// FX: SET TEMPO: tempo = 24
0x9F,			// FX: STOP CURRENT CHANNEL
//"Track channel 1"
0x9F,			// FX: STOP CURRENT CHANNEL
//"Track channel 2"
0x9F,			// FX: STOP CURRENT CHANNEL
//"Track channel 3"
0x9F,			// FX: STOP CURRENT CHANNEL
//"Track Track 1"
0x00 + 29,		// NOTE ON: note = 29
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 2,		// DELAY: ticks = 2
0x00 + 22,		// NOTE ON: note = 22
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 2,		// DELAY: ticks = 2
0x00 + 22,		// NOTE ON: note = 22
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 2,		// DELAY: ticks = 2
0x00 + 34,		// NOTE ON: note = 34
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 5,		// DELAY: ticks = 5
0x00 + 15,		// NOTE ON: note = 15
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 2,		// DELAY: ticks = 2
0x00 + 22,		// NOTE ON: note = 22
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 5,		// DELAY: ticks = 5
0x00 + 12,		// NOTE ON: note = 12
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 5,		// DELAY: ticks = 5
0x00 + 13,		// NOTE ON: note = 13
0x9F + 1,		// DELAY: ticks = 1
0x00 + 0,		// NOTE ON: note = 0
0x9F + 14,		// DELAY: ticks = 14
0xFE,			// RETURN

};



#endif
