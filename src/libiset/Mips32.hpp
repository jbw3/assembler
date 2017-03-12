#include "InstructionSet.h"

namespace Mips32
{

// R-type
const InstructionType RType{6, 5, 5, 5, 5, 6};

// I-type
const InstructionType IType{6, 5, 5, 16};

// J-Type
const InstructionType JType{6, 26};

const std::initializer_list<Argument> RArgs = {{Argument::eRegister, 3}, {Argument::eRegister, 1}, {Argument::eRegister, 2}};

const std::initializer_list<Argument> RShiftArgs = {{Argument::eRegister, 3}, {Argument::eRegister, 2}, {Argument::eImmediate, false, 0, 4}};

const std::initializer_list<Argument> IArgsSigned = {{Argument::eRegister, 2}, {Argument::eRegister, 1}, {Argument::eImmediate, true, 0, 3}};

const std::initializer_list<Argument> IArgsUnsigned = {{Argument::eRegister, 2}, {Argument::eRegister, 1}, {Argument::eImmediate, false, 0, 3}};

const std::initializer_list<Argument> JArgs = {{Argument::eImmediate, false, 2, 1}};

const std::initializer_list<Argument> BranchRRIArgs = {{Argument::eRegister, 1}, {Argument::eRegister}, {Argument::eImmediate, false, 0}};

const std::initializer_list<Argument> BranchRIArgs = {{Argument::eRegister, 1}, {Argument::eImmediate, false, 0, 2}};

const InstructionSet ISET(
    // name
    "MIPS32",

    // word size
    32,

    // registers
    {
        { "R", 0, 31, 0 },
        { "ZERO", 0 },
        { "AT", 1 },
        { "V", 0, 1, 2 },
        { "A", 0, 3, 4 },
        { "T", 0, 7, 8 },
        { "S", 0, 7, 16 },
        { "T", 8, 9, 24 },
        { "K", 0, 1, 26 },
        { "GP", 28 },
        { "SP", 29 },
        { "FP", 30 },
        { "RA", 31 }
    },

    // instructions
    {
        { "ADD",   RType, {0x00, {0x20, 5}}, RArgs },
        { "ADDI",  IType, {0x08}, IArgsSigned },
        { "ADDIU", IType, {0x09}, IArgsSigned },
        { "ADDU",  RType, {0x00, {0x21, 5}}, RArgs },
        { "AND",   RType, {0x00, {0x24, 5}}, RArgs },
        { "ANDI",  IType, {0x0c}, IArgsUnsigned },
        { "BEQ",   IType, {0x04}, BranchRRIArgs },          // not correct (need relative immediate value)
        { "BGEZ",  IType, {0x01, {0x1, 2}}, BranchRIArgs }, // not correct (need relative immediate value)
        { "BGTZ",  IType, {0x07, {0x0, 2}}, BranchRIArgs }, // not correct (need relative immediate value)
        { "BLEZ",  IType, {0x06, {0x0, 2}}, BranchRIArgs }, // not correct (need relative immediate value)
        { "BLTZ",  IType, {0x01, {0x0, 2}}, BranchRIArgs }, // not correct (need relative immediate value)
        { "BNE",   IType, {0x05}, BranchRRIArgs },          // not correct (need relative immediate value)
        { "J",     JType, {0x02}, JArgs },
        { "JAL",   JType, {0x03}, JArgs },
        { "JR",    RType, {0x00, {0x08, 5}}, RArgs },
        { "LUI",   IType, {0x0f}, IArgsUnsigned },
        { "LW",    IType, {0x23}, IArgsUnsigned },          // not correct (need optional immediate offset)
        { "NOR",   RType, {0x00, {0x27, 5}}, RArgs },
        { "OR",    RType, {0x00, {0x25, 5}}, RArgs },
        { "ORI",   IType, {0x0d}, IArgsUnsigned },
        { "SLL",   RType, {0x00, {0x00, 5}}, RShiftArgs },
        { "SLLV",  RType, {0x00, {0x04, 5}}, RArgs },
        { "SRA",   RType, {0x00, {0x03, 5}}, RShiftArgs },
        { "SRAV",  RType, {0x00, {0x07, 5}}, RArgs },
        { "SRL",   RType, {0x00, {0x02, 5}}, RShiftArgs },
        { "SRLV",  RType, {0x00, {0x06, 5}}, RArgs },
        { "SUB",   RType, {0x00, {0x22, 5}}, RArgs },
        { "SUBU",  RType, {0x00, {0x23, 5}}, RArgs },
        { "SW",    IType, {0x2b}, IArgsUnsigned },          // not correct (need optional immediate offset)
        { "XOR",   RType, {0x00, {0x26, 5}}, RArgs },
        { "XORI",  IType, {0x0e}, IArgsUnsigned }
    }
);

} // namespace Mips32
