#include "InstructionSet.h"
#include "InstructionSetRegister.h"
#include "Mips32.hpp"
#include "W8.hpp"
#include "W16.hpp"
#include "machI.hpp"

using namespace std;

InstructionSetRegister& InstructionSetRegister::getInstance()
{
    static InstructionSetRegister instance;
    return instance;
}

InstructionSetRegister::InstructionSetRegister()
{
    addInstructionSet(Mips32::ISET);
    addInstructionSet(W8::ISET);
    addInstructionSet(W16::ISET);
    addInstructionSet(machI::ISET);
}

vector<string> InstructionSetRegister::getInstructionSetNames() const
{
    vector<string> names;
    names.reserve(instructionSets.size());

    for (auto pair : instructionSets)
    {
        names.push_back(pair.first);
    }

    return names;
}

const InstructionSet* InstructionSetRegister::getInstructionSet(const string& name) const
{
    auto iter = instructionSets.find(name);
    if (iter == instructionSets.end())
    {
        return nullptr;
    }
    else
    {
        return iter->second;
    }
}

void InstructionSetRegister::addInstructionSet(const InstructionSet& iSet)
{
    instructionSets.insert({iSet.getName(), &iSet});
}
