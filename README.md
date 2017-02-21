# Assembler


## Quick Start

Run `asm` as follows:
```
asm in.s -i W8 -o out.txt
```

Option `-i` is used to specify the instruction set, and option `-o` is the output file.
Option `-h` may be used to display a help message.


## Assembly Syntax

The syntax is case insensitive.
Comments start with an octothorpe (#) and continue to the end of the line.
```
# This is a comment
```

There may be no more than one instruction per line.
Each instruction starts with its mnemonic and is followed by zero or more arguments.
Arguments are separated by commas.
```
NOP
ADD R0
MOV R0, 7
SUB R0, R1, R2
```

Arguments may be either register names or immediate values.
Immediate values may only be integers greater than or equal to zero and may be in binary, octal, decimal, or hexadecimal format.
```
MOV R5, 0b101  # binary
MOV R10, 0o777 # octal
ADDI R0, 42    # decimal
MOV R2, 0xD2   # hexadecimal
```

A label may be placed at the beginning of a line followed by `:`.
No more than one label may be placed on a line.
```
label1: MOV R1, R2
label2:
    ADD R1, R3
```

A constant may be defined with the assignment operator (`=`).
It may be assigned to a numeric value or a previously defined constant or label.
No more than one constant may be defined on a line.
```
x = 2
y = 0xABC

label:
    ADDI R1, x

z = label
```


## Adding an Instruction Set

Perform the following steps to add an instruction set:

1. Create an `InstructionSet` instance defining the instruction set.
An example file is in the doc directory

2. Add the instruction set's .hpp file to the `libiset` library's makefile

3. Add the instruction set to the map in `InstructionSetRegister`'s constructor
