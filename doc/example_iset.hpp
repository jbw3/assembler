////////////////////////////////////////////////////////////
// Example file for creating an instruction set.
////////////////////////////////////////////////////////////

#include "InstructionSet.h"

namespace Example
{

////////////////////////////////////////////////////////////
// Define instruction types here.
// An instruction type take 3 parameters. The first 2 are
// required while the third is optional:
// 1. Op code size in bits
// 2. Op code offset (shift) in bits
// 3. Argument list
////////////////////////////////////////////////////////////

// no-arg type
const InstructionType NType{8, 8};

// Register-Register type
const InstructionType RRType{8, 8, {{Argument::eRegister, 4, 4}, {Argument::eRegister, 4, 0}}};

// Register-Immediate type
const InstructionType RIType{4, 12, {{Argument::eRegister, 4, 8}, {Argument::eImmediate, 8, 0}}};

// Register-Register-Immediate type
const InstructionType RRIType{4, 12, {{Argument::eRegister, 4, 8}, {Argument::eRegister, 4, 4}, {Argument::eImmediate, 4, 0}}};

////////////////////////////////////////////////////////////
// Define instruction set here.
////////////////////////////////////////////////////////////

const InstructionSet ISET(
    // Instruction set name.
    // This is used with the assembler's -i command line option
    // to specify the instruction set to use.
    "Example",

    // Word size in bits
    16,

    // Define registers here. Registers can be defined
    // in two ways:
    {
        // 1. Single register (name, code)
        // The below defines a register named SP with code 0.
        { "SP", 0 },

        // 2. Register range (prefix, start, end, code start)
        // The below defines a range of registers
        // named R0, R1, ..., R7 with codes 1 to 8:
        { "R", 0, 7, 1 }
    },

    // Define instructions here. Three parameters are
    // required for each instruction:
    // 1. Mnemonic
    // 2. Op code
    // 3. Instruction type (defined above)
    {
        { "NOP",  0b00000, NType  },  // no operation
        { "MOV",  0b00001, RRType },  // move register
        { "NOT",  0b00100, RRType },  // not
        { "ADD",  0b01000, RRType },  // add
        { "ADDI", 0b01000, RIType },  // add immediate
        { "SUB",  0b01001, RRType },  // subtract
    }
);

}
