# ESP8266 port of the Arduboy2 library

This port of the Arduboy library compiles for the ESP8266 from Arduino Studio, existing Arduboy2-compatible games like CastleBoy by TeamARG can use it as a drop-in replacement for the original Arduboy2 library, other games such as Squario or Virus LQP-79 run with slight modifications, but hopefully they will run natively in future releases.

# Usage

Simply replace the Arduboy2 library in your Arduino Studio libraries folder with this version.

# Hardware

The library was tested on a NodeMCU module with a ESP-12N (yes, N) microcontroller.
It requires an SSD1306 based OLED module



# Motivation

I wanted to build the simplest (and cheapest) portable and programmable game console, so that new tinkerers can have an accessible experience of putting it together, and experience the thrill that I felt when I started editing gorilla.bas and nibbles.bas in DOS in the early 90's.

After looking around, I found the awesome Arduboy project, and the community it generated, which inspired me to join that effort and contribute if possible. 

# Credits and Documentation

99% of the work on the library was done by the contributors to the following repositories:

https://github.com/Arduboy/Arduboy 
https://github.com/MLXXXp/Arduboy2
https://github.com/harbaum/Arduboy2 
https://github.com/hartmann1301/Arduboy2 Migrated the library to the ESP8266 platform, from there just a few tweaks were needed.

Please do read through the extensive documentation included there.
