#include "instrhelp.h"
#include "strutils.h"
#include "wii.h"
#include "chunk.h"

// RIFF / WAVE header, should be identical header in every wave file
const char check_header[] = {0x52, 0x49, 0x46, 0x46, 0x48, 0x10, 0x0E, 0x00, 0x57, 0x41, 0x56, 0x45};


// analyzes chunks from given file
chunk_t** analyze_chunks(const char* path, size_t* num_chunks, size_t* filesize)
{
	size_t fs = 12;						// filesize in bytes
	FILE* file = fopen(path, "r");		// file pointer
	chunk_t** chunks = NULL;			// chunks array
	size_t count = 0;					// number of chunks


	// check for correct header
	u8 header[12];
	fread(header, sizeof(u8), 12, file);

	if (memcmp(header, check_header, 12))
		printf("file is not a wave file\n");


	// read chunks from file
	while (TRUE)
	{
		// read char ID
		u8 charID[4];
		if (fread(charID, sizeof(u8), 4, file) < 4)
			break;	// end of file


		// read size
		u8 size_in_bytes[4];
		int p = 0;
		fread(size_in_bytes, sizeof(u8), 4, file);
		
		// use 'next' method to read out the size into a u32
		u32 size = next_u32(size_in_bytes, &p);


		// read chunk data
		u8* data = (u8*) malloc(sizeof(u8) * size);
		fread(data, sizeof(u8), size, file);


		// cumulate data in chunk
		chunk_t* chunk = new_chunk(charID, size, data);

		// advance filesize by chunk size (plus ID and size field)
		fs += size + 8;
		count++;

		// dynamically reallocate memory for chunks
		chunks = (chunk_t**) realloc(chunks, sizeof(chunk_t*) * count);

		// latest chunk
		chunks[count-1] = chunk;
	}

	fclose(file);

	*filesize = fs;
	*num_chunks = count;
	return chunks;
}

// write chunks to file
void write_chunks(const char* filepath, chunk_t** chunks, int nchunks, int size)
{
	u8 output[size];					// output byte array
	memcpy(output, check_header, 12);	// copy the header into first output region
	size_t address_pointer = 12;		// keep track of position

	for (int i = 0; i < nchunks; i++)
	{
		// write chunk ID
		memcpy(&output[address_pointer], &chunks[i]->chunkID, 4);
		address_pointer += 4;

		// write chunk size
		memcpy(&output[address_pointer], &chunks[i]->size, 4);
		address_pointer += 4;

		// write data
		for (size_t j = 0; j < (u32) chunks[i]->size; j++)
			output[address_pointer + j] = chunks[i]->data[j];

		address_pointer += chunks[i]->size;
	}

	// write to file
	FILE* fptr = fopen(filepath, "wb+");

	fwrite(output, sizeof(u8), address_pointer, fptr);

	fclose(fptr);
}


int main(int argc, char** args)
{

    // check if args are given
    if (argc != 4)
    {
        printf("wii <sample path> <output path>\n");
        return -1;
    }

    // run over whole file per default
    run_once = FALSE;

	// read and analyze file
	size_t count, filesize;
	chunk_t** chunks = analyze_chunks(args[1], &count, &filesize);
	format_info_t* info;

	// loop through chunks and decode format
	for (size_t i = 0; i < count; i++)
	{
		if (chunk_is(chunks[i], "fmt "))
			info = decode_format_chunk(chunks[i]);
		else if (chunk_is(chunks[i], "data"))
			data_chunk = chunks[i];
	}

	print_format_info(info);

	printf("\n");
	printf("*------------------------*\n");
	printf("| BEGINNING WAAF PROGRAM |\n");
	printf("*------------------------*\n");
	printf("\n");
    printf("---------------------------------------------------------------");
	printf("\n");


	// ---------------------- PROCESSING ----------------------


    srand(time(NULL));
    // roll 100 random numbers because the rng is weird
    for (size_t i = 0; i < 100; i++)
        rand();

    size_t instr_size = 0;
    char* instructions = read_file(args[3], &instr_size);
    
    size_t spl_size = 0;
    char** spl_instrs = str_split(instructions, "\n", &spl_size);


    size_t skip = 0;
    block_t* program = blockify_instructions(spl_instrs, &skip);


    for (size_t i = 0; i < data_chunk->size; i++)
    {
        current_data_pointer = i;
        progress = i / (float) data_chunk->size;

        run_block(program);
        
        if (run_once)
            break;
    }

	// --------------------------------------------------------

	write_chunks(args[2], chunks , 3, filesize);

	printf("\n");
	return 0;
}