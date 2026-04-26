#pragma once

#include "util.h"

typedef struct Variable
{
    const char* identifier;
    char* value;
    bool global;
} variable_t;


variable_t* new_variable(const char* identifier, char* value, bool global);

u8 value_to_u8(variable_t* var);
double value_to_dbl(variable_t* var);