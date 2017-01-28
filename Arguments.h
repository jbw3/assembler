#ifndef ARGUMENTS_H_
#define ARGUMENTS_H_

#include "ICodeGenerator.h"
#include "InstructionSet.h"

class Arguments
{
public:
    Arguments();

    ~Arguments();

    bool parse(int argc, const char* argv[]);

    InstructionSet instructionSet;
    ICodeGenerator* codeGenerator;
    std::istream* is;

private:
    std::ostream* os;

    bool parseNextArgs(int& idx, int argc, const char* argv[]);
};

#endif // ARGUMENTS_H_
