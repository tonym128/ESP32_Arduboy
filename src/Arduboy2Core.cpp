/**
 * @file Arduboy2Core.cpp
 * \brief
 * The Arduboy2Core class for Arduboy hardware initilization and control.
 */

#include "Arduboy2Core.h"

ESPboyInit myESPboy;


uint8_t Arduboy2Core::sBuffer[];
uint16_t Arduboy2Core::colors[19] = { 
            TFT_BLACK, TFT_NAVY, TFT_DARKGREEN, TFT_DARKCYAN, TFT_MAROON,
            TFT_PURPLE, TFT_OLIVE, TFT_LIGHTGREY, TFT_DARKGREY, 
            TFT_BLUE, TFT_GREEN, TFT_CYAN,
            TFT_RED, TFT_MAGENTA, TFT_YELLOW, TFT_WHITE, 
            TFT_ORANGE, TFT_GREENYELLOW, TFT_PINK};

uint8_t Arduboy2Core::foregroundclr = 14;
uint8_t Arduboy2Core::backgroundclr = 0;


Arduboy2Core::Arduboy2Core() {}


void Arduboy2Core::boot(){
  Serial.begin(115200);         
  EEPROM.begin(2400);

  //Init ESPboy
  myESPboy.begin(((String)F("Arduboy2 lib port")).c_str());
}




void Arduboy2Core::setCPUSpeed8MHz() {};
void Arduboy2Core::bootPins() {};
void Arduboy2Core::bootOLED() {};
void Arduboy2Core::LCDDataMode() {};
void Arduboy2Core::LCDCommandMode() {};
void Arduboy2Core::bootSPI(){}
void Arduboy2Core::SPItransfer(uint8_t data) {};
void Arduboy2Core::safeMode(){}
void Arduboy2Core::bootPowerSaving() {};
void Arduboy2Core::sendLCDCommand(uint8_t command){};
void Arduboy2Core::exitToBootloader() {};
void Arduboy2Core::mainNoUSB(){};
// turn all display pixels on, ignoring buffer contents
// or set to normal buffer display
void Arduboy2Core::allPixelsOn(bool on){};
void Arduboy2Core::blank() {};
// invert the display or set to normal
// when inverted, a pixel set to 0 will be on
void Arduboy2Core::invert(bool inverse){};
// flip the display vertically or set to normal
void Arduboy2Core::flipVertical(bool flipped){};
// flip the display horizontally or set to normal
void Arduboy2Core::flipHorizontal(bool flipped){};
void Arduboy2Core::paint8Pixels(uint8_t pixels){}
void Arduboy2Core::freeRGBled(){};


/* Power Management */
void Arduboy2Core::idle() {
delay(1);
};


// Shut down the display
void Arduboy2Core::displayOff() {
myESPboy.dac.setVoltage(0, false);
};


// Restart the display after a displayOff()
void Arduboy2Core::displayOn() {
myESPboy.dac.setVoltage(4095, false);
};

uint8_t Arduboy2Core::width() { return WIDTH; }

uint8_t Arduboy2Core::height() { return HEIGHT; }


/* Drawing */

void Arduboy2Core::paintScreen(const uint8_t *image){
  //memcpy(sBuffer, image, HEIGHT*WIDTH/8);
};

// paint from a memory buffer, this should be FAST as it's likely what
// will be used by any buffer based subclass
//
// The following assembly code runs "open loop". It relies on instruction
// execution times to allow time for each byte of data to be clocked out.
// It is specifically tuned for a 16MHz CPU clock and SPI clocking at 8MHz.
void Arduboy2Core::paintScreen(uint8_t image[], bool clear) {
//  memcpy(sBuffer, image, HEIGHT*WIDTH/8);
//  if (clear) memset(sBuffer, 0, HEIGHT*WIDTH/8);
};


/* RGB LED */
void Arduboy2Core::setRGBled(uint8_t red, uint8_t green, uint8_t blue){
  myESPboy.myLED.setRGB (red, green, blue);
};

void Arduboy2Core::setRGBled(uint8_t color, uint8_t val){
  if (color == RED_LED)   myESPboy.myLED.setR (val);
  else
    if (color == GREEN_LED) myESPboy.myLED.setG (val);
    else
      if (color == BLUE_LED)  myESPboy.myLED.setB (val);
};


void Arduboy2Core::digitalWriteRGB(uint8_t red, uint8_t green, uint8_t blue){
  if (red) myESPboy.myLED.setR (200); else myESPboy.myLED.setR (0); 
  if (green) myESPboy.myLED.setG (200); else myESPboy.myLED.setG (0); 
  if (blue) myESPboy.myLED.setB (200); else myESPboy.myLED.setB (0); 
};


void Arduboy2Core::digitalWriteRGB(uint8_t color, uint8_t val){
  if (color == 0)
  	if(val) myESPboy.myLED.setR (200); else myESPboy.myLED.setR (0);
  if (color == 1)
    if(val) myESPboy.myLED.setG (200); else myESPboy.myLED.setG (0);
  if (color == 2)
    if(val) myESPboy.myLED.setB (200); else myESPboy.myLED.setB (0);
}


/* Buttons */
uint8_t Arduboy2Core::buttonsState(){
  static uint8_t buttons;	
  static uint16_t keystate;
  buttons = 0;
  keystate = myESPboy.getKeys();
    // LEFT_BUTTON, RIGHT_BUTTON, UP_BUTTON, DOWN_BUTTON, A_BUTTON, B_BUTTON
  if (keystate&PAD_ANY){
    if (keystate&PAD_LEFT)  { buttons |= LEFT_BUTTON; }  // left
    if (keystate&PAD_RIGHT) { buttons |= RIGHT_BUTTON; }  // right
    if (keystate&PAD_UP)    { buttons |= UP_BUTTON; }  // up
    if (keystate&PAD_DOWN)  { buttons |= DOWN_BUTTON; }  // down
    if (keystate&PAD_ACT)   { buttons |= A_BUTTON; }  // a?
    if (keystate&PAD_ESC)   { buttons |= B_BUTTON; }  // b?
    if (keystate&PAD_RGT)   { foregroundclr++; if (foregroundclr>18) foregroundclr=0; delay(300);} 
    if (keystate&PAD_LFT)   { backgroundclr++; if (backgroundclr>18) backgroundclr=0; delay(300);} 
  }
  return buttons;
}


// delay in ms with 16 bit duration
void Arduboy2Core::delayShort(uint16_t ms) {
  delay((unsigned long) ms);
};


