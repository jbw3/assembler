#include "InstructionSet.h"

namespace W8
{

const InstructionType Type{5, 3};

// register argument
const Argument RegArg{ Argument::eRegister, 1 };

// immediate argument
const Argument ImmArg{ Argument::eImmediate, 1, false };

const InstructionSet ISET(
    // name
    "W8",

    // word size
    8,

    // endianness
    Endianness::Big,

    // registers
    {
        { "R", 0, 7, 0b000 }
    },

    // instructions
    {
        { "NOP",  Type, {0b00000} },            // no operation
        { "MOV",  Type, {0b00001}, {RegArg} },  // move register to R0
        { "MOVI", Type, {0b00010}, {ImmArg} },  // move constant to R0
        { "MVR0", Type, {0b00011}, {RegArg} },  // move R0 to register
        { "NOT",  Type, {0b00100}, {RegArg} },  // not
        { "AND",  Type, {0b00101}, {RegArg} },  // and
        { "OR",   Type, {0b00110}, {RegArg} },  // or
        { "XOR",  Type, {0b00111}, {RegArg} },  // xor
        { "ADD",  Type, {0b01000}, {RegArg} },  // add
        { "SUB",  Type, {0b01001}, {RegArg} },  // sub
        { "SHLL", Type, {0b01010}, {RegArg} },  // shift left logical
        { "SHRL", Type, {0b01011}, {RegArg} },  // shift right logical
        { "SHRA", Type, {0b01100}, {RegArg} },  // shift right arithmetic
        { "JMP",  Type, {0b01101}, {RegArg} },  // jump
        { "BEZ",  Type, {0b01110}, {RegArg} },  // branch equal zero
        { "BNEZ", Type, {0b01111}, {RegArg} },  // branch not equal zero
        { "BGTZ", Type, {0b10000}, {RegArg} },  // branch greater than zero
        { "BLTZ", Type, {0b10001}, {RegArg} },  // branch less than zero
    }
);

} // namespace W8
