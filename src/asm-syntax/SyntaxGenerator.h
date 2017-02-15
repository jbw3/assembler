#ifndef SYNTAX_GENERATOR_H_
#define SYNTAX_GENERATOR_H_

#include <ostream>

#include "SyntaxInfo.h"

class InstructionSet;

class SyntaxGenerator
{
public:
    void generate(std::ostream& os, const InstructionSet* iSet);

private:
    SyntaxInfo createSyntaxInfo(const InstructionSet* iSet);
};

#endif // SYNTAX_GENERATOR_H_
