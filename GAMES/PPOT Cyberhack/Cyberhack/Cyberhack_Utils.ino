#include "src/utils/Arduboy2Ext.h"

void clearLEDs() {

    #ifdef USE_LED_ANALOGUE
    arduboy.setRGBled(RED_LED, 0);
    arduboy.setRGBled(GREEN_LED, 0);
    arduboy.setRGBled(BLUE_LED, 0);
    #endif

}

void printHex(uint8_t data) {

  if (data < 16) { font3x5.print("0"); }
  font3x5.print(data, HEX);

}

void initByteBeat() {

	#ifdef BYTE_BEAT_SOUNDS

        // Timer 1 setup for ByteBeat interrupts and red and blue RGB LED PWM.
        // Phase correct PWM 8 bit.
        // OC1A and OC1B set on up-counting / clear on down-counting (inverted).
        // Top set by ICR1. Clock set to system clock/8 (from prescaler).
        // Counting frequency will be 2MHz resulting in interrupts at 7812.5Hz
        TCCR1A = _BV(COM1A1) | _BV(COM1A0) | _BV(COM1B1) | _BV(COM1B0) | _BV(WGM11);
        TCCR1B = _BV(WGM13) | _BV(CS11);
        // set top count
        ICR1 = 255;
        // interrupt on capture (top) and overflow (bottom)
        TIMSK1 = _BV(ICIE1) | _BV(TOIE1);

        // make sure blue and red RGB LEDs are turned off
        OCR1A = 0;
        OCR1B = 0;

        // // set up Timer 3
        // TCCR1A = 0; // normal operation
        // TCCR1B = _BV(WGM32) | _BV(CS30); // CTC, no pre-scaling
        // OCR1A = 1999; // compare A register value (8000Hz)
        // TIMSK1 = _BV(OCIE1A); // interrupt on Compare A Match		

        // //set up Timer 4
        //TCCR4A = _BV(COM4A1) | _BV(PWM4A); // Fast-PWM 8-bit, toggle speaker pin 2 

        // to mute ByteBeat (but continue with the score while muted):
        TCCR4A = _BV(PWM4A);
        TCCR4B = _BV(CS40); // 62500Hz
        OCR4C = 0xFF; // Resolution to 8-bit (TOP=0xFF)   

        // Set up Timer0 for PWM control of the green RGB LED and make sure it's off
        TCCR0A = _BV(COM0A1) | _BV(WGM01) | _BV(WGM00);
        OCR0A = 255;

    #endif

}

void disableByteBeat() {

//RS    TCCR4A = _BV(PWM4A);
    
}

void enableByteBeat() {

    // to unmute ByteBeat with both pins used for high volume:
    // TCCR4A = _BV(COM4A0) | _BV(PWM4A);

    // to unmute ByteBeat only on speaker pin 2 for normal volume
    // leaving speaker pin 1 free for ArduboyTones or BeepPin1:
//RS    TCCR4A = _BV(COM4A1) | _BV(PWM4A);

}

uint8_t isSubArray(uint8_t fullArray[], uint8_t subArray[], uint8_t fullArrayLength, uint8_t subArrayLength)  { 

    // Two pointers to traverse the arrays ..

    uint8_t i = 0;
    uint8_t j = 0; 
  
    // Traverse both arrays simultaneously ..

    while (i < fullArrayLength && j < subArrayLength) { 
  
        // If element matches increment both pointers ..

        if (fullArray[i] == subArray[j]) { 
  
            i++; 
            j++; 
  
            // If fullArrayLength is completely traversed ?

            if (i == fullArrayLength) return j; 
  
            // If subArray is completely traversed ?

            if (j == subArrayLength) return subArrayLength; 
        } 

        // If not, increment i and reset j 

        else { 
            i = i - j + 1; 
            j = 0; 
        } 

    } 
  
    return 0; 

} 

void flickering(uint8_t y) {

    uint8_t frameCount = arduboy.getFrameCount(192);

    #ifdef FLICKERING
        switch (frameCount) {

            case 0:
            case 1:
            case 5:
            case 7:
            case 16:
            case 61:
                arduboy.fillRect(0, y, WIDTH, 24, BLACK);
                break;

            case 64 ... 65:            
                arduboy.fillRect(0, y, WIDTH, 3, BLACK);
                break;

            case 66:
            case 75:
                arduboy.fillRect(0, y + 16, WIDTH, 5, BLACK);
                break;

            case 69 ... 70:       
                arduboy.fillRect(0, y + 5, WIDTH, 8, BLACK);
                break;

            case 132:       
            case 136:       
                arduboy.drawFastHLine(0, y + 2, 40, BLACK);
                arduboy.drawFastHLine(30, y + 17, 50, BLACK);
                arduboy.drawFastHLine(60, y + 25, 50, BLACK);
                break;

            case 134:       
                arduboy.drawFastHLine(60, y + 7, 40, BLACK);
                arduboy.drawFastHLine(30, y + 17, 50, BLACK);
                arduboy.drawFastHLine(0, y + 25, 50, BLACK);
                break;

            default:
                break;
                
        }
    #endif

}
