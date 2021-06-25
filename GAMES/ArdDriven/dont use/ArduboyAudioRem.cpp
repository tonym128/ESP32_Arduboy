#include "ArduboyRem.h"
#include "ArduboyAudioRem.h"

bool ArduboyAudioRem::audio_enabled = false;

void ArduboyAudioRem::on()
{
  // fire up audio pins
#ifdef ARDUBOY_10
  pinMode(PIN_SPEAKER_1, OUTPUT);
  pinMode(PIN_SPEAKER_2, OUTPUT);
#else
  pinMode(PIN_SPEAKER_1, OUTPUT);
#endif
  audio_enabled = true;
}

void ArduboyAudioRem::off()
{
  audio_enabled = false;
  // shut off audio pins
#ifdef ARDUBOY_10
  pinMode(PIN_SPEAKER_1, INPUT);
  pinMode(PIN_SPEAKER_2, INPUT);
#else
  pinMode(PIN_SPEAKER_1, INPUT);
#endif
}

void ArduboyAudioRem::saveOnOff()
{
  EEPROM.update(EEPROM_AUDIO_ON_OFF, audio_enabled);
}

void ArduboyAudioRem::begin()
{
  if (EEPROM.read(EEPROM_AUDIO_ON_OFF))
    on();
}

bool ArduboyAudioRem::enabled()
{
  return audio_enabled;
}
