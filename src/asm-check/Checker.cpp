#include <functional>
#include <iostream>

#include "Checker.h"
#include "InstructionSet.h"
#include "InstructionSetRegister.h"

using namespace std;

typedef bool (*CheckFunc)(string&);

Checker::Checker(const string& iSetName) :
    iSetName(iSetName)
{
    iSet = InstructionSetRegister::getInstance().getInstructionSet(iSetName);
}

void Checker::check()
{
    if (iSet == nullptr)
    {
        cerr << "ERROR: \"" << iSetName << "\" is not a known instruction set.\n";
        return;
    }

    // create checks
    vector<function<bool(string&)>> checks = {
        [this](string& errorMsg)
        {
            if (iSet->getWordSize() % 8 != 0)
            {
                errorMsg = "Word size is not a multiple of 8.";
                return false;
            }
            return true;
        },

        [this](string& errorMsg)
        {
            if (iSet->getInstructions().size() == 0)
            {
                errorMsg = "No instructions are defined.";
                return false;
            }
            return true;
        }
    };

    // run checks
    size_t numPassed = 0;
    string errorMsg;
    for (auto check : checks)
    {
        errorMsg.clear();

        bool passed = check(errorMsg);
        if (passed)
        {
            ++numPassed;
        }
        else
        {
            cerr << "ERROR: " << errorMsg << "\n";
        }
    }

    size_t totalChecks = checks.size();
    cout << numPassed << " of " << totalChecks << " check" << (totalChecks == 1 ? "" : "s") << " passed.\n";
}
