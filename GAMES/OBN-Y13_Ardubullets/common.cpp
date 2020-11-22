#include "common.h"

/*  Defines  */

#define EEPROM_ADDR_BASE    1008
uint32_t EEPROM_SIGNATURE=0x134E424FUL; // "OBN\x13"

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

boolean outEn() { return true;}
ESPboyPlaytune pt(outEn);

MyArduboy2  arduboy;
RECORD_T    record;
int     counter;
int8_t      padX, padY, padRepeatCount;
bool        isInvalid, isRecordDirty;

/*  Local Functions  */

static uint16_t calcCheckSum();

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
    uint32_t addr = EEPROM_ADDR_BASE;
    uint32_t readedsignature;
    uint16_t readedchecksum;
    
    EEPROM.get(addr,readedsignature);
    addr+=sizeof(EEPROM_SIGNATURE);
        
    if (readedsignature == EEPROM_SIGNATURE) {
        EEPROM.get(addr,record);
        addr+=sizeof(record);
        EEPROM.get(addr,readedchecksum);
        isVerified = (readedchecksum == calcCheckSum());
        dprintln(F("EEPROM signature OK"));
    }

    if (isVerified) dprintln(F("EEPROM checksumm OK"));
    else dprintln(F("EEPROM checksumm FAIL"));

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
        writeRecord();
    }
    setSound(arduboy.isAudioEnabled()); // Load Sound ON/OFF
}

void writeRecord(void)
{
    uint32_t addr = EEPROM_ADDR_BASE;
    if (isRecordDirty) clearRecord;
    EEPROM.put(addr, EEPROM_SIGNATURE);
    addr += sizeof(EEPROM_SIGNATURE);
    EEPROM.put(addr, record);
    addr += sizeof(record);
    EEPROM.put(addr,calcCheckSum());
    arduboy.audio.saveOnOff(); // Save Sound ON/OFF
    EEPROM.commit();
    recordState = RECORD_STORED;
    isRecordDirty = false;
    dprintln(F("Write record to EEPROM"));
}

static uint16_t calcCheckSum()
{
    uint16_t checkSum = 0x13;
    uint8_t *p = (uint8_t *) &record;
    for (int i = 0; i < sizeof(record); i++)  
      checkSum += p[i];
    return checkSum;
}

void clearRecord(void)
{
    uint32_t addr = EEPROM_ADDR_BASE;
    for (int i = 0; i < (sizeof(record)+sizeof(EEPROM_SIGNATURE)+sizeof(uint16_t)); i++) {
        EEPROM.put(addr+i,0);
    }
    EEPROM.commit();
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
    pt.tone(440, 10);
}

void playSoundClick(void)
{
    pt.tone(587, 20);
}
