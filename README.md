# Assembler

## Syntax

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
Immediate values may only be integers greater than or equal to zero.
