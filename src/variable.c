#include "variable.h"
#include "instrhelp.h"

variable_t* new_variable(char* identifier, char* value, bool global)
{
    variable_t* ret = (variable_t*) malloc(sizeof(variable_t));

    // copy identifier and value (as string)

    ret->identifier = (char*) malloc(strlen(identifier));
    strcpy(ret->identifier, identifier);

    ret->value = (char*) malloc(strlen(value));
    strcpy(ret->value, value);

    ret->global = global;

    return ret;
}


void print_variable(variable_t* variable)
{
    printf("{ VAR: '%s', value: '%s', global: %d }\n", variable->identifier, variable->value, variable->global);
}

void print_stack()
{
    for (size_t i = 0; i < stack_size; i++)
        print_variable(stack[i]);
}


void try_init_stack()
{
    if (stack == NULL)
    {
        stack = (variable_t**) malloc(0);
        stack_size = 0;
    }
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



bool v_is_declared(char* identifier)
{
    for (size_t i = 0; i < stack_size; i++)
        if (strcmp(stack[i]->identifier, identifier) == 0)
            return TRUE;
    
    return FALSE;
}

void v_declare(char* identifier, bool global)
{
    try_init_stack();

    stack_size++;
    stack = (variable_t**) realloc(stack, stack_size * sizeof(variable_t*));

    // TODO global logic
    stack[stack_size - 1] = new_variable(identifier, "0", global);
}

void v_set(char* identifier, char* value)
{
    for (size_t i = 0; i < stack_size; i++)
    {
        if (strcmp(stack[i]->identifier, identifier) == 0)
        {
            stack[i]->value = realloc(stack[i]->value, strlen(value));
            strcpy(stack[i]->value, value);
        }
    }
}

size_t v_idx(char* identifier)
{
    for (size_t i = 0; i < stack_size; i++)
        if (strcmp(stack[i]->identifier, identifier) == 0)
            return i;

    return 0;
}