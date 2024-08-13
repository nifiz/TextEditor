#include "../headers/editor.h"
#include "../headers/parse.h"
#include "../headers/char_operations.h"
#include "../headers/text_structure.h"
#include "../headers/text_buffer.h"
#include "../headers/display.h"
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

const char DEBUG_HEADER[] = " :: CTRL+X to eXit :: CTRL+S to Save";

void run_editor(char* fname, char* p_buffer, uint32 bufferSize) {

    clear_screen(' ');

    p_buffer = "TEST TEST";

    //initalize stuff

    uint8 displayWidth = w_get_display_width();
    uint8 displayHeight = w_get_display_height();

    COORD consoleWindowSize = {displayWidth, displayHeight};
    CHAR_INFO characterConsoleWindowBuffer[ consoleWindowSize.X * consoleWindowSize.Y];

    const COORD homeCoords = {0, 0};

    unsigned char logicBuffer[consoleWindowSize.X * consoleWindowSize.Y];

    uint8 fnameLength = strlen(fname);

    // FIDDLING WITH WINDOWS.H STUFF

    BOOL running = TRUE;
    uint32 sizeOfReceivedBuffer = (uint32)strlen(p_buffer);

    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    
    COORD cursorPosition = {0, 0};

    CONSOLE_SCREEN_BUFFER_INFO ScreenInfo;

    GetConsoleScreenBufferInfo(hStdout, &ScreenInfo); //load information about the screen

    uint8 FRAME_TIME_MILISECONDS = 17;

    BOOL arrKeyWasPressed = FALSE;

    SMALL_RECT screenRect = {0,0,displayWidth-1,displayHeight-1};

    // END OF WINDOWS.H SHANANIGANS

    // ==============================================================================================================

    textBuffer editorTextStruct;

    initTextStructureEmpty(&editorTextStruct, (uint32)displayHeight*displayWidth);

    clear_screen(' ');
    WriteConsoleOutputA(hStdout, characterConsoleWindowBuffer, consoleWindowSize, cursorPosition, &screenRect);

    unsigned char keyPressed;

    while(running) {

        Sleep(FRAME_TIME_MILISECONDS);
        
        keyPressed = _getch();

        switch (keyPressed) {

            case 8: //BACKSPACE
                break;
            case 13: // ENTER key
                break;
            case 19: //CTRL+S - Save
                break;
            case 24: //CTRL+X - Exit
                running = FALSE;
                clear_screen(' ');
                return;
                break;
            case 224: //arrow key detected
                arrKeyWasPressed = TRUE;
                break;
            // Default: a character key was pressed
            default:
                
                if (!arrKeyWasPressed) {
                    TBInsertCharacterAt(&editorTextStruct, cursorPosition, consoleWindowSize);
                }
                else { //ARR or END/HOME key was pressed!
                    uint32 maxAvailableColumn = last_character_in_string_CHARINFO(&characterConsoleWindowBuffer[cursorPosition.Y*consoleWindowSize.X], 
                                                                                  consoleWindowSize.X, 
                                                                                  ' ');
                    maxAvailableColumn = getLastCharInLine(&editorTextStruct, cursorPosition.Y);
                    move_cursor((KEY_ARROW)keyPressed, &cursorPosition, consoleWindowSize, maxAvailableColumn);
                    arrKeyWasPressed = FALSE;
                }
        }

        WriteConsoleOutputA(hStdout, characterConsoleWindowBuffer, consoleWindowSize, homeCoords, &screenRect);
        SetConsoleCursorPosition(hStdout, cursorPosition);
    }

    return;
}

void insert_character(CHAR_INFO* p_buffer, const COORD sizeOfBuffer, COORD cursorPosition, const char CHAR_TO_INSERT) {
    
    uint16 buffLimit = sizeOfBuffer.X * sizeOfBuffer.Y;
    uint16 positionIndex = cursorPosition.Y * sizeOfBuffer.X + cursorPosition.X;

    char pickUp = (p_buffer+positionIndex)->Char.AsciiChar;
    char putDown;
    (p_buffer+positionIndex)->Char.AsciiChar = CHAR_TO_INSERT;


    // TODO
    // Can this be switched to memmove? It'd be great but I can't be bothered atm.
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
    
    uint8 columnsLeftInCurrentLine = sizeOfBuffer.X - cursorPosition.X ;

    for (int i = 0; i < columnsLeftInCurrentLine; i++) {
        insert_character(p_buffer, sizeOfBuffer, cursorPosition, CHAR_TO_INSERT);
        cursorPosition.X += 1;
    }
}

uint8 w_get_display_width(void) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) return (uint8)(csbi.srWindow.Right - csbi.srWindow.Left + 1);
    else return 0;
}

uint8 w_get_display_height(void) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) return (uint8)(csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
    else return 0;
}

// TODO:
// Currently this function is unfinished, I want it to put the cursor at the end of
// text that has been loaded into the buffer on startup - usually when loading an exisiting
// text file.
COORD initCursorPos(const char* p_buffer, COORD ScreenBufferDims) {

    size_t charBufferSize = strlen(p_buffer);
    
    COORD cursorPos = {0,0};

    char currentChar;

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

// Function responsible for securely moving the cursor within console window.
// It does not perform the given move if it puts the cursor outside of screen boundaries.
// What's important, it also might move the cursor up or down a line if it was to exceed bounds otherwise.
//
// Params:
// KEY_ARROW key                   - enum for move operations, such as arr. keys or END/HOME etc.
// COORD* cursor                   - pointer to a cursor struct, to be modified if move is within bounds.
// const COORD SCREEN_SIZE         - for bound checking
// const uint32 MAX_ALLOWED_COLUMN - for bound checking, but when we want to move the cursor
// to the end of text
void move_cursor(KEY_ARROW key, COORD* cursor, const COORD SCREEN_SIZE, const uint32 MAX_ALLOWED_COLUMN) {

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
        if (SCREEN_SIZE.X != MAX_ALLOWED_COLUMN) { // User just pressed an arrow key but there are no chars further to the right
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
