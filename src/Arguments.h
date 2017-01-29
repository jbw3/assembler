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
    std::ostream* os;

private:
    static const char* HELP_MESSAGE;

    bool done;
    bool error;
    std::string inFilename;
    std::string outFilename;

    void parseNextArgs(int& idx, int argc, const char* argv[]);

    void configIO();
};

#endif // ARGUMENTS_H_
