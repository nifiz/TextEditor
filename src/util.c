#include "../headers/util.h"

uint8 TAB_SIZE = 3;
const uchar WHITESPACE = ' ';

// TODO:
// Out-of-bounds checking for these functions

// Translates a (X,Y) coordinate within a rectangle to 1D, 0-indexed position within a provided boundingRectangle
uint32 planarCoordToLinear(const COORD point, const COORD boundingRectangle) {
    
    uint32 result = 0;

    result = point.Y * boundingRectangle.X + point.X;

    return result;

}

// Translates a 0-indexed position on a 1D axis to 2D coords within a rectangle of given size
// Returns {-1, -1} on error
COORD linearCoordToPlanar(const uint32 point, const COORD boundingRectangle) {
    
    COORD result;

    result.Y = (short)(point / boundingRectangle.X);

    result.X = point - result.Y * boundingRectangle.X;

    return result;

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