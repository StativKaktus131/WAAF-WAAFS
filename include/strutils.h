// in here there will be multiple string utility functions to make manipulating strings easier
#pragma once

#include "util.h"

// splits a string by delimiter and returns the new size in 'str_size'
char** str_split(char* str, const char* delim, size_t* str_size);

// replaces 'rep' with 'with' in str 'orig'
char* str_replace(char* orig, char* rep, char* with);


// checks if a string contains certain character; if not, will return -1
bool str_contains(char* str, char* c);

char* str_trim(char* s);

// finds the index inside of the string
int str_find_index(char* str, const char* find, size_t size);