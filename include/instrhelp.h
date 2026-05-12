#pragma once

#include "chunk.h"

typedef enum Mode
{
    BYTE,
    SAMPLE_MONO,
    SAMPLE_STEREO
} intp_mode_t;

// reads a file and returns its contents in a char "array" with the size 'size'
char* read_file(const char* filepath, size_t* size);


// running args
// typedef enum {RANDOM} inbuilts_t;

size_t current_data_pointer;
chunk_t* data_chunk;
intp_mode_t mode;
bool run_once;
float progress;

// data
float bpm;


// running methods
char* decode_str(char* orig);           // inserts variables in string
void* decode_eval(char* value);
bool eval_condition(char* condition);

void set(char* arg1, char* arg2);