/**
 * @file Arduboy2Beep.cpp
 * \brief
 * Classes to generate simple square wave tones on the Arduboy speaker pins.
 */

#include <Arduino.h>
#include "Arduboy2Beep.h"
#include "Arduboy2Core.h"

uint8_t BeepPin1::duration = 0;

void BeepPin1::begin() {}

void BeepPin1::tone(uint16_t count){
#ifdef ADAFRUIT
  ::tone(PIN_SPEAKER_1, count);
#endif
}

void BeepPin1::tone(uint16_t count, uint8_t dur){
#ifdef ADAFRUIT
  ::tone(PIN_SPEAKER_1, count, dur);
#endif
}

void BeepPin1::timer(){}

void BeepPin1::noTone(){
#ifdef ADAFRUIT
  ::noTone(PIN_SPEAKER_1);
#endif
}

uint8_t BeepPin2::duration = 0;

void BeepPin2::begin(){}

void BeepPin2::tone(uint16_t count){
#ifdef ADAFRUIT
  ::tone(PIN_SPEAKER_2, count);
#endif
}

void BeepPin2::tone(uint16_t count, uint8_t dur){
#ifdef ADAFRUIT
  ::tone(PIN_SPEAKER_2, count, dur);
#endif
}

void BeepPin2::timer(){}

void BeepPin2::noTone(){
#ifdef ADAFRUIT
  ::noTone(PIN_SPEAKER_2);
#endif
}