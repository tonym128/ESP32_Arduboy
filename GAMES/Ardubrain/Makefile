# Remember that the max binary file should be 28672 for the Program sections!

ARDUINO_LIBS = Arduboy2 EEPROM SPI
BOARD_TAG    = leonardo
ARDUINO_PORT = /dev/ttyACM0

CPPFLAGS += -Wall -DARDUINO_AVR_LEONARDO
CFLAGS   += -Wall -DARDUINO_AVR_LEONARDO

# Setup the 7.3.0 avr toolchain that is bundled with the arduino ide
# AVR_TOOLS_DIR = ~/.local/share/umake/ide/arduino/hardware/tools/avr/
# Setup the path to the arduino dir that has libraries/Arduboy2
# ARDUINO_DIR   = ~/.local/share/umake/electronics/arduino/

include /usr/share/arduino/Arduino.mk
