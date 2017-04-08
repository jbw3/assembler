////////////////////////////////////////////////////////////
// Example file for creating an instruction set.
////////////////////////////////////////////////////////////

#include "InstructionSet.h"

namespace Example
{

////////////////////////////////////////////////////////////
// Define instruction types here. This is a list of field
// sizes.
////////////////////////////////////////////////////////////

// Register-Register type: 8-bit op code and 2 4-bit register fields
const InstructionType RRType{8, 4, 4};

// Register-Immediate type: 4-bit op code, 4-bit register field, and
// 8-bit immediate field
const InstructionType RIType{4, 4, 8};

// Register-Register-Immediate type: 4-bit op code, 2 4-bit register fields,
// and 4-bit immediate field
const InstructionType RRIType{4, 4, 4, 4};

////////////////////////////////////////////////////////////
// Define arguments here. An argument may either be a
// register or an immediate value. Each argument is mapped
// to a field in the type above with a zero based index.
////////////////////////////////////////////////////////////

// Two register arguments for RRType instructions.
const std::initializer_list<Argument> RRArgs = {
    // The first argument is mapped to field 1 in
    // the instruction type.
    { Argument::eRegister, 1 },

    // The second argument is not explicitly mapped
    // to a field, so it will be mapped to the field
    // after the argument above (i.e. 2).
    { Argument::eRegister }
};

// A register argument and an immediate argument
// for RIType instructions.
const std::initializer_list<Argument> RIArgs = {
    { Argument::eRegister, 1 },
    { Argument::eImmediate, -1, true }
};

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

    // endianness
    Endianness::Big,

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

    // Define instructions here. The following parameters are
    // specified for each instruction:
    // 1. Mnemonic
    // 2. Instruction type (defined above)
    // 3. Constant values (e.g. op code)
    // 4. Arguments (optional, defined above)
    {
        { "NOP",  RRType, {0b00000} },          // no operation
        { "MOV",  RRType, {0b00001}, RRArgs },  // move register
        { "NOT",  RRType, {0b00100}, RRArgs },  // not
        { "ADD",  RRType, {0b01000}, RRArgs },  // add
        { "ADDI", RIType, {0b01000}, RIArgs },  // add immediate
        { "SUB",  RRType, {0b01001}, RRArgs },  // subtract
    }
);

}
