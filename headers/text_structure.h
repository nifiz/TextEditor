#ifndef TEXT_STRUCTURE
#define TEXT_STRUCTURE

#include "parse.h"
#include <windows.h>

typedef struct {
    uint32 amtOfLines;
    uint32* carriageReturnPosition;
    uint32 endOfText;
} textStructure;

uint8 removeLine                  (textStructure* textStruct, uint32 lineNumber);
uint8 appendLine                  (textStructure* textStruct);
uint8 insertLine                  (textStructure* textStruct, uint32 cursorYPosition);
uint8 initTextStructureEmpty      (textStructure* textStructToInit, uint32 amtOfLines);
uint8 initTextStructureFromBuffer (textStructure* textStructToInit, const CHAR_INFO* initCharBuffer, const COORD initCharBuffDimensions);
uint8 freeTextStructure           (textStructure* textStructToFree);


#endif