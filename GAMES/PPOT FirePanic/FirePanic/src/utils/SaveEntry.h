#pragma once

// For uint16_t
#include <stdint.h>
#include "Enums.h"

struct SaveEntry{
	uint16_t score;
	char name[NAME_LENGTH_PLUS_TERM];
};