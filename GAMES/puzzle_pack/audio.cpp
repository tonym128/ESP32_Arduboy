
#include "audio.h"

void sfx(const SFX_Data * data){
  uint16_t freqplay = data->frequency;
  
  for (uint16_t i=0; i< data->duration; i++){
    if(arduboy.audio.enabled())tone(D3, freqplay, data->pulse_width);
    delay(data->pulse_width);
    freqplay+=data->frequency_slide;
    freqplay+=data->frequency_accel*i;
  };   
}
