#include "../headers/display.h"
#include "../headers/operation.h"

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

void updateScreenBuffer(CHAR_INFO* pDisplayCharBuffer, textBuffer* pLogicCharBuffer, const operation _operation) {
    
}