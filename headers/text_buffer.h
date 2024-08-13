#include "parse.h"
#include "util.h"
#include <windows.h>

#define PLACEHOLDER (unsigned char)1

typedef struct {
    uchar* p_logicUCharBuffer;
    uint32 logicBufferSize;
    uint32 amtOfCharacters;
    uint32 internalCursorPosition;
} textBuffer;

// Constructor and Destructor of sorts
uint8 initTextBufferEmpty   (textBuffer* TBuffer, const uint32 bufferSize);
uint8 freeTextBuffer        (textBuffer* TBuffer);

// Functions
uint8 TBRemoveCharacter     (textBuffer* TBuffer, const COORD cursorPosition, const COORD boundingRectangle);
uint8 TBInsertCharacterAt   (textBuffer* TBuffer, const COORD cursorPosition, const COORD boundingRectangle, const uchar character);