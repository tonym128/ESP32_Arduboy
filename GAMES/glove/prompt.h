#ifndef _PROMPT_H
#define _PROMPT_H

#include "globals.h"

void prompt_draw(const char* const* prompts, uint8_t promptCount, int8_t choice);
char prompt_start(const char* const* prompts, uint8_t promptCount);
void error(const char* message);
int8_t prompt_keyboard(int8_t& cursorPos);
void prompt_flash(const char* msg);

#endif
