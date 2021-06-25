#pragma once

// For uint16_t
#include <stdint.h>

// For size_t
#include <stddef.h>

#include "Enums.h"

struct SaveEntry
{
	static constexpr size_t nameSize = NAME_LENGTH_PLUS_TERM;
	static constexpr size_t nameCount = (nameSize - 1);

	uint16_t score;
	char name[nameSize];
};