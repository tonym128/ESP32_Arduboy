#include <Arduino.h>
#include <Arduboy2.h>

extern Arduboy2 arduboy;

typedef struct{
  uint16_t frequency;
  int16_t frequency_slide;
  int16_t frequency_accel;
  uint16_t pulse_width;
  uint16_t pulse_freq;
  uint16_t duration;
  uint8_t methd;
} SFX_Data;

void sfx(const SFX_Data * data);
