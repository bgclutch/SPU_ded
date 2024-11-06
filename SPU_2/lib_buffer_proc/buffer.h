#ifndef BUFFER_H_
#define BUFFER_H_

#include <stdio.h>
#include <math.h>

// ssize_t count_prefix_sum(size_t size, char* array, size_t elem_size);

size_t symbols_number(FILE* file);

char* buffer_create(size_t size, size_t symbol_size, FILE* file);

char* buffer_create_raw(size_t size, size_t symbol_size);

int res_checker(const ssize_t num, size_t line, const char* name);

size_t allocation_size(FILE* file);


#endif // BUFFER_H_