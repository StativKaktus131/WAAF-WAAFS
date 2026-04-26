#include "util.h"
#include "stack.h"
#include "wii.h"
#include "instrhelp.h"

void print_block(block_t* block)
{
	printf("--- BLOCK -------------------------------------------\n");
	printf("CONDITION: \t\t'%s'\n", block->condition);
	printf("CONDITION TYPE: \t'%d'\n", block->condition_type);
	printf("N ISTRUCTIONS: \t\t[%zu]\n", block->n_instructions);
	printf("INSTR STRING: \t'%s'\n", block->instruction_string);
	printf("-----------------------------------------------------\n");
}

block_t* new_block(char* condition, condition_type_t condition_type, block_t** instructions, size_t n_instructions)
{
	block_t* ret = (block_t*) malloc(sizeof(block_t));

	ret->condition = condition;
	ret->condition_type = condition_type;
	ret->instructions = instructions;
	ret->instruction_string = "-";
	ret->n_instructions = n_instructions;

	return ret;
}


block_t* new_command(char* command)
{
	block_t* ret = (block_t*) malloc(sizeof(block_t));

	ret->condition = "-";
	ret->condition_type = NONE;
	ret->instructions = NULL;
	ret->instruction_string = command;
	ret->n_instructions = 1;

	return ret;
}


block_t* blockify_instructions(char** instructions, size_t* skip)
{
	// skips all tabstops and spaces
	size_t first_char = 0;
	while (instructions[0][first_char] == '\t' || instructions[0][first_char] == ' ')
		first_char++;
	
	// condition of block (first block will be 'WAAF')
	char* condition = &instructions[0][first_char + 2];

	// keeping track of block open and of block count
	bool block_open = TRUE;
	size_t count = 0;

	// instructions array
	block_t** b_arr = NULL;

	// while loop with index
	for (size_t i = 1; block_open; i++)
	{
		count++;

		// block to add
		block_t* b = NULL;

		// skip tabstops and whitespaces again
		first_char = 0;
		while (instructions[i][first_char] == '\t' || instructions[i][first_char] == ' ')
			first_char++;

		// IF block
		if (instructions[i][first_char] == '?')
		{
			// recursively find nested blocks
			b = blockify_instructions(&instructions[i], skip);

			// skip in instructions
			i += *skip;
		}
		// end block
		else if (instructions[i][first_char] == '}')
		{
			// break out of loop
			block_open = FALSE;
			
			// increase skip
			*skip += count;
			
			// return block with given condition (count - 1 because it's being increased at the beginning of the loop)
			return new_block(condition, IF, b_arr, count - 1);
		}
		// end WAAF file
		else if (strcmp(instructions[i], "/ ENDWAAF") == 0)
		{
			// break out of loop and return waaf block
			block_open = FALSE;
			return new_block("WAAF", NONE, b_arr, count - 1);
		}
		// single line instructions
		else
		{
			b = new_command(&instructions[i][first_char]);
		}

		// dynamically add block to array
		b_arr = (block_t**) realloc(b_arr, count * sizeof(block_t*));
		b_arr[count - 1] = b;
	}

	// return waaf block
	return new_block("WAAF", NONE, b_arr, count);
}


void run_block(block_t* block)
{
	// only checks for blocks without condition rn
	if (block->condition_type == IF)
	{
		bool condition_met = eval_condition(block->condition);

		if (!condition_met)
			return;
	}

	// empty instructions means that its a container block
	if (strncmp(block->instruction_string, "-", 1) == 0)
	{
		// runs every instruction block inside
		for (size_t i = 0; i < block->n_instructions; i++)
		{
			run_block(block->instructions[i]);
		}
	}
	// if not run command
	else
		run_command(block);
}



void run_command(block_t* block)
{
	char* command = (char*) malloc(strlen(block->instruction_string));
	memcpy(command, block->instruction_string, strlen(block->instruction_string));

	char id = command[0];

	size_t args_len = 0;
	char** args = split(&command[2], ",", &args_len);


	switch (id)
	{
		case '>':
			set(args[0], args[1]);
			break;
		
		case '/':
			if (strcmp(args[0], "ONCE") == 0)
				run_once = TRUE;
			break;

		case '.':
			printf("%s\n", args[0]);
			break;
		
	}

	free(command);
}