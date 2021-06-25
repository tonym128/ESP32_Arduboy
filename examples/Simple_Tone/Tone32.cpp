#include "Tone32.h"

bool tone32setup = false;
bool timerRunning = false;

void setupTimer() {
    // Use 1st timer of 4  - 1 tick take 1/(80MHZ/80) = 1us so we set divider 80 and count up 
    timer = timerBegin(0, 80, true);//div 80
    timerAttachInterrupt(timer, &onTimer, true);
}

void setFrequency(long frequencyHz){
    if (timerRunning) {
        timerAlarmDisable(timer);
        timerRunning = false;
    }

    if (frequency > 0)
    {
        timerAlarmWrite(timer, 1000000l / frequencyHz, true);
        timerAlarmEnable(timer);
        timerRunning = true;
    }
}

void tone32(uint8_t pin, unsigned int frequency, unsigned long duration, uint8_t channel)
{
    if (!tone32setup) {
        pinMode(PIN, OUTPUT);    // sets the digital pin as output
        setupTimer();
        tone32setup = true;
    }

    setFrequency(frequencyHz);
    if (durationMs > 0) {
        delay(durationMs);
        noTone32(pin, channel);
    }
}

void noTone32(uint8_t pin, uint8_t channel)
{
    setFrequency(0);
    digitalWrite(PIN, 0); 
}
