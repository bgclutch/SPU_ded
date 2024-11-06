#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>


#include "../lib_buffer_proc/buffer.h"
#include "../stack_ded/src/stack_headers/stack.h"

// ssize_t count_prefix_sum(size_t size, char* array, size_t elem_size)
// {
//     ssize_t sum = 0;

//     for(size_t str_n = 0; str_n < size; str_n++)
//     {
//         sum += *(StackElem_t*)(array + str_n * elem_size);
//     }

//     return sum;
// }


size_t symbols_number(FILE* file)
{
    if(file == nullptr)
    {
        fprintf(stderr, "file couldn't be open\n");
        return 0;
    }

    int fseek_res = fseek(file, 0, SEEK_END);

    if(fseek_res != 0)
    {
        fprintf(stderr, "invalid file\n");
        return 0;
    }

    size_t result = (size_t)ftell(file);

    fseek_res = fseek(file, 0, SEEK_SET);

    if(fseek_res != 0)
    {
        fprintf(stderr, "invalid file\n");
        return 0;
    }

    return result;
}

size_t allocation_size(FILE* file)
{
    size_t size = 0;

    fread(&size, sizeof(size_t), sizeof(char), file);

    return size;
}


char* buffer_create(size_t size, size_t symbol_size, FILE* file)
{
    char* buffer = (char*) calloc(size + 1, sizeof(char) * symbol_size);

    size_t fread_res = fread(buffer, sizeof(char) * symbol_size, size, file);

    if(fread_res != size)
    {
        fprintf(stderr, "fread returned: %lu instead of %lu\n", fread_res, size);
        free(buffer);
        return 0;
    }

    buffer[size] = '\0';

    return buffer;
}

char* buffer_create_raw(size_t size, size_t symbol_size)
{
    char* buffer = (char*) calloc(size + 1, sizeof(char) * symbol_size);

    return buffer;
}


int res_checker(const ssize_t num, size_t line, const char* name)
{
    if(num)
    {
        return 1;
    }
    else
    {
        fprintf(stderr, "error here %s:%lu", name, line);
        return 0;
    }
}