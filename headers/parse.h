#ifndef PARSE1_H
#define PARSE1_H

#include "util.h"

extern const uint8 FILENAME_MAX_LEN;
extern const uint8    INPUT_MAX_LEN;
extern const char*        EXTENSION;
extern const char*     INSTRUCTIONS;
extern const uint32 TEXT_BUFFER_SIZE_BYTES;
extern uint8       FOPEN_SAFE_ERROR;

ActionType resolve                      (const char*);
void       action_manager               (ActionType, const char*);
uint8      list_files_in_directory      (void);
uint8      count_input_length           (const char*);
char*      append_file_extension        (const char*, const char*);
uint8      extract_filename_from_input  (const char*, uint8, char*);
char*      input_to_filename            (const char* const, const char*);
uint8      create_new_file              (const char*);
uint8      save_to_file                 (const char*, const char* const);
uint32     load_file_content_to_buffer  (const char*, char* const, uint32);

#endif