/**
 * @file ArduboyTones.cpp
 * \brief An Arduino library for playing tones and tone sequences, 
 * intended for the Arduboy game system.
 */

/*****************************************************************************
  ArduboyTones

An Arduino library to play tones and tone sequences.

Specifically written for use by the Arduboy miniature game system
https://www.arduboy.com/
but could work with other Arduino AVR boards that have 16 bit timer 3
available, by changing the port and bit definintions for the pin(s)
if necessary.

Copyright (c) 2017 Scott Allen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*****************************************************************************/

// this library is needed to get easy access to the esp82666 timer functions 
#include <Ticker.h>


Ticker tonesTicker;

#include "ArduboyTones.h"

// pointer to a function that indicates if sound is enabled
static bool (*outputEnabled)();

static volatile long durationToggleCount = 0;
static volatile bool tonesPlaying = false;
static volatile bool toneSilent;

uint16_t *tonesStart;	
uint16_t tonesIndex;	
uint16_t toneSequence[MAX_TONES * 2 + 1];

static volatile bool inProgmem;

void updateTones();

ArduboyTones::ArduboyTones(bool (*outEn)()){
  outputEnabled = outEn;
  toneSequence[MAX_TONES * 2] = TONES_END;

  // sets the update call interval
  //tonesTicker.attach_ms(1000, updateTones);
}

void ArduboyTones::tone(uint16_t freq, uint16_t dur){
  inProgmem = false;
  tonesStart = toneSequence; 
  tonesIndex =0; // set to start of sequence array
  toneSequence[0] = freq;
  toneSequence[1] = dur;
  toneSequence[2] = TONES_END; // set end marker
  nextTone(); // start playing
}

void ArduboyTones::tone(uint16_t freq1, uint16_t dur1, uint16_t freq2, uint16_t dur2){
  inProgmem = false;
  tonesStart = toneSequence; 
  tonesIndex = 0;// set to start of sequence array
  toneSequence[0] = freq1;
  toneSequence[1] = dur1;
  toneSequence[2] = freq2;
  toneSequence[3] = dur2;
  toneSequence[4] = TONES_END; // set end marker
  nextTone(); // start playing
}

void ArduboyTones::tone(uint16_t freq1, uint16_t dur1, uint16_t freq2, uint16_t dur2, uint16_t freq3, uint16_t dur3){
  inProgmem = false;
  tonesStart = toneSequence;
  tonesIndex = 0; // set to start of sequence array
  toneSequence[0] = freq1;
  toneSequence[1] = dur1;
  toneSequence[2] = freq2;
  toneSequence[3] = dur2;
  toneSequence[4] = freq3;
  toneSequence[5] = dur3;
  // end marker was set in the constructor and will never change
  nextTone(); // start playing
}

void ArduboyTones::tones(const uint16_t *tones){
  inProgmem = true;
  tonesStart = (uint16_t *)tones; // set to start of sequence array
  tonesIndex = 0;
  nextTone(); // start playing
}

void ArduboyTones::tonesInRAM(uint16_t *tones){
  inProgmem = false;
  tonesStart = tones;
  tonesIndex = 0; // set to start of sequence array
  nextTone(); // start playing
}

void ArduboyTones::noTone(){
  ::noTone(TONES_PIN);
}

void ArduboyTones::volumeMode(uint8_t mode){
}

bool ArduboyTones::playing(){
  return tonesPlaying;
}

void ArduboyTones::nextTone(){
  static uint16_t freq;
  static uint16_t dur;
  
  freq = getNext(); // get tone frequency
  if (freq == TONES_END) { // if freq is actually an "end of sequence" marker
    noTone(); // stop playing
    tonesPlaying = false;
    return;
  }

  tonesPlaying = true;

  if (freq == TONES_REPEAT) { // if frequency is actually a "repeat" marker
    tonesIndex = 0; // reset to start of sequence
    freq = getNext();
    return;
  }

  freq &= ~TONE_HIGH_VOLUME; // strip volume indicator from frequency

	if (freq == 0) { // if tone is silent
		noTone();
		toneSilent = true;
	}
	else {
		toneSilent = false;
	}

  if (!outputEnabled()) { // if sound has been muted
    toneSilent = true;
  }
	
	// play the actual tone with the std tone library if not muted
	if (toneSilent) {
		noTone();
	} else {
		::tone(TONES_PIN, freq);
	}	

  dur = getNext(); // get tone duration
	
#ifdef TONES_SERIAL_DEBUG
    Serial.println("INDEX START: " + (String)((uint32_t)tonesStart));
    Serial.println("INDEX INDEX: " + (String)tonesIndex);
	Serial.print("freq:");
	Serial.println(freq, DEC);
	Serial.print("delay:");
	Serial.println(dur, DEC);
	Serial.println("");
#endif 
	// set a timer to update the tone after its duration
	tonesTicker.once_ms(dur, updateTones);
}


uint16_t ArduboyTones::getNext(){
  static uint16_t getTone;
  if (inProgmem)
    getTone = pgm_read_word((uint16_t *)((uint32_t)tonesStart+tonesIndex*sizeof(uint16_t)));
  else getTone = tonesStart[tonesIndex];
  tonesIndex++;
  return (getTone);
}

void updateTones(){
#ifdef TONES_SERIAL_DEBUG
	Serial.print(millis(), DEC);
	Serial.println(" nextTone()");
#endif 
	ArduboyTones::nextTone();
}
