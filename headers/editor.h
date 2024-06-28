#ifndef EDITOR_H
#define EDITOR_H
#include "parse.h"
#include <windows.h>


typedef enum {
    EDITOR_ERROR, EDITOR_EXIT, EDITOR_SAVE
} EDITOR_ACTION;

typedef enum {
    ARR_UP= 72, ARR_DOWN = 80, ARR_LEFT = 75, ARR_RIGHT = 77, LINE_BEGIN = 71, LINE_END = 79
} KEY_ARROW;

void  run_editor                    (char* fname, char* const p_buffer, uint32 bufferSize);
void  clear_screen                  (void);
void  insert_character              (CHAR_INFO* p_buffer, const COORD sizeOfBuffer, COORD cursorPosition, const char CHAR_TO_INSERT);
void  remove_character              (CHAR_INFO* p_buffer, const COORD sizeOfBuffer, COORD cursorPosition);
void  move_character_newline        (CHAR_INFO* p_buffer, const COORD sizeOfBuffer, COORD cursorPosition, const char CHAR_TO_INSERT);
void  print_top_bar_to_screen       (const char* filename, const uint32 wordCount, unsigned char isEdited);
void  print_to_screen               (char* filename, const char* const p_buffer, const uint32 wordCount, const uint8 FRAMES, char isEdited);
uint8 w_get_display_width           (void);
uint8 w_get_display_height          (void);
void  move_cursor                   (KEY_ARROW key, COORD* cursor, const COORD SCREEN_SIZE);
void  adjust_screen_buffer_size     (COORD cursorPosition, COORD* currentScrBuffSize);

#endif