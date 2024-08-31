#include "../headers/parse.h"
#include "../headers/editor.h"
#include "../headers/text_buffer.h"
#include "../headers/display.h"
#include <stdio.h>

int main(void) {
    
    SetConsoleTitle("Konsolatnik");

    char input[INPUT_MAX_LEN];
    
    textBuffer headerTBuff, inputTBuff;
    initTextBufferFromString(&headerTBuff, w_get_display_width(), INSTRUCTIONS, strlen(INSTRUCTIONS));
    initTextBufferEmpty(&inputTBuff, INPUT_MAX_LEN);

    COORD screen = {w_get_display_width(), w_get_display_height()};
    COORD cursor = {0,0};

    CHAR_INFO screenBuff[screen.X * screen.Y];
    flushScreenBuffer(screenBuff, screen.X * screen.Y);
    SMALL_RECT screenRect = {0,0,screen.X-1,screen.Y-1};
    SMALL_RECT editRect = {5,5,screen.X-1, screen.Y-1};
    setDisplayColor(backgroundBlack | letterWhite, screen, screenBuff);
    updateScreenBuffer(screenBuff, &headerTBuff, screenRect, editRect, &cursor);
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    WriteConsoleOutputA(hStdout, screenBuff, screen, (COORD){0,0}, &screenRect);
    while (1) {


        gets_s(input, INPUT_MAX_LEN);
        ActionType userAction = resolve(input);

        action_manager(userAction, input);
        clear_screen(' ');
    }

    freeTextBuffer(&headerTBuff);
    freeTextBuffer(&inputTBuff);

    return 0;
}