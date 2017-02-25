#include <cstring>
#include <fstream>
#include <iostream>

#include "Arguments.h"
#include "InstructionSetRegister.h"

using namespace std;

const char* Arguments::HELP_MESSAGE =
R"(asm <-i iSet> [input] [-h] [-o output] [-s]

Assemble the input file and output the result to the output file. If no input
file is specified, stdin is used. If no output file is specified, stdout is
used.

  --help        -h  display help message
  --i-set       -i  instruction set name
  --list-i-set      list available instruction sets
  --no-color        do not color output messages
  --output      -o  output file
  --symbols     -s  print symbol table

Examples:

Assemble test.s and output to test.out:
  asm test.s -i W8 -o test.out

Print symbol table for test.s:
  asm test.s -i W16 -s
)";

Arguments::Arguments() :
    instructionSetName(""),
    is(nullptr),
    os(nullptr),
    colorOutput(true),
    outputSymbols(false),
    done(false),
    error(false)
{
}

Arguments::~Arguments()
{
    if (is != &cin)
    {
        delete is;
    }

    if (os != &cout)
    {
        delete os;
    }
}

bool Arguments::isDone() const
{
    return done;
}

bool Arguments::isError() const
{
    return error;
}

void Arguments::parse(int argc, const char* argv[])
{
    for (int idx = 1; idx < argc; ++idx)
    {
        parseNextArgs(idx, argc, argv);
        if (done || error)
        {
            return;
        }
    }

    if (instructionSetName.empty())
    {
        cerr << "Error: No instruction set was given.\n";
        error = true;
        return;
    }

    configIO();
    if (error)
    {
        return;
    }
}

void Arguments::parseNextArgs(int& idx, int argc, const char* argv[])
{
    const char* arg = argv[idx];

    if (std::strcmp(arg, "-o") == 0 || std::strcmp(arg, "--output") == 0)
    {
        if (idx + 1 >= argc)
        {
            cerr << "Error: Expected an argument after " << arg << ".\n";
            error = true;
        }
        else if (!outFilename.empty())
        {
            cerr << "Error: Argument " << arg << " was given more than once.\n";
            error = true;
        }
        else
        {
            outFilename = argv[idx + 1];

            // increment index
            ++idx;
        }
    }
    else if (std::strcmp(arg, "-h") == 0 || std::strcmp(arg, "--help") == 0)
    {
        cout << HELP_MESSAGE;
        done = true;
    }
    else if (std::strcmp(arg, "-i") == 0 || std::strcmp(arg, "--i-set") == 0)
    {
        if (idx + 1 >= argc)
        {
            cerr << "Error: Expected an argument after " << arg << ".\n";
            error = true;
        }
        else if (!instructionSetName.empty())
        {
            cerr << "Error: Argument " << arg << " was given more than once.\n";
            error = true;
        }
        else
        {
            const char* name = argv[idx + 1];
            if (InstructionSetRegister::getInstance().getInstructionSet(name) == nullptr)
            {
                cerr << "Error: " << name << " is not a valid instruction set.\n";
                error = true;
            }
            else
            {
                instructionSetName = name;

                // increment index
                ++idx;
            }
        }
    }
    else if (std::strcmp(arg, "--list-i-set") == 0)
    {
        for (string name : InstructionSetRegister::getInstance().getInstructionSetNames())
        {
            cout << name << '\n';
        }
        done = true;
    }
    else if (std::strcmp(arg, "--no-color") == 0)
    {
        colorOutput = false;
    }
    else if (std::strcmp(arg, "-s") == 0 || std::strcmp(arg, "--symbols") == 0)
    {
        outputSymbols = true;
    }
    else // arg is the input file
    {
        if (!inFilename.empty())
        {
            cerr << "Error: Cannot process more than one input file.\n";
            error = true;
        }
        else
        {
            inFilename = arg;
        }
    }
}

void Arguments::configIO()
{
    // --- Input ---

    if (inFilename.empty())
    {
        is = &cin;
    }
    else
    {
        fstream* inFile = new fstream;
        inFile->open(inFilename, ios_base::in);

        is = inFile;
        if (is->fail())
        {
            cerr << "Error: Could not open file \"" << inFilename << "\".\n";
            error = true;
        }
    }

    // --- Output ---

    if (outFilename.empty())
    {
        os = &cout;
    }
    else
    {
        fstream* outFile = new fstream;
        outFile->open(outFilename, ios_base::out);

        os = outFile;
        if (os->fail())
        {
            cerr << "Error: Could not open file \"" << outFilename << "\".\n";
            error = true;
        }
    }
}
