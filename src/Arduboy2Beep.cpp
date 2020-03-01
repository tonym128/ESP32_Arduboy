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
  ::tone(PIN_SPEAKER_1, count);
}

void BeepPin1::tone(uint16_t count, uint8_t dur){
  ::tone(PIN_SPEAKER_1, count, dur);
}

void BeepPin1::timer(){}

void BeepPin1::noTone(){
  ::noTone(PIN_SPEAKER_1);
}

uint8_t BeepPin2::duration = 0;

void BeepPin2::begin(){}

void BeepPin2::tone(uint16_t count){
  ::tone(PIN_SPEAKER_2, count);
}

void BeepPin2::tone(uint16_t count, uint8_t dur){
  ::tone(PIN_SPEAKER_2, count, dur);
}

void BeepPin2::timer(){}

void BeepPin2::noTone(){
  ::noTone(PIN_SPEAKER_2);
}