#include "InstructionSet.h"

namespace W16
{

// Register-Register type
const InstructionType RRType{8, 4, 4};

// Register-Immediate type
const InstructionType RIType{4, 4, 8};

// Register-Register-Immediate type
const InstructionType RRIType{4, 4, 4, 4};

// Jump type
const InstructionType JType{4, 12};

// immediate argument
const Argument ImmArg{ Argument::eImmediate, -1, true };

// jump immediate argument
const Argument JImmArg{ Argument::eImmediate, 1, false, 1 };

const std::initializer_list<Argument> RRArgs = {{ Argument::eRegister, 1 }, { Argument::eRegister }};

const std::initializer_list<Argument> RIArgs = {{ Argument::eRegister, 1 }, ImmArg};

const InstructionSet ISET(
    // name
    "W16",

    // word size
    16,

    // endianness
    Endianness::Big,

    // registers
    {
        { "R", 0, 15, 0b0000 }
    },

    // instructions
    {
        { "NOP",  RRType, {0x00}, },           // no operation
        { "MOV",  RRType, {0x01}, RRArgs },    // move
        { "NOT",  RRType, {0x02}, RRArgs },    // not
        { "ADDI", RIType, {0xb},  RIArgs },    // add immediate
        { "JMP",  JType,  {0xc},  {JImmArg} }  // jump
    }
);

} // namespace W16
