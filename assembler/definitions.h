#ifndef definitions
#define definitions
///////////////////////////////////////////////////////////////////////////////////
#define true 1
#define false 0

#define MAXTOKENS 4
#define MAXTOKENLEN 5

#define STL_OFFSET 12
#define OP1_OFFSET 5
#define OP2_OFFSET 2
// Registers

#define PC (uint16_t)7
#define JR (uint16_t)6
#define ML (uint16_t)5
#define R2 (uint16_t)4
#define R1 (uint16_t)3
#define R0 (uint16_t)2
#define MD (uint16_t)1
#define ZR (uint16_t)0

// Masks for instructions just needing to or in operands
#define ADD_MASK 	0b1000000000000000
#define SUB_MASK   	0b1000001100000000
#define NSM_MASK	0b1000001000000000
#define MOV_MASK	0b1000111100000000
#define LOR_MASK	0b1000100000000000
#define NOT_MASK 	0b1000001000000000 
#define ADI_MASK	0b1000000100000000
#define INC_MASK	0b1000010100000000
#define DEC_MASK	0b1000011000000000
#define LDL_MASK	0b0000000000000000
#define LDM_MASK	0b0000010000000000
#define BNE_MASK	0b1000001100000010
#define BEQ_MASK	0b1000001100000001
#define BLE_MASK	0b1000001100000011

// Individual assemble functions
// 3 aargument instructions
uint16_t assmAdd(char* arguments[3]);
uint16_t assmSub(char* arguments[3]);
uint16_t assmNsm(char* arguments[3]);
uint16_t assmLor(char* arguments[3]);
uint16_t assmAdi(char* arguments[3]);
// 2 argument instructions
uint16_t assmInc(char* arguments[2]);
uint16_t assmDec(char* arguments[2]);
uint16_t assmNot(char* arguments[2]);
uint16_t assmMov(char* arguments[2]);
uint16_t assmBne(char* arguments[2]);
uint16_t assmBeq(char* arguments[2]);
uint16_t assmBle(char* arguments[2]);
// load instructions
uint16_t assmLdl(char* arguments[2]);
uint16_t assmLdm(char* arguments[2]);


// General instruction assembler
uint16_t assmInstFromTokens(char** tokens);

// register translation utility
uint8_t getRegister(char* regMnemonic_s);

// Tokenizing utility and helper
int isWhitespace(char character);
int gettokens(char** buffer,char* instruction);


////////////////////////////////////////////////////////////////////////////////
#endif
