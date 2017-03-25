#ifndef INSTRUCTION_SET_H_
#define INSTRUCTION_SET_H_

#include <cstdint>
#include <initializer_list>
#include <map>
#include <string>
#include <vector>

/**
 * @brief A hard-coded value in an instruction
 */
class Code
{
public:
    friend class Instruction;

    Code(std::uint64_t value, int index = -1);

    std::uint64_t getValue() const;

    int getIndex() const;

private:
    std::uint64_t value;
    int index;
};

/**
 * @brief Configuration for an instruction argument
 */
class Argument
{
public:
    friend class Instruction;

    enum EType
    {
        eRegister,
        eImmediate
    };

    /**
     * @brief Constructor
     */
    Argument(EType type, bool isSigned = true, unsigned int shift = 0, int index = -1);

    /**
     * @brief Get the argument type (register or immediate)
     */
    EType getType() const;

    /**
     * @brief Get the index
     */
    int getIndex() const;

    /**
     * @brief Get whether or not this argument is a signed number
     * (only meaningful for immediate arguments)
     */
    bool getIsSigned() const;

    /**
     * @brief Shift an immediate argument to the right before
     * encoding it (only meaningful for immediate arguments)
     *
     * @details This may be useful for jump instructions. For
     * example, when jumping to a 32-bit aligned address, the
     * 2 least significant bits are unnecessary. Thus, a shift
     * of 2 will allow 2 more most significant bits in the
     * address.
     */
    unsigned int getShift() const;

private:
    EType type;
    int index;
    bool isSigned;
    unsigned int shift;
};

class InstructionType
{
public:
    friend class Instruction;

    InstructionType(std::initializer_list<unsigned int> fieldSizes);

    unsigned int getFieldSize(int index) const;

    unsigned int getFieldOffset(int index) const;

private:
    std::vector<unsigned int> fieldSizes;
};

class Instruction
{
public:
    Instruction(const std::string& mnemonic, const InstructionType& type, std::initializer_list<Code> codeList, std::initializer_list<Argument> argumentList = {});

    std::string getMnemonic() const;

    const InstructionType& getType() const;

    const std::vector<Code>& getCodes() const;

    const std::vector<Argument>& getArguments() const;

private:
    std::string mnemonic;
    const InstructionType& type;
    std::vector<Code> codes;
    std::vector<Argument> arguments;
};

class Register
{
public:
    Register(const std::string& name, std::uint64_t code);

    std::string getName() const;

    std::uint64_t getCode() const;

private:
    std::string name;
    std::uint64_t code;
};

class RegisterSet
{
public:
    RegisterSet(const std::string& name, std::uint64_t code);

    RegisterSet(const std::string& prefix, unsigned int startNum, unsigned int endNum, std::uint64_t startCode);

    std::map<std::string, Register> getRegisters() const;

private:
    std::map<std::string, Register> registers;
};

class InstructionSet
{
public:
    InstructionSet(const std::string& name, unsigned int wordSize, std::initializer_list<RegisterSet> registerList, std::initializer_list<Instruction> instructionList);

    std::string getName() const;

    unsigned int getWordSize() const;

    std::map<std::string, Register> getRegisters() const;

    std::map<std::string, Instruction> getInstructions() const;

private:
    std::string name;
    unsigned int wordSize;
    std::map<std::string, Register> registers;
    std::map<std::string, Instruction> instructions;
};

#endif // INSTRUCTION_SET_H_
