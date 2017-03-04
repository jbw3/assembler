#ifndef I_OUTPUT_FORMATTER_H_
#define I_OUTPUT_FORMATTER_H_

#include <ostream>

#include "CodeGenerator.h"

class IOutputFormatter
{
public:
    virtual ~IOutputFormatter() = default;

    virtual void generate(std::ostream& os, unsigned int wordSize, const CodeGenerator::InstructionCodeList&) = 0;
};

#endif // I_OUTPUT_FORMATTER_H_
