#include "InstructionSet.h"

Argument::Argument(EType type) :
    type(type)
{}

Argument::EType Argument::getType() const
{
    return type;
}

Instruction::Instruction(const std::string& mnemonic, std::uint64_t opCode, std::initializer_list<Argument> arguments) :
    mnemonic(mnemonic),
    opCode(opCode),
    arguments(arguments)
{}

std::string Instruction::getMnemonic() const
{
    return mnemonic;
}

Register::Register(const std::string& name, std::uint64_t byteCode) :
    name(name),
    byteCode(byteCode)
{}

InstructionSet::InstructionSet(unsigned int wordSize, std::initializer_list<Instruction> instructionList) :
    wordSize(wordSize)
{
    for (const Instruction& inst : instructionList)
    {
        instructions.insert({inst.getMnemonic(), inst});
    }
}

unsigned int InstructionSet::getWordSize() const
{
    return wordSize;
}

std::map<std::string, Instruction> InstructionSet::getInstructions() const
{
    return instructions;
}
