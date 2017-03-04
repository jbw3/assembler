#ifndef TEXT_OUTPUT_FORMATTER_H_
#define TEXT_OUTPUT_FORMATTER_H_

#include "IOutputFormatter.h"

class TextOutputFormatter : public IOutputFormatter
{
public:
    bool isBinaryOutput() const override;

    void generate(std::ostream& os, unsigned int wordSize, const CodeGenerator::InstructionCodeList& instCodeList) override;
};

#endif // TEXT_OUTPUT_FORMATTER_H_
