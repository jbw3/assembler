#ifndef ARGUMENTS_H_
#define ARGUMENTS_H_

#include "ICodeGenerator.h"
#include "InstructionSet.h"

class Arguments
{
public:
    Arguments();

    ~Arguments();

    bool isDone() const;

    bool isError() const;

    void parse(int argc, const char* argv[]);

    InstructionSet instructionSet;
    ICodeGenerator* codeGenerator;
    std::istream* is;

private:
    static const char* HELP_MESSAGE;

    std::ostream* os;
    bool done;
    bool error;

    void parseNextArgs(int& idx, int argc, const char* argv[]);
};

#endif // ARGUMENTS_H_
