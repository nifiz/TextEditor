#include "../headers/util.h"

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