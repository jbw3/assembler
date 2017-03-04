#ifndef BINARY_OUTPUT_FORMATTER_H_
#define BINARY_OUTPUT_FORMATTER_H_

#include "IOutputFormatter.h"

class BinaryOutputFormatter : public IOutputFormatter
{
public:
    void generate(std::ostream& os, unsigned int wordSize, const CodeGenerator::InstructionCodeList& instCodeList) override;
};

#endif // BINARY_OUTPUT_FORMATTER_H_
