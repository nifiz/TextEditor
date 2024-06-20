#include "../headers/parse.h"
#include "../headers/editor.h"
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

const uint8 INPUT_MAX_LEN = 67;
const uint8 FILENAME_MAX_LEN = 64;
const uint32 TEXT_BUFFER_SIZE_BYTES = 500000;
const char* EXTENSION = ".txt";
uint8 FOPEN_SAFE_ERROR = 0;
const char* INSTRUCTIONS = "N - NEW FILE, O - OPEN FILE, LS - LIST FILES, E - EXIT";

ActionType resolve(const char* input) {
    
    uint8 retVal = 0;

    if (input[0] == 'n' && input[1] == ' ') return ACTION_NEW_FILE;
    if (input[0] == 'o' && input[1] == ' ') return ACTION_OPEN_FILE;
    if (input[0] == 'l' && input[1] == 's') return ACTION_LIST;
    if (input[0] == 'c' && input[1] == 'd') 
    {
        if (input[3] == '.' && input[4] == '.') return ACTION_MOVE_DIR_DOWN;
        else return ACTION_MOVE_DIR_UP;
    }
    if (input[0] == 'e') return ACTION_EXIT;
    else return ACTION_ERROR;
    
}

void action_manager(ActionType actionGiven, const char* input) {
    if (actionGiven == ACTION_ERROR) {
        printf("Unknown command\n");
    }
    else if (actionGiven == ACTION_NEW_FILE || actionGiven == ACTION_OPEN_FILE)
    {
        /* code */
        char textEditorCharBuffer[TEXT_BUFFER_SIZE_BYTES];
        run_editor("fname", textEditorCharBuffer, TEXT_BUFFER_SIZE_BYTES);
        
    }
    else if (actionGiven == ACTION_OPEN_FILE)
    {
        /* code */
    }
    else if (actionGiven == ACTION_LIST)
    {
        /* code */
    }
    else if (actionGiven == ACTION_MOVE_DIR_DOWN)
    {
        /* code */
    }
    else if (actionGiven == ACTION_MOVE_DIR_UP)
    {
        /* code */
    }
    else if (actionGiven == ACTION_EXIT)
    {
        exit(0);
    }
}

uint8 list_files_in_directory(void) {
    //returns 0 on success, errno code received otherwise

    DIR* current_directory = opendir("."); //"." stands for current directory
    errno = 0;
    struct dirent* p_dir;

    while (current_directory) {
        if ((p_dir = readdir(current_directory)) != NULL) {
            printf("%s\n", p_dir->d_name);
        }
        else {
            closedir(current_directory);
            return (uint8)errno;
        }
    }
    printf("\n");
}

uint8 extract_filename_from_input(const char* input, uint8 input_len, char* returnString) {
    
    //this function will probably be only used for new file creation and opening an existing file - therefore
    //we probably can manually discard 2 chars at the beginning - 'n' or 'o' and a whitespace that follows
    //we also want to remove a file extension if it was provided by the user since we only will
    //create text files in this version

    char* p_substr = strstr(input, EXTENSION);

    uint8 filename_len = input_len - 2;

    uint8 position_idx = input_len; //putting position_idx at the null term. character

    if (p_substr != NULL) { //EXTENSION was found
        position_idx = p_substr - input;
        filename_len -= strlen(EXTENSION); //remove len of .txt
    }

    for (int i = 2, idx = 0; i < position_idx; i++, idx++) returnString[idx] = input[i]; 
    returnString[position_idx-2] = '\0'; //its stack allocated so its ok

    return filename_len;
}

char* input_to_filename(const char* const input, const char* extension) {

    //n file.txt
    //rawInputLen = 10
    uint8 rawInputLen = strlen(input);

    char* p_substr = strstr(input, extension);

    uint8 filenameLen = rawInputLen - 2;
    //filenameLen = 8
    uint8 positionIndex = rawInputLen;
    //positionIndex = 10

    if (p_substr != NULL) { //EXTENSION was found
    positionIndex = p_substr - input;
    //positionIndex = 6
    filenameLen -= strlen(extension); //remove len of .txt
    //filenameLen = 4
    }

    char* returnString = malloc(sizeof(char)*(filenameLen+strlen(extension))+1);

    for (int i = 2, idx = 0; i < positionIndex; i++, idx++) returnString[idx] = input[i]; 
    returnString[filenameLen] = '\0'; //its stack allocated so its ok
    strcat(returnString, extension);

    return returnString;

}

char* append_file_extension(const char* fileName, const char* extension) {
    //returns a null terminated fname+extension
    //returned pointer has to be freed by calling free()!
    
    size_t fullStringLength = strlen(fileName) + strlen(extension) + 1; //including the null term. char. 

    char* returnString = (char*)malloc(sizeof(char)*fullStringLength);
    strcpy(returnString, fileName); //copy the filename
    strcpy(&returnString[ strlen(fileName) ], extension); //copy the extension right afterwards

    returnString[fullStringLength-1] = '\0'; //terminate the string

    return returnString;
}

FILE* fopen_safe(const char* filename, const char* mode) {
    //wrapper function so we dont destroy the file if it already exists
    //returns 1 if the file already exists, 0 otherwise

    FILE* p_file = fopen(filename, "r");

    if (p_file != NULL) { //file already exists
        FOPEN_SAFE_ERROR = 1;
        return NULL;
    }

    fclose(p_file);
    p_file = fopen(filename, mode);
    return p_file;
}

uint8 create_new_file(const char* input) {

    uint8 input_len = strlen(input);
    char fileNameClean[input_len-2];

    if (extract_filename_from_input(input, input_len, fileNameClean) == -1) return -1;

    char* fullFileName = append_file_extension(fileNameClean, ".txt");

    FILE *p_file = fopen_safe(fullFileName, "w");
    if (p_file == NULL && FOPEN_SAFE_ERROR == 1) {
        //file with such a name already exists and could not be open
        printf("File with that name already exists, you can edit it instead!\n");
        FOPEN_SAFE_ERROR = 0;
    }


    fclose(p_file);

    free(fullFileName);

    return 0;
}

uint8 save_to_file(const char* filename, const char* const source) {

    printf("Saving to file: %s\n", filename);
    
    FILE* p_file = fopen_safe(filename, "w");
    if (p_file == NULL && FOPEN_SAFE_ERROR == 1) {
        //file with such a name already exists or there was a different error
        printf("Operation failed.\n");
        FOPEN_SAFE_ERROR = 0;
        return -1;
    }

    if (fputs(source, p_file) < 0) {
        printf("Could not put the contents of buffer to file stream\n");
        return -1;
    }

    fclose(p_file);

    return 0;
}

uint32 load_file_content_to_buffer(const char* filename, char* const destBuffer, uint32 destBufferSize) {
    
    FILE* p_file = fopen(filename, "r");
    if (p_file == NULL) return -1;

    int c; // note: int, not char, required to handle EOF
    uint32 charCounter = 0;
    while ((c = fgetc(p_file)) != EOF) {
        if (charCounter == destBufferSize) {
            printf("Buffer full! Can't read any further");
            break;
        }
        printf("[%d] = %c\n", charCounter, c);
        destBuffer[charCounter] = c;
        charCounter++;
    }
    if (ferror(p_file))
        return -1;
 
    fclose(p_file);
    remove(filename);
    return charCounter;
}
