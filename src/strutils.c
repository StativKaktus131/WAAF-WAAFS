#include "strutils.h"

// splits a string by delimiter and returns the new size in 'str_size'
char** str_split(char* str, const char* delim, size_t* str_size)
{
    // buffer is a dynamic array being able to be filled with every iteration
	char** buffer = NULL;
	size_t count = 0;
	char* token = strtok(str, delim);


	while (token)
	{
        // fill buffer
		count++;
		buffer = (char**) realloc(buffer, count * sizeof(char*));
		buffer[count - 1] = token;

		token = strtok(NULL, delim);
	}

	*str_size = count;

	return buffer;
}

// replaces 'rep' with 'with' in str 'orig'
char* str_replace(char* orig, char* rep, char* with)
{
    char* result;
    char* ins = orig;
    char* tmp;
    int len_rep = strlen(rep);
    int len_with = strlen(with);
    int len_front;
    size_t count;

    for (count = 0; (tmp = strstr(ins, rep)); count++)
        ins = tmp + len_rep;

    tmp = result = (char*) malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    while (count--)
    {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep;
    }

    strcpy(tmp, orig);
    return result;
}


// checks if a string contains certain character; if not, will return -1
bool str_contains(char* str, char* c)
{
	return strstr(str, c) != NULL;
}

char* str_trim(char* s)
{
    while (isspace(*s))
        s++;
    
    char* back = s + strlen(s);
    while (isspace(*--back))
        *(back+1) = '\0';
    
    return s;
}

// finds the index inside of the string
int str_find_index(char* str, const char* find, size_t size)
{
    size_t len = strlen(str) - size;

    for (size_t i = 0; i < len; i++)
    {
        if (strncmp(&str[i], find, size) == 0)
            return i;
    }
    
    return -1;
}