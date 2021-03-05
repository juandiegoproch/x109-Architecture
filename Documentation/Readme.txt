I am terrible writing formal documentation, so here is a more casual explaination of the cpu and its instruction set

This machine is a 16 bit von neumann architecture, wich means that both memory and the registers are 16 bits wide, and data and instructions live on the same adress space (IE you can manipulate programs as data).

REGISTERS:
There are 8 cpu registers ( in order from 0 to 7: ZR,MD,R0,R1,R2,ML,JR and PC) all of wich can be accesed by any instruction, however some of these registers have special behaviour.
ZR (Zero Register) yields allways zero when read, but moving a value into the register is undefined.
MD (Memory Data) is an access window to system memory, to the adress pointed to by ML (Memory Location)
JR (Jump Register) this register holds the value that the cpu will go to if a conditional branch is executed.
PC (Program Counter) This holds the adress of the next instruction to be executed

Registers are specified by their 3 bit ID, shown below:

 Reg| Decimal |  Binary |
__________________________
 ZR |    0    |   000   |
 MD |    1    |   001   |
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

   PSEUDOINSTRUCTIONS
At this point you should be able to read binary instructions for the cpu. However you may have noticed that these real instructions are not so helpfull.
Here come Pseudoinstructions.
Pseudoinstructions are particular configurations of an instruction that do defined tasks, like copy a register's value to another register, or increment a register, etc.
Here are all of them

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
LDL STR (8bHEX)  | 0b0 STR 0000  8-bit-lit
LDM STR (8bHEX)	 | 0b0 STR 0100  8-bit-lit
BNE OP1 OP2      | 0b1 000 0011 OP1 OP2 10
BEQ OP1 OP2      | 0b1 000 0011 OP1 OP2 01
BLE OP1 OP2      | 0b1 000 0011 OP1 OP2 11

So these are basically assembly mnemonics for usefull instructions.


You should now understand how this machine operates at the lowest level unless I have missed something.
Please send any questions or suggestions to juandiegopro1@gmail.com!