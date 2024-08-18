#ifndef EDITOR_H
#define EDITOR_H
#include "parse.h"
#include <windows.h>

void  run_editor                    (char* fname, char* const p_buffer, uint32 bufferSize);
void  insert_character              (CHAR_INFO* p_buffer, const COORD sizeOfBuffer, COORD cursorPosition, const char CHAR_TO_INSERT);
void  remove_character              (CHAR_INFO* p_buffer, const COORD sizeOfBuffer, COORD cursorPosition);
void  move_character_newline        (CHAR_INFO* p_buffer, const COORD sizeOfBuffer, COORD cursorPosition, const char CHAR_TO_INSERT);
void  move_cursor                   (KEY_ARROW key, COORD* cursor, const COORD SCREEN_SIZE, const uint32 MAX_ALLOWED_COLUMN);

#endif