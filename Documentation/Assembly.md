# Assembly

Description of the assembly language used for this machine.

### Table of Contents
* [Registers](#registers)
* [Notation](#notation)
* [Instructions](#instructions)
* [Labels](#labels)
* [Directives](#directives)
  * [Entry](#entry)
  * [Var](#var)
  * [Ram_start](#ram_start)
  * [Peripheral](#peripheral)

## Registers

There is one general purpose register, the accumulator.

**PC** is a special register that contains the program counter. It cannot be read at this time and can only be written to by using a jump instruction (`JMP`, or `JZ`), see below.

**SP** is a special register that contains the stack pointer. It is not possible to read from and write to it. The stack pointer is automatically reset to point to the end of RAM.

## Notation

In the explanation, the operands are labelled:
```
OPCODE Imm, Addr
```
and so on.

Notation | Description
:---: | ---
OPCODE | Instruction opcode, 6 bits unsigned.
Imm | Immediate value, 13 bits unsigned.
Addr | Address value, 13 bits unsigned.

## Encoding

Each instruction is encoded as a 32 bit signed integer. With the highest 13 bits being the immediate value, the next highest 13 bits being the address value, and the lowest 6 being the opcode. In the ROM, the each instruction is stored on signal D.
Immediate | Address | Opcode
:---: | :---: | :---:
13 bits | 13 bits | 6 bits

## Instructions

All arithmetic and logical operations take an immediate value and an address and calculate:
Accumulator operation (immediate + value at address).
The memory map for the computer starts at address 1, so address 0 will always have the value 0 in it, so leaving the address as 0 performs the operation with only the immediate value.

Instruction | Opcode | Description
--- | --- | ---
NOP | 0  | No operation
ADD | 1  | Add to the accumulator and save that value to the accumulator
SUB | 2  | Subtract from the accumulator and save to it.
MUL | 3  | Multiply by the accumulator and save to it.
DIV | 4  | Divide accumulator and save to it.
MOD | 5  | Divide accumulator and save remainder to it.
EXP | 6  | Exponentiate the accumulator and save to it.
SHL | 7  | Shift accumulator left and save to it.
SHR | 8  | Shift accumulator right and save to it.
AND | 9  | Bitwise and with accumulator and save to it.
OR  | 10 | Bitwise or with accumulator and save to it.
XOR | 11 | Bitwise xor with accumulator and save to it.
GT  | 12 | Test if accumulator > , and store result to accumulator.
LT  | 13 | Test if accumulator <
GE  | 14 | Test if accumulator >=
LE  | 15 | Test if accumulator <=
EQ  | 16 | Test if accumulator ==
NE  | 17 | Test of accumulator !=
LD  | 18 | Load value at imm + addr to accumulator.
ST  | 19 | Store accumulator to memory location imm + addr.
STA | 20 | Store imm to accumulator + addr.
JMP | 21 | Jump to imm + addr.
JZ  | 22 | Performs a jump if the value in the accumulator is 0.
PSH | 23 | Push a value onto the stack.
POP | 24 | Pop a value off of the stack.
HLT | 63 | Halt the computer.

## Labels

A label is a destination for jump instructions. Internally the label is the absolute PC address, and it can be used as any other number.

## Directives

There are currently 4 types of compiler directives, the entry, var, ram_start, and peripheral directives.

### Entry

The entry directive tells the assembler where the program starts. All other directives must come before it.

### Var

The var directive defines a variable. It essentially creates an alias from a vairiable name to an automatically determined memory address.

### Ram_start

The ram_start directive tells the assembler what address in the memory map corresponds to the beggining of RAM. The ram_start defaults to 513 if it is not specified. This directive should come before any variables are declared, since declaring a variable relies on this value.

### Peripheral

A peripheral is the same as a variable, except the address it points to is manually assigned. Like this: 
```
peripheral display 769
```
This is useful if any devices are added to the computers memory map.