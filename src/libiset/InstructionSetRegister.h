#ifndef INSTRUCTION_SET_REGISTER_H_
#define INSTRUCTION_SET_REGISTER_H_

#include <map>
#include <string>
#include <vector>

class InstructionSet;

class InstructionSetRegister
{
public:
    static InstructionSetRegister& getInstance();

    std::vector<std::string> getInstructionSetNames() const;

    const InstructionSet* getInstructionSet(const std::string& name) const;

private:
    std::map<std::string, const InstructionSet*> instructionSets;

    InstructionSetRegister();

    ~InstructionSetRegister() = default;

    InstructionSetRegister(const InstructionSetRegister&) = default;

    InstructionSetRegister(InstructionSetRegister&&) = default;

    InstructionSetRegister& operator =(const InstructionSetRegister&) = default;

    InstructionSetRegister& operator =(InstructionSetRegister&&) = default;

    void addInstructionSet(const InstructionSet& iSet);
};

#endif // INSTRUCTION_SET_REGISTER_H_
