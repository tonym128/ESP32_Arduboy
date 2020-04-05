CPPFLAGS += -DSERIAL_LOG=0

ARDUINO_DIR = $(HOME)/.arduino_ide
ARDMK_DIR = $(HOME)/.arduino_mk
BOARD_TAG = leonardo

LOCAL_CPP_SRCS = $(wildcard src/*.cpp) $(wildcard src/entities/*.cpp)

include $(ARDMK_DIR)/Arduino.mk

emu: build-leonardo/tacklebox.hex
	pabe build-leonardo/tacklebox.hex

bmp:
	node ./bmptool/ --src maskPngs --dest src/maskBitmaps.h --type combined && \
    node ./bmptool/ --src nonMaskPngs --dest src/nonMaskBitmaps.h --type bitmap

map:
	node ./maptool/ -s tiled -o src

strings:
	node ./stringtool/ -s strings.json -o src/strings.h

fish:
	node ./fishtool/ -s fish.json -o src
