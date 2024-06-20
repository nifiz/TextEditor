#include "../headers/parse.h"
#include "../headers/editor.h"
#include <stdio.h>

int main(void) {
    
    char input[INPUT_MAX_LEN];

    while (1) {

        printf("%s\n", INSTRUCTIONS);

        gets_s(input, INPUT_MAX_LEN);
        ActionType userAction = resolve(input);

        action_manager(userAction, input);
    }

    return 0;
}