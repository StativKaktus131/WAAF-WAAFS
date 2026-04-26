#include "util.h"
#include "instrhelp.h"

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


char** split(char* str, const char* delim, size_t* str_size)
{
	char** buffer = NULL;
	size_t count = 0;
	char* token = strtok(str, delim);


	while (token)
	{
		count++;
		buffer = (char**) realloc(buffer, count * sizeof(char*));
		buffer[count - 1] = token;

		token = strtok(NULL, delim);
	}

	*str_size = count;

	return buffer;
}


// You must free the result if result is non-NULL.
char* replace(char *orig, char *rep, char *with) {
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep (the string to remove)
    int len_with; // length of with (the string to replace rep with)
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    // sanity checks and initialization
    if (!orig || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = strlen(with);

    // count the number of replacements needed
    ins = orig;
    for (count = 0; (tmp = strstr(ins, rep)); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}


bool contains(char* str, char* c)
{
	return strstr(str, c) != NULL;
	// for (size_t i = 0; i <= strlen(str) - strlen(c); i++)
	// 	if (strncmp(&str[i], c, strlen(c)) == 0)
	// 		return TRUE;

	// return FALSE;
}

char *ltrim(char *s)
{
    while(isspace(*s)) s++;
    return s;
}

char *rtrim(char *s)
{
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}

char *trim(char *s)
{
    return rtrim(ltrim(s)); 
}


int find_index(char* str, const char* find, size_t size)
{
    size_t len = strlen(str) - size;

    for (size_t i = 0; i < len; i++)
    {
        if (strncmp(&str[i], find, size) == 0)
            return i;
    }
    
    return -1;
}



// running methods

void* decode_eval(char* value)
{
	// printf("VALUE: %s\n", value);
	char* stripped = trim(value);

	// printf("len of: %s; %zu\n", stripped, strlen(stripped));

	while (contains(stripped, "#RANDOM_FLOAT"))
	{
		float random = rand() / (float) RAND_MAX;
		char str[16];
		sprintf(str, "%f", random);

		char* rplcd = replace(stripped, "#RANDOM_FLOAT", str);
		strcpy(stripped, rplcd);
	}
	while (contains(stripped, "#RANDOM"))
	{
		u8 random = rand();
		char str[8];
		sprintf(str, "%hhu", random);

		char* rplcd = replace(stripped, "#RANDOM", str);
		strcpy(stripped, rplcd);
	}
	while (contains(stripped, "#PROGRESS"))
	{
		char str[16];
		snprintf(str, 10, "%f", progress);

		char* rplcd = replace(stripped, "#PROGRESS", str);
		strcpy(stripped, rplcd);
	}
    while (contains(stripped, "#STEP"))
    {
        char str[8];
        sprintf(str, "%zu", current_data_pointer);

		char* rplcd = replace(stripped, "#STEP", str);
		strcpy(stripped, rplcd);
    }
    while (contains(stripped, "#DATA"))
    {
        char str[8];
        sprintf(str, "%hhu", data_chunk->data[current_data_pointer]);

		char* rplcd = replace(stripped, "#DATA", str);
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
		idx = find_index(condition, comparators[i], strlen(comparators[i]));
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