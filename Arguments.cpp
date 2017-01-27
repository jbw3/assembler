#include <fstream>
#include <iostream>

#include "Arguments.h"
#include "InstructionSet8Bit.h"
#include "TextCodeGenerator.h"

using namespace std;

Arguments::Arguments() :
    instructionSet(ISET_8_BIT),
    codeGenerator(nullptr),
    is(nullptr)
{
}

Arguments::~Arguments()
{
    delete codeGenerator;
    if (is != &cin)
    {
        delete is;
    }
}

bool Arguments::parseArgs(int argc, const char* argv[])
{
    bool ok = true;

    if (argc > 2)
    {
        ok = false;
        cout << "Error: Expected 1 argument, got " << (argc - 1) << ".\n";
    }
    else if (argc == 2)
    {
        const char* filename = argv[1];

        fstream* inFile = new fstream;
        inFile->open(filename, ios_base::in);

        is = inFile;
        if (is->fail())
        {
            ok = false;
            cout << "Error: Could not open file \"" << filename << "\".\n";
        }
    }
    else
    {
        is = &cin;
    }

    codeGenerator = new TextCodeGenerator(cout);

    return ok;
}
