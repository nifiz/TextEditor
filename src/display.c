#include "../headers/display.h"

void clear_screen(const char fillerChar) {

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD count;
    DWORD cellCount;
    const COORD homeCoords = {0, 0};

    if (hConsole == INVALID_HANDLE_VALUE) return;

    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return;
    cellCount = csbi.dwSize.X * csbi.dwSize.Y;

    if (!FillConsoleOutputCharacter(hConsole, (TCHAR)fillerChar, cellCount, homeCoords, &count)) return;

    if (!FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, homeCoords, &count)) return;
    SetConsoleCursorPosition(hConsole, homeCoords);

    return;
}

// Join background and foreground colors as follows: 
// BACKGROUND_COLOR | FOREGROUND_COLOR
uint8 setDisplayColor(const Color color, const COORD displayRectangle, CHAR_INFO* pBuffer) {

    uint32 linearScreenSize = displayRectangle.X * displayRectangle.Y;

    for (uint32 idx = 0; idx < linearScreenSize; idx++) {
        pBuffer[idx].Attributes = color;
    }

    return 0;
}

uint8 updateScreenBuffer(CHAR_INFO* pDisplayCharBuffer, const textBuffer* pTBuffer, const SMALL_RECT displayRect, const SMALL_RECT drawingRect, COORD* pCursor) {
    
    uint32 linearDisplayCharSize = smallRectArea(displayRect);

    BOOL ETX_FOUND = FALSE;
    BOOL CursorFound = FALSE;
    uint32 displayCursorPosition = 0;
    COORD cursor = {0,0};

    if (pDisplayCharBuffer == NULL || pTBuffer == NULL) return 1;

    uchar pickup;
    uint32 logicIdx = 0;
    //uint32 idx = drawingRect.X;
    uint32 idx = planarCoordToLinear((COORD){drawingRect.Left, drawingRect.Top}, (COORD)(smallRectToCoord(displayRect)));

    for (; (idx < linearDisplayCharSize && ETX_FOUND == FALSE); idx++, logicIdx++) {
        
        pickup = *(pTBuffer->p_logicUCharBuffer + logicIdx);

        switch (pickup){
            case 0:
                // null character - '\n'
                (pDisplayCharBuffer + idx)->Char.AsciiChar = WHITESPACE;
                ETX_FOUND = TRUE;
            break;
            case 1:
                // placeholder character - let's display it as a whitespace
                (pDisplayCharBuffer + idx)->Char.AsciiChar = WHITESPACE;
            break;
            case 9:
                // tab encountered - enter a few whitespaces

                // TODO: tab can't go past the line it is in!
                
                for (uint8 whitespaces = 0; whitespaces < TAB_SIZE; whitespaces++) {
                    (pDisplayCharBuffer + idx + whitespaces)->Char.AsciiChar = WHITESPACE;
                }
                idx += (TAB_SIZE - 1);
            break;
            case 13:
                // vertical feed - 13, what to do with \n?
                cursor = linearCoordToPlanar(idx, smallRectToCoord(displayRect));
                uint8 whitespaces = 0;
                for (; whitespaces < (displayRect.Right-displayRect.Left) - cursor.X; whitespaces++) {
                    (pDisplayCharBuffer + idx + whitespaces)->Char.AsciiChar = WHITESPACE;
                }
                idx += (whitespaces - 1);
            break;
            default:
                // Regular character encountered - check for sure!
                if (pickup >= 32 && pickup <= 126) {
                    (pDisplayCharBuffer+idx)->Char.AsciiChar = pickup;
                }

            break;

        }

        if (CursorFound == FALSE) displayCursorPosition = idx;
        // -1 because logicIdx is 0 - indexed while ICP is 1 indexed
        if (logicIdx == pTBuffer->internalCursorPosition - 1) CursorFound = TRUE;
    }

    // Cursor management
    // The +1 works nicely probably because indexing blah blah blah
    // On the other hand, indexing should always start with 1 xd
    *pCursor = linearCoordToPlanar(displayCursorPosition+1, smallRectToCoord(displayRect));

    return 0; 
}

// Flushes the entire buffer with values of 0 and black color
void flushScreenBuffer(CHAR_INFO* pScreenBuffer, const uint32 sizeOfScreenBuffer) {

    for (uint32 idx = 0; idx < sizeOfScreenBuffer; idx++) {
        (pScreenBuffer + idx)->Char.AsciiChar = ' ';
        (pScreenBuffer + idx)->Attributes = 0x0;
    }
    return;
}