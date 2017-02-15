#ifndef I_OUTPUT_FORMATTER_H_
#define I_OUTPUT_FORMATTER_H_

#include "CodeGenerator.h"

class IOutputFormatter
{
public:
    virtual ~IOutputFormatter() = default;

    virtual void generate(const CodeGenerator::InstructionCodeList&) = 0;
};

#endif // I_OUTPUT_FORMATTER_H_
