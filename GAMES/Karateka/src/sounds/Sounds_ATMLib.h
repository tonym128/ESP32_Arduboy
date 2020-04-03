#pragma once

#define Song const uint8_t PROGMEM 

Song ouch[] = {
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
0x00 + 2,		// NOTE ON: note = 2
0x9F + 1,		// DELAY: ticks = 1
0x00 + 1,		// NOTE ON: note = 1
0x9F + 1,		// DELAY: ticks = 1
0xFE,			// RETURN

};