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

    char keyPressed;
    const uint8 FRAMES = 60; //subject to change
    const float FRAME_TIME_MILISECONDS = ceil(1000.f/FRAMES);
    uint8 running = 1;
    uint32 charCounter = strlen(p_buffer);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD count;
    DWORD cellCount;
    COORD homeCoords = {0, 0};

    char isEdited = ' ';

    print_to_screen(fname, p_buffer, charCounter, FRAMES, isEdited);

    while(running) {

        Sleep(FRAME_TIME_MILISECONDS);
        
        char keyPressed = _getche();

        isEdited = '*';

        switch (keyPressed) {
            case 24: //CTRL+X - Exit
                running = 0;
                clear_screen();
                return;
                break;
            case 8: //BACKSPACE
                content_remove_character(p_buffer+charCounter);
                if (charCounter) charCounter--;
                break;
            
            case 19: //CTRL+S - Save

            default:
                //printf("CODE: %d, CHAR: %c", keyPressed, keyPressed);
                content_append_character(p_buffer+charCounter, keyPressed);
                charCounter++;
        }
        print_to_screen(fname, p_buffer, charCounter, FRAMES, isEdited);
    }

    return;
}

void content_append_character(char* const p_buffer, char character) {
    //does NOT check for p_buffer overlow yet!
    *p_buffer = character;
    *(p_buffer+1) = '\0';
    return;
}

void content_remove_character(char* const p_buffer) {
    *(p_buffer-1) = '\0';
}

void print_to_screen(char* filename, const char* const p_buffer, const uint32 wordCount, const uint8 FRAMES, char isEdited) {

    clear_screen();

    uint8 screenWidth = w_get_display_width();

    printf("%s%c%s :: Chars: %d\n", filename, isEdited, DEBUG_HEADER, wordCount);

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


