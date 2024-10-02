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
                // null character - '\0'
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
                for (; whitespaces <= (displayRect.Right-displayRect.Left) - cursor.X; whitespaces++) {
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
        if (logicIdx == pTBuffer->internalCursorPosition -1) CursorFound = TRUE;
    }

    /* Align cursor to printed text. */
    *pCursor = linearCoordToPlanar(displayCursorPosition + 1, smallRectToCoord(displayRect));
    /* ICP points to 0 - cursor should be on (0, 0). */
    if (!pTBuffer->internalCursorPosition) pCursor->X = 0;

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

uchar readConsoleCharacter(void) {

    HANDLE hConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
    if (hConsoleInput == INVALID_HANDLE_VALUE) {
        return 0;
    }

    DWORD mode;
    // Get the current input mode
    if (!GetConsoleMode(hConsoleInput, &mode)) {
        return 0;
    }

    // Disable line input and echo input
    mode &= ~ENABLE_LINE_INPUT;
    mode &= ~ENABLE_ECHO_INPUT; // Optional: Disable echoing

    if (!SetConsoleMode(hConsoleInput, mode)) {
        return 0;
    }

    char ch;
    DWORD charsRead;

    if (!ReadConsole(hConsoleInput, &ch, 1, &charsRead, NULL)) {
        return 0;
    }

    // Restore the original mode (if needed)
    SetConsoleMode(hConsoleInput, mode | ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);

    return (uchar)ch;
}

uint8 readConsoleString(uchar* pBuffer, const uint16 buffSize, const BOOL echoInput) {

    HANDLE hConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
    if (hConsoleInput == INVALID_HANDLE_VALUE) {
        return 1;
    }
    DWORD consoleMode; 
    GetConsoleMode(hConsoleInput, &consoleMode);

    if (!echoInput) {
        consoleMode &= ~ENABLE_ECHO_INPUT;
        SetConsoleMode(hConsoleInput, consoleMode);
    }

    DWORD charsRead;

    if (!ReadConsole(hConsoleInput, pBuffer, buffSize, &charsRead, NULL)) {
        return 1;
    }

    if (!echoInput) {
        SetConsoleMode(hConsoleInput, consoleMode | ENABLE_ECHO_INPUT);
    }

    return (uint8)charsRead;
}