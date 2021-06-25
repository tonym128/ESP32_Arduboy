#ifndef util_h
#define util_h

#define SERIAL_LOG 0

#include <Arduino.h>
#include "math.h"

#define LOG(msg) \
    do { if (SERIAL_LOG) Serial.println(msg); } while (0)

#define LOGV(var) \
    do { if (SERIAL_LOG) Serial.print(#var); Serial.print(": "); Serial.println(var); } while (0)
#endif

