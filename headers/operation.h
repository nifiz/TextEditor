#ifndef OPERATION_H
#define OPERATION_H

#include <windows.h>
#include "util.h"

typedef struct {
    COORD coord;
    OperationType type;
    uchar character;
} operation;

#endif