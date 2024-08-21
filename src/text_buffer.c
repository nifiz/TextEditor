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

    if (TBuffer->internalCursorPosition < ULONG_MAX && TBuffer->internalCursorPosition < TBuffer->amtOfCharacters) {
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

uint8 TBMoveCursorUp(textBuffer* TBuffer, const COORD displayRectangle, const COORD cursorPosition) {
    if (cursorPosition.Y == 0) return 1;
    COORD cursorPositionTarget = {cursorPosition.X, cursorPosition.Y - 1};
    TBuffer->internalCursorPosition = findLogicIndex(TBuffer, displayRectangle, cursorPositionTarget);
    return 0;
}
uint8 TBMoveCursorDown(textBuffer* TBuffer, const COORD displayRectangle, const COORD cursorPosition) {
    if (cursorPosition.Y == displayRectangle.Y) return 1;
    COORD cursorPositionTarget = {cursorPosition.X, cursorPosition.Y + 1};

    TBuffer->internalCursorPosition = findLogicIndex(TBuffer, displayRectangle, cursorPositionTarget);
    return 0;
}

static uint32 findLogicIndex(const textBuffer* TBuffer, const COORD displayRectangle, const COORD cursorPosition) {
    
    uint32 idx = 0;
    uint32 logicIdx = 0;
    BOOL ETX_FOUND = FALSE;
    BOOL foundTargetDisplayLocation = FALSE;
    uchar pickup;
    COORD cursor = {0,0};
    uint32 targetLocationOnScreenLinear = planarCoordToLinear(cursorPosition, displayRectangle);

    for (; (idx < displayRectangle.X*displayRectangle.Y && ETX_FOUND == FALSE); idx++, logicIdx++) {
        
        pickup = *(TBuffer->p_logicUCharBuffer + logicIdx);
        if (idx == targetLocationOnScreenLinear) return logicIdx;
        // if we reached the end of logic buffer but still didnt find the position
        if (logicIdx == TBuffer->amtOfCharacters - 1) return ++logicIdx;
        else {
            if (idx > targetLocationOnScreenLinear) return --logicIdx;
        }

        switch (pickup){
            case 0:
                // null character - '\n'
                ETX_FOUND = TRUE;
            break;
            case 1:
                // placeholder character - let's display it as a whitespace
            break;
            case 9:
                idx += (TAB_SIZE - 1);
            break;
            case 13:
                // vertical feed - 13, what to do with \n?
                cursor = linearCoordToPlanar(idx, displayRectangle);
                uint8 whitespaces = 0;
                for (; whitespaces < displayRectangle.X - cursor.X; whitespaces++) {
                    
                }
                idx += (whitespaces - 1);
            break;
            default:
                // Regular character encountered - check for sure
            break;

        }
    }
}