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
    Instruction(const std::string& mnemonic, std::uint64_t opCode, const std::vector<Argument>& arguments);

private:
    std::string mnemonic;
    std::uint64_t opCode;
    std::vector<Argument> arguments;
};

class Register
{
public:
    Register(const std::string& name, std::uint64_t byteCode);

private:
    std::string name;
    std::uint64_t byteCode;
};

class RegisterRange
{
public:
    RegisterRange(const Register& reg);

    RegisterRange(const std::string& prefix, unsigned int startNum, unsigned int endNum, std::uint64_t startByteCode);

    std::vector<Register> getRegisters() const;

private:
    std::vector<Register> registers;
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
