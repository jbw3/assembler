#pragma once

#include "InstructionSet.h"

namespace machI {

    const InstructionType regImm
    { 
        6, 0,
        {
            {Argument::eRegister, 5, 6},             
            {Argument::eImmediate, 21, 11} 
        }
    };

    const InstructionSet ISET("machI", 32, 
        {
            { "r", 0, 31, 0b00000 }
        },
        {
            { "mov", 0x08, regImm }
        }
    );
}