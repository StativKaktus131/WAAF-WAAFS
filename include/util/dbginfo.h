#pragma once

#include "util/util.h"


char** idx_matches;
u32 dbginfo;
size_t n_debug_flags;

void init_dbginfo();
bool dbg_value_of(const char* identifier);