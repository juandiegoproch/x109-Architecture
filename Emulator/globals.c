#include <stdint.h>

#include "declarations.h"

// Globals

// Storage
uint16_t memory[RAMSIZE] = {0};
uint16_t registers[REGISTERNUM] = {0};

// flags
bool zeroFlag = 0;
bool negativeFlag = 0;

