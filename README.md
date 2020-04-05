# ESPboy (ESP8266 gadget) port of the Arduboy2 library
This port of **"Arduboy2"** and **"Arduboy PlayTones"** libraries from [Arduboy project ](https://arduboy.com/ "Arduboy project ") intended to run on[ ESPboy project](https://hackaday.io/project/164830-espboy-games-iot-stem-for-education-fun " ESPboy project").

It supports ESPboy buttons, LED, sound (thanks to ported **"Arduboy PlayTones" library**) and TFT display.

This port compiles for ESP8266 ESPboy, existing Arduboy2-compatible games and apps. 
Some of them can use it as a drop-in replacement for the original **"Arduboy2" library**, other games will run after the slight modifications.

# Migrating the game from Arduboy to ESPboy
1. replace the **"Arduboy2" library** and **"Arduboy tones" library** in your Arduino Studio libraries folder with these versions.
2. do ingame modifications according to the following notes:
- change **"#include arduboy.h"** to **"#include arduboy2.h"**
- some games use a function pointer array to pass control to a different parts of the code as the game state changes. In ATMEGA32U4 the memory address is 2 bytes (single word) long, in ESP8266, the memory addresses is 4 bytes (double word) long, So you need to change all "pgm_read_word" to "pgm_read_dword" at the pointers
- if EEPROM is used by the game to keep configs/high scores,
-- add EEPROM.begin(100) at setup() // 100 is just a rough max no. need to check the size
-- add EEPROM.commit() after the last EEPROM.put(), EEPORM.write() and EEPROM.update() of each blocks of code.
- remove any reference to the **"ATMlib"**, **"ArduboyPlaytune"** that require timers to play background music. That libraries has not been ported yet.
- games that directly control the SPI or I2C bus to write to OLED display need much more work to port instead of the simple steps above.
- font() array is used in TFT_eSPI display library so you have to change all "font" to "font_"
- min() and max() macros are used in TFT_eSPI display library so you have to change all min() and max() to minVal() and maxVal() correspondenly
- there is a problem with a "char" data (this is the signed/unsigned problem). The game variables which are not really char or char* should be changed correspondingly "unsigned char" to "uint8_t" and "char" or "signed char" to "int8t". Don't forget to set #include "Arduino.h" at the first line.
- it's better to change all "short" to "int16_t", "unsigned shot" to "uint16_t", "byte" to "uint8_t", "int" to "int16_t", "unsigned int" to "uint16_t", "long" to "int32_t", "unsigned long" to "uint32_t" 


# Advantages of ESPboy (ESP8266)
- 8 keys
- TFT color LCD 128x128
- Faster MCU 80Mh/160Mhz
- RGB neopixel LED
- More memory for program and data.  http://0x04.net/~mwk/doc/xtensa.pdf
- Internal flash memory mounted as SPIFFS disk 4-16mb for additional data
- WiFi onboard opens up the possibility for online games, top scores posting online
- OTA (on the air firmware update) update the firmware through WiFi from a web site, the same way as Android playmarket/ Apple istore.
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

If authors against posting the code in this repository for the purpose of learning programming and fun, please let me know, I'll remove it.

- **Catacombs of the damned** (MIT) by [James Howard](https://community.arduboy.com/t/catacombs-of-the-damned-formerly-another-fps-style-3d-demo/6565 "James Howard")
- **Mystic Balloon** (MIT) by [TEAM ARG](http://www.team-arg.org/ "by TEAM ARG")
- **Sirene** (MIT) by [TEAM ARG](http://www.team-arg.org/ " by TEAM ARG")
- **Arduventure** (MIT) by [TEAM ARG](http://www.team-arg.org/ "by TEAM ARG")
- **CastleBoy** (MIT) by [jlauener](https://github.com/ArduboyCollection/CastleBoy)
- **Dark And Under** (BSD-3-Clause) by [Garage Collective (Cyril Guichard (Luxregina), Simon Holmes (Filmote), Pharap)](https://github.com/ArduboyCollection/Dark-And-Under)
- **HelloCommander** (MIT) by [Felipe Manga (FManga)](https://github.com/ArduboyCollection/HelloCommander)
- **Blob Attack** (MIT) by [TEAM ARG](http://www.team-arg.org/ "by TEAM ARG")
- **Virus-LQP-79** (MIT) by [TEAM ARG](http://www.team-arg.org/ "by TEAM ARG")
- **Kong-II** (MIT) by [Vampirics and Filmote](https://github.com/Press-Play-On-Tape/Kong-II)
- **LodeRunner (all 154 levels in a single game!)** (BSD-3) by [Simon Holmes (filmote)](https://community.arduboy.com/t/lode-runner-a-trilogy-in-four-parts/5385)
- **MicroCity** (GPL-3.0) by [James Howard](https://community.arduboy.com/t/microcity-city-simulation-in-your-pocket/4808)
- **SpaceCab** (GPL-3.0) by [Stephane C (vampirics) and Simon Holmes (filmote)](https://community.arduboy.com/t/space-cab-v1-0-1/6055)
- **The Curse Of Astarok** (MIT) by [Simon Holmes (filmote)](https://community.arduboy.com/t/the-curse-of-astarok-v1-0-with-sound/8203)
- **Rayne the rogue** (MIT) by [shdwwzrd](http://neoretro.games)
- **Space Battle - Trench Run** (MIT) by [Lucas Cardinali (lscardinali)](https://community.arduboy.com/t/space-battle-trench-run/8075)
- **MicroTD** (CC0-1.0) by [drummyfish](https://community.arduboy.com/t/microtd-tower-defense-for-arduboy/6223)
- **Shadow-Runner-1.6.1** (MIT) by [TEAM ARG](http://www.team-arg.org/)
- **Evade** (MIT) by [Modus Create](https://community.arduboy.com/t/evade-side-scrolling-space-shooter/2712)
- **Rooftop Rescue** (MIT) by [Bert van't Veer](https://community.arduboy.com/t/rooftop-rescue/7269)
- **Crates 3D** (MIT) by [Badja (original) & Brian (port)](https://community.arduboy.com/t/crates-3d-a-port-of-the-ti-83-puzzle-game/1540)
- **JetPac** (Proprietary) by [Mike McRoberts](https://community.arduboy.com/t/arduboy-jet-pac/2888)
- **Squario** (Proprietary) Squario by [arduboychris](https://community.arduboy.com/t/squario-the-ultimate-adventure/1257)
- **Circuit Dude** (Proprietary) by [Jonathan Holmes](http://www.crait.net)
- **Omega Horizon** (Proprietary) by [shdwwzrd](http://neoretro.games)
- **Ard-Drivin** (MIT) by [Rem and LP](https://community.arduboy.com/t/ard-drivin-fast-paced-racing-sim/3178)
- **Tamaguino** (GNU GPL v3.0) by [Alojz Jakob](https://alojzjakob.github.io/Tamaguino/), ported to Arduboy by [Scott R](https://community.arduboy.com/t/tamaguino-ab-tamagotchi-for-arduino/4806)
- **Glove** (MIT) by [fuopy](https://github.com/ArduboyCollection/glove)
- **Blocks** (GPL-3.0) by [w3woody](https://github.com/ArduboyCollection/blocks)
- **Galaxion** (MIT) by [tako2](https://github.com/ArduboyCollection/galaxion)
- **Karateka** (BSD-3-Clause) by [Simon Holmes (filmote)](https://community.arduboy.com/t/karateka-v0-9/4852)
- **Tackle Box** (Proprietary) by [Matt](https://www.city41.games/tacklebox)
- **Not Just a Hat Rack** (Proprietary) by [Hundstrasse](https://hundstrasse.com/arduboy-games/not-just-a-hat-rack/)
- **Super-Crate-Buino-branched** (LGPL-3.0-or-later) by [Aurélien Rodot](http://legacy.gamebuino.com/forum/viewtopic.php?f=17&t=3151) [(ported by uXe)](https://github.com/Rodot/Super-Crate-Buino)
- **ESP8266_ArduBOYNG** - UNKNOWN
- **ESP8266_breakout-v** - UNKNOWN
- **ESP8266_picovaders** - UNKNOWN

# Contributions and Disclaimer
Contributions to the documentation, code or electronics are welcome. 

Use this library at your own risk.

# ESPboy project links
- [Youtube videos](https://www.youtube.com/channel/UCsMjlCb8CK-Cw54lRrkpbQw "Youtube videos")
- [Community](https://www.espboy.com "Community")
- [Hackaday page](https://hackaday.io/project/164830-espboy-games-iot-stem-for-education-fun "Hackaday page")
- [Software](https://github.com/ESPboy-edu "Software")
- [Schematic, PCB, Gerber](https://easyeda.com/roman.sokolov/espboy-rev3-5 "Schematic, PCB, Gerber")
- E-mail: espboy.edu@gmail.com
