#ifndef INSTRUCTIONS_H_
#define INSTRUCTIONS_H_

#include <stdio.h>
#include <stdlib.h>


Error_Codes_SPU instructions(Struct_SPU* SPU, size_t buffer_size);

Error_Codes_SPU run_code(const char* file_name); // WTF again

#endif // INSTRUCTIONS_H_