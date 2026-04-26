#include "util.h"

typedef struct Stack 
{
    void** nodes;
    size_t size;
} s_stack_t;

s_stack_t* new_stack()
{
    s_stack_t* ret = (s_stack_t*) malloc(sizeof(s_stack_t));
    ret->nodes = NULL;
    ret->size = 0;
    return ret;
}

void st_push(s_stack_t* stack, void* node)
{
    stack->size++;
    stack->nodes = (void**) realloc(stack->nodes, stack->size * sizeof(void*));
    stack->nodes[stack->size - 1] = node;
}

void* st_pop(s_stack_t* stack)
{
    if (stack->size == 0)
        return NULL;

    void* ret = stack->nodes[stack->size - 1];

    if (stack->size == 0) {
        free(stack->nodes);
        stack->nodes = NULL;
    } else {
        stack->nodes = (void**) realloc(stack->nodes, stack->size * sizeof(void*));
    }

    return ret;
}