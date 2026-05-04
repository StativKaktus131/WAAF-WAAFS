#pragma once

#include "util.h"

typedef struct Variable
{
    char* identifier;
    char* value;
    bool global;
} variable_t;


variable_t** stack;
size_t stack_size;


void print_variable(variable_t* var);
void print_stack();

void try_init_stack();

variable_t* new_variable(char* identifier, char* value, bool global);

bool v_is_declared(char* identifier);
void v_declare(char* identifier, bool global);
void v_set(char* identifier, char* value);

size_t v_idx(char* identifier);