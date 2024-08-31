#ifndef TEXT_BUFFER_H
#define TEXT_BUFFER_H

#include "parse.h"
#include "util.h"

#define PLACEHOLDER (unsigned char)1

typedef struct {
    uchar* p_logicUCharBuffer;
    uint32 logicBufferSize;
    uint32 amtOfCharacters;
    uint32 internalCursorPosition;
    uint8  tabSize;
} textBuffer;

// Constructor and Destructor of sorts
uint8 initTextBufferEmpty                   (textBuffer* TBuffer, const uint32 bufferSize);
uint8 initTextBufferFromString              (textBuffer* TBuffer, const uint32 bufferSize, const char* source, uint32 sourceSize);
uint8 freeTextBuffer                        (textBuffer* TBuffer);

// Functions
uint8 TBRemoveCharacterWithInternalCursor   (textBuffer* TBuffer);
uint8 TBInsertCharacterWithInternalCursor   (textBuffer* TBuffer, const uchar character);

// Deprecated at this point
uint8 TBRemoveCharacter                     (textBuffer* TBuffer, const COORD cursorPosition, const COORD boundingRectangle);
uint8 TBInsertCharacterAt                   (textBuffer* TBuffer, 
                                             const COORD cursorPosition, 
                                             const COORD boundingRectangle, 
                                             const uchar character);

// Cursor
uint8 TBMoveCursorFront                     (textBuffer* TBuffer);
uint8 TBMoveCursorBack                      (textBuffer* TBuffer);
uint8 TBMoveCursorUp                        (textBuffer* TBuffer, const COORD displayRectangle, const COORD cursorPosition);
uint8 TBMoveCursorDown                      (textBuffer* TBuffer, const COORD displayRectangle, const COORD cursorPosition);

// Auxilliary static function(s)

static uint32 findLogicIndex                (const textBuffer* TBuffer, const COORD displayRectangle, const COORD cursorPosition);

#endif