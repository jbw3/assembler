#include "InstructionSet.h"

Argument::Argument(EType type, unsigned int size, unsigned int offset) :
    type(type),
    size(size),
    offset(offset)
{}

Argument::EType Argument::getType() const
{
    return type;
}

unsigned int Argument::getSize() const
{
    return size;
}

unsigned int Argument::getOffset() const
{
    return offset;
}

InstructionType::InstructionType(unsigned int opCodeSize, unsigned int opCodeOffset, std::initializer_list<Argument> arguments) :
    opCodeSize(opCodeSize),
    opCodeOffset(opCodeOffset),
    arguments(arguments)
{}

unsigned int InstructionType::getOpCodeSize() const
{
    return opCodeSize;
}

unsigned int InstructionType::getOpCodeOffset() const
{
    return opCodeOffset;
}

std::vector<Argument> InstructionType::getArguments() const
{
    return arguments;
}

Instruction::Instruction(const std::string& mnemonic, std::uint64_t opCode, const InstructionType& type) :
    mnemonic(mnemonic),
    opCode(opCode),
    type(type)
{}

std::string Instruction::getMnemonic() const
{
    return mnemonic;
}

std::uint64_t Instruction::getOpCode() const
{
    return opCode;
}

const InstructionType& Instruction::getType() const
{
    return type;
}

Register::Register(const std::string& name, std::uint64_t code) :
    name(name),
    code(code)
{}

std::string Register::getName() const
{
    return name;
}

std::uint64_t Register::getCode() const
{
    return code;
}

RegisterSet::RegisterSet(const std::string& name, std::uint64_t code)
{
    registers.insert({name, Register(name, code)});
}

RegisterSet::RegisterSet(const std::string& prefix, unsigned int startNum, unsigned int endNum, std::uint64_t startCode)
{
    std::string name;
    std::uint64_t code = startCode;
    for (unsigned int i = startNum; i <= endNum; ++i)
    {
        name = prefix + std::to_string(i);
        registers.insert({name, Register(name, code)});
        ++code;
    }
}

std::map<std::string, Register> RegisterSet::getRegisters() const
{
    return registers;
}

InstructionSet::InstructionSet(unsigned int wordSize, std::initializer_list<RegisterSet> registerList, std::initializer_list<Instruction> instructionList) :
    wordSize(wordSize)
{
    for (const RegisterSet& regSet : registerList)
    {
        const std::map<std::string, Register>& regs = regSet.getRegisters();
        registers.insert(regs.cbegin(), regs.cend());
    }

    for (const Instruction& inst : instructionList)
    {
        instructions.insert({inst.getMnemonic(), inst});
    }
}

unsigned int InstructionSet::getWordSize() const
{
    return wordSize;
}

std::map<std::string, Register> InstructionSet::getRegisters() const
{
    return registers;
}

std::map<std::string, Instruction> InstructionSet::getInstructions() const
{
    return instructions;
}