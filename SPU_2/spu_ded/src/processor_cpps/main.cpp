#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include "../../../stack_ded/src/stack_headers/stack.h"
#include "../../../stack_ded/src/stack_headers/colorised_output.h"
#include "../processor_headers/processor.h"
#include "../processor_headers/instructions.h"
#include "../../../lib_buffer_proc/buffer.h"
#include "../../../lib_file_proc/file.h"



int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        fprintf(stderr, GREEN_TEXT("No No No ") "mister fish\n\n");
        Error_Codes_SPU run_code_err = run_code("spu_ded/src/processor_execode/machine_code.bin");

        if(run_code_err != NORMAL_COND)
            return 1;
    }
    else
    {
        fprintf(stderr, "input your numbers\n\n");
        Error_Codes_SPU run_code_err = run_code(argv[1]);

        if(run_code_err != NORMAL_COND)
            return 1;
    }

    return 0;
}