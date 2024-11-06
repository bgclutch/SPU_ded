#include <stdio.h>
#include <math.h>
#include <assert.h>


#include "../lib_buffer_proc/buffer.h"
#include "../lib_file_proc/file.h"

// TODO errors enum
Files_Errors file_read_open(FILE** file, const char* file_read_open)
{
    assert(!*file);

    *file = fopen(file_read_open,  "r");

    if(file == nullptr)
    {
        fprintf(stderr, "file read couldn't open fread open\n");
        return FILE_R_OPEN_ERR;
    }

    return ALL_GOOD_RET_F;
}


Files_Errors file_write_open(FILE** file, const char* file_write_open)
{
    assert(!*file);

    *file = fopen(file_write_open, "w");

    if(file == nullptr)
    {
        fprintf(stderr, "file write couldn't open\n");
        return FILE_W_OPEN_ERR;
    }

    return ALL_GOOD_RET_F;
}


Files_Errors file_close(FILE* file)
{
    int file_cl_res = fclose(file);

    if(file_cl_res != 0)
    {
        fprintf(stderr, "file close error\n");
        return FILE_CLOSE_ERR;
    }

    return ALL_GOOD_RET_F;
}