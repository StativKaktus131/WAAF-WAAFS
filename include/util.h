#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

#include "tinyexpr.h"

#define u32 unsigned long   // %lu
#define u16 unsigned int    // %u
#define u8 unsigned char    // %c, %x, %zu

/* 
 * chunk printing logic:
 * DONT_PRINT = doesn't print the data block at all
 * PRINT_STRING = prints the data as a string of characters
 * PRINT_HEX = prints the data as a list of hex codes
*/
#define DONT_PRINT 0
#define PRINT_STRING 1
#define PRINT_HEX 2

// bool definitions
#define bool int
#define TRUE 1
#define FALSE 0

