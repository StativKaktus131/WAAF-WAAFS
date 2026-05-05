#include "util.h"
#include "stack.h"
#include "wii.h"
#include "instrhelp.h"
#include "strutils.h"
#include "dbginfo.h"

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


block_t* blockify_instructions(char** instructions, size_t* skip, condition_type_t condition_type)
{
    char* first_line = str_trim(instructions[0]);

	// condition of block (first block will be 'WAAF')
	char* condition = &first_line[2];

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

        char* line = str_trim(instructions[i]);


		// IF block
		if (line[0] == '?' || line[0] == '@' || line[0] == 'x')
		{
            condition_type_t c = line[0] == '?' ? IF : line[0] == '@' ? CALL : LOOP;

			// recursively find nested blocks
			b = blockify_instructions(&instructions[i], skip, c);

            printf("I: %zu, skip: %zu\n", i, *skip);
			// skip in instructions
			i += *skip;

		}
		// end block
		else if (line[0] == '}')
		{
			// break out of loop
			block_open = FALSE;
			
			// increase skip
            // TODO: CHECK MAYBE +=?
			*skip = count;
            printf("COUNT: %zu\n", count);

            char stripped_cond[32];
            strcpy(stripped_cond, condition);
            if (condition_type == CALL && condition[strlen(condition) - 1] == '{')
                stripped_cond[strlen(condition) - 2] = '\0';
            
			// return block with given condition (count - 1 because it's being increased at the beginning of the loop)
			return new_block(condition, condition_type, b_arr, count - 1);
		}
		// end WAAF file
		else if (strcmp(line, "/ ENDWAAF") == 0)
		{
			// break out of loop and return waaf block
			block_open = FALSE;
			return new_block("WAAF", NONE, b_arr, count - 1);
		}
		// single line instructions
		else
		{
			b = new_command(line);
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
    // how often block will be run (for loops)
    size_t amount = 1;

    // DEBUGGER -----------------
    if (dbg_value_of("show_block_when_run"))
        print_block(block);
    // --------------------------

	// only checks for blocks without condition rn
	if (block->condition_type == IF)
	{
		bool condition_met = eval_condition(block->condition);

		if (!condition_met)
			return;
	}
    else if (block->condition_type == LOOP)
    {
        amount = atoi(block->condition);
    }


    for (size_t i = 0; i < amount; i++)
    {
        // empty instructions means that its a container block
        if (strncmp(block->instruction_string, "-", 1) == 0 && block->condition_type != CALL)
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
}



void run_command(block_t* block)
{
	char* command = (char*) malloc(strlen(block->instruction_string) + 1);
	memcpy(command, block->instruction_string, strlen(block->instruction_string) + 1);

	char id = command[0];

	size_t args_len = 0;
	char** args = str_split(&command[2], ",", &args_len);


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
            printf("%s\n", decode_str(args[0]));
			break;

        case ':':
            printf("%f\n", *((double*)(decode_eval(args[0]))));
            break;

        case '!':
            try_call_method(args[0]);
            break;
		
	}

    free(args);
	free(command);
}

bool block_with_condition_exists(block_t* head, char* condition, block_t** out)
{
    for (size_t i = 0; i < head->n_instructions; i++)
    {
        if (strncmp(head->instructions[i]->condition, condition, strlen(condition)) == 0)
        {
            *out = head->instructions[i];
            return TRUE;
        }
        else if (strcmp(head->instructions[i]->instruction_string, "-") == 0)
        {
            if (block_with_condition_exists(head->instructions[i], condition, out))
                return TRUE;
        }
    }
    return FALSE;
}

void try_call_method(char* method)
{
    block_t* out = NULL;

    if (block_with_condition_exists(program, method, &out))
    {
        for (size_t i = 0; i < out->n_instructions; i++)
        {
            run_block(out->instructions[i]);
        }
    }
}