#ifndef EDITOR_H
#define EDITOR_H
#include "parse.h"
#include <windows.h>

void run_editor(char*, char* const, uint32);

typedef enum {
    EDITOR_ERROR, EDITOR_EXIT, EDITOR_SAVE
} EDITOR_ACTION;

void clear_screen(void);
void content_append_character(char* const, char);
void content_remove_character(char* const);
void print_to_screen(char*, const char* const, const uint32, const uint8, char);
uint8 w_get_display_width(void);

#endif