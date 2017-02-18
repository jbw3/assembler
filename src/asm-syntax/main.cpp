#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "InstructionSet.h"
#include "InstructionSetRegister.h"
#include "SyntaxGenerator.h"

using namespace std;

int main(int argc, const char* argv[])
{
    if (argc < 2)
    {
        cout << "No instruction set was given.\n";
        return 1;
    }
    else if (argc > 3)
    {
        cout << "Too many arguments were given.\n";
        return 1;
    }

    // get instruction set
    string iSetName = argv[1];
    const InstructionSet* iSet = InstructionSetRegister::getInstance().getInstructionSet(iSetName);
    if (iSet == nullptr)
    {
        cout << "\"" << iSetName << "\" is not a known instruction set.\n";
        return 1;
    }

    // get output stream
    ostream* os = nullptr;
    fstream file;
    if (argc == 3)
    {
        string filename = argv[2];
        file.open(filename, ios::out);
        os = &file;
    }
    else
    {
        os = &cout;
    }

    SyntaxGenerator generator;
    generator.generate(*os, iSet);

    if (argc == 3)
    {
        file.close();
    }

    return 0;
}
