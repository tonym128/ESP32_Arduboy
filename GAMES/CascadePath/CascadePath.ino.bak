#include <Arduboy2.h>
#include "CascadePathGame.h"

Arduboy2 display;
CascadePathGame Game(&display);

void setup() {
  Serial.begin(115200);
  display.begin();
  Game.Begin();
}
void loop() {

  Game.Cycle();

}
