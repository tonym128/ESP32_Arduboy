#include "common.h"

static MODE_T mode;

void callInitFunc(MODE_T mode_){
  if(mode == MODE_LOGO) initLogo();
  if(mode == MODE_TITLE) initTitle();
  if(mode == MODE_GAME) initGame();
};

MODE_T callUpdateFunc(MODE_T mode_){
  if(mode == MODE_LOGO) return (updateLogo());
  if(mode == MODE_TITLE) return (updateTitle());
  if(mode == MODE_GAME) return (updateGame());
};

void callDrawFunc(MODE_T mode_){
  if(!pt.playing()) pt.stopScore();
  if(mode == MODE_GAME) drawGame();
  if(mode == MODE_LOGO) drawLogo();
  if(mode == MODE_TITLE) drawTitle();
};


/*  For Debugging  */
#ifdef DEBUG
bool    dbgPrintEnabled = true;
char    dbgRecvChar = '\0';

static void dbgCheckSerialRecv(void)
{
    int recv;
    while ((recv = Serial.read()) != -1) {
        switch (recv) {
        case 'd':
            dbgPrintEnabled = !dbgPrintEnabled;
            Serial.print("Debug output ");
            Serial.println(dbgPrintEnabled ? "ON" : "OFF");
            break;
        case 'r':
            clearRecord();
            break;
        }
        if (recv >= ' ' && recv <= '~') {
            dbgRecvChar = recv;
        }
    }
}
#endif

/*---------------------------------------------------------------------------*/

void setup()
{
#ifdef DEBUG
    Serial.begin(115200);
#endif
    arduboy.beginNoLogo();
    arduboy.setFrameRate(FPS);
    //arduboy.setTextColors(WHITE, WHITE);
    mode = MODE_LOGO;
    callInitFunc(mode);
}

void loop()
{
  delay(0);
#ifdef DEBUG
    dbgCheckSerialRecv();
#endif
    if (!(arduboy.nextFrame())) return;
    MODE_T nextMode = callUpdateFunc(mode);
    callDrawFunc(mode);
#ifdef DEBUG
    dbgRecvChar = '\0';
#endif
    arduboy.display();
    if (mode != nextMode) {
        mode = nextMode;
#ifdef DEBUG        
        dprint(F("mode="));
        dprintln(mode);
#endif
        callInitFunc(mode);
    }
}
