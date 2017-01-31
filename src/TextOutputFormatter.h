#ifndef TEXT_OUTPUT_FORMATTER_H_
#define TEXT_OUTPUT_FORMATTER_H_

#include <string>

#include "IOutputFormatter.h"

class TextOutputFormatter : public IOutputFormatter
{
public:
    TextOutputFormatter(std::ostream& os, unsigned int wordSize);

    void generate(const SyntaxAnalyzer::InstructionCodeList& instCodeList) override;

private:
    std::ostream& os;
    unsigned int wordSize;
};

#endif // TEXT_OUTPUT_FORMATTER_H_
