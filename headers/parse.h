#ifndef PARSE1_H
#define PARSE1_H

typedef unsigned char uint8;
typedef char int8;
typedef unsigned short uint16;
typedef short int16;
typedef long int int32;
typedef unsigned long int uint32;

const extern uint8 FILENAME_MAX_LEN;
const extern uint8    INPUT_MAX_LEN;
const extern char*        EXTENSION;
extern uint8       FOPEN_SAFE_ERROR;
const extern char*     INSTRUCTIONS;
const extern uint32 TEXT_BUFFER_SIZE_BYTES;

typedef enum {
    ACTION_ERROR, ACTION_NEW_FILE, ACTION_OPEN_FILE, ACTION_LIST, ACTION_MOVE_DIR_UP, ACTION_MOVE_DIR_DOWN, ACTION_EXIT
} ActionType;

ActionType resolve(const char*);
void action_manager(ActionType, const char*);
uint8 list_files_in_directory(void);
uint8 count_input_length(const char*);
char* append_file_extension(const char*, const char*);
uint8 extract_filename_from_input(const char*, uint8, char*);
char* input_to_filename(const char* const, const char*);
uint8 create_new_file(const char*);
uint8 save_to_file(const char*, const char* const);
uint32 load_file_content_to_buffer(const char*, char* const, uint32);

#endif