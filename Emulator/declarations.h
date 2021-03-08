#ifndef _DECLARATIONS
#define _DECLARATIONS

#include <stdbool.h>

#define RAMSIZE 65535
#define REGISTERNUM 8

#define PC (uint16_t)7
#define JR (uint16_t)6
#define ML (uint16_t)5
#define R2 (uint16_t)4
#define R1 (uint16_t)3
#define R0 (uint16_t)2
#define MD (uint16_t)1
#define ZR (uint16_t)0

// Functions

// command line parsing
void parseCliArguments(int argc,char* argv[],char** fileToRead, bool* dumpMemoryOption);

// ALU operation (alu.c)
uint16_t aluOperation(uint8_t aluConfig,uint16_t operand1,uint16_t operand2);

// Register operations (registers.c)
uint16_t registerop(int reg,char mode,uint16_t value);

// Load and dump system ram,
void memload(char* memimage_s);
void memdump();

// Instruction decoding (decode.c)
int decode(unsigned int instruction);
int decodeStoreReg(unsigned int instruction);
int decodeOp1reg(unsigned int instruction);
int decodeOp2reg(unsigned int instruction);

// Instruction Execution (instructions.c)
uint16_t fetchInstruction();

void executeAluOpInstruction(uint16_t instruction);
void executeMLoadInstruction(uint16_t instruction);

// Globals
extern uint16_t memory[RAMSIZE];
extern uint16_t registers[REGISTERNUM];

extern bool zeroFlag;
extern bool negativeFlag;

#endif
