# ESP32 port of the ESPboy Arduboy2 library

# ESPboy (ESP8266 gadget) port of the Arduboy2 library
This port of **"Arduboy2"** and **"Arduboy PlayTones"** libraries from [Arduboy project ](https://arduboy.com/ "Arduboy project ") intendet to run on[ ESPboy project](https://hackaday.io/project/164830-espboy-games-iot-stem-for-education-fun " ESPboy project") and thorougly simplified.

It supports ESPboy buttons, LED, sound (thanks to ported **"Arduboy PlayTones" library**) and TFT display.

This port compiles for ESP8266 ESPboy, existing Arduboy2-compatible games and apps. 
Some of them can use it as a drop-in replacement for the original **"Arduboy2" library**, other games will run after the slight modifications.

# Migrating the game from Arduboy to ESPboy
1. replace the **"Arduboy2" library** and **"Arduboy tones" library** in your Arduino Studio libraries folder with this versions.
2. do ingame modifications according to following notes:
- change **"#include arduboy.h"** to **"#include arduboy2.h"**
- some games use a function pointer arrary to pass control to different part of the codes as the game state changes. In ATMEGA32U4 the memory address are 2 bytes (single word) long, in ESP8266, the memory addresses are 4 bytyes (doube word) long, So you need to change all "pgm_read_word" to "pgm_read_dword"
- if EEPROM is used by the game to keep configs/high scores,
-- add EEPROM.begin(100) at setup() // 100 is just a rough max no. need to check the size
-- add EEPROM.commit() after the last EEPROM.put(), EEPORM.write() and EEPROM.update() of each blocks of code.
- remove any reference to the **"ATMlib"**, **"ArduboyPlaytune"** that require timers to play back ground musics. That libraries has not yet been ported
- games that directly control the SPI or I2C bus to write to OLED display need much more work to port instead of the simple steps above.
- font() array is used in TFT_eSPI display library so you have to chage all "font" to "font_"
- min() and max() macros are used in TFT_eSPI display library so you have to chage all min() and max() to minVal() and maxVal() correspondenly


# Advantages of ESPboy (ESP8266)
- 8 keys
- TFT color LCD 128õ128
- Faster MCU 80Mh/160Mhz
- RGB neopixel LED
- More memory for program and data.  http://0x04.net/~mwk/doc/xtensa.pdf
- Internal flash memory mounted as SPIFFS disk 4-16mb for additional data
- WiFi onboard, opens up possibility for online games, top scores posting online
- OTA (on the air firmare update) update the firmeare through WiFi from a web site, the same way as Android playstore/ Apple istore.
- Cheaper overall cost


# Credits and Documentation

**99% of the work on the library was done by the contributors to the following repositories:**

**Arduboy2 library:**
- https://github.com/Arduboy/Arduboy 
- https://github.com/MLXXXp/Arduboy2
- https://github.com/harbaum/Arduboy2 
- https://github.com/hartmann1301/Arduboy2 hartmann1301 migrated the library to the ESP8266 platform
- https://github.com/cheungbx/esp8266_arduboy2
- https://github.com/edgarborja/Arduboy2ESP

**Arduboy tones library:**
- https://github.com/MLXXXp/ArduboyTones
- https://github.com/hartmann1301/ArduboyTones

**And all the [ARDUBOY COMMUNITY!](https://community.arduboy.com/ "ARDUBOY COMMUNITY")**

*Please do read through the extensive documentation included there.*

# There are a few games in this package
**Check the LICENCE files in every folder!**

If authors against posting the code in this repository for the purpose of learning programming and fun, please let me know, i'll remove it.

- **Catacombs of the damned** (MIT) [by James Howard](https://community.arduboy.com/t/catacombs-of-the-damned-formerly-another-fps-style-3d-demo/6565 "James Howard")
- **Mystic Balloon** (MIT) [by TEAM ARG](http://www.team-arg.org/ "by TEAM ARG")
- **Sirene** (MIT) [ by TEAM ARG](http://www.team-arg.org/ " by TEAM ARG")
- **Arduventure** (MIT) [by TEAM ARG](http://www.team-arg.org/ "by TEAM ARG")
- **CastleBoy** (MIT) by jlauener
- **Dark And Under** (BSD-3-Clause) by Garage Collective (Cyril Guichard (Luxregina), Simon Holmes (Filmote), Pharap)
- **HelloCommander** (MIT) by Felipe Manga (FManga)
- **Blob Attack** (MIT) [by TEAM ARG](http://www.team-arg.org/ "by TEAM ARG")
- **Virus-LQP-79** (MIT) [by TEAM ARG](http://www.team-arg.org/ "by TEAM ARG")
- **Kong-II** (MIT) by Vampirics and Filmote
- **LodeRunner (all 154 levels in a single game!)** (BSD-3) by Simon Holmes (filmote)
- **MicroCity** (GPL-3.0) by James Howard
- **SpaceCab** (GPL-3.0) by by Stephane C (vampirics) and Simon Holmes (filmote)
- **The Curse Of Astarok** (MIT) by Simon Holmes (filmote)
- **Rayne the rogue** (MIT) [by shdwwzrd](http://neoretro.games)
- **Space Battle - Trench Run** (MIT) by Lucas Cardinali (lscardinali)
- **MicroTD** (CC0-1.0) by drummyfish
- **Shadow-Runner-1.6.1** (MIT) [by TEAM ARG](http://www.team-arg.org/)
- **Evade** (MIT) by Modus Create
- **Rooftop Rescue** (MIT) by Bert van't Veer
- **Crates 3D** (MIT) by Badja (original) & Brian (port)
- **JetPac** (Proprietary) by Mike McRoberts
- **Squario** (Proprietary) Squario by arduboychris 
- **Circuit Dude** (Proprietary) [by Jonathan Holmes](http://www.crait.net)
- **Omega Horizon** (Proprietary) [by shdwwzrd](http://neoretro.games)
- **ESP8266_ArduBOYNG** - UNKNOWN
- **ESP8266_breakout-v** - UNKNOWN
- **ESP8266_picovaders** - UNKNOWN

# Contributions and Disclaimer
Contributions to the documentation, code or electronics are welcome. 

Use this library at your own risk.

# ESPboy project links
- ##[Youtube videos](https://www.youtube.com/channel/UCsMjlCb8CK-Cw54lRrkpbQw "Youtube videos")
- ##[Community](https://www.espboy.com "Community")
- ##[Hackaday page](https://hackaday.io/project/164830-espboy-games-iot-stem-for-education-fun "Hackaday page")
- ##[Software](https://github.com/ESPboy-edu "Software")
- ##[Schematic, PCB, Gerber](https://easyeda.com/roman.sokolov/espboy-rev3-5 "Schematic, PCB, Gerber")
- ##E-mail: espboy.edu@gmail.com
