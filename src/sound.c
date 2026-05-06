#include "sound.h"
#include "chunk.h"
#include "wii.h"
#include "instrhelp.h"

void populate_samples()
{
    int address_pointer = 0;
    
    size_t count = 0;

    samples_left = (f32*) malloc((data_chunk->size / file_format_info->blockAlign) * sizeof(f32));
    samples_right = (f32*) malloc((data_chunk->size / file_format_info->blockAlign) * sizeof(f32));

    while (address_pointer < data_chunk->size)
    {
        i32 sample_l = next_u24_as_u32(data_chunk->data, &address_pointer);
        i32 sample_r = next_u24_as_u32(data_chunk->data, &address_pointer);

        if (sample_l & 0x800000)
            sample_l |= 0xFF000000;
        
        if (sample_r & 0x800000)
            sample_r |= 0xFF000000;

        samples_left[count] = sample_l / ((float) 0x7FFFFF);
        samples_right[count] = sample_r / ((float) 0x7FFFFF);

        count++;
    }

    n_samples = count;
}

void write_samples_to_data_chunk()
{
    size_t block_align = (size_t) file_format_info->blockAlign;


    for (size_t i = 0; i < n_samples; i++)
    {
        i32 s_l = (i32) (samples_left[i] * 0x7FFFFF);
        i32 s_r = (i32) (samples_right[i] * 0x7FFFFF);

        if (s_l >  0x7FFFFF)  s_l =  0x7FFFFF;
        if (s_l < -0x800000) s_l = -0x800000;
        if (s_r >  0x7FFFFF)  s_r =  0x7FFFFF;
        if (s_r < -0x800000) s_r = -0x800000;


        
        for (size_t j = 0; j < block_align / 2; j++)
        {
            data_chunk->data[j + i * block_align] = (u8) ((s_l >> (j * 8)) & 0xFF);
            data_chunk->data[j + i * block_align + (block_align / 2)] = (u8) ((s_r >> (j * 8)) & 0xFF);
        }
    }
}