#pragma once

#include "util/util.h"

f32* samples_left;
f32* samples_right;

size_t n_samples;
size_t current_sample_pointer;


// methods to handle different bitrates in wav files
f32 read_next_sample(int* adress_pointer);
i32 from_sample(f32 sample);

void populate_samples();
void write_samples_to_data_chunk();
