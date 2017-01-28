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

    Argument(EType type, unsigned int size, unsigned int offset);

    EType getType() const;

private:
    EType type;
    unsigned int size;
    unsigned int offset;
};

class InstructionType
{
public:
    friend class Instruction;

    InstructionType(unsigned int opCodeSize, unsigned int opCodeOffset, std::initializer_list<Argument> arguments = {});

    unsigned int getOpCodeSize() const;

    unsigned int getOpCodeOffset() const;

    std::vector<Argument> getArguments() const;

private:
    unsigned int opCodeSize;
    unsigned int opCodeOffset;
    std::vector<Argument> arguments;
};

class Instruction
{
public:
    Instruction(const std::string& mnemonic, std::uint64_t opCode, const InstructionType& type);

    std::string getMnemonic() const;

    std::uint64_t getOpCode() const;

    std::vector<Argument> getArguments() const;

private:
    std::string mnemonic;
    std::uint64_t opCode;
    const InstructionType& type;
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
