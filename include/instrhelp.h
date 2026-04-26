#pragma once

#include "util.h"
#include "chunk.h"

// reads a file and returns its contents in a char "array" with the size 'size'
char* read_file(const char* filepath, size_t* size);

// splits a string by delimiter and returns the new size in 'str_size'
char** split(char* str, const char* delim, size_t* str_size);

// replaces 'rep' with 'with' in str 'orig'
char* replace(char* orig, char* rep, char* with);

// checks if a string contains certain character; if not, will return -1
bool contains(char* str, char* c);

char* ltrim(char* s);
char* rtrim(char* s);
char* trim(char* s);

// finds the index inside of the string
int find_index(char* str, const char* find, size_t size);



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