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
    uint8 displayHeight = w_get_display_height();

    COORD consoleWindowSize = {displayWidth, displayHeight};
    CHAR_INFO characterConsoleWindowBuffer[consoleWindowSize.X * consoleWindowSize.Y];

    uint8 fnameLength = strlen(fname);
    const char FILLER = 10;

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
        characterConsoleWindowBuffer[i].Char.AsciiChar = 0x0;
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

    SMALL_RECT screenRect = {0,0,displayWidth-1,displayHeight-1};

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
                //content_remove_character(characterConsoleWindowBuffer + (cursorPosition.Y)*displayWidth + cursorPosition.X-1);
                move_cursor(ARR_LEFT, &cursorPosition, consoleWindowSize);
                remove_character(characterConsoleWindowBuffer, consoleWindowSize, cursorPosition);
                break;
            case 13: // ENTER key
                move_character_newline(characterConsoleWindowBuffer, consoleWindowSize, cursorPosition, ' ');
                move_cursor(ARR_DOWN, &cursorPosition, consoleWindowSize);
                move_cursor(LINE_BEGIN, &cursorPosition, consoleWindowSize);
                break;
            case 19: //CTRL+S - Save
                break;
            case 71: //HOME key - move cursor to the beginning of the current row
                move_cursor(LINE_BEGIN, &cursorPosition, consoleWindowSize);
                break;
            case 79: //END key - move cursor to the end of the current row
                move_cursor(LINE_END, &cursorPosition, consoleWindowSize);
                break;
            case 224: //arrow key detected
                arrKeyWasPressed = TRUE;
                break;
            default:
                //printf("CODE: %d, CHAR: %c", keyPressed, keyPressed);
                if (!arrKeyWasPressed) {
                    //content_append_character(characterConsoleWindowBuffer + (cursorPosition.Y)*displayWidth + cursorPosition.X, keyPressed);
                    insert_character(characterConsoleWindowBuffer, consoleWindowSize, cursorPosition, keyPressed);
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
        printf("\nW: %d\n", consoleWindowSize.X);
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

void insert_character(CHAR_INFO* p_buffer, const COORD sizeOfBuffer, COORD cursorPosition, const char CHAR_TO_INSERT) {
    
    uint16 buffLimit = sizeOfBuffer.X * sizeOfBuffer.Y;
    uint16 positionIndex = cursorPosition.Y * sizeOfBuffer.X + cursorPosition.X;

    //if we'd go over the buffer - increase it's size

    //resize_console_screen_buffer((COORD){sizeOfBuffer.X + 1, sizeOfBuffer.Y});

    char pickUp = (p_buffer+positionIndex)->Char.AsciiChar;
    char putDown;
    (p_buffer+positionIndex)->Char.AsciiChar = CHAR_TO_INSERT;

    while (pickUp != '\0' || positionIndex + cursorPosition.Y * sizeOfBuffer.X + cursorPosition.X < buffLimit) {

        //printf("psidx: %d\n", positionIndex);

        putDown = pickUp;

        positionIndex++;

        pickUp = (p_buffer+positionIndex)->Char.AsciiChar;

        (p_buffer+positionIndex)->Char.AsciiChar = putDown;

    }

    (p_buffer+positionIndex+1)->Char.AsciiChar = '\0';

}

BOOL resize_console_screen_buffer(const COORD NEW_SIZE) {
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    return SetConsoleScreenBufferSize(hStdout, NEW_SIZE);
}

void remove_character(CHAR_INFO* p_buffer, const COORD sizeOfBuffer, COORD cursorPosition) {

    uint16 buffLimit = sizeOfBuffer.X * sizeOfBuffer.Y;
    uint16 positionIndex = cursorPosition.Y * sizeOfBuffer.X + cursorPosition.X;

    char nextOne = (p_buffer+positionIndex+1)->Char.AsciiChar;

    while (nextOne != '\0' || positionIndex + cursorPosition.Y * sizeOfBuffer.X + cursorPosition.X < buffLimit) {
        
        (p_buffer+positionIndex)->Char.AsciiChar = nextOne;
        positionIndex++;
        nextOne = (p_buffer+positionIndex+1)->Char.AsciiChar;

    }

    (p_buffer+positionIndex)->Char.AsciiChar = nextOne;

    return;
}

void move_character_newline(CHAR_INFO* p_buffer, const COORD sizeOfBuffer, COORD cursorPosition, const char CHAR_TO_INSERT) {
    //buggy, characters start to disappear after a few enter presses - why? 
    uint8 columnsLeftInCurrentLine = sizeOfBuffer.X - (cursorPosition.X + 1);

    for (int i = 0; i <= columnsLeftInCurrentLine; i++) {
        insert_character(p_buffer, sizeOfBuffer, cursorPosition, CHAR_TO_INSERT);
        cursorPosition.X += 1;
    }
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

    const int MIN_AVAILABLE_ROW = 0;
    const int MIN_AVAILABLE_COLUMN = 0;

    int maxAvailableRow = SCREEN_SIZE.Y;
    int maxAvailableColumn = SCREEN_SIZE.X;


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
        if (cursor->X+1 > SCREEN_SIZE.X - 1) { //overflow
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
        cursor->X = maxAvailableColumn-1;
    default:
        break;
    }
}


