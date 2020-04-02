/**
 * @file Arduboy2Core.cpp
 * \brief
 * The Arduboy2Core class for Arduboy hardware initilization and control.
 */

#include "Arduboy2Core.h"
TFT_eSPI screen;

#ifdef ADAFRUIT
Adafruit_MCP23017 mcp;
Adafruit_MCP4725 dac;
#endif

#ifdef ESP8266
ESPboyLED myled;
#endif

uint8_t Arduboy2Core::sBuffer[];

Arduboy2Core::Arduboy2Core() {}

void Arduboy2Core::boot(){
  Serial.begin(115200);       
  #ifdef ESP32
  esp_timer_init();
  #endif
  //WiFi.mode(WIFI_OFF);
  delay(100);
//LED init
#ifdef ESP8266
  myled.begin();
#endif

#ifdef ADAFRUIT
//DAC init, LCD backlit off
  dac.begin(MCP4725address);
  delay(50);
  dac.setVoltage(0, false);
  delay(100);

//MCP23017 and buttons init, should preceed the TFT init
  mcp.begin(MCP23017address);
  delay(100);

  for (int i = 0; i < 8; ++i){
    mcp.pinMode(i, INPUT);
    mcp.pullUp(i, HIGH);
  }

//Sound init and test
  pinMode(PIN_SPEAKER_1, OUTPUT);
  tone(PIN_SPEAKER_1, 200, 100);
  delay(100);
  tone(PIN_SPEAKER_1, 100, 100);
  delay(100);
  noTone(PIN_SPEAKER_1);

//TFT init
  mcp.pinMode(CSTFTPIN, OUTPUT);
  mcp.digitalWrite(CSTFTPIN, LOW);
#endif

  screen.begin();
  delay(200);
  screen.setRotation(0);
  screen.fillScreen(TFT_BLACK);
  Serial.write("Screen Init\r\n");

#ifdef ADAFRUIT
//LCD backlit on
  for (uint8_t bcklt=0; bcklt<100; bcklt++){
    dac.setVoltage(bcklt*20, false);
    delay(10);
  }

  dac.setVoltage(4095, true);
  delay(500);
#else
    delay(100);
#endif

  screen.fillScreen(TFT_BLACK);
  Serial.write("Boot Done!");
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
#ifdef ADAFRUIT
dac.setVoltage(0, false);
#endif
};

// Restart the display after a displayOff()
void Arduboy2Core::displayOn() {
#ifdef ADAFRUIT
dac.setVoltage(4095, false);
#endif
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
#ifdef ESP8266
  myled.setRGB (red, green, blue);
#endif
};

void Arduboy2Core::setRGBled(uint8_t color, uint8_t val){
#ifdef ESP8266
  if (color == RED_LED)   myled.setR (val);
  else
    if (color == GREEN_LED) myled.setG (val);
    else
      if (color == BLUE_LED)  myled.setB (val);
#endif
};


void Arduboy2Core::digitalWriteRGB(uint8_t red, uint8_t green, uint8_t blue){
#ifdef ESP8266
  if (red) myled.setR (200); else myled.setR (0); 
  if (green) myled.setG (200); else myled.setG (0); 
  if (blue) myled.setB (200); else myled.setB (0); 
#endif
};


void Arduboy2Core::digitalWriteRGB(uint8_t color, uint8_t val){
#ifdef ESP8266
  if (color == 0)
  	if(val) myled.setR (200); else myled.setR (0);
  if (color == 1)
    if(val) myled.setG (200); else myled.setG (0);
  if (color == 2)
    if(val) myled.setB (200); else myled.setB (0);
#endif
}

#ifdef ESP32
static const byte TOUCH_SENSE_MAX = 50;
static const byte TOUCH_SENSE_MIN = 20;
static int inputVal = 0;
static int TOUCH_SENSE[8];
static int touch_values[8];
static bool inputSetup = false;

// Button definitions
static const int P1_Left = 0;
static const int P1_Top = 1;
static const int P1_Right = 2;
static const int P1_Bottom = 3;

static const int P2_Right = 4;
static const int P2_Bottom = 5;
static const int P2_Left = 6;
static const int P2_Top = 7;

static const int BIT_P1_Left = 1;
static const int BIT_P1_Top = 2;
static const int BIT_P1_Right = 4;
static const int BIT_P1_Bottom = 8;

static const int BIT_P2_Right = 16;
static const int BIT_P2_Bottom = 32;
static const int BIT_P2_Left = 64;
static const int BIT_P2_Top = 128;

static bool readAnalogSensor(int pin, int touch_sense)
{
  inputVal = touchRead(pin);
  return inputVal < touch_sense && inputVal > 0;
}

static int readAnalogSensorRaw(int pin)
{
  inputVal = touchRead(pin);
  return inputVal;
}

static byte getReadShiftAnalog()
{
  byte buttonVals = 0;
  buttonVals = buttonVals | (readAnalogSensor(32, TOUCH_SENSE[0]) << P1_Left);
  buttonVals = buttonVals | (readAnalogSensor(14, TOUCH_SENSE[1]) << P1_Top);
  buttonVals = buttonVals | (readAnalogSensor(27, TOUCH_SENSE[2]) << P1_Right);
  buttonVals = buttonVals | (readAnalogSensor(33, TOUCH_SENSE[3]) << P1_Bottom);

  buttonVals = buttonVals | (readAnalogSensor(15, TOUCH_SENSE[4]) << P2_Right);
  buttonVals = buttonVals | (readAnalogSensor(13, TOUCH_SENSE[5]) << P2_Bottom);
  buttonVals = buttonVals | (readAnalogSensor(2, TOUCH_SENSE[6]) << P2_Left);
  buttonVals = buttonVals | (readAnalogSensor(12, TOUCH_SENSE[7]) << P2_Top);

  return buttonVals;
}

static void getRawInput()
{
  for (int i = 0; i < 8; i++)
  {
    touch_values[i] = 0;
  }

  int i = 0;
  touch_values[i++] = readAnalogSensorRaw(32); // Left
  touch_values[i++] = readAnalogSensorRaw(14); // Up
  touch_values[i++] = readAnalogSensorRaw(27); // Right
  touch_values[i++] = readAnalogSensorRaw(33); // Down
  touch_values[i++] = readAnalogSensorRaw(15); // A
  touch_values[i++] = readAnalogSensorRaw(13); // Start
  touch_values[i++] = readAnalogSensorRaw(2);  // B
  touch_values[i++] = readAnalogSensorRaw(12); // Select
}

static byte getReadShift()
{
  return getReadShiftAnalog();
}

static uint16_t keyStateThread;
static SemaphoreHandle_t xSemaphoreInput;

static void inputThread(void *empty) {
  for ( ;; ) {
    if (xSemaphoreTake(xSemaphoreInput, (TickType_t)100) == pdTRUE) {
      keyStateThread = getReadShift();
      xSemaphoreGive(xSemaphoreInput);
      vTaskDelay(10);
    }
  }
}

#endif

/* Buttons */
static uint16_t keystate;
uint8_t buttons = 0;	

uint8_t Arduboy2Core::buttonsState(){
buttons = 0;
#ifdef ESP8266
  keystate = ~mcp.readGPIOAB() & 255;

  // LEFT_BUTTON, RIGHT_BUTTON, UP_BUTTON, DOWN_BUTTON, A_BUTTON, B_BUTTON
  if (keystate&PAD_ANY){
    if (keystate&PAD_LEFT)  { buttons |= LEFT_BUTTON; }  // left
    if (keystate&PAD_RIGHT) { buttons |= RIGHT_BUTTON; }  // right
    if (keystate&PAD_UP)    { buttons |= UP_BUTTON; }  // up
    if (keystate&PAD_DOWN)  { buttons |= DOWN_BUTTON; }  // down
    if (keystate&PAD_ACT)   { buttons |= A_BUTTON; }  // a?
    if (keystate&PAD_ESC)   { buttons |= B_BUTTON; }  // b?
  }

#else
    // Initial Setup
    if (inputSetup) {
      if (xSemaphoreTake(xSemaphoreInput, (TickType_t)100) == pdTRUE) {
        keystate = keyStateThread;
        xSemaphoreGive(xSemaphoreInput);
        }

      if (keystate & BIT_P1_Left) buttons |= LEFT_BUTTON;//Left
      if (keystate & BIT_P1_Right) buttons |= RIGHT_BUTTON;//Right
      if (keystate & BIT_P1_Top) buttons |= UP_BUTTON;//Up
      if (keystate & BIT_P1_Bottom) buttons |= DOWN_BUTTON;;//Down
      if (keystate & BIT_P2_Right) buttons |= A_BUTTON;;//A
      if (keystate & BIT_P2_Bottom) buttons |= A_BUTTON;//A
      if (keystate & BIT_P2_Left) buttons |= B_BUTTON;//B
      if (keystate & BIT_P2_Top) buttons |= B_BUTTON;//B
    } else {
      TaskHandle_t xHandle = NULL;
      xSemaphoreInput = xSemaphoreCreateMutex();
 
      getRawInput();
      for (int i = 0; i < 8; i++)
      {
        TOUCH_SENSE[i] /= 2;
        if (TOUCH_SENSE[i] > TOUCH_SENSE_MAX)
          TOUCH_SENSE[i] = TOUCH_SENSE_MAX;
        if (TOUCH_SENSE[i] < TOUCH_SENSE_MIN)
          TOUCH_SENSE[i] = TOUCH_SENSE_MIN;
        
      }

      xTaskCreatePinnedToCore(inputThread, "Input", 16384, nullptr, 1, &xHandle, 0);

      inputSetup = true;
    }
#endif

  return buttons;
}


// delay in ms with 16 bit duration
void Arduboy2Core::delayShort(uint16_t ms) {
  delay((unsigned long) ms);
};


