#include <cstring>
#include <fstream>
#include <iostream>

#include "Arguments.h"
#include "InstructionSetRegister.h"

using namespace std;

const char* Arguments::HELP_MESSAGE =
R"(asm <-i iSet> [input] [-f format] [-h] [-o output] [-s]

Assemble the input file and output the result to the output file. If no input
file is specified, stdin is used. If no output file is specified, stdout is
used.

  --format      -f  output format (bin or text)
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
    outputFormat(EOutputFormat::eText),
    colorOutput(true),
    outputSymbols(false),
    done(false),
    error(false),
    seenFormatFlag(false)
{
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

    if (error)
    {
        return;
    }
}

void Arguments::parseNextArgs(int& idx, int argc, const char* argv[])
{
    const char* arg = argv[idx];

    if (std::strcmp(arg, "-f") == 0 || std::strcmp(arg, "--format") == 0)
    {
        if (idx + 1 >= argc)
        {
            cerr << "Error: Expected an argument after " << arg << ".\n";
            error = true;
        }
        else if (seenFormatFlag)
        {
            cerr << "Error: Argument " << arg << " was given more than once.\n";
            error = true;
        }
        else
        {
            const char* format = argv[idx + 1];
            if (std::strcmp(format, "bin") == 0)
            {
                outputFormat = EOutputFormat::eBinary;
            }
            else if (std::strcmp(format, "text") == 0)
            {
                outputFormat = EOutputFormat::eText;
            }
            else
            {
                cerr << "Error: \"" << format << "\" is not a valid format.\n";
                error = true;
            }

            // increment index
            ++idx;
        }

        seenFormatFlag = true;
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
                cerr << "Error: \"" << name << "\" is not a valid instruction set.\n";
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
    else if (std::strcmp(arg, "-o") == 0 || std::strcmp(arg, "--output") == 0)
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
