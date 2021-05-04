#define MSB_MASK 0b1111111100000000
#define LSB_MASK 0b0000000011111111
#include "declarations.h"
#include <stdio.h>
// The return value is 
int execute_devices()
{
    // Character Output
    if (memory[33])
    {
        putchar(memory[34] & LSB_MASK);
        memory[33] = 0;
    }

    // No signal Emmited
    return 0;
}
