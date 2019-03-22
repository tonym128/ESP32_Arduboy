/*
   a little test for the esp8266
*/

#include "Arduboy2.h"

#ifdef ESP8266
#define I2C_SDA       D2
#define I2C_SCL       D1
SSD1306Brzo oled(OLED_I2C_ADRESS, I2C_SDA, I2C_SCL);

#define MY_BUTTON_A     D0
#define MY_BUTTON_B     D4
#define MY_BUTTON_UP    D6
#define MY_BUTTON_DOWN  D7
#define MY_BUTTON_LEFT  D5
#define MY_BUTTON_RIGHT D8
#endif

// Make an instance of arduboy used for many functions
Arduboy2 arduboy;

// some variables to count the frames pro seconde
uint8_t framesNow = 0;
uint8_t framesCurrent = 0;
uint8_t secondsNow = 0;
uint8_t secondsCurrent = -1;

// returns the frames displayed in the last second
uint8_t getFramesPerSecond() {

  // get the current second
  secondsNow = millis() / 1000;

  if (secondsNow != secondsCurrent) {

    // to reset only with every new second
    secondsCurrent = secondsNow;

    // save the last frames per second
    framesCurrent = framesNow;

    // reset the counter
    framesNow = 0;

  } else {
    // count
    framesNow++;
  }

  return framesCurrent;
}

#ifdef ESP8266
void getButtons() {

  uint8_t buttons = 0;

  if (digitalRead(MY_BUTTON_A))
    buttons |= A_BUTTON;

  if (digitalRead(MY_BUTTON_B))
    buttons |= B_BUTTON;

  if (digitalRead(MY_BUTTON_UP))
    buttons |= UP_BUTTON;

  if (digitalRead(MY_BUTTON_DOWN))
    buttons |= DOWN_BUTTON;

  if (digitalRead(MY_BUTTON_LEFT))
    buttons |= LEFT_BUTTON;

  if (digitalRead(MY_BUTTON_RIGHT))
    buttons |= RIGHT_BUTTON;

  arduboy.setExternalButtons(buttons);
}
#endif

void setup() {

#ifdef ESP8266
  arduboy.setExternalButtonsHandler(getButtons);
#endif

  // not sure if this is necessary
  pinMode(MY_BUTTON_A, INPUT);
  pinMode(MY_BUTTON_B, INPUT);
  pinMode(MY_BUTTON_UP, INPUT);
  pinMode(MY_BUTTON_DOWN, INPUT);
  pinMode(MY_BUTTON_LEFT, INPUT);
  pinMode(MY_BUTTON_RIGHT, INPUT);

  arduboy.begin();

  // set impossilbe high frame rate for max frames
  arduboy.setFrameRate(1000);
}

void printPressedButtonAt(uint8_t yPos, String buttonName) {

  arduboy.setCursor(20, yPos);
  arduboy.print(buttonName + " Pressed");
}

void loop() {

  if (!(arduboy.nextFrame()))
    return;

  // we clear our screen to black
  arduboy.clear();

  // need to change the screen because oled library updates only changed areas
  //if (arduboy.everyXFrames(2))
  //  arduboy.fillScreen(WHITE);

  arduboy.setCursor(0, 0);
  arduboy.print(F("ESP8266 Test "));

  arduboy.print(getFramesPerSecond());

  // print buttons
  if (arduboy.pressed(A_BUTTON))
    printPressedButtonAt(9 * 1, "A");

  if (arduboy.pressed(B_BUTTON))
    printPressedButtonAt(9 * 2, "B");

  if (arduboy.pressed(UP_BUTTON))
    printPressedButtonAt(9 * 3, "UP");

  if (arduboy.pressed(DOWN_BUTTON))
    printPressedButtonAt(9 * 4, "DOWN");

  if (arduboy.pressed(LEFT_BUTTON))
    printPressedButtonAt(9 * 3, "LEFT");

  if (arduboy.pressed(RIGHT_BUTTON))
    printPressedButtonAt(9 * 4, "RIGHT");

  arduboy.display();
}
