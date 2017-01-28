#include <cstring>
#include <fstream>
#include <iostream>

#include "Arguments.h"
#include "InstructionSet8Bit.h"
#include "TextCodeGenerator.h"

using namespace std;

Arguments::Arguments() :
    instructionSet(ISET_8_BIT),
    codeGenerator(nullptr),
    is(nullptr),
    os(nullptr)
{
}

Arguments::~Arguments()
{
    delete codeGenerator;

    if (is != &cin)
    {
        delete is;
    }

    if (os != &cout)
    {
        delete os;
    }
}

bool Arguments::parse(int argc, const char* argv[])
{
    for (int idx = 1; idx < argc; ++idx)
    {
        bool ok = parseNextArgs(idx, argc, argv);
        if (!ok)
        {
            return false;
        }
    }

    // choose default values if not specified by user
    if (is == nullptr)
    {
        is = &cin;
    }
    if (os == nullptr)
    {
        os = &cout;
    }

    codeGenerator = new TextCodeGenerator(*os);

    return true;
}

bool Arguments::parseNextArgs(int& idx, int argc, const char* argv[])
{
    const char* arg = argv[idx];

    if (std::strcmp(arg, "-o") == 0 || std::strcmp(arg, "--output") == 0)
    {
        if (idx + 1 >= argc)
        {
            cout << "Error: Expected an argument after " << arg << ".\n";
            return false;
        }
        else if (os != nullptr)
        {
            cout << "Error: Argument " << arg << " was given more than once.\n";
            return false;
        }
        else
        {
            const char* filename = argv[idx + 1];

            fstream* outFile = new fstream;
            outFile->open(filename, ios_base::out);

            os = outFile;
            if (os->fail())
            {
                cout << "Error: Could not open file \"" << filename << "\".\n";
                return false;
            }

            // increment index
            ++idx;
        }
    }
    else
    {
        if (is != nullptr)
        {
            cout << "Error: More than one input file cannot be given.\n";
            return false;
        }
        else
        {
            fstream* inFile = new fstream;
            inFile->open(arg, ios_base::in);

            is = inFile;
            if (is->fail())
            {
                cout << "Error: Could not open file \"" << arg << "\".\n";
                return false;
            }
        }
    }

    return true;
}
