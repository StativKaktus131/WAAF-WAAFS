#pragma once

#include "util.h"
#define N_DEBUG_FLAGS 3


char** idx_matches;
u8 dbginfo;

void init_dbginfo();
bool dbg_value_of(const char* identifier);