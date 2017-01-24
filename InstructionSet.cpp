#include "InstructionSet.h"

Argument::Argument(EType type) :
    type(type)
{}

Argument::EType Argument::getType() const
{
    return type;
}

Instruction::Instruction(std::string mnemonic, std::uint64_t opCode, const std::vector<Argument>& arguments) :
    mnemonic(mnemonic),
    opCode(opCode),
    arguments(arguments)
{}

InstructionSet::InstructionSet(unsigned int bitSize, const std::vector<Instruction>& instructions) :
    bitSize(bitSize),
    instructions(instructions)
{}

unsigned int InstructionSet::getBitSize() const
{
    return bitSize;
}

std::vector<Instruction> InstructionSet::getInstructions() const
{
    return instructions;
}
