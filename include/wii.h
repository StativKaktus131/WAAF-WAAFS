// waaf instant interpreter
#pragma once

#include "util.h"
#include "chunk.h"

// IF only runs block once, WHILE runs while condition stays true
typedef enum ConditionType 
{
    NONE,
    IF,
    WHILE
} condition_type_t;

// Every line will be a block. Conditionals can be packed into a block and will contain blocks of blocks
typedef struct Block
{
    char* condition;
    condition_type_t condition_type;
    struct Block** instructions;
    char* instruction_string;
    size_t n_instructions;
} block_t;


// prints a block
void print_block(block_t* block);

// initializes a new block
block_t* new_block(char* condition, condition_type_t condition_type, block_t** instructions, size_t n_instructions);

// returns a pointer to a command block
block_t* new_command(char* command);

// recursive method to find (nested) instruction blocks
block_t* blockify_instructions(char** instructions, size_t* skip);



// interprets and executes the block
void run_block(block_t* block);

// runs a block only if known that it is a command
void run_command(block_t* block);