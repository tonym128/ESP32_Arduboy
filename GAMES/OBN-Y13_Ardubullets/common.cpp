#include "common.h"

/*  Defines  */

#define EEPROM_ADDR_BASE    1008
#define EEPROM_SIGNATURE    0x134E424FUL // "OBN\x13"

#define PAD_REPEAT_DELAY    (FPS / 4)
#define PAD_REPEAT_INTERVAL (FPS / 12)

#define IMG_BUTTONS_W       7
#define IMG_BUTTONS_H       7

enum RECORD_STATE_T {
    RECORD_NOT_READ = 0,
    RECORD_INITIAL,
    RECORD_STORED,
};

/*  Global Variables  */

MyArduboy2  arduboy;
boolean outEn() { return true;}
ESPboyPlaytune plTune(outEn);

RECORD_T    record;
uint8_t     counter;
int8_t      padX, padY, padRepeatCount;
bool        isInvalid, isRecordDirty;

/*  Local Functions  */

uint16_t calcCheckSum();

void     eepSeek(uint32_t addr);
uint8_t  eepRead8(void);
uint16_t eepRead16(void);
uint32_t eepRead32(void);
void     eepReadBlock(uint8_t  *p, uint8_t n);
void     eepWrite8(uint8_t val);
void     eepWrite16(uint16_t val);
void     eepWrite32(uint32_t val);
void     eepWriteBlock(uint8_t *p, uint8_t n);

/*  Local Variables  */

PROGMEM static const uint8_t imgButtons[][7] = { // 7x7 x2
    { 0x3E, 0x47, 0x6B, 0x6D, 0x6D, 0x41, 0x3E },
    { 0x3E, 0x41, 0x55, 0x55, 0x51, 0x65, 0x3E }
};

static RECORD_STATE_T   recordState = RECORD_NOT_READ;
static int16_t          eepAddr;

/*---------------------------------------------------------------------------*/
/*                             Common Functions                              */
/*---------------------------------------------------------------------------*/

void readRecord(void)
{
    bool isVerified = false;
    eepSeek(EEPROM_ADDR_BASE);
    if (eepRead32() == EEPROM_SIGNATURE) {
        eepReadBlock((uint8_t *)&record, sizeof(record));
        isVerified = (eepRead16() == calcCheckSum());
    }

    if (isVerified) {
        recordState = RECORD_STORED;
        isRecordDirty = false;
        dprintln(F("Read record from EEPROM"));
    } else {
        memset(&record, 0, sizeof(record));
        record.gameRank = GAME_RANK_DEFAULT;
        record.gameSeed = GAME_SEED_MAX;
        record.isBlinkLED = true;
        recordState = RECORD_INITIAL;
        isRecordDirty = true;
    }
    setSound(arduboy.isAudioEnabled()); // Load Sound ON/OFF
}

void writeRecord(void)
{
    if (!isRecordDirty) return;
    if (recordState == RECORD_INITIAL) {
        eepSeek(EEPROM_ADDR_BASE);
        eepWrite32(EEPROM_SIGNATURE);
    } else {
        eepSeek(EEPROM_ADDR_BASE + 4);
    }
    eepWriteBlock((uint8_t *)&record, sizeof(record));
    eepWrite16(calcCheckSum());
    arduboy.audio.saveOnOff(); // Save Sound ON/OFF
    recordState = RECORD_STORED;
    isRecordDirty = false;
    dprintln(F("Write record to EEPROM"));
}

uint16_t calcCheckSum()
{
    uint16_t checkSum = (EEPROM_SIGNATURE & 0xFFFF) + (EEPROM_SIGNATURE >> 16) * 3;
    uint16_t *p = (uint16_t *) &record;
    for (int i = 0; i < sizeof(record) / 2; i++) {
        checkSum += *p++ * (i * 2 + 5);
    }
    return checkSum;
}

void clearRecord(void)
{
    eepSeek(EEPROM_ADDR_BASE);
    for (int i = 0; i < (sizeof(record) + 6) / 4; i++) {
        eepWrite32(0);
    }
    recordState = RECORD_INITIAL;
    dprintln(F("Clear EEPROM"));
}

void handleDPad(void)
{
    padX = padY = 0;
    if (arduboy.buttonPressed(LEFT_BUTTON | RIGHT_BUTTON | UP_BUTTON | DOWN_BUTTON)) {
        if (++padRepeatCount >= (PAD_REPEAT_DELAY + PAD_REPEAT_INTERVAL)) {
            padRepeatCount = PAD_REPEAT_DELAY;
        }
        if (padRepeatCount == 1 || padRepeatCount == PAD_REPEAT_DELAY) {
            if (arduboy.buttonPressed(LEFT_BUTTON))  padX--;
            if (arduboy.buttonPressed(RIGHT_BUTTON)) padX++;
            if (arduboy.buttonPressed(UP_BUTTON))    padY--;
            if (arduboy.buttonPressed(DOWN_BUTTON))  padY++;
        }
    } else {
        padRepeatCount = 0;
    }
}

void drawTime(int16_t x, int16_t y, uint32_t frames)
{
    uint16_t h = frames / (FPS * 3600UL);
    uint8_t m = frames / (FPS * 60) % 60;
    uint8_t s = frames / FPS % 60;
    arduboy.setCursor(x, y);
    if (h == 0 && m == 0) {
        if (s < 10) arduboy.print('0');
        arduboy.print(s);
        arduboy.print('.');
        arduboy.print(frames / (FPS / 10) % 10);
    } else {
        if (h > 0) {
            arduboy.print(h);
            arduboy.print(':');
            if (m < 10) arduboy.print('0');
        }
        arduboy.print(m);
        arduboy.print(':');
        if (s < 10) arduboy.print('0');
        arduboy.print(s);
    }
}

void printGameSeed(int16_t x, int16_t y, uint32_t seed)
{
    arduboy.setCursor(x, y);
    for (int i = 0; i < GAME_SEED_TOKEN_MAX; i++) {
        int token = seed % GAME_SEED_TOKEN_VAL;
        char c = (token < GAME_SEED_TOKEN_ALP) ? 'A' + token : '.';
        arduboy.print(c);
        seed /= GAME_SEED_TOKEN_VAL;
    }
}

void drawButtonIcon(int16_t x, int16_t y, bool isB)
{
    arduboy.drawBitmap(x, y, imgButtons[isB],
            IMG_BUTTONS_W, IMG_BUTTONS_H, arduboy.getTextColor());
}

/*---------------------------------------------------------------------------*/
/*                              Sound Functions                              */
/*---------------------------------------------------------------------------*/

void setSound(bool on)
{
    arduboy.setAudioEnabled(on);
    dprint(F("audioEnabled="));
    dprintln(on);
}

void playSoundTick(void)
{
    plTune.tone(440, 10);
}

void playSoundClick(void)
{
    plTune.tone(587, 20);
}

/*---------------------------------------------------------------------------*/
/*                             EEPROM Functions                              */
/*---------------------------------------------------------------------------*/

void eeprom_busy_wait(){};

uint8_t eeprom_read_byte(uint16_t eepAddr){
  uint8_t dta;
  EEPROM.get(eepAddr, dta);
  return (dta);  
};

uint16_t eeprom_read_word(uint16_t eepAddr){
  uint16_t dta;
  EEPROM.get(eepAddr, dta);
  return (dta);  
};

uint32_t eeprom_read_dword(uint16_t eepAddr){
  uint32_t dta;
  EEPROM.get(eepAddr, dta);
  return (dta); 
};

void eeprom_read_block(uint8_t *p, uint16_t  eepAddr, uint8_t n){
  uint8_t dta;
  for (uint8_t i=0; i<n; i++){
    EEPROM.get(eepAddr+i, dta);
    p[i]=dta;
    };
};

void eeprom_write_byte(uint16_t eepAddr, uint8_t val){
  EEPROM.put(eepAddr, val);
  EEPROM.commit();
};

void eeprom_write_word(uint16_t eepAddr, uint16_t val){
  EEPROM.put(eepAddr, val);
  EEPROM.commit();
  };

void eeprom_write_dword(uint16_t eepAddr, uint32_t val){
  EEPROM.put(eepAddr, val);
  EEPROM.commit();
  };

void eeprom_write_block(uint8_t *p, uint16_t eepAddr, uint8_t n){
  for (uint8_t i=0; i<n; i++){
    EEPROM.put(eepAddr+i, p[i]);
  }
  EEPROM.commit();
};



void eepSeek(uint32_t addr)
{
    //eepAddr = max(addr, EEPROM_STORAGE_SPACE_START);
    eepAddr = addr;
}

uint8_t eepRead8(void)
{
    eeprom_busy_wait();
    return eeprom_read_byte(eepAddr++);
}

uint16_t eepRead16(void)
{
    eeprom_busy_wait();
    uint16_t ret = eeprom_read_word(eepAddr);
    eepAddr += 2;
    return ret;
}

uint32_t eepRead32(void)
{
    eeprom_busy_wait();
    uint32_t ret = eeprom_read_dword(eepAddr);
    eepAddr += 4;
    return ret;
}

void eepReadBlock(uint8_t *p, uint8_t n)
{
    eeprom_busy_wait();
    eeprom_read_block(p,eepAddr, n);
    eepAddr += n;
}

void eepWrite8(uint8_t val)
{
    eeprom_busy_wait();
    eeprom_write_byte(eepAddr, val);
    eepAddr++;
}

void eepWrite16(uint16_t val)
{
    eeprom_busy_wait();
    eeprom_write_word(eepAddr, val);
    eepAddr += 2;
}

void eepWrite32(uint32_t val)
{
    eeprom_busy_wait();
    eeprom_write_dword(eepAddr, val);
    eepAddr += 4;
}

void eepWriteBlock(uint8_t *p, uint8_t n)
{
    eeprom_busy_wait();
    eeprom_write_block(p, eepAddr, n);
    eepAddr += n;
}
