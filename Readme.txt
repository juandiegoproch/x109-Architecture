Instructions

#registers
PC - POINTS TO NEXT INSTRUCTION TO BE EXECUTED

#ALU instruction

BITS FIELDS: (leftmost bits first)
name      length  value
OPCODE       1      1
STREG        3      ID of store register
ALUCFG       4      ALU configuration flags
OP1REG       3      ID of operand 1 register
OP2REG       3      ID of operand 2 register
ACCOUNTNEG   1      Jump on negative?
ACCOUNTZ     1      Jump on Zero?

An ALU instruction makes use of the processor's ALU.
They take 3 arguments, two operands and one storage location.
They also take a 4 bit ALU configuration.
    ALU configuration:
    (leftmost bit first)
    BIT    Meaning
    1       operation type: 1 = Bitwise Logical OR, 0 = Addition
    2       zero out operand1 if set
    3       negate operand1 if set
    4       carry in 1 to operand1 (if addition is selected)

The ALU instruction will perform the operation in the ALU. 

If either ACCOUNTNEG or ACCOUNTZ was set,it will jump to the adress in JR if any of the conditions matches. 
(Formally, jump if ((accountNegative && negativeFlag) || (accountZero && zeroFlag)))
The result of the operation will be discarded.

If no flags where set, the instruction will evaluate the operands according to the 
ALU configuration and store the result on the STR register operand.

#LOAD instructions

BITS FIELDS: (leftmost bits first)
name      length  value
OPCODE       1      0
STR          3      ID of store register
STORE        1      if 1 will store at memory location, if 0 will load from memory location
MSB          1      if set will load immediate value into msb of register, else lsb
STACK        1      if 1 will perform a push/pop on the stack or use indirect adressing to fetch/write from SP if ADRESSING also set
ADRESSING    1      if 1 will use indirect adressing to fetch/write from ML
IMM 1        4      Immediate value (first nibble)
IMM 2        4      Immediate value (second nibble)

Load Instructions are of 3 types. Immediate Load, stackop, and indirect_adressing

Immediate Load instructions: (both stackop and adressing flags 0)
    immediate load instructions take an 8 bit immediate value divided into 2 nibbles, a register, and an msb flag.
    it will override the msb of the selected register with the immediate value.

stackop (only stackop flag 1)
    stack operations take a register, the store flag and a 4 bit immediatval stored in the second nibble.
    if store is 1 they will push the value plus the second nibble immediateval of the selected register into the stack and increment the stack pointer
    if store is 0 they will pop  the value plus the second nibble immediateval into the selected register and decrement the stack pointer.

indirect adressing: (indirect adressing 1 and stackop either 0 or 1)
    if stackop is 1 and store is 1 will store the register value into SP+-immediateval (interpreted as a 8bit two's complement)
    if stackop is 0 and store is 1 will store the register value into ML+-immediateval
    if stackop is 1 and store is 0 will overwrite the register value from SP+-immediateval (interpreted as a 8bit two's complement)
    if stackop is 0 and store is 0 will overwrite the register value from ML+-immediateval



##unimplemented: Attempting a write operation on the zero register will cause an interrupt with the code of the value attempted to be written. Zero values will be ignored


##unimplemented: interrupts

interrupts are called immediatelly after a non-zero value INTCODE is written to the zero register.

an interrupt will:
push the PC to the stack.
jump to the location specified on INTTABLE + (INTCODE-1).

# Valid instructions and descriptions


ADD (add)
Arguments: 
STR: Store to register S
OP1: Operand 1         O
OP2: Operand 2         X
Adds the two operands and stores them in STR
Binary:

SUB (subtract)
Arguments: 
STR: Store to register
OP1: Operand 1
OP2: Operand 2
Subtracts OP1 from OP2 and stores the result in STR

LOR (logical or)
Arguments: 
STR: Store to register
OP1: Operand 1
OP2: Operand 2
Bitwise ORs Operand1 and Operand2

INC (increment)
Arguments:
STR
Increments STR

DEC (decrement)
Arguments:
STR
Decrements STR

NOT (logical not)
Arguments
STR
OP1
Bitwise logical negation of OP1 stored at STR

MOV (move)
Arguments:
FROM 
TO
Copies value from FROM to TO.

BNE (branch if negative)
Arguments
OP1
OP2
Subtracts OP1 from OP2 and branches to the location in JR if result is negative

BEQ (branch if equal)
Arguments
OP1
OP2
Subtracts OP1 from OP2 and branches to the location in JR if result is exactly zero

BLE (branch if less or equal)
Arguments
OP1
OP2
Subtracts OP1 from OP2 and branches to the location in JR if result is zero or negative;

LLS (load less significant)
Arguments:
STR
8 bit value
overwrites the lsb of STR with the 8 bit value provided

LMS (load most significant)
Arguments:
STR
8 bit value
overwrites the msb of STR with the 8 bit value provided

PUSH (add and push)
Arguments
STR
4 bit immediate
decrements SP and writes STR + 4 bit immediate into location of SP

POP (pop and add)
Arguments
STR
4 bit immediate
Loads 4 bit immediate plus the value at SP into STR and increments SP


STRST (store at stack)
Arguments:
REG 
8 bit signed immediateval
store value of REG with an offset from the SP of +- 128.

LDST (load from stack)
Arguments:
REG 
8 bit signed immediateval
load value of REG from memory at SP with a +- 128 offset.

LDH (load from heap)
Arguments:
REG 
8 bit signed immediateval
load value of REG from memory at ML with a +- 128 offset.

STH (store to heap)
Arguments:
REG 
8 bit signed immediateval
store value of REG with an offset from the ML of +- 128.

NOOP (no operation)
Arguments:
None
No operation is performed

### NOTICE: THE FOLLOWING CONTENT IS DEPRECATED AND MAY CONTAIN MISLEADIN INFORMATION

I am terrible writing formal documentation, so here is a more casual explaination of the cpu and its instruction set

This machine is a 16 bit von neumann architecture, wich means that both memory and the registers are 16 bits wide, and data and instructions live on the same adress space (IE you can manipulate programs as data).

REGISTERS:
There are 8 cpu registers ( in order from 0 to 7: ZR,MD,R0,R1,R2,ML,JR and PC) all of wich can be accesed by any instruction, however some of these registers have special behaviour.
ZR (Zero Register) yields allways zero when read, but moving a value into the register is undefined.
SP (Stack Pointer) is the location of the top of the stack, wich grows from high to low memory addresses.
JR (Jump Register) this register holds the value that the cpu will go to if a conditional branch is executed.
PC (Program Counter) This holds the adress of the next instruction to be executed

Registers are specified by their 3 bit ID, shown below:

 Reg| Decimal |  Binary |
__________________________
 ZR |    0    |   000   |
 SP |    1    |   001   |
 R0 |    2    |   010   |
 R1 |    3    |   011   |
 R2 |    4    |   100   |
 ML |    5    |   101   |
 JR |    6    |   110   |
 PC |    7    |   111   |

ALU:
The machine´s ALU accepts two operands and four configuration bits, ZERO,NEGATE,CARRYIN and OPTYPE. The first three bits apply the corresponding unary operations (if set to 1) to the first operand before the binary operation is executed
The Binary operation that follows can be addition (OPTYPE = 0) or bitwise logical or (OPTYPE = 1)
Furthermore, the ALU produces two flags: the Zero flag and the negative flag, these are set to 1 if the number complies to the condition that the flag signifies (IE, the zero flag is 1 if the last operation resulted in 0, and 1 otherwise). 

INSTRUCTIONS:
There are two distict instructions, the Operation instruction and the Load instruction, though these have great flexibility wich allows for many "pseudoinstructions"
The instructions are represented in machine language (binary) as follows:

          | 15 | 14 | 13 | 12 | 11 | 10      | 9  |  8 |    7 |    6 |   5 |  4 |  3 |  2      |     1 |    0 |
_______________________________________________________________________________________________________________
Operation | 1  |STORE REGISTER|    ALU CONFIGURATION   | OPERAND1 REGISTER | OPERAND2 REGISTER | BRANCH FLAGS |
Detailed  |    |              | OP |    Z    | N  | CIN|                                       |  Neg  |  Z   |
_______________________________________________________________________________________________________________
Load      | 0  |STORE REGISTER| 0  | MSB-LSB | 0  | 0  |             LITERAL 8 BIT VALUE                      |
_______________________________________________________________________________________________________________

The Load instruction is pretty self explainatory if you take into account the fact that the MSB/LSB bit indicates whether the value should be loaded to the less significant byte (0) or the most significant byte (1) of the target register.
The Operation instruction is more complex. It stores the result of an operation between operand 1 and operand 2 according to the alu configuration into the store register. Also, if any of the branch flags are set, the instruction will check if the corresponding alu flag is set, if so it will copy the JR's contents to the PC.
Basically, the "branching" considers the alu flag only if the branch flag that corresponds to that one is set. The logic is (alu flag & branch flag) | (other alu flag & other branch flag).

The LD instructions also are the way that the CPU interacts with RAM, and the LDM and STM instructions read into a register or store the value of aregister into a ram adress given as an offset of ML.

Recently, the PUSH and POP instructions have been added to the architecture under the LD opcode. These instructions facilitate stack operations.
   PSEUDOINSTRUCTIONS
At this point you should be able to read binary instructions for the cpu. However you may have noticed that these real instructions are not so helpfull.
Here come Pseudoinstructions.
Pseudoinstructions are particular configurations of an instruction that do defined tasks, like copy a register's value to another register, or increment a register, etc.
Here are all of them.

 MNEMONIC/EXAMPLE|    BINARY TRANSLATION   
___________________________________________
ADD STL OP1 OP2  | 0b1 STR 0000 OP1 OP2 00
SUB STR OP1 OP2  | 0b1 STR 0011 OP1 OP2 00
NOT STR OP1      | 0b1 STR 0010 OP1 000 00
MOV STR OP1      | 0b1 STR 1111 000 OP1 00
LOR STR OP1 OP2  | 0b1 STR 1000 OP1 OP2 00
ADI STR OP1 OP2  | 0b1 STR 0001 OP1 OP2 00
INC STR OP1 OP2  | 0b1 STR 0101 000 OP1 00
DEC STR OP1 OP2  | 0b1 STR 0110 000 OP1 00
LDL STR  (8bHEX) | 0b0 STR 0000  8-bit-lit
LDM STR  (8bHEX) | 0b0 STR 0100  8-bit-lit
BNE OP1 OP2      | 0b1 000 0011 OP1 OP2 10
BEQ OP1 OP2      | 0b1 000 0011 OP1 OP2 01
BLE OP1 OP2      | 0b1 000 0011 OP1 OP2 11
PUSH OP1 (8bHEX) | 0b0 OP1 1010   00000000
POP  OP1 (8bHEX) | 0b0 OP1 0010   00000000
ST   OP1 (8bHEX) | 0b0 OP1 1001  8-bit-lit
LD   OP1 (8bHEX) | 0b0 OP1 0001  8-bit-lit

So these are basically assembly mnemonics for usefull instructions.
Even if it is possible to use other non-documented pseudoinstructions I recommend that you dont, as those ALU configurations are liable to change in order to impement new features into the ISA.


You should now understand how this machine operates at the lowest level unless I have missed something.
Please send any questions or suggestions to juandiegopro1@gmail.com!
