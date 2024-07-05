#include "../headers/editor.h"
#include "../headers/parse.h"
#include "../headers/char_operations.h"
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

const char DEBUG_HEADER[] = " :: CTRL+X to eXit :: CTRL+S to Save";

void clear_screen(void) {

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD count;
    DWORD cellCount;
    COORD homeCoords = {0, 0};

    if (hConsole == INVALID_HANDLE_VALUE) return;

    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return;
    cellCount = csbi.dwSize.X * csbi.dwSize.Y;

    if (!FillConsoleOutputCharacter(hConsole, (TCHAR) ' ', cellCount, homeCoords, &count)) return;

    if (!FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, homeCoords, &count)) return;
    SetConsoleCursorPosition(hConsole, homeCoords);

    return;
}

void run_editor(char* fname, char* p_buffer, uint32 bufferSize) {

    clear_screen();

    p_buffer = "WHY IS IT EVEN THERE NIGGAAAAAAAAAAAAAA";

    //initalize stuff

    uint8 displayWidth = w_get_display_width();
    uint8 displayHeight = w_get_display_height();

    COORD consoleWindowSize = {displayWidth, displayHeight};
    CHAR_INFO characterConsoleWindowBuffer[ consoleWindowSize.X * consoleWindowSize.Y]; //does NOT support increasing window size

    uint8 fnameLength = strlen(fname);
    const char FILLER = ' ';

    uint16* pArr_lastCharPosition = (uint16*)calloc(consoleWindowSize.Y, sizeof(uint16)); //initialized with 0's

    

    uint8 fillerCharsCount = displayWidth - 55 - fnameLength;
    if (fillerCharsCount % 2 != 0) fillerCharsCount--;  //make it even

    for (uint8 i = 0; i < displayWidth; i++) {
        characterConsoleWindowBuffer[i].Char.AsciiChar = FILLER;
        characterConsoleWindowBuffer[i].Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | BACKGROUND_BLUE;
    }

    for (uint32 i = displayWidth; i < displayWidth*displayHeight; i++) {
        characterConsoleWindowBuffer[i].Char.AsciiChar = FILLER;
        characterConsoleWindowBuffer[i].Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
    }

    for (uint32 i = displayWidth*15; i < displayWidth*16; i++) {
        characterConsoleWindowBuffer[i].Char.AsciiChar = FILLER;
        characterConsoleWindowBuffer[i].Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
    }

    BOOL running = TRUE;
    uint32 sizeOfReceivedBuffer = strlen(p_buffer);

    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    
    COORD cursorPosition = {0, 0};

    CONSOLE_SCREEN_BUFFER_INFO ScreenInfo;

    GetConsoleScreenBufferInfo(hStdout, &ScreenInfo); //load information about the screen

    //TODO
    //initialize the buffer with desired color values or sum

    DWORD count;
    DWORD cellCount;
    COORD homeCoords = {0, 0};

    uint8 FRAME_TIME_MILISECONDS = 17;

    BOOL arrKeyWasPressed = FALSE;

    SMALL_RECT screenRect = {0,0,displayWidth-1,displayHeight-1};

    clear_screen();
    WriteConsoleOutputA(hStdout, characterConsoleWindowBuffer, consoleWindowSize, homeCoords, &screenRect);

    unsigned char keyPressed;

    while(running) {

        Sleep(FRAME_TIME_MILISECONDS);
        
        keyPressed = _getch();

        switch (keyPressed) {

            case 8: //BACKSPACE
                move_cursor(ARR_LEFT, &cursorPosition, consoleWindowSize, consoleWindowSize.X);
                remove_character(characterConsoleWindowBuffer, consoleWindowSize, cursorPosition);
                break;
            case 13: // ENTER key
                move_character_newline(characterConsoleWindowBuffer, consoleWindowSize, cursorPosition, ' ');
                move_cursor(LINE_BEGIN, &cursorPosition, consoleWindowSize, consoleWindowSize.X);
                move_cursor(ARR_DOWN, &cursorPosition, consoleWindowSize, consoleWindowSize.X);
                break;
            case 19: //CTRL+S - Save
                break;
            case 24: //CTRL+X - Exit
                running = FALSE;
                clear_screen();
                return;
                break;
            case 224: //arrow key detected
                arrKeyWasPressed = TRUE;
                break;
            default:
                //printf("CODE: %d, CHAR: %c", keyPressed, keyPressed);
                if (!arrKeyWasPressed) {
                    //content_append_character(characterConsoleWindowBuffer + (cursorPosition.Y)*displayWidth + cursorPosition.X, keyPressed);
                    insert_character(characterConsoleWindowBuffer, consoleWindowSize, cursorPosition, keyPressed);
                    move_cursor(ARR_RIGHT, &cursorPosition, consoleWindowSize, consoleWindowSize.X);
                }
                else { //ARR or END/HOME key was pressed!
                    uint32 maxAvailableColumn = last_character_in_string_CHARINFO(&characterConsoleWindowBuffer[cursorPosition.Y*consoleWindowSize.X], 
                                                                                  consoleWindowSize.X, 
                                                                                  FILLER);
                    move_cursor((KEY_ARROW)keyPressed, &cursorPosition, consoleWindowSize, maxAvailableColumn);
                    arrKeyWasPressed = FALSE;
                }
        }

        //extend the screen if needed
        WriteConsoleOutputA(hStdout, characterConsoleWindowBuffer, consoleWindowSize, homeCoords, &screenRect);
        SetConsoleCursorPosition(hStdout, cursorPosition);
        //printf("  C.X: %d, C.Y: %d", cursorPosition.X, cursorPosition.Y);
    }

    free(pArr_lastCharPosition);

    return;
}

void insert_character(CHAR_INFO* p_buffer, const COORD sizeOfBuffer, COORD cursorPosition, const char CHAR_TO_INSERT) {
    
    uint16 buffLimit = sizeOfBuffer.X * sizeOfBuffer.Y;
    uint16 positionIndex = cursorPosition.Y * sizeOfBuffer.X + cursorPosition.X;

    char pickUp = (p_buffer+positionIndex)->Char.AsciiChar;
    char putDown;
    (p_buffer+positionIndex)->Char.AsciiChar = CHAR_TO_INSERT;

    while (pickUp != '\0' && positionIndex < buffLimit) {

        //printf("psidx: %d\n", positionIndex);

        putDown = pickUp;

        positionIndex++;

        pickUp = (p_buffer+positionIndex)->Char.AsciiChar;

        (p_buffer+positionIndex)->Char.AsciiChar = putDown;

    }

    //(p_buffer+positionIndex+1)->Char.AsciiChar = '\0';

}

void remove_character(CHAR_INFO* p_buffer, const COORD sizeOfBuffer, COORD cursorPosition) {

    uint16 buffLimit = sizeOfBuffer.X * sizeOfBuffer.Y;
    uint16 positionIndex = cursorPosition.Y * sizeOfBuffer.X + cursorPosition.X;
    char nextOne = (p_buffer+positionIndex+1)->Char.AsciiChar;

    while (nextOne != '\0' && positionIndex + cursorPosition.Y * sizeOfBuffer.X + cursorPosition.X < buffLimit) {
        
        (p_buffer+positionIndex)->Char.AsciiChar = nextOne;
        positionIndex++;
        nextOne = (p_buffer+positionIndex+1)->Char.AsciiChar;

    }

    (p_buffer+positionIndex)->Char.AsciiChar = nextOne;

    return;
}

void move_character_newline(CHAR_INFO* p_buffer, const COORD sizeOfBuffer, COORD cursorPosition, const char CHAR_TO_INSERT) {
    //buggy, characters start to disappear after a few enter presses - why? 
    uint8 columnsLeftInCurrentLine = sizeOfBuffer.X - cursorPosition.X ;

    for (int i = 0; i < columnsLeftInCurrentLine; i++) {
        insert_character(p_buffer, sizeOfBuffer, cursorPosition, CHAR_TO_INSERT);
        cursorPosition.X += 1;
    }
}

uint8 w_get_display_width(void) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) return (uint8)csbi.srWindow.Right - csbi.srWindow.Left + 1;
    else return 0;
}

uint8 w_get_display_height(void) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) return (uint8)csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    else return 0;
}

COORD initCursorPos(const char* p_buffer, COORD ScreenBufferDims) {

    size_t charBufferSize = strlen(p_buffer);
    
    COORD cursorPos = {0,0};

    char currentChar;

    //5x5
    //something\rstupid
    //somet
    //hing

    for (size_t row = 0; row < ScreenBufferDims.Y; row++) {
        for (size_t col = 0; col < ScreenBufferDims.X; col++) {
            currentChar = p_buffer[row * ScreenBufferDims.X + col];
            if (currentChar == '\0') return cursorPos;
            if (currentChar == '\r') {
                cursorPos.Y++;
                cursorPos.X = 0;
            }
            else cursorPos.X++;
        }
        cursorPos.X = 0;
        cursorPos.Y++;
    }

    //never found the NULL char

}

void move_cursor(KEY_ARROW key, COORD* cursor, const COORD SCREEN_SIZE, const uint32 MAX_ALLOWED_COLUMN) {

    //TODO: 
    //Currently cursor can go out of buffer/screen boundaries
    //implement keeping it within in the future

    const int MIN_AVAILABLE_ROW = 0;
    const int MIN_AVAILABLE_COLUMN = 0;
    int maxAvailableColumn = MAX_ALLOWED_COLUMN;

    switch (key)
    {
    case ARR_UP:
        if (cursor->Y > MIN_AVAILABLE_ROW) cursor->Y--;
        break;
    case ARR_DOWN:
        cursor->Y++;
        break;
    case ARR_LEFT:
        if (cursor->X > MIN_AVAILABLE_COLUMN) cursor->X--;
        else if (cursor->X == MIN_AVAILABLE_COLUMN && cursor->Y > 0) //we can go back up and to the end of line above
        {
            cursor->X = SCREEN_SIZE.X - 1;
            cursor->Y--;
        }
        
        break;
    case ARR_RIGHT:
        if (SCREEN_SIZE.X != MAX_ALLOWED_COLUMN) { //value mismatch - user just pressed an arrow key, stop cursor from going into whitespaces behind text
            if (cursor->X <= MAX_ALLOWED_COLUMN && cursor->X != 0) {
                cursor->X++;
            }
        }
        else if (cursor->X+1 > SCREEN_SIZE.X - 1) { //overflow while writing
            cursor->X = 0;
            cursor->Y += 1;
        }
        else {
            cursor->X++;
        }
        break;
    case LINE_BEGIN:
        cursor->X = MIN_AVAILABLE_COLUMN;
        break;
    case LINE_END:
        if (maxAvailableColumn == 0) break;
        cursor->X = maxAvailableColumn+1;
        break;
    default:
        break;
    }
}
