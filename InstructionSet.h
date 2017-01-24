#ifndef INSTRUCTION_SET_H_
#define INSTRUCTION_SET_H_

#include <cstdint>
#include <string>
#include <vector>

class Argument
{
public:
    enum EType
    {
        eRegister,
        eImmediate
    };

    Argument(EType type);

    EType getType() const;

private:
    EType type;
};

class Instruction
{
public:
    Instruction(std::string mnemonic, std::uint64_t opCode, const std::vector<Argument>& arguments);

private:
    std::string mnemonic;
    std::uint64_t opCode;
    std::vector<Argument> arguments;
};

class InstructionSet
{
public:
    InstructionSet(unsigned int bitSize, const std::vector<Instruction>& instructions);

    unsigned int getBitSize() const;

    std::vector<Instruction> getInstructions() const;

private:
    unsigned int bitSize;
    std::vector<Instruction> instructions;
};

#endif // INSTRUCTION_SET_H_
