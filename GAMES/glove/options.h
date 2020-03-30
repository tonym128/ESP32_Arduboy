#include "Arduino.h"
#ifndef _OPTIONS_H
#define _OPTIONS_H

#define SETTING_FINISHED 0
#define SETTING_CHANGED 1
#define SETTING_REBOOT 2

uint8_t displayOptionsMenu();
uint8_t displayClearMenu();

#endif
