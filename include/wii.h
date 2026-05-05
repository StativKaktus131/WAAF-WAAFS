// waaf instant interpreter
#pragma once

#include "util.h"
#include "chunk.h"

// IF only runs block once, WHILE runs while condition stays true
typedef enum ConditionType 
{
    NONE,
    IF,
    WHILE,
    CALL,
    LOOP
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

// program block, master block
block_t* program;


// prints a block
void print_block(block_t* block);

// initializes a new block
block_t* new_block(char* condition, condition_type_t condition_type, block_t** instructions, size_t n_instructions);

// returns a pointer to a command block
block_t* new_command(char* command);

// recursive method to find (nested) instruction blocks
block_t* blockify_instructions(char** instructions, size_t* skip, condition_type_t condition_type);


// interprets and executes the block
void run_block(block_t* block);

// runs a block only if known that it is a command
void run_command(block_t* block);


// RUNNING METHODS

// checks if any block with the given condition exists and sets a pointer to it in 'out'. Head should be program
bool block_with_condition_exists(block_t* head, char* condition, block_t** out);

// tries to call a method if it exists
void try_call_method(char* method);