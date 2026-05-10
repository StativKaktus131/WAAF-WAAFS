#pragma once

#include "util/util.h"

f32* samples_left;
f32* samples_right;

size_t n_samples;
size_t current_sample_pointer;

void populate_samples();
void write_samples_to_data_chunk();