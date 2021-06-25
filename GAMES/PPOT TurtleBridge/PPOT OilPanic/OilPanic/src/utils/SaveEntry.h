#pragma once

#include <stdint.h>
#include <stddef.h>
#include "consts.h"

struct SaveEntry{
	uint16_t score;
	char name[4];
};