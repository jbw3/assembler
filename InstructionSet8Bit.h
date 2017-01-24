#ifndef INSTRUCTION_SET_8_BIT_H_
#define INSTRUCTION_SET_8_BIT_H_

#include "InstructionSet.h"

InstructionSet IS_8_BIT(
    // word size
    8,

    // instructions
    {
        { "NOP",  0b00000 },                         // no operation
        { "MOV",  0b00001, {Argument::eRegister} },  // move register to R0
        { "MOVI", 0b00010, {Argument::eImmediate} }, // move constant to R0
        { "MVR0", 0b00011, {Argument::eRegister} },  // move R0 to register
        { "NOT",  0b00100, {Argument::eRegister} },  // not
        { "AND",  0b00101, {Argument::eRegister} },  // and
        { "OR",   0b00110, {Argument::eRegister} },  // or
        { "XOR",  0b00111, {Argument::eRegister} },  // xor
        { "ADD",  0b01000, {Argument::eRegister} },  // add
        { "SUB",  0b01001, {Argument::eRegister} },  // sub
        { "SHLL", 0b01010, {Argument::eRegister} },  // shift left logical
        { "SHRL", 0b01011, {Argument::eRegister} },  // shift right logical
        { "SHRA", 0b01100, {Argument::eRegister} },  // shift right arithmetic
        { "JMP",  0b01101, {Argument::eRegister} },  // jump
        { "BEZ",  0b01110, {Argument::eRegister} },  // branch equal zero
        { "BNEZ", 0b01111, {Argument::eRegister} },  // branch not equal zero
        { "BGTZ", 0b10000, {Argument::eRegister} },  // branch greater than zero
        { "BLTZ", 0b10001, {Argument::eRegister} },  // branch less than zero
    }
);

#endif // INSTRUCTION_SET_8_BIT_H_
