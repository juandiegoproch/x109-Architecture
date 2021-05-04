#ifndef _DECLARATIONS
#define _DECLARATIONS

#include <stdbool.h>
#include <stdint.h>

#define RAMSIZE 65535
#define REGISTERNUM 8

#define PC 7
#define JR 6
#define ML 5
#define R2 4
#define R1 3
#define R0 2
#define SP 1
#define ZR 0

// Functions

// command line parsing
void parseCliArguments(int argc,char* argv[],char** fileToRead, bool* dumpMemoryOption,bool* debug);

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

// Signal Handling

int userInterruptHandler(int signum);


//debugger functions

void printRegisterValues();
void securePrintRam(int location, int look_width);

// Globals
extern uint16_t memory[RAMSIZE];
extern uint16_t registers[REGISTERNUM];

extern bool zeroFlag;
extern bool negativeFlag;

#endif
