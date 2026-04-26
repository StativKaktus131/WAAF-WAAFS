#pragma once

#include "util.h"
#include "chunk.h"

// reads a file and returns its contents in a char "array" with the size 'size'
char* read_file(const char* filepath, size_t* size);


// running args
// typedef enum {RANDOM} inbuilts_t;

size_t current_data_pointer;
chunk_t* data_chunk;
bool run_once;
float progress;


// running methods
void* decode_eval(char* value);
bool eval_condition(char* condition);

void set(char* arg1, char* arg2);