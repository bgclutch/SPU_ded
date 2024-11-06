#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>

#include "../asm_headers/assembler.h"
#include "../../../lib_file_proc/file.h"
#include "../../../lib_buffer_proc/buffer.h"


int main(int argc, char** argv)
{
    if(argc < 3)
    {
        fprintf(stderr, "try again (and add more files)\n");
        return 0;
    }

    Asm_Errors get_bincode_err = get_bincode(argv[1], argv[2]);

    ERROR_PROC(get_bincode_err);

    return 0;
}