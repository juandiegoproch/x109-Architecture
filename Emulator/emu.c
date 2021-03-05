#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define RAMSIZE 65535

#define PC (uint16_t)7
#define JR (uint16_t)6
#define ML (uint16_t)5
#define R2 (uint16_t)4
#define R1 (uint16_t)3
#define R0 (uint16_t)2
#define MD (uint16_t)1 //- Special Behaviour
#define ZR (uint16_t)0 //- Special Behaviour

 // Allocate system memory
uint16_t memory[RAMSIZE+1];
// Allocate storage for the registers
uint16_t registers[8];
// set up internal flags
int zeroFlag = 0;
int negativeFlag = 0;

// Load Memory with data:
void memload(char* memimage_s)
{
	FILE* memimage_F = fopen(memimage_s,"r");
	fread(memory,sizeof(uint16_t),(RAMSIZE+1),memimage_F);
	fclose(memimage_F);
}
// Dump RAM to file

void memdump()
{
	FILE* ramdump_F = fopen("memdump.memimg","w");
	fwrite(memory,sizeof(uint16_t),(RAMSIZE+1),ramdump_F);
}
// Register Handler

uint16_t registerop(int reg,char mode,uint16_t value)
{
	if (mode == 'r') // Read Operation handler
	{
		if (reg>MD) // All non special behaviour regs
		{
			return registers[reg];
		} 
		else if (reg == MD) // Memory Data reg handler
		{
			return memory[registers[ML]]; 
		}
		else if (reg == ZR) // Zero register handler
		{
			return (uint16_t)0x0000;
		}
	}
	else if (mode == 'w')
	{
		if (reg == MD)
		{
			//MD storage handler
			memory[registers[ML]] = value;
			return value;
		}
		else
		{
			registers[reg] = value;
			return value;
		}
	}
}

int decode(unsigned int instruction) // Detects if the instruction is an OP or a LD
{
	int instType = instruction >> 15; //isolate opcode
	return instType;
}
int storeReg(unsigned int instruction) //Returns register that the operation should save the reult into
{
	unsigned int regMask = 0b0111000000000000;      //sets a mask to zero all bits less the ones that represent the register
	unsigned int reg = instruction & regMask;
	reg = reg>>12; 					//shifts all bits into the first position, extracting the registerID
	return reg;					// the same principle applies to op1reg and op2reg
}
int op1reg(unsigned int instruction) // extracts the register that the op instruction takes as first operand
{
	unsigned int regMask = 0b0000000011100000;
	unsigned int reg = instruction & regMask;
	reg = reg>>5;
	return reg;
}
int op2reg(unsigned int instruction) // extracts register that the op instruction takes as second operand
{
	unsigned int regMask = 0b0000000000011100;
	unsigned int reg = instruction & regMask;
	reg = reg>>2;
	return reg;
}

uint16_t aluOp( uint16_t instruction, uint16_t operand1,uint16_t operand2)
{	
	// Extract alu configuration flags:
	int opType =   (instruction & 0b0000100000000000) >> 12;
	int zOperand = (instruction & 0b0000010000000000) >> 10;
	int nOperand = (instruction & 0b0000001000000000) >> 9 ;
	int carryIn =  (instruction & 0b0000000100000000) >> 8 ;
	
	// execute preprocesor on operand 1;
	
	operand1*= !zOperand; // Z config applied;
	operand1 = nOperand?~operand1:operand1; // conditional negation of operand 1
	operand1+= carryIn; // add carry to operand
	
	// alu operation
	uint16_t result = opType? (operand1 | operand2):(operand1+operand2); //execute logical or arit operation
	
	zeroFlag = !result; //set zero flag if result == 0
	
	negativeFlag = result>>15; // set negative flag
	
	return result;
}

int main(int argc,char* argv[])
{
	// Load memory from memimage if so provided
	
	if (argc-1) // if there are ANY arguments
	{
		if (*argv[1] != '-') //if first argument is not a "flag"
			memload(argv[1]);
	}
	
// Main Execution Loop
	while (!memory[RAMSIZE])
	{	 // Zero out the ZR to deal with operation stuff!.
		uint16_t currentInstruction = memory[registerop(PC,'r',0)]; // load the current instruction as the instruction currently pointed to by PC
		registerop(PC,'w',registerop(PC,'r',0)+1); // increment the program counter
		if (decode(currentInstruction))
		{
			//If the instruction is an OP type:
			int op1 = registerop(op1reg(currentInstruction),'r',0); //extract the value of the register that is op1
			int op2 = registerop(op2reg(currentInstruction),'r',0); //extract value from register op2
			int str = storeReg(currentInstruction); // get the regiter that the operation should be stored into
			int accountNegative = (currentInstruction & 0b0000000000000010)>>1;
			int accountZero = (currentInstruction & 0b0000000000000001);
			
			// execute the operation
			uint16_t operationResult = aluOp(currentInstruction,op1,op2);
			
			// conditional jumps
			
			if ( accountNegative || accountZero)
			{
				if ((accountNegative && negativeFlag) || (accountZero && zeroFlag))
				{
					uint16_t jumpto = registerop(JR,'r',0);
					registerop(PC,'w',jumpto);
				}
			}	
			else
			{
				registerop(str,'w',operationResult);
			}			
		}
		else // If instruction is LD type
		{
			int stl = storeReg(currentInstruction);		
					  // Extracts msb/lsb config line
			int bytePos = 8*((currentInstruction & 0b0000010000000000)>>10); // Specifies the bit shift that should happen, can be 0 or 8
			uint16_t bPtoload = (currentInstruction & 0b0000000011111111)<<bytePos; // Bit pattern to be loaded
			
			// Perform register operation
			uint16_t locationInitialValue = registerop(stl,'r',0);
			locationInitialValue = locationInitialValue & (bytePos?0x00FF:0xFF00); // Remove part that needs replacing;
			uint16_t valueToStore = locationInitialValue | bPtoload;
			registerop(stl,'w',valueToStore);
			
			
			
		}
		
	}
	
	// deal with program options (-d)
	for (int i = argc-1;i>=0;i--)
	{
		// Dump
		if (*(argv[i]+1) == 'd') // if ram is to be dumped
		{ 
			printf("Dumping RAM to file: \"memdump.memimg\"... \n");
			memdump();
			printf("RAM dumped \n");
		}
	}
	printf("Program Halted with code %X \n",memory[RAMSIZE]);
	return 0;
}
