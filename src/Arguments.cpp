#include <cstring>
#include <fstream>
#include <iostream>

#include "Arguments.h"

using namespace std;

const char* Arguments::HELP_MESSAGE =
R"(asm [input] [-h] [-o output]

Assemble the input file and output the result to the output file. If no input
file is specified, stdin is used. If no output file is specified, stdout is
used.

  --help, -h        display help message
  --output, -o      output file
)";

Arguments::Arguments() :
    instructionSetName("W8"),
    is(nullptr),
    os(nullptr),
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
