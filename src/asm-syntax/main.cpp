#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "InstructionSet.h"
#include "InstructionSetRegister.h"
#include "SyntaxGenerator.h"

using namespace std;

constexpr char HELP_MESSAGE[] = R"(asm-syntax <instruction set> [output file] [-h]

Generate a text editor syntax file for an instruction set. If no output file is
specified, stdout is used.

Example:
  asm-syntax W8 W8.tmLanguage
)";

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
    else if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
    {
        cout << HELP_MESSAGE;
        return 0;
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
