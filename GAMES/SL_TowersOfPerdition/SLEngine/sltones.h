/*****************************************************************************
 *  ArduboyTones
 * 
 * An Arduino library to play tones and tone sequences.
 * 
 * Specifically written for use by the Arduboy miniature game system
 * https://www.arduboy.com/
 * but could work with other Arduino AVR boards that have 16 bit timer 3
 * available, by changing the port and bit definintions for the pin(s)
 * if necessary.
 * 
 * Copyright (c) 2017 Scott Allen
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *****************************************************************************/

//This file is a stripped down version of ArduboyTones to conserve space for procedural music while sounding better then BeepPin

#ifndef SLTONES_H
#define SLTONES_H

#include <Arduino.h>


#ifndef AB_DEVKIT
// ***** SPEAKER ON TWO PINS *****
// Indicates that each of the speaker leads is attached to a pin, the way
// the Arduboy is wired. Allows tones of a higher volume to be produced.
// If commented out only one speaker pin will be used. The other speaker
// lead should be attached to ground.
#define TONES_2_SPEAKER_PINS
#endif

#ifndef AB_DEVKIT
// Arduboy speaker pin 1 = Arduino pin 5 = ATmega32u4 PC6
#define TONE_PIN_PORT PORTC
#define TONE_PIN_DDR DDRC
#define TONE_PIN PORTC6
#define TONE_PIN_MASK _BV(TONE_PIN)
// Arduboy speaker pin 2 = Arduino pin 13 = ATmega32u4 PC7
#define TONE_PIN2_PORT PORTC
#define TONE_PIN2_DDR DDRC
#define TONE_PIN2 PORTC7
#define TONE_PIN2_MASK _BV(TONE_PIN2)
#else
// DevKit speaker pin 1 = Arduino pin A2 = ATmega32u4 PF5
#define TONE_PIN_PORT PORTF
#define TONE_PIN_DDR DDRF
#define TONE_PIN PORTF5
#define TONE_PIN_MASK _BV(TONE_PIN)
#endif

// The minimum frequency that can be produced without a clock prescaler.
#define MIN_NO_PRESCALE_FREQ ((uint16_t)(((F_CPU / 2L) + (1L << 16) - 1L) / (1L << 16)))

static bool (*outputEnabled)();
static volatile long durationToggleCount = 0;
static volatile bool tonesPlaying = false;

class SLTones
{
public:
	SLTones(bool (*outEn)())
	{}/*
		outputEnabled = outEn;
		
		bitClear(TONE_PIN_PORT, TONE_PIN); // set the pin low
		bitSet(TONE_PIN_DDR, TONE_PIN); // set the pin to output mode
		#ifdef TONES_2_SPEAKER_PINS
		bitClear(TONE_PIN2_PORT, TONE_PIN2); // set pin 2 low
		bitSet(TONE_PIN2_DDR, TONE_PIN2); // set pin 2 to output mode
		#endif
	}*/
	static void tone(uint16_t freq, uint16_t dur = 0)
	{}/*
		uint32_t ocrValue;
		#ifdef TONES_ADJUST_PRESCALER
		uint8_t tccrxbValue;
		#endif
		bitWrite(TIMSK3, OCIE3A, 0); // disable the output compare match interrupt
		tonesPlaying = true;
		#ifdef TONES_ADJUST_PRESCALER
		if (freq >= MIN_NO_PRESCALE_FREQ) {
			tccrxbValue = _BV(WGM32) | _BV(CS30); // CTC mode, no prescaling
			ocrValue = F_CPU / freq / 2 - 1;
		}
		else {
			tccrxbValue = _BV(WGM32) | _BV(CS31); // CTC mode, prescaler /8
			#endif
			ocrValue = F_CPU / 8 / freq / 2 - 1;
			#ifdef TONES_ADJUST_PRESCALER
		}
		#endif
		if (!outputEnabled()) { // if sound has been muted
			durationToggleCount = 0;
			return;
		}
		
		#ifdef TONES_2_SPEAKER_PINS
		// set pin 2 to the compliment of pin 1
		if (bitRead(TONE_PIN_PORT, TONE_PIN)) {
			bitClear(TONE_PIN2_PORT, TONE_PIN2);
		}
		else {
			bitSet(TONE_PIN2_PORT, TONE_PIN2);
		}
		#endif
		if (dur != 0) {
			// A right shift is used to divide by 512 for efficency.
			// For durations in milliseconds it should actually be a divide by 500,
			// so durations will by shorter by 2.34% of what is specified.
			durationToggleCount = ((long)dur * freq) >> 9;
		}
		else {
			durationToggleCount = -1; // indicate infinite duration
		}
		
		TCCR3A = 0;
		#ifdef TONES_ADJUST_PRESCALER
		TCCR3B = tccrxbValue;
		#else
		TCCR3B = _BV(WGM32) | _BV(CS31); // CTC mode, prescaler /8
		#endif
		OCR3A = ocrValue;
		bitWrite(TIMSK3, OCIE3A, 1); // enable the output compare match interrupt
	}*/
	static void noTone()
	{/*
		bitWrite(TIMSK3, OCIE3A, 0); // disable the output compare match interrupt
		TCCR3B = 0; // stop the counter
		bitClear(TONE_PIN_PORT, TONE_PIN); // set the pin low
		#ifdef TONES_2_SPEAKER_PINS
		bitClear(TONE_PIN2_PORT, TONE_PIN2); // set pin 2 low
		#endif
		tonesPlaying = false;*/
	}
	static bool playing() {return tonesPlaying;}
};

/*ISR(TIMER3_COMPA_vect)
{
	if (durationToggleCount != 0) {
		*(&TONE_PIN_PORT) ^= TONE_PIN_MASK; // toggle the pin
		#ifdef TONES_2_SPEAKER_PINS
		*(&TONE_PIN2_PORT) ^= TONE_PIN2_MASK; // toggle pin 2
		#endif
		--durationToggleCount;
	}
	else
		tonesPlaying = false;
}*/

#endif
