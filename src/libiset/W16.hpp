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
// const InstructionType JType{4, 12, {{Argument::eImmediate, 12, 0, false, 1}}};
const InstructionType JType{4, 12};

// register argument
const Argument RegArg{ Argument::eRegister };

// immediate argument
const Argument ImmArg{ Argument::eImmediate, true };

// jump immediate argument
const Argument JImmArg{ Argument::eImmediate, false, 1 };

const std::initializer_list<Argument> RRArgs = {RegArg, RegArg};

const std::initializer_list<Argument> RIArgs = {RegArg, ImmArg};

const InstructionSet ISET(
    // name
    "W16",

    // word size
    16,

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
