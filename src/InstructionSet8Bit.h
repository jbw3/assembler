#ifndef INSTRUCTION_SET_8_BIT_H_
#define INSTRUCTION_SET_8_BIT_H_

#include "InstructionSet.h"

// immediate type
const InstructionType IType{5, 3, {{Argument::eImmediate, 3, 0}}};

// no-arg type
const InstructionType NType{5, 3};

// register type
const InstructionType RType{5, 3, {{Argument::eRegister, 3, 0}}};

const InstructionSet ISET_8_BIT(
    // word size
    8,

    // registers
    {
        { "R", 0, 7, 0b000 }
    },

    // instructions
    {
        { "NOP",  0b00000, NType },  // no operation
        { "MOV",  0b00001, RType },  // move register to R0
        { "MOVI", 0b00010, IType },  // move constant to R0
        { "MVR0", 0b00011, RType },  // move R0 to register
        { "NOT",  0b00100, RType },  // not
        { "AND",  0b00101, RType },  // and
        { "OR",   0b00110, RType },  // or
        { "XOR",  0b00111, RType },  // xor
        { "ADD",  0b01000, RType },  // add
        { "SUB",  0b01001, RType },  // sub
        { "SHLL", 0b01010, RType },  // shift left logical
        { "SHRL", 0b01011, RType },  // shift right logical
        { "SHRA", 0b01100, RType },  // shift right arithmetic
        { "JMP",  0b01101, RType },  // jump
        { "BEZ",  0b01110, RType },  // branch equal zero
        { "BNEZ", 0b01111, RType },  // branch not equal zero
        { "BGTZ", 0b10000, RType },  // branch greater than zero
        { "BLTZ", 0b10001, RType },  // branch less than zero
    }
);

#endif // INSTRUCTION_SET_8_BIT_H_
