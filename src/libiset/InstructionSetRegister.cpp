#include "InstructionSet.h"
#include "InstructionSetRegister.h"
#include "W8.hpp"
#include "W16.hpp"

using namespace std;

InstructionSetRegister& InstructionSetRegister::getInstance()
{
    static InstructionSetRegister instance;
    return instance;
}

InstructionSetRegister::InstructionSetRegister()
{
    instructionSets.insert({W8::ISET.getName(), &W8::ISET});
    instructionSets.insert({W16::ISET.getName(), &W16::ISET});
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
