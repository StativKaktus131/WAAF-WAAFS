#include "variable.h"
#include "instrhelp.h"

variable_t* new_variable(const char* identifier, char* value, bool global)
{
    variable_t* ret = (variable_t*) malloc(sizeof(variable_t));

    ret->identifier = identifier;
    ret->value = value;
    ret->global = global;

    return ret;
}

u8 value_to_u8(variable_t* var)
{
    void* v = decode_eval(var->value);

    return 0;
}

double value_to_dbl(variable_t* var)
{
    return 0.0;
}