#pragma once

#include "InstructionSet.h"

namespace machI {

    const unsigned int OpcodeSize = 6;
    const unsigned int OpcodeOffset = 0;

    const InstructionType regImm21
    { 
        OpcodeSize, OpcodeOffset,
        {
            {Argument::eRegister, 5, 6},             
            {Argument::eImmediate, 21, 11} 
        }
    };

    const InstructionType regImm16
    {
        OpcodeSize, OpcodeOffset,
        {
            { Argument::eRegister, 5, 6 },
            { Argument::eImmediate, 16, 16 }
        }
    };

    const InstructionType reg
    {
        OpcodeSize, OpcodeOffset,
        {
            { Argument::eRegister, 5, 6 }            
        }
    };

    const InstructionType regReg
    {
        OpcodeSize, OpcodeOffset,
        {
            { Argument::eRegister, 5, 6 },
            { Argument::eRegister, 5, 11 }
        }
    };

    const InstructionSet ISET("machI", 32, 
        {
            { "r", 0, 31, 0b00000 }
        },
        {
            { "movr", 0x05, regReg },       // move rs into rd
            { "xor", 0x07, regImm21 },      // xor rd and imm21
            { "mov", 0x08, regImm21 },      // move imm21 to rd
            { "add", 0x09, regImm21 },      // add rd and imm21
            { "jmp", 0x0A, reg },           // jump to address in rd
            { "out", 0x11, regImm21 },      // output a word from rd into I/O port imm21
            { "jne", 0x12, reg },           // jump to address in rd if ZF != 1
            { "movh", 0x16, regImm16 },     // move imm16 to high hw of rd
            { "movl", 0x18, regImm16 },     // move imm16 to low hw of rd
            { "cmp", 0x19, regReg },        // compare rd and rs, set flags
        }
    );
}