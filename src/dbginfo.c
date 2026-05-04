#include "dbginfo.h"
#include "strutils.h"


void init_dbginfo()
{
	char** idx_matches_buffer = NULL;;
	char* content = NULL;
	
	// creating scope where dbgfile and count can exist, shouldn't access them from outside
	// TODO: move to separate method (more than one occurence)
	{
		size_t count = 0;
		FILE* dbgfile = fopen("debugflags.txt", "r");
	
		while (TRUE)
		{
			char c;
			
			if (fread(&c, 1, 1, dbgfile) < 1)
				break;
			
			count++;
			content = (char*) realloc(content, count);
			content[count - 1] = c;
		}
	
		content = (char*) realloc(content, count + 1);
		content[count] = '\0';
		fclose(dbgfile);
	}


	size_t spl_array_size = 0;
	char** dbglines = str_split(content, "\n", &spl_array_size);

	size_t count = 0;

	for (size_t i = 0; i < spl_array_size; i++)
	{
		char* line = dbglines[i];

		if (!str_contains(line, ":"))
			continue;

		idx_matches_buffer = (char**) realloc(idx_matches_buffer, (count + 1) * sizeof(char*));
		
		size_t n_sides = 0;
		char** sides = str_split(line, ":", &n_sides);

		char* id = str_trim(sides[0]);
		char* c_value = str_trim(sides[1]);
		bool value = (strcmp(c_value, "true") == 0) ? TRUE : FALSE;

		idx_matches_buffer[count] = id;
		dbginfo |= (value << count);
		count++;

		free(sides);
	}
	
	free(dbglines);

	// copy array
	idx_matches = (char**) malloc(count * sizeof(char*));

	for (size_t i = 0; i < count; i++)
	{
		idx_matches[i] = (char*) malloc(strlen(idx_matches_buffer[i]));
		strcpy(idx_matches[i], idx_matches_buffer[i]);
	}

	free(idx_matches_buffer);
	free(content);
}

bool dbg_value_of(const char* identifier)
{
	for (size_t i = 0; i < N_DEBUG_FLAGS; i++)
	{
		if (strcmp(idx_matches[i], identifier) == 0)
			return (dbginfo >> i) & 1;
	}
	return FALSE;
}