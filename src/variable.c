#include "variable.h"
#include "instrhelp.h"

variable_t* new_variable(char* identifier, char* value)
{
    variable_t* ret = (variable_t*) malloc(sizeof(variable_t));

    // copy identifier and value (as string)

    ret->identifier = (char*) malloc(strlen(identifier) + 1);
    strcpy(ret->identifier, identifier);

    ret->value = (char*) malloc(strlen(value) + 1);
    strcpy(ret->value, value);

    return ret;
}


void print_variable(variable_t* variable)
{
    printf("{ VAR: '%s', value: '%s' }\n", variable->identifier, variable->value);
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


bool v_is_declared(char* identifier)
{
    for (size_t i = 0; i < stack_size; i++)
        if (strcmp(stack[i]->identifier, identifier) == 0)
            return TRUE;
    
    return FALSE;
}

void v_declare(char* identifier)
{
    try_init_stack();

    stack_size++;
    stack = (variable_t**) realloc(stack, stack_size * sizeof(variable_t*));

    // TODO global logic
    stack[stack_size - 1] = new_variable(identifier, "0");
}

void v_set(char* identifier, char* value)
{
    for (size_t i = 0; i < stack_size; i++)
    {
        if (strcmp(stack[i]->identifier, identifier) == 0)
        {
            stack[i]->value = realloc(stack[i]->value, strlen(value) + 1);
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