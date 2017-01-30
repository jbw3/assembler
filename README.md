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
Immediate values may only be integers greater than or equal to zero and may be in decimal or hexadecimal format.
```
MOV R2, 0xD2
ADDI R0, 42
```


## Adding an Instruction Set

An example instruction set file is in the doc directory.
An instruction set may be added by simply adding its .cpp file to the makefile to compile it into the assembler.
