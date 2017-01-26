#ifndef I_CODE_GENERATOR_H_
#define I_CODE_GENERATOR_H_

#include "SyntaxAnalyzer.h"

class ICodeGenerator
{
public:
    virtual void generate(const SyntaxAnalyzer::InstructionCodeList&) = 0;
};

#endif // I_CODE_GENERATOR_H_
