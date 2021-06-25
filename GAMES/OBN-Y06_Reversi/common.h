#pragma once

#include "MyArduboy.h"
#include <ESPboyPlaytune.h>

/*  Defines  */

//#define DEBUG
#define FPS             60
#define APP_TITLE       "REVERSI"
#define APP_CODE        "OBN-Y06"
#define APP_VERSION     "0.11"
#define APP_RELEASED    "NOVEMBER 2018"

enum MODE_T {
    MODE_LOGO = 0,
    MODE_TITLE,
    MODE_GAME,
};

enum GAME_MODE_T {
    GAME_MODE_BLACK = 0,
    GAME_MODE_WHITE,
    GAME_MODE_2PLAYERS,
};

#define WIN_LOSE_MAX        10
#define WIN_LOSE_VALUE_MAX  (((WIN_LOSE_MAX + 1) * (WIN_LOSE_MAX + 1)) - 1)

#define SETTING_BIT_THINK_LED       0x1
#define SETTING_BIT_STONES_COUNTER  0x2
#define SETTING_BIT_SHOW_PLACEABLE  0x4
#define SETTING_BIT_SHOW_FIXED      0x8

/*  Typedefs  */

typedef struct {
    uint8_t x:3;
    uint8_t y:3;
} POS_T;

typedef struct {
    uint8_t     black[8], white[8];
    uint8_t     gameMode:2;
    uint8_t     cpuLevel:3;
    uint8_t     canContinue:1;
    uint8_t     isWhiteTurn:1;
    uint8_t     isLastPassed:1;
    POS_T       cursorPos;
    uint8_t     isAdvancedLevel:1;
    uint8_t     vsCpuWinLose:7;
    uint8_t     settings;
    uint32_t    playFrames;
    uint16_t    playCount;
} RECORD_T; // sizeof(RECORD_T) musb be 26 bytes

/*  Global Functions (Common)  */

void    readRecord(void);
void    writeRecord(void);
void    clearRecord(void);

void    handleDPad(void);
void    drawNumber(int16_t x, int16_t y, int32_t value);
void    drawTime(int16_t x, int16_t y, uint32_t frames);

void    setSound(bool on);
void    playSoundTick(void);
void    playSoundClick(void);

void    eepSeek(uint32_t addr);
uint8_t eepRead8(void);
uint16_t eepRead16(void);
uint32_t eepRead32(void);
void    eepReadBlock(uint8_t *p, uint8_t n);
void    eepWrite8(uint8_t val);
void    eepWrite16(uint16_t val);
void    eepWrite32(uint32_t val);
void    eepWriteBlock(uint8_t *p, uint8_t n);

/*  Global Functions (Menu)  */

void    clearMenuItems(void);
void    addMenuItem(const __FlashStringHelper *label, void (*func)(void));
int8_t  getMenuItemPos(void);
int8_t  getMenuItemCount(void);
void    setMenuCoords(int8_t x, int8_t y, int8_t w, int8_t h, bool f, bool s);
void    setMenuItemPos(int8_t pos);
void    handleMenu(void);
void    drawMenuItems(bool isForced);
void    drawSoundEnabled(void);

/*  Global Functions (Each Mode)  */

void    initLogo(void);
MODE_T  updateLogo(void);
void    drawLogo(void);

void    initTitle(void);
MODE_T  updateTitle(void);
void    drawTitle(void);

void    initGame(void);
MODE_T  updateGame(void);
void    drawGame(void);

/*  Global Variables  */

extern MyArduboy    arduboy;
extern ESPboyPlaytune pTunes;
extern RECORD_T     record;

extern bool     isRecordDirty;
extern int8_t   padX, padY, padRepeatCount;
extern bool     isInvalid;

/*  For Debugging  */

#ifdef DEBUG
extern bool             dbgPrintEnabled;
extern char             dbgRecvChar;
#define dprint(...)     (!dbgPrintEnabled || Serial.print(__VA_ARGS__))
#define dprintln(...)   (!dbgPrintEnabled || Serial.println(__VA_ARGS__))
#else
#define dprint(...)
#define dprintln(...)
#endif
