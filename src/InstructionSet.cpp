#include "InstructionSet.h"
#include "utils.h"

using namespace std;

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
    mnemonic(toUpper(mnemonic)),
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
    name(toUpper(name)),
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
    Register reg(name, code);
    registers.insert({reg.getName(), reg});
}

RegisterSet::RegisterSet(const std::string& prefix, unsigned int startNum, unsigned int endNum, std::uint64_t startCode)
{
    std::string name;
    std::uint64_t code = startCode;
    for (unsigned int i = startNum; i <= endNum; ++i)
    {
        name = prefix + std::to_string(i);

        Register reg(name, code);
        registers.insert({reg.getName(), reg});

        ++code;
    }
}

std::map<std::string, Register> RegisterSet::getRegisters() const
{
    return registers;
}

map<string, const InstructionSet*> InstructionSet::allInstructionSets;

const InstructionSet* InstructionSet::getInstructionSet(const string& name)
{
    auto iter = allInstructionSets.find(name);
    if (iter == allInstructionSets.end())
    {
        return nullptr;
    }
    else
    {
        return iter->second;
    }
}

InstructionSet::InstructionSet(const string& name, unsigned int wordSize, initializer_list<RegisterSet> registerList, initializer_list<Instruction> instructionList) :
    name(name),
    wordSize(wordSize)
{
    for (const RegisterSet& regSet : registerList)
    {
        const map<string, Register>& regs = regSet.getRegisters();
        registers.insert(regs.cbegin(), regs.cend());
    }

    for (const Instruction& inst : instructionList)
    {
        instructions.insert({inst.getMnemonic(), inst});
    }

    // add self to map of instruction sets
    allInstructionSets.insert({name, this});
}

string InstructionSet::getName() const
{
    return name;
}

unsigned int InstructionSet::getWordSize() const
{
    return wordSize;
}

map<string, Register> InstructionSet::getRegisters() const
{
    return registers;
}

map<string, Instruction> InstructionSet::getInstructions() const
{
    return instructions;
}
