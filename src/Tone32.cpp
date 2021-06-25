#include "Tone32.h"
#ifdef DEBUG
char s[80];
#endif

bool sound_setup = false;

void tone32(uint8_t pin, unsigned int frequency, unsigned long duration, uint8_t channel)
{
#ifdef DEBUG
    sprintf(s,"Playing pin:%d freq:%d freq2:%d duration:%d channel:%d\r\n",pin, frequency,frequency >> 6, duration,channel);
    Serial.write(s);
#endif
    if (!sound_setup) {
      sound_setup = true;    
    }
}

void noTone32(uint8_t pin, uint8_t channel)
{
#ifdef DEBUG
    sprintf(s,"Stop Sound\r\n");
    Serial.write(s);
#endif
}
