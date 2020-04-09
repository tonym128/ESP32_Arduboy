/**
 * @file Arduboy2Core.cpp
 * \brief
 * The Arduboy2Core class for Arduboy hardware initilization and control.
 */

#include "Arduboy2Core.h"

#include "secrets.h"
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

const int XRES = SCREEN_WIDTH;
const int YRES = SCREEN_HEIGHT;

//Graphics using the defined resolution for the backbuffer
CompositeGraphics graphics(XRES, YRES);
//Composite output using the desired mode (PAL/NTSC) and twice the resolution. 
//It will center the displayed image automatically
CompositeOutput composite(CompositeOutput::NTSC, XRES * XMULT, YRES * YMULT);

#include <Ps3Controller.h>

static uint64_t inputlastTime = 0;
static uint64_t inputcurrentTime = 0;
static uint64_t inputframeTime = 0;
extern uint64_t inputfps;

uint8_t Arduboy2Core::sBuffer[];

void compositeCore(void *data)
{
  while (true)
  {
    //just send the graphics frontbuffer whithout any interruption 
    composite.sendFrameHalfResolution(&graphics.frame);
    //Serial.write(".");
  }
}

void initOTA() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.print("Attempting to connect to network, SSID: ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    // wait 10 seconds for connection:
    delay(10000);
  }

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

Arduboy2Core::Arduboy2Core() {}

void Arduboy2Core::boot()
{
  Serial.begin(115200);
  esp_timer_init();

  // Start OTA
  Serial.write("Start OTA\n");
  initOTA();
  Serial.write("Start OTA Done\n");

  //initializing DMA buffers and I2S
  composite.init();
  //initializing graphics double buffer
  graphics.init();
  Serial.write("Screen Init\n");
  delay(100);
  Serial.write("Boot Done!");
  xTaskCreatePinnedToCore(compositeCore, "c", 1024, NULL, 1, NULL, 1);
}

void Arduboy2Core::setCPUSpeed8MHz(){};
void Arduboy2Core::bootPins(){};
void Arduboy2Core::bootOLED(){};
void Arduboy2Core::LCDDataMode(){};
void Arduboy2Core::LCDCommandMode(){};
void Arduboy2Core::bootSPI() {}
void Arduboy2Core::SPItransfer(uint8_t data){};
void Arduboy2Core::safeMode() {}
void Arduboy2Core::bootPowerSaving(){};
void Arduboy2Core::sendLCDCommand(uint8_t command){};
void Arduboy2Core::exitToBootloader(){};
void Arduboy2Core::mainNoUSB(){};
// turn all display pixels on, ignoring buffer contents
// or set to normal buffer display
void Arduboy2Core::allPixelsOn(bool on){};
void Arduboy2Core::blank(){};
// invert the display or set to normal
// when inverted, a pixel set to 0 will be on
void Arduboy2Core::invert(bool inverse){};
// flip the display vertically or set to normal
void Arduboy2Core::flipVertical(bool flipped){};
// flip the display horizontally or set to normal
void Arduboy2Core::flipHorizontal(bool flipped){};
void Arduboy2Core::paint8Pixels(uint8_t pixels) {}
void Arduboy2Core::freeRGBled(){};

/* Power Management */
void Arduboy2Core::idle()
{
  delay(1);
};

// Shut down the display
void Arduboy2Core::displayOff()
{
#ifdef ADAFRUIT
  dac.setVoltage(0, false);
#endif
};

// Restart the display after a displayOff()
void Arduboy2Core::displayOn()
{
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
void Arduboy2Core::paintScreen(uint8_t image[], bool clear){
    //  memcpy(sBuffer, image, HEIGHT*WIDTH/8);
    //  if (clear) memset(sBuffer, 0, HEIGHT*WIDTH/8);
};

/* RGB LED */
void Arduboy2Core::setRGBled(uint8_t red, uint8_t green, uint8_t blue)
{
#ifdef ESP8266
  myled.setRGB(red, green, blue);
#endif
};

void Arduboy2Core::setRGBled(uint8_t color, uint8_t val)
{
};

void Arduboy2Core::digitalWriteRGB(uint8_t red, uint8_t green, uint8_t blue)
{
};

void Arduboy2Core::digitalWriteRGB(uint8_t color, uint8_t val)
{

}

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
static bool inputSetup = false;

static bool buttonRun = false;
int ps3player = 0;
int battery = 0;

void onConnect(){
    Serial.println("PS3 GamePad Connected.");
    Serial.print("Setting LEDs to player "); Serial.println(ps3player, DEC);
    Ps3.setPlayer(ps3player);
    ps3player += 1;
    if(ps3player > 10) ps3player = 0;
}

bool ps3left = 0;
bool ps3right = 0;
bool ps3up = 0;
bool ps3down = 0;
bool ps3cross = 0;
bool ps3square = 0;
bool ps3circle = 0;
bool ps3triangle = 0;

void notify() {
    if( Ps3.event.button_down.up )
      ps3up = 1;
    if( Ps3.event.button_up.up )
      ps3up = 0;

    if( Ps3.event.button_down.down )
      ps3down = 1;
    if( Ps3.event.button_up.down )
      ps3down = 0;

    if( Ps3.event.button_down.left )
      ps3left = 1;
    if( Ps3.event.button_up.left )
      ps3left = 0;

    if( Ps3.event.button_down.right )
      ps3right = 1;
    if( Ps3.event.button_up.right )
      ps3right = 0;

    if( Ps3.event.button_down.cross )
      ps3cross = 1;
    if( Ps3.event.button_up.cross )
      ps3cross = 0;

    if( Ps3.event.button_down.square )
      ps3square = 1;
    if( Ps3.event.button_up.square )
      ps3square = 0;

    if( Ps3.event.button_down.triangle )
      ps3triangle = 1;
    if( Ps3.event.button_up.triangle )
      ps3triangle = 0;

    if( Ps3.event.button_down.circle )
      ps3circle = 1;
    if( Ps3.event.button_up.circle )
      ps3circle = 0;
}

void ps3gamepad_init() {
    Ps3.attach(notify);
    Ps3.attachOnConnect(onConnect);
    Ps3.begin("01:02:03:04:05:06");
}

byte ps3gamepad_loop() {
  byte buttonVals = 0;

  if(!Ps3.isConnected())
    return 0;

  buttonVals = buttonVals | (ps3left  << P1_Left);
  buttonVals = buttonVals | (ps3right << P1_Right);
  buttonVals = buttonVals | (ps3up    << P1_Top);
  buttonVals = buttonVals | (ps3down  << P1_Bottom);

  buttonVals = buttonVals | (ps3circle << P2_Right);
  buttonVals = buttonVals | (ps3cross << P2_Bottom);
  buttonVals = buttonVals | (ps3square << P2_Left);
  buttonVals = buttonVals | (ps3triangle << P2_Top);

  return buttonVals;
}

static byte getReadPS3GamePad()
{
  if (!buttonRun)
  {
    ps3gamepad_init();
    buttonRun = true;
  }
  else
  {
    return ps3gamepad_loop();
  }

  return 0;
}

static byte getReadShift()
{
  return getReadPS3GamePad();
}

/* Buttons */
static uint16_t keystate;
uint8_t buttons = 0;

uint8_t Arduboy2Core::buttonsState()
{
  buttons = 0;
  // Initial Setup
  if (inputSetup)
  {
    keystate = getReadShift();

    if (keystate & BIT_P1_Left)
      buttons |= LEFT_BUTTON; //Left
    if (keystate & BIT_P1_Right)
      buttons |= RIGHT_BUTTON; //Right
    if (keystate & BIT_P1_Top)
      buttons |= UP_BUTTON; //Up
    if (keystate & BIT_P1_Bottom)
      buttons |= DOWN_BUTTON;
    ; //Down
    if (keystate & BIT_P2_Right)
      buttons |= A_BUTTON;
    ; //A
    if (keystate & BIT_P2_Bottom)
      buttons |= A_BUTTON; //A
    if (keystate & BIT_P2_Left)
      buttons |= B_BUTTON; //B
    if (keystate & BIT_P2_Top)
      buttons |= B_BUTTON; //B
  }
  else
  {
    inputSetup = true;
  }

  return buttons;
}

// delay in ms with 16 bit duration
void Arduboy2Core::delayShort(uint16_t ms)
{
  delay((unsigned long)ms);
};
