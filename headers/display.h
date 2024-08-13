#ifndef DISPLAY_H
#define DISPLAY_H

#include <Windows.h>
#include "operation.h"
#include "text_buffer.h"

// Flushes the screen with a given character
void clear_screen(const char fillerChar);
void updateScreenBuffer(CHAR_INFO* pDisplayCharBuffer, textBuffer* pLogicCharBuffer, const operation _operation);

#endif