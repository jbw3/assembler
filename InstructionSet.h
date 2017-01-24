#ifndef INSTRUCTION_SET_H_
#define INSTRUCTION_SET_H_

#include <cstdint>
#include <initializer_list>
#include <map>
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
    Instruction(const std::string& mnemonic, std::uint64_t opCode, std::initializer_list<Argument> arguments = {});

    std::string getMnemonic() const;

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

class RegisterSet
{
public:
    RegisterSet(const std::string& name, std::uint64_t byteCode);

    RegisterSet(const std::string& prefix, unsigned int startNum, unsigned int endNum, std::uint64_t startByteCode);

    std::map<std::string, Register> getRegisters() const;

private:
    std::map<std::string, Register> registers;
};

class InstructionSet
{
public:
    InstructionSet(unsigned int wordSize, std::initializer_list<RegisterSet> registerList, std::initializer_list<Instruction> instructionList);

    unsigned int getWordSize() const;

    std::map<std::string, Register> getRegisters() const;

    std::map<std::string, Instruction> getInstructions() const;

private:
    unsigned int wordSize;
    std::map<std::string, Register> registers;
    std::map<std::string, Instruction> instructions;
};

#endif // INSTRUCTION_SET_H_
