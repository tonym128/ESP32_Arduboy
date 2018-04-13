/**
 * @file Arduboy2Beep.cpp
 * \brief
 * Classes to generate simple square wave tones on the Arduboy speaker pins.
 */

#include <Arduino.h>
#include "Arduboy2Beep.h"

#if !defined(AB_DEVKIT)

// Speaker pin 1, Timer 3A, Port C bit 6, Arduino pin 5
//       SLIMBOY: Timer 1A, Port B bit 1, Arduino pin 9

uint8_t BeepPin1::duration = 0;

void BeepPin1::begin()
{
#ifdef SLIMBOY
  TCCR1A = 0;
  TCCR1B = (bit(WGM12) | bit(CS11)); // CTC mode. Divide by 8 clock prescale
#else
  TCCR3A = 0;
  TCCR3B = (bit(WGM32) | bit(CS31)); // CTC mode. Divide by 8 clock prescale
#endif
}

void BeepPin1::tone(uint16_t count)
{
  tone(count, 0);
}

void BeepPin1::tone(uint16_t count, uint8_t dur)
{
  duration = dur;
#ifdef SLIMBOY
  TCCR1A = bit(COM1A0); // set toggle on compare mode (which connects the pin)
  OCR1A = count; // load the count (16 bits), which determines the frequency
#else
  TCCR3A = bit(COM3A0); // set toggle on compare mode (which connects the pin)
  OCR3A = count; // load the count (16 bits), which determines the frequency
#endif
}

void BeepPin1::timer()
{
  if (duration && (--duration == 0)) {
#ifdef SLIMBOY
    TCCR1A = 0; // set normal mode (which disconnects the pin)
#else
    TCCR3A = 0; // set normal mode (which disconnects the pin)
#endif
  }
}

void BeepPin1::noTone()
{
  duration = 0;
#ifdef SLIMBOY
  TCCR1A = 0; // set normal mode (which disconnects the pin)
#else
  TCCR3A = 0; // set normal mode (which disconnects the pin)
#endif
}


// Speaker pin 2, Timer 4A, Port C bit 7, Arduino pin 13

uint8_t BeepPin2::duration = 0;

void BeepPin2::begin()
{
#ifdef SLIMBOY
  TCCR2A = 0; // normal mode. Disable PWM
  TCCR2B = bit(CS22) | bit(CS20); // divide by 128 clock prescale
  OCR2A = 0; //  "
#else
  TCCR4A = 0; // normal mode. Disable PWM
  TCCR4B = bit(CS43); // divide by 128 clock prescale
  TCCR4D = 0; // normal mode
  TC4H = 0;  // toggle pin at count = 0
  OCR4A = 0; //  "
#endif
}

void BeepPin2::tone(uint16_t count)
{
  tone(count, 0);
}

void BeepPin2::tone(uint16_t count, uint8_t dur)
{
  duration = dur;
#ifdef SLIMBOY
  TCCR2A = bit(WGM21) | bit(COM2A0); // CTC mode, toggle on compare mode (which connects the pin)
  OCR2A = lowByte(count); //  which determines the frequency
#else
  TCCR4A = bit(COM4A0); // set toggle on compare mode (which connects the pin)
  TC4H = highByte(count); // load the count (10 bits),
  OCR4C = lowByte(count); //  which determines the frequency
#endif
}

void BeepPin2::timer()
{
  if (duration && (--duration == 0)) {
#ifdef SLIMBOY
    TCCR2A = 0; // set normal mode (which disconnects the pin)
#else   
    TCCR4A = 0; // set normal mode (which disconnects the pin)
#endif
  }
}

void BeepPin2::noTone()
{
  duration = 0;
#ifdef SLIMBOY
  TCCR2A = 0; // set normal mode (which disconnects the pin)
#else   
  TCCR4A = 0; // set normal mode (which disconnects the pin)
#endif
}


#else /* AB_DEVKIT */

// *** The pins used for the speaker on the DevKit cannot be directly
// controlled by a timer/counter. The following "dummy" functions will
// compile and operate properly but no sound will be produced

uint8_t BeepPin1::duration = 0;

void BeepPin1::begin()
{
}

void BeepPin1::tone(uint16_t count)
{
  tone(count, 0);
}

void BeepPin1::tone(uint16_t count, uint8_t dur)
{
  (void) count; // parameter not used

  duration = dur;
}

void BeepPin1::timer()
{
  if (duration) {
    --duration;
  }
}

void BeepPin1::noTone()
{
  duration = 0;
}


uint8_t BeepPin2::duration = 0;

void BeepPin2::begin()
{
}

void BeepPin2::tone(uint16_t count)
{
  tone(count, 0);
}

void BeepPin2::tone(uint16_t count, uint8_t dur)
{
  (void) count; // parameter not used

  duration = dur;
}

void BeepPin2::timer()
{
  if (duration) {
    --duration;
  }
}

void BeepPin2::noTone()
{
  duration = 0;
}

#endif
