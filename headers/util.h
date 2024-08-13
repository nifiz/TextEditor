#ifndef UTIL_H
#define UTIL_H

#include <windows.h>

uint32 planarCoordToLinear(const COORD point, const COORD boundingRectangle);
COORD linearCoordToPlanar(const uint32 point, const COORD boundingRectangle);

typedef unsigned char uint8;
typedef char int8;
typedef unsigned short uint16;
typedef short int16;
typedef long int int32;
typedef unsigned long int uint32;
typedef unsigned char uchar;

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
} OperationType


#endif