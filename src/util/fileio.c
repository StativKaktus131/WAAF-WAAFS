#include "util/fileio.h"
#include "util/util.h"

int read_file_into_char_array(char* path, char** buffer)
{
    size_t count = 0;
    FILE* file = fopen(path, "r");

    while (TRUE) 
    {
        char c;

        if (fread(&c, 1, 1, file) < 1)
            break;
    }
}