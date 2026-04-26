#include "util.h"
#include "instrhelp.h"
#include "strutils.h"

char* read_file(const char* filepath, size_t* size)
{
	FILE* file = fopen(filepath, "r");
	size_t count = 0;
	char* buffer = NULL;

	while (TRUE)
	{
		// reads every character one by one and appends it to the string
		char c;
		if (fread(&c, sizeof(char), 1, file) < 1)
			break;
		
		count++;

		buffer = (char*) realloc(buffer, count);
		buffer[count - 1] = c;
	}

	// close file
	fclose(file);

	// store size
	*size = count;

	return buffer;
}

// running methods

void* decode_eval(char* value)
{
	// printf("VALUE: %s\n", value);
	char* stripped = str_trim(value);

	// printf("len of: %s; %zu\n", stripped, strlen(stripped));

	while (str_contains(stripped, "#RANDOM_FLOAT"))
	{
		float random = rand() / (float) RAND_MAX;
		char str[16];
		sprintf(str, "%f", random);

		char* rplcd = str_replace(stripped, "#RANDOM_FLOAT", str);
		strcpy(stripped, rplcd);
	}
	while (str_contains(stripped, "#RANDOM"))
	{
		u8 random = rand();
		char str[8];
		sprintf(str, "%hhu", random);

		char* rplcd = str_replace(stripped, "#RANDOM", str);
		strcpy(stripped, rplcd);
	}
	while (str_contains(stripped, "#PROGRESS"))
	{
		char str[16];
		snprintf(str, 10, "%f", progress);

		char* rplcd = str_replace(stripped, "#PROGRESS", str);
		strcpy(stripped, rplcd);
	}
    while (str_contains(stripped, "#STEP"))
    {
        char str[8];
        sprintf(str, "%zu", current_data_pointer);

		char* rplcd = str_replace(stripped, "#STEP", str);
		strcpy(stripped, rplcd);
    }
    while (str_contains(stripped, "#DATA"))
    {
        char str[8];
        sprintf(str, "%hhu", data_chunk->data[current_data_pointer]);

		char* rplcd = str_replace(stripped, "#DATA", str);
		strcpy(stripped, rplcd);
    }

	// printf("STRIPPED: %s\n", stripped);

    double* result = (double*) malloc(sizeof(double));
    *result = te_interp(stripped, 0);

	return (void*) result;
}

bool eval_condition(char* condition)
{
	if (condition[strlen(condition) - 1] == '{')
		condition[strlen(condition) - 2] = '\0';

	// if array is changed: change switch statement below
    const char* comparators[] = {"==", "!=", "<=", ">=", "<", ">"};
	
	// find index of comparator
	int idx = -1;
	int comparator_idx = -1;
	// char* comparator = NULL;
	for (int i = 0; i < sizeof(comparators) / sizeof(char*); i++)
	{
		idx = str_find_index(condition, comparators[i], strlen(comparators[i]));
		if (idx > 0)
		{
			comparator_idx = i;
			break;
		}
	}

	// printf("IDX: %d\n", idx);

	char* left_side_c = (char*) malloc(idx);
	char* right_side_c = (char*) malloc(strlen(condition) - idx - strlen(comparators[comparator_idx]));

	strncpy(left_side_c, condition, idx);
	strncpy(right_side_c, &condition[idx + strlen(comparators[comparator_idx])], strlen(condition) - idx - strlen(comparators[comparator_idx]));

	double left_side = *((double*) decode_eval(left_side_c));
	double right_side = *((double*) decode_eval(right_side_c));

	// printf("\nLEFT_SIDE_C: %s\nRIGHT_SIDE_C: %s\n\n", left_side_c, right_side_c);
	// printf("\nLEFT_SIDE: %g\nRIGHT_SIDE: %g\n\n", left_side, right_side);

	double gratitude = 0.005;
	bool ret = FALSE;

	switch (comparator_idx)
	{
		// ==
		case 0:
			// printf("%f\n", fabs(left_side - right_side));
			ret = fabs(left_side - right_side) < gratitude;
			break;

		// !=
		case 1:
			// printf("%f\n", fabs(left_side - right_side));
			ret = fabs(left_side - right_side) > gratitude;
			break;

		// <=
		case 2:
			ret = left_side <= right_side;
			break;
		
		// >=
		case 3:
			ret = left_side >= right_side;
			break;

		// <
		case 4:
			ret = left_side < right_side;
			break;
		
		// >
		case 5:
			ret = left_side > right_side;
			break;
	}

	return ret;
}


void set(char* arg1, char* arg2)
{
	void* ret = decode_eval(arg2);

	double ret_dbl = *((double*) ret);

	if (strcmp(arg1, "DATA") == 0)
	{
		// printf("data is found, value will be set to %hhu\n", (u8) ret_dbl);
		// set data
		data_chunk->data[current_data_pointer] = (u8) ret_dbl;
	}

    free(ret);
}