#include "chunk.h"

// 'next'-method implementations
u32 next_u32(u8* data, int* address_pointer)
{
	u32 ret = 0;

    // shift to left because of little endian
	for (int i = 0; i < 4; i++) 
		ret |= data[*address_pointer + i] << (i * 8);

    // advance pointer
	*address_pointer += 4;

	return ret;
}

u16 next_u16(u8* data, int* address_pointer)
{
	u16 ret = 0;

    // shift to left because of little endian
	for (int i = 0; i < 2; i++) 
		ret |= data[*address_pointer + i] << (i * 8);

    // advance pointer
	*address_pointer += 2;

	return ret; 
}


chunk_t* new_chunk(u8* chunkID, u32 size, u8* data)
{
    // allocate chunk
	chunk_t* ret = (chunk_t*) malloc(sizeof(chunk_t));

    // set values
    memcpy(ret->chunkID, chunkID, 4);
    ret->size = size;
    ret->data = data;

	return ret;
}

bool chunk_is(chunk_t* chunk, const char* check)
{
    return strncmp(chunk->chunkID, check, 4) == 0;
}

void print_chunk_data(chunk_t* chunk, int data_mode)
{
	printf("\n----------\nID: %s\nsize: %lu\n", chunk->chunkID, chunk->size);
	
	switch (data_mode)
	{
		case PRINT_STRING:
			printf("%s\n", chunk->data);
			break;

		case PRINT_HEX:
			for (int i = 0; i < chunk->size; i++)
				printf("%x ", chunk->data[i] & 0xff);
			printf("\n");
			break;
	}

	printf("----------\n");
}

void print_format_info(format_info_t* info)
{
	printf("\n");
	printf("*------------------*\n");
	printf("| FORMATTING INFOS |\n");
	printf("*------------------*\n");
	printf("\n");

	printf("format tag: %u\n", info->formatTag);
	printf("channels: %u\n", info->channels);
	printf("sample rate: %lu\n", info->sampleRate);
	printf("average byte rate: %lu\n", info->avgByteRate);
	printf("block align: %u\n", info->blockAlign);
	printf("bits per sample: %u\n\n", info->bitsPerSample);
}


format_info_t* decode_format_chunk(chunk_t* format)
{
	u32 size = format->size;
	u8* data = format->data;
	int data_pointer = 0;

    format_info_t* ret = (format_info_t*) malloc(sizeof(format_info_t));

	ret->formatTag = next_u16(data, &data_pointer);
	ret->channels = next_u16(data, &data_pointer);
	ret->sampleRate = next_u32(data, &data_pointer);
	ret->avgByteRate = next_u32(data, &data_pointer);
	ret->blockAlign = next_u16(data, &data_pointer);
	ret->bitsPerSample = next_u16(data, &data_pointer);

    return ret;
}