#ifndef W16_H_
#define W16_H_

#include "InstructionSet.h"

namespace W16
{

// no-arg type
const InstructionType NType{5, 11};

// Register-Register type
const InstructionType RRType{8, 8, {{Argument::eRegister, 4, 4}, {Argument::eRegister, 4, 0}}};

// Register-Immediate type
const InstructionType RIType{4, 12, {{Argument::eRegister, 4, 8}, {Argument::eImmediate, 8, 0}}};

// Register-Register-Immediate type
const InstructionType RRIType{4, 12, {{Argument::eRegister, 4, 8}, {Argument::eRegister, 4, 4}, {Argument::eImmediate, 4, 0}}};

// Jump type
const InstructionType JType{4, 12, {{Argument::eImmediate, 12, 0}}};

const InstructionSet ISET_W16(
    // word size
    16,

    // registers
    {
        { "R", 0, 15, 0b0000 }
    },

    // instructions
    {
        { "NOP",  0x00, NType   },  // no operation
        { "MOV",  0x01, RRType  },  // move
        { "NOT",  0x02, RRType  },  // not
        { "ADDI", 0x0b, RIType  },  // add immediate
    }
);

} // namespace W16

#endif // W16_H_