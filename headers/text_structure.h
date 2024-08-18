#ifndef TEXT_STRUCTURE_H
#define TEXT_STRUCTURE_H

#include "parse.h"
#include <windows.h>
#include <limits.h>

// Struct that helps managing info about last characters in each line.
// Has to be freed after being initialized
typedef struct {
    uint32  amtOfLines;
    uint32* carriageReturnPosition;
    uint32  endOfText;
} textStructure;

uint8 removeLine                  (textStructure* textStruct, uint32 lineNumber);
uint8 appendLine                  (textStructure* textStruct, uint32 lastCharacterPosition);
uint8 insertLine                  (textStructure* textStruct, uint32 cursorYPosition, uint32 lastCharacterPosition);
uint8 initTextStructureEmpty      (textStructure* textStructToInit, uint32 amtOfLines);
uint8 initTextStructureFromBuffer (textStructure* textStructToInit, const CHAR_INFO* initCharBuffer, const COORD initCharBuffDimensions);
uint8 freeTextStructure           (textStructure* textStructToFree);
uint32 getLastCharInLine          (const textStructure* textStruct, const uint32 lineNumber);

#endif