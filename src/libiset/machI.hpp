#pragma once

#include "InstructionSet.h"

namespace machI {

    const unsigned int OpcodeSize = 6;
    const unsigned int OpcodeOffset = 0;

    const InstructionType regImm21Type { 6, 5, 21 };
    const InstructionType regsImm16Type { 6, 5, 5, 16 };
    const InstructionType imm26Type{ 6, 26 };

    const std::initializer_list<Argument> regArgs =
    {
        { Argument::eRegister, 1 },        
    };

    const std::initializer_list<Argument> regRegArgs = 
    {
        { Argument::eRegister, 1 },
        { Argument::eRegister, 2 },
    };

    const std::initializer_list<Argument> regImm21Args =
    {
        { Argument::eRegister, 1 },
        { Argument::eImmediate, /*index*/ 2, /*signed*/ true },
    };

    const std::initializer_list<Argument> regImm16Args =
    {
        { Argument::eRegister, 1 },
        { Argument::eImmediate, /*index*/ 3, /*signed*/ true },
    };

    const std::initializer_list<Argument> imm26Args =
    {
        { Argument::eImmediate, /*index*/ 1, /*isSigned*/ true, /*rightShift*/ 2, /*isRelativeAddress*/ true }
    };

    const InstructionSet ISET("machI", 32, Endianness::Big,
        {
            { "r", 0, 31, 0b00000 }
        },
        {
            { "movr", regsImm16Type,    {0x05}, regRegArgs },       // move rs into rd
            { "xor", regImm21Type,      {0x07}, regImm21Args },     // xor rd and imm21
            { "mov", regImm21Type,      {0x08}, regImm21Args },     // move imm21 to rd
            { "add", regImm21Type,      {0x09}, regImm21Args },     // add rd and imm21
            { "jmpf", regImm21Type,     {0x0A}, regArgs },          // jump to address in rd
            { "jmp", imm26Type,         {0x0B}, imm26Args },        // jump to relative address (imm26<<2)
            { "out", regImm21Type,      {0x11}, regImm21Args },     // output a word from rd into I/O port imm21
            { "jne", regImm21Type,      {0x12}, regArgs },          // jump to address in rd if ZF != 1
            { "movh", regsImm16Type,    {0x16}, regImm16Args },     // move imm16 to high hw of rd
            { "movl", regsImm16Type,    {0x18}, regImm16Args },     // move imm16 to low hw of rd
            { "cmp", regsImm16Type,     {0x19}, regRegArgs },       // compare rd and rs, set flags
        }
    );
}