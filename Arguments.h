#ifndef ARGUMENTS_H_
#define ARGUMENTS_H_

#include "ICodeGenerator.h"
#include "InstructionSet.h"

class Arguments
{
public:
    Arguments();

    ~Arguments();

    bool parseArgs(int argc, const char* argv[]);

    InstructionSet instructionSet;
    ICodeGenerator* codeGenerator;
    std::istream* is;
};

#endif // ARGUMENTS_H_
