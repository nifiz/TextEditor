#include "../headers/parse.h"
#include <windows.h>

uint32 last_character_in_string(const unsigned char* P_STR, const uint32 SIZEOFSTR, const unsigned char WHITESPACE) {
    
    uint32 lastCharPosition = 0;
    
    //Implement less handicapped error handling in the future
    //TODO (maybe someday...)

    if (P_STR == 0x0 || SIZEOFSTR < 1) return 0;
    
    for (uint32 i = 0; i < SIZEOFSTR; i++) {

        if (*(P_STR+i) != WHITESPACE) { lastCharPosition = i; }
    
    }

    return lastCharPosition;
}

uint32 last_character_in_string_CHARINFO(CHAR_INFO* P_STR, const uint32 SIZEOFSTR, const unsigned char WHITESPACE) {
    
    uint32 lastCharPosition = 0;
    
    //Implement less handicapped error handling in the future
    //TODO (maybe someday...)

    if (P_STR->Char.AsciiChar == 0x0 || SIZEOFSTR < 1) return 0;
    
    for (uint32 i = 0; i < SIZEOFSTR; i++) {

        if ((P_STR+i)->Char.AsciiChar != WHITESPACE) { lastCharPosition = i; }
    
    }

    return lastCharPosition;
}