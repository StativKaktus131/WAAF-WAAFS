#pragma once

#include "util.h"

// chunk of data stored in wave file
typedef struct Chunk 
{
	char chunkID[4];    // IDs like 'fmt ' or 'data'
	u32 size;           // size in bytes
	u8* data;           // byte array of length size
} chunk_t;

// formatting of the wave file. useful for manipulation
typedef struct FormatInfo
{
	u16 formatTag;
	u16 channels;
	u16 blockAlign;
	u16 bitsPerSample;
	u32 sampleRate;
	u32 avgByteRate;
} format_info_t;


// next methods: advance the pointer by the appropriate size and returns the next unsigned value (little-endian format)
u32 next_u32(u8* data, int* address_pointer);
u16 next_u16(u8* data, int* address_pointer);

// returns a pointer to a new chunk with the given id, size and data
chunk_t* new_chunk(u8* chunkID, u32 size, u8* data);

// checks wether chunk id is the same as check
bool chunk_is(chunk_t* chunk, const char* check);

// debug method: prints the chunk and its data in the given mode (see lines 7-9)
void print_chunk_data(chunk_t* chunk, int data_mode);
// debug method: prints the format info
void print_format_info(format_info_t* info);

// decodes the format info from the given chunk. Doesn't check for appropriate ID
format_info_t* decode_format_chunk(chunk_t* format);