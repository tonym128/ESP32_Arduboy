#ifndef _TONE32_h
#define _TONE32_h

#define TONE_CHANNEL 0
#include "Arduino.h"
#include "pitches.h"

hw_timer_t* timer = NULL;
bool value = true;
int frequency = 20; // 20 to 20000
static const int timerPIN = 26;

void IRAM_ATTR onTimer() {
  value = !value;
  digitalWrite(PIN, value); 
}

void tone32(uint8_t pin, unsigned int frequency, unsigned long duration = 0, uint8_t channel = TONE_CHANNEL);
void noTone32(uint8_t pin, uint8_t channel = TONE_CHANNEL);
#endif
