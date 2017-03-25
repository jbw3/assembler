#include "InstructionSet.h"
#include "utils.h"

using namespace std;

FieldType::FieldType(int index) :
    instType(nullptr),
    index(index)
{}

int FieldType::getIndex() const
{
    return index;
}

unsigned int FieldType::getFieldSize() const
{
    return instType->getFieldSize(index);
}

unsigned int FieldType::getFieldOffset() const
{
    return instType->getFieldOffset(index);
}

Code::Code(uint64_t value, int index) :
    FieldType(index),
    value(value)
{}

uint64_t Code::getValue() const
{
    return value;
}

Argument::Argument(EType type, bool isSigned, unsigned int shift, int index) :
    FieldType(index),
    type(type),
    isSigned(isSigned),
    shift(shift)
{}

Argument::EType Argument::getType() const
{
    return type;
}

bool Argument::getIsSigned() const
{
    return isSigned;
}

unsigned int Argument::getShift() const
{
    return shift;
}

InstructionType::InstructionType(std::initializer_list<unsigned int> fieldSizes) :
    fieldSizes(fieldSizes)
{}

unsigned int InstructionType::getFieldSize(int index) const
{
    return fieldSizes.at(index);
}

unsigned int InstructionType::getFieldOffset(int index) const
{
    unsigned int offset = 0;
    for (size_t i = index + 1; i < fieldSizes.size(); ++i)
    {
        offset += fieldSizes[i];
    }

    return offset;
}

Instruction::Instruction(const string& mnemonic, const InstructionType& type, initializer_list<Code> codeList, initializer_list<Argument> argumentList) :
    mnemonic(toUpper(mnemonic)),
    type(type)
{
    int index = 0;
    for (Code code : codeList)
    {
        code.instType = &type;
        if (code.index < 0)
        {
            code.index = index;
        }

        codes.push_back(code);
        index = code.index + 1;
    }

    index = 0;
    for (Argument arg : argumentList)
    {
        arg.instType = &type;
        if (arg.index < 0)
        {
            arg.index = index;
        }

        arguments.push_back(arg);
        index = arg.index + 1;
    }
}

string Instruction::getMnemonic() const
{
    return mnemonic;
}

const InstructionType& Instruction::getType() const
{
    return type;
}

const vector<Code>& Instruction::getCodes() const
{
    return codes;
}

const vector<Argument>& Instruction::getArguments() const
{
    return arguments;
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
