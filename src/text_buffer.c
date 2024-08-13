#include "../headers/text_buffer.h"
#include "../headers/util.h"

uint8 TBRemoveCharacter(textBuffer* TBuffer, const COORD cursorPosition, const COORD boundingRectangle) {
    
    uint32 linearCoord = COORDToLinearBuffer(cursorPosition, boundingRectangle);

    memmove((TBuffer->p_logicUCharBuffer + linearCoord),(TBuffer->p_logicUCharBuffer + linearCoord + 1), TBuffer->amtOfCharacters - linearCoord);

    *(TBuffer->p_logicUCharBuffer + (TBuffer->amtOfCharacters-1)) = PLACEHOLDER;

    TBuffer->amtOfCharacters--;
}

// Initializes the entire text buffer with PLACEHOLDER value (1)
uint8 initTextBufferEmpty(textBuffer* TBuffer, const uint32 bufferSize) {
    TBuffer->logicBufferSize = bufferSize;
    TBuffer->amtOfCharacters = 0;
    TBuffer->internalCursorPosition = 0;
    TBuffer->p_logicUCharBuffer = calloc(bufferSize, sizeof(unsigned char));

    for (uint32 idx = 0; idx < bufferSize; idx++) {
        *(TBuffer->p_logicUCharBuffer + idx) = PLACEHOLDER;
    }
}

uint8 TBInsertCharacterAt(textBuffer* TBuffer, const COORD cursorPosition, const COORD boundingRectangle, const uchar character) {
    
    uint32 linearPosition = planarCoordToLinear(cursorPosition, boundingRectangle);

    if (linearPosition >= TBuffer->logicBufferSize) return 1;

    memmove((TBuffer->p_logicUCharBuffer + linearPosition + 2),(TBuffer->p_logicUCharBuffer + linearPosition + 1), TBuffer->amtOfCharacters - linearPosition - 1);

    *(TBuffer->p_logicUCharBuffer + linearPosition + 1) = character;

    TBuffer->amtOfCharacters++;
    
    return 0;
}

uint8 freeTextBuffer(textBuffer* TBuffer) {
    void* memToFree = (void*)TBuffer->p_logicUCharBuffer;
    free(memToFree);
    TBuffer->p_logicUCharBuffer = NULL;
    return 0;
}