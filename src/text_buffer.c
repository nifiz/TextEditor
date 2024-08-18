#include "../headers/text_buffer.h"

// Initializes the entire text buffer with PLACEHOLDER value (1)
uint8 initTextBufferEmpty(textBuffer* TBuffer, const uint32 bufferSize) {
    TBuffer->logicBufferSize = bufferSize;
    TBuffer->amtOfCharacters = 0;
    TBuffer->internalCursorPosition = 0;
    TBuffer->p_logicUCharBuffer = calloc(bufferSize, sizeof(unsigned char));

    // ETX - End of text sign
    *(TBuffer->p_logicUCharBuffer) = (uchar)0;

    for (uint32 idx = 1; idx < bufferSize; idx++) {
        *(TBuffer->p_logicUCharBuffer + idx) = PLACEHOLDER;
    }
}

uint8 freeTextBuffer(textBuffer* TBuffer) {
    void* memToFree = (void*)TBuffer->p_logicUCharBuffer;
    free(memToFree);
    TBuffer->p_logicUCharBuffer = NULL;
    return 0;
}

uint8 TBRemoveCharacterWithInternalCursor(textBuffer* TBuffer) {

    uint32 linearCoord = TBuffer->internalCursorPosition;

    memmove((TBuffer->p_logicUCharBuffer + linearCoord - 1),(TBuffer->p_logicUCharBuffer + linearCoord), TBuffer->amtOfCharacters - linearCoord + 1);

    if (TBuffer->internalCursorPosition == 0) return 0;

    *(TBuffer->p_logicUCharBuffer + (TBuffer->amtOfCharacters - 1)) = (uchar)0;

    TBuffer->amtOfCharacters--;

    TBMoveCursorBack(TBuffer);

    return 0;

}

uint8 TBInsertCharacterWithInternalCursor(textBuffer* TBuffer, const uchar character) {


    if (TBuffer->internalCursorPosition == ULONG_MAX) return 1;

    uint32 linearPosition = TBuffer->internalCursorPosition;

    // DEBUG VARIABLE
    uchar KURWA = *(TBuffer->p_logicUCharBuffer + linearPosition + 1);
    // END OF DEBUG VARIABLE

    memmove((TBuffer->p_logicUCharBuffer + linearPosition + 1),(TBuffer->p_logicUCharBuffer + linearPosition), TBuffer->amtOfCharacters - linearPosition + 1);

    *(TBuffer->p_logicUCharBuffer + linearPosition) = character;

    TBuffer->amtOfCharacters++;

    TBMoveCursorFront(TBuffer);
    
    return 0;
}

// [DEPRECATED]
uint8 TBInsertCharacterAt(textBuffer* TBuffer, const COORD cursorPosition, const COORD boundingRectangle, const uchar character) {
    
    uint32 linearPosition = planarCoordToLinear(cursorPosition, boundingRectangle);

    if (linearPosition >= TBuffer->logicBufferSize) return 1;

    memmove((TBuffer->p_logicUCharBuffer + linearPosition + 2),(TBuffer->p_logicUCharBuffer + linearPosition + 1), TBuffer->amtOfCharacters - linearPosition - 1);

    *(TBuffer->p_logicUCharBuffer + linearPosition + 1) = character;

    TBuffer->amtOfCharacters++;
    
    return 0;
}

// [DEPRECATED]
uint8 TBRemoveCharacter(textBuffer* TBuffer, const COORD cursorPosition, const COORD boundingRectangle) {
    
    uint32 linearCoord = planarCoordToLinear(cursorPosition, boundingRectangle);

    memmove((TBuffer->p_logicUCharBuffer + linearCoord),(TBuffer->p_logicUCharBuffer + linearCoord + 1), TBuffer->amtOfCharacters - linearCoord);

    *(TBuffer->p_logicUCharBuffer + (TBuffer->amtOfCharacters-1)) = PLACEHOLDER;

    TBuffer->amtOfCharacters--;
}

// Securely advances the 1D cursor or returns 1 on failure
uint8 TBMoveCursorFront(textBuffer* TBuffer) {

    if (TBuffer->internalCursorPosition < ULONG_MAX) {
        TBuffer->internalCursorPosition++;
        return 0;
    }

    else {
        return 1;
    }
}

// Securely moves the 1D cursor back or returns 1 on failure
uint8 TBMoveCursorBack(textBuffer* TBuffer) {

    if (TBuffer->internalCursorPosition >= 1) {
        TBuffer->internalCursorPosition--;
        return 0;
    }

    else {
        return 1;
    }

}