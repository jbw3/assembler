#include <functional>
#include <iostream>

#include "Checker.h"
#include "InstructionSet.h"
#include "InstructionSetRegister.h"
#include "utils.h"

using namespace std;

Checker::Checker(const string& iSetName) :
    iSetName(iSetName),
    errors(false)
{
    iSet = InstructionSetRegister::getInstance().getInstructionSet(iSetName);
}

void Checker::check()
{
    // check if instruction set exists
    if (iSet == nullptr)
    {
        logError("\"" + iSetName + "\" is not a known instruction set.");
        return;
    }

    // check if word size is a multiple of 8
    if (iSet->getWordSize() % 8 != 0)
    {
        logError("Word size is not a multiple of 8.");
    }

    // check if instructions have been defined
    if (iSet->getInstructions().size() == 0)
    {
        logError("No instructions are defined.");
    }

    string instName;
    for (auto pair : iSet->getInstructions())
    {
        const Instruction& inst = pair.second;
        instName = inst.getMnemonic();
        const InstructionType& type = inst.getType();

        // check if instruction names are valid
        if (!isIdentifierString(instName))
        {
            logError("\"" + instName + "\" is not a valid instruction name.");
        }

        // check if op code will be truncated
        uint64_t opCode = inst.getOpCode();
        uint64_t opCodeMask = bitMask(type.getOpCodeSize());
        if ((opCodeMask & opCode) != opCode)
        {
            logError(instName + " op code will be truncated.");
        }
    }

    // check if registers have been defined
    if (iSet->getRegisters().size() == 0)
    {
        logError("No registers are defined.");
    }

    // check if register names are valid
    string regName;
    for (auto pair : iSet->getRegisters())
    {
        regName = pair.first;
        if (!isIdentifierString(regName))
        {
            logError("\"" + regName + "\" is not a valid register name.");
        }
    }

    if (!errors)
    {
        cout << "All checks passed!\n";
    }
}

void Checker::logError(const std::string& errorMsg)
{
    cerr << "ERROR: " << errorMsg << "\n";
    errors = true;
}
