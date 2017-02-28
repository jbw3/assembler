#ifndef INSTRUCTION_SET_H_
#define INSTRUCTION_SET_H_

#include <cstdint>
#include <initializer_list>
#include <map>
#include <string>
#include <vector>

/**
 * @brief Configuration for an instruction argument
 */
class Argument
{
public:
    enum EType
    {
        eRegister,
        eImmediate
    };

    /**
     * @brief Constructor
     */
    Argument(EType type, unsigned int size, unsigned int offset, bool isSigned = true, unsigned int shift = 0);

    /**
     * @brief Get the argument type (register or immediate)
     */
    EType getType() const;

    /**
     * @brief Get the number of bits the argument will occupy
     * in the instruction code
     */
    unsigned int getSize() const;

    /**
     * @brief Get the offset in bits of the argument in the
     * instruction code
     */
    unsigned int getOffset() const;

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
    unsigned int size;
    unsigned int offset;
    bool isSigned;
    unsigned int shift;
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

    const InstructionType& getType() const;

private:
    std::string mnemonic;
    std::uint64_t opCode;
    const InstructionType& type;
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
