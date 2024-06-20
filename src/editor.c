#include "../headers/editor.h"
#include "../headers/parse.h"
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

    char keyPressed;

    p_buffer = "ciota i chuj pozdro";

    //initalize stuff

    uint8 displayWidth = w_get_display_width();
    uint8 displayHeight = 50;

    COORD consoleWindowSize = {displayWidth, displayHeight};
    CHAR_INFO characterConsoleWindowBuffer[consoleWindowSize.X * consoleWindowSize.Y];

    uint8 fnameLength = strlen(fname);
    const char FILLER = '-';

    uint8 fillerCharsCount = displayWidth - 55 - fnameLength;
    if (fillerCharsCount % 2 != 0) fillerCharsCount--;  //make it even

    for (uint8 i = 0; i < displayWidth; i++) {
        characterConsoleWindowBuffer[i].Char.AsciiChar = FILLER;
        characterConsoleWindowBuffer[i].Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
    }

    for (uint8 i = fillerCharsCount/2; i < displayWidth - fillerCharsCount/2; i++) {
        characterConsoleWindowBuffer[i].Char.AsciiChar = DEBUG_HEADER[i];
    }

    for (uint32 i = displayWidth; i < displayWidth*displayHeight; i++) {
        characterConsoleWindowBuffer[i].Char.AsciiChar = ' ';
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
    uint8 FRAMES = 60;      //temporary solution
    char isEdited = '*';    //temporary solution

    //print_to_screen(fname, p_buffer, 0, FRAMES, isEdited);

    SMALL_RECT screenRect = {0,0,displayWidth-1,displayHeight-1};

    //WriteConsoleOutputA(hStdout, characterConsoleWindowBuffer, consoleWindowSize, homeCoords, &screenRect);
    clear_screen();
    WriteConsoleOutputA(hStdout, characterConsoleWindowBuffer, consoleWindowSize, homeCoords, &screenRect);


    while(running) {

        Sleep(FRAME_TIME_MILISECONDS);
        
        unsigned char keyPressed = _getch();


        switch (keyPressed) {
            case 24: //CTRL+X - Exit
                running = FALSE;
                clear_screen();
                return;
                break;
            case 8: //BACKSPACE
                content_remove_character(characterConsoleWindowBuffer + (cursorPosition.Y)*displayWidth + cursorPosition.X-1);
                move_cursor(ARR_LEFT, &cursorPosition, consoleWindowSize);
                break;
            case 19: //CTRL+S - Save
                break;
            case 224: //arrow key detected
                arrKeyWasPressed = TRUE;
                break;
            default:
                //printf("CODE: %d, CHAR: %c", keyPressed, keyPressed);
                if (!arrKeyWasPressed) {
                    content_append_character(characterConsoleWindowBuffer + (cursorPosition.Y)*displayWidth + cursorPosition.X, keyPressed);
                    move_cursor(ARR_RIGHT, &cursorPosition, consoleWindowSize);
                }
                else { //arr key was pressed!
                    
                    move_cursor((KEY_ARROW)keyPressed, &cursorPosition, consoleWindowSize);
                    arrKeyWasPressed = FALSE;
                }
        }
        //clear_screen();
        //print_to_screen(fname, p_buffer, sizeOfReceivedBuffer, FRAMES, isEdited);

        WriteConsoleOutputA(hStdout, characterConsoleWindowBuffer, consoleWindowSize, homeCoords, &screenRect);

        //printf("\ncode %d\n", keyPressed);
        //printf("CX %d: CY: %d\n", cursorPosition.X, cursorPosition.Y);
        SetConsoleCursorPosition(hStdout, cursorPosition);
    }

    return;
}

void content_append_character(CHAR_INFO* const p_buffer, char character) {
    //does NOT check for p_buffer overlow yet!
    p_buffer->Char.AsciiChar = character;
    //(p_buffer+1)->Char.AsciiChar = 0x0;
    return;
}

void content_remove_character(CHAR_INFO* const p_CharToDelete) {
    p_CharToDelete->Char.AsciiChar = 0x0;
    //p_CharToDelete->Char.UnicodeChar = '\0';
    return;
}

void print_top_bar_to_screen(const char* filename, const uint32 wordCount, unsigned char isEdited) {
    uint8 screenWidth = w_get_display_width();

    printf("%s%c%s :: Chars: %d\n", filename, isEdited, DEBUG_HEADER, wordCount);
}

void print_to_screen(char* filename, const char* const p_buffer, const uint32 wordCount, const uint8 FRAMES, char isEdited) {

    print_top_bar_to_screen(filename, wordCount, isEdited);

    // for(uint8 i = 0; i < screenWidth; i++) {
    //     printf("^");
    // }
    // printf("\n");

    char currentCharacter;
    uint8 stop = 0;
    for (uint32 i = 0; stop != 1 ;i++) {

        currentCharacter = *(p_buffer+i);

        switch (currentCharacter) {
            case '\0':
                stop = 1;
                break;
            case '\r': //enter
                printf("\n");
            default:
                printf("%c", currentCharacter);
        }

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
            currentChar = p_buffer[row*ScreenBufferDims.X + col];
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

void move_cursor(KEY_ARROW key, COORD* cursor, const COORD SCREEN_SIZE) {
    //TODO: 
    //we can increase cursor position beyond whats possible to display and draw currently
    //implement it in the future

    const int MAX_AVAILABLE_ROW = 1;
    const int MAX_AVAILABLE_COLUMN = 0;

    switch (key)
    {
    case ARR_UP:
        if (cursor->Y > MAX_AVAILABLE_ROW) cursor->Y--;
        break;
    case ARR_DOWN:
        cursor->Y++;
        break;
    case ARR_LEFT:
        if (cursor->X > MAX_AVAILABLE_COLUMN) cursor->X--;
        else if (cursor->X == MAX_AVAILABLE_COLUMN && cursor->Y > 0) //we can go back up and to the end of line above
        {
            cursor->X = SCREEN_SIZE.X - 1;
            cursor->Y--;
        }
        
        break;
    case ARR_RIGHT:
        if (cursor->X+1 > SCREEN_SIZE.X - 1) { //overflow
            cursor->X = 0;
            cursor->Y += 1;
        }
        else {
            cursor->X++;
        }
        break;
    default:
        break;
    }
}


