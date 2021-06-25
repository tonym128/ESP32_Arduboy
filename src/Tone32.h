#ifndef _TONE32_h
#define _TONE32_h
#include <Arduino.h>

#define DEBUG 1
     
#define TONE_CHANNEL 0
#include "pitches.h"
#define DAC_PIN 26

void tone32(uint8_t pin, unsigned int frequency, unsigned long duration = 0, uint8_t channel = TONE_CHANNEL);
void noTone32(uint8_t pin, uint8_t channel = TONE_CHANNEL);

#endif



