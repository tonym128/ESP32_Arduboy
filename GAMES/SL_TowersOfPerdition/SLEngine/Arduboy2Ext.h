#ifndef ARDUBOY2EXT_H
#define ARDUBOY2EXT_H

#include <Arduboy2.h>

class Arduboy2Ext : public Arduboy2Base {

  public:

    Arduboy2Ext() : Arduboy2Base() {}

    uint8_t justPressedButtons() const
		{
			return (~previousButtonState & currentButtonState);
		}
    uint8_t pressedButtons() const
		{
			return currentButtonState;
		}
    void clearButtonState()
		{
			currentButtonState = previousButtonState = 0;
		}
};

#endif //ARDUBOY2EXT_H
