#include "instrhelp.h"
#include "sound.h"
#include "wii.h"

void populate_samples()
{
    // adress pointer shows wherever you in the data array are
    int adress_pointer = 0;
    
    // counts the number of samples
    size_t count = 0;

    // allocate memory for samples
    samples_left = (f32*) malloc((data_chunk->size / file_format_info->blockAlign) * sizeof(f32));
    
    if (file_format_info->channels == 2)
        samples_right = (f32*) malloc((data_chunk->size / file_format_info->blockAlign) * sizeof(f32));

    
    // read all the samples
    while (adress_pointer < data_chunk->size)
    {
        samples_left[count] = read_next_sample(&adress_pointer);
        
        if (file_format_info->channels == 2)
            samples_right[count] = read_next_sample(&adress_pointer);

        count++;
    }
    n_samples = count;
}

f32 read_next_sample(int* adress_pointer)
{
    i32 sample;

    switch (file_format_info->bitsPerSample)
    {
        case 16:
            sample = next_u16(data_chunk->data, adress_pointer);
            return sample / ((float) I16_MAX);

        // 24 bit fuckery because twos compliment and stuff
        case 24:
            sample = next_u24_as_u32(data_chunk->data, adress_pointer);
            if (sample & 0x800000)
                sample |= 0xFF000000;
            
            return sample / ((float) I24_MAX);

        case 32:
            sample = next_u32(data_chunk->data, adress_pointer);
            return sample / ((float) I32_MAX);
        
        default:
            return 0.0;
    }
}




void write_samples_to_data_chunk()
{
    size_t block_align = (size_t) file_format_info->blockAlign;
    size_t bits_per_channel_per_sample = block_align / file_format_info->channels;


    for (size_t i = 0; i < n_samples; i++)
    {
        i32 s_l = from_sample(samples_left[i]);

        i32 s_r;
        if (file_format_info->channels == 2)
            s_r = from_sample(samples_right[i]);


        
        for (size_t j = 0; j < bits_per_channel_per_sample; j++)
        {
            data_chunk->data[j + i * bits_per_channel_per_sample] = (u8) ((s_l >> (j * 8)) & 0xFF);
            
            if (file_format_info->channels == 2)
                data_chunk->data[j + i * bits_per_channel_per_sample + bits_per_channel_per_sample] = (u8) ((s_r >> (j * 8)) & 0xFF);
        }
    }
}

i32 from_sample(f32 sample)
{
    i32 s;
    switch (file_format_info->bitsPerSample)
    {
        case 16:
            s = (i32) (sample * I16_MAX);
            break;

        case 24:
            s = (i32) (sample * I24_MAX);
            if (s >  0x7FFFFF) s =  0x7FFFFF;
            if (s < -0x800000) s = -0x800000;
            break;

        case 32:
            s = (i32) (sample * I32_MAX);
            break;
    }

    return s;
}