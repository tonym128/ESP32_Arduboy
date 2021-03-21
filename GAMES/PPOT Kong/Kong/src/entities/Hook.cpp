#include "Hook.h"

#include "../utils/Utils.h"
#include "../utils/Enums.h"
#include "../sounds/Sounds.h"

Hook::Hook() : Base() {

  this->counter = 4;

}
/*
void Hook::setSound(ArduboyTonesExt *sound) {

  this->sound = sound;

}
*/
void Hook::decCounter() {

  if (this->counter > 0) this->counter--;
#ifdef PLAY_SOUNDS 
  sound.tones(Sounds::hammer_time);
#endif

}