#include "../headers/text_structure.h"
#include "../headers/char_operations.h"
#include <string.h>
#include <limits.h>

// uint8 removeLine(const uint32 lineNumber);
// uint8 appendLine(textStructure* textStruct);
// uint8 insertLine(textStructure* textStruct, uint32 cursorYPosition);
// uint8 freeTextStructure(textStructure* TSToFree);

uint8 removeLine(textStructure* textStruct, uint32 lineNumber) {

    // shifts an entire following block of memory over the line being removed
    memmove(textStruct->carriageReturnPosition + lineNumber, textStruct->carriageReturnPosition + lineNumber + 1, textStruct->amtOfLines - lineNumber);

    // ULONG_MAX is going to mean a deleted line since line can't have a carriage return at such position
    *(textStruct->carriageReturnPosition + (textStruct->amtOfLines-1)) = ULONG_MAX;

    textStruct->amtOfLines--;
    
    return 0;
}
// Reads CHAR_INFO buffer, fills internal carriage return array and returns
uint8 initTextStructureFromBuffer (textStructure* textStructToInit, const CHAR_INFO* initCharBuffer, const COORD initCharBuffDimensions) {    

    uint32 lastCharPosition = 0;

    initTextStructureEmpty(textStructToInit, initCharBuffDimensions.Y);

    for (uint32 Y = 0; Y < initCharBuffDimensions.Y; Y++) {
        
        lastCharPosition = last_character_in_string_CHARINFO(initCharBuffer, initCharBuffDimensions.X, (unsigned char)32);

        *(textStructToInit->carriageReturnPosition + Y) = lastCharPosition;
        
    }
    
    return 0;
}

// Fills internal array of length "amtOfLines" with ULONG_MAX (max for uint32)
uint8 initTextStructureEmpty(textStructure* textStructToInit, uint32 amtOfLines) {
    
    textStructToInit->amtOfLines = amtOfLines;

    textStructToInit->carriageReturnPosition = calloc(amtOfLines, sizeof(uint32));

    for (uint32 idx = 0; idx < amtOfLines; idx++) { *(textStructToInit->carriageReturnPosition+idx) = ULONG_MAX; }
    
    return 0;
}

uint8 freeTextStructure(textStructure* textStructToFree) {
    
    free((void*)textStructToFree->carriageReturnPosition);

    textStructToFree->carriageReturnPosition = NULL;

    return 0;
}

// Appends a line, returns 1 if max size was reached.
// 2 if textStruct pointer is NULL
// 0 on success
uint8 appendLine(textStructure* textStruct, uint32 lastCharacterPosition) {
    
    // Check for overflow
    if (textStruct->amtOfLines == ULONG_MAX) return 1;

    if (textStruct == NULL) return 2;

    *(textStruct->carriageReturnPosition + textStruct->amtOfLines) = lastCharacterPosition;

    textStruct->amtOfLines++;

    return 0;
}

// Sets the horizontal position lastCharacterPosition on line below cursorYPosition
uint8 insertLine(textStructure* textStruct, uint32 cursorYPosition, uint32 lastCharacterPosition) {

    if (textStruct->amtOfLines == ULONG_MAX) return 1;

    memmove(textStruct->carriageReturnPosition + cursorYPosition + 2, textStruct->carriageReturnPosition + cursorYPosition + 1, textStruct->amtOfLines - cursorYPosition);

    textStruct->amtOfLines++;

    *(textStruct->carriageReturnPosition + cursorYPosition + 1) = lastCharacterPosition;
    
    return 0;
}

uint32 getLastCharInLine(const textStructure* textStruct, const uint32 lineNumber) {
    if (lineNumber > textStruct->amtOfLines) return ULONG_MAX;
    return *(textStruct->carriageReturnPosition + lineNumber);
}