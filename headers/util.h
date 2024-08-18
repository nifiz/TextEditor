#ifndef UTIL_H
#define UTIL_H

#include <windows.h>

#define MAX_LOGIC_CHARBUFF_SIZE 1000000

typedef char              int8;
typedef short             int16;
typedef long int          int32;
typedef unsigned char     uint8;
typedef unsigned short    uint16;
typedef unsigned long int uint32;
typedef unsigned char     uchar;
typedef uchar             byte;

extern uint8              TAB_SIZE;
extern const uchar        WHITESPACE; 

typedef enum {
    ACTION_ERROR, ACTION_NEW_FILE, 
    ACTION_OPEN_FILE, ACTION_LIST, 
    ACTION_MOVE_DIR_UP, ACTION_MOVE_DIR_DOWN, 
    ACTION_EXIT
} ActionType;

typedef enum {
    OP_INSERT, OP_REMOVE,
    OP_APPEND, OP_NEWLINE,
    OP_ARR_UP, OP_ARR_DOWN,
    OP_ARR_LEFT, OP_ARR_RIGHT
} OperationType;

typedef enum {
    NULLCHAR = 0, BACKSPACE = 8, TAB = 9, ARROW_KEY = 224
} SpecialKeys;

typedef enum {
    EDITOR_ERROR, EDITOR_EXIT, EDITOR_SAVE
} EDITOR_ACTION;

typedef enum {
    ARR_UP= 72, ARR_DOWN = 80, ARR_LEFT = 75, ARR_RIGHT = 77, LINE_BEGIN = 71, LINE_END = 79
} KEY_ARROW;

uint32 planarCoordToLinear  (const COORD point, const COORD boundingRectangle);
COORD linearCoordToPlanar   (const uint32 point, const COORD boundingRectangle);
uint8 w_get_display_width   (void);
uint8 w_get_display_height  (void);

#endif