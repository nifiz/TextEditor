#ifndef DISPLAY_H
#define DISPLAY_H

#include "operation.h"
#include "text_buffer.h"

// TODO: Implement more colors

#define LETTER_WHITE          (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY)
#define LETTER_DARK_WHITE     (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED)
#define LETTER_BLUE           (FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define LETTER_DARK_BLUE      (FOREGROUND_BLUE)
#define LETTER_GREEN          (FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define LETTER_DARK_GREEN     (FOREGROUND_GREEN)
#define LETTER_RED            (FOREGROUND_RED | FOREGROUND_INTENSITY)
#define LETTER_DARK_RED       (FOREGROUND_RED)
#define BACKGROUND_WHITE      (BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY)
#define BACKGROUND_DARK_WHITE (BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED)
//#define BACKGROUND_BLUE       (BACKGROUND_BLUE | BACKGROUND_INTENSITY)
#define BACKGROUND_DARK_BLUE  (BACKGROUND_BLUE)
//#define BACKGROUND_GREEN      (BACKGROUND_GREEN | BACKGROUND_INTENSITY)
#define BACKGROUND_DARK_GREEN (BACKGROUND_GREEN)
//#define BACKGROUND_RED        (BACKGROUND_RED | BACKGROUND_INTENSITY)
#define BACKGROUND_DARK_RED   (BACKGROUND_RED)

typedef enum {
    backgroundBlack = 0,
    letterWhite = LETTER_WHITE,
    letterBlue = LETTER_BLUE,
    letterRed = LETTER_RED,
    letterGreen = LETTER_GREEN,
    backgroundWhite = BACKGROUND_WHITE,
    backgroundGrey = BACKGROUND_DARK_WHITE,
    backgroundBlue = (BACKGROUND_BLUE | BACKGROUND_INTENSITY),
    backgroundDarkBlue = (BACKGROUND_BLUE),
    backgroundRed = (BACKGROUND_RED | BACKGROUND_INTENSITY),
    backgroundGreen = (BACKGROUND_GREEN | BACKGROUND_INTENSITY)
} Color;

// Flushes the screen with a given character
void  clear_screen          (const char fillerChar);
void  flushScreenBuffer     (CHAR_INFO* pScreenBuffer, const uint32 sizeOfScreenBuffer);
uint8 setDisplayColor       (const Color color, const COORD displayRectangle, CHAR_INFO* pBuffer);

uint8 updateScreenBuffer    (CHAR_INFO* pDisplayCharBuffer, 
                            const textBuffer* pTBuffer, 
                            const SMALL_RECT displayRect, 
                            const SMALL_RECT drawingRect, 
                            COORD* pCursor);

uchar readConsoleCharacter  (void);
uint8 readConsoleString     (uchar* pBuffer, const uint16 buffSize, const BOOL echoInput);

#endif