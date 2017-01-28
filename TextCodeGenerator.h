#ifndef TEXT_CODE_GENERATOR_H_
#define TEXT_CODE_GENERATOR_H_

#include <string>

#include "ICodeGenerator.h"

class TextCodeGenerator : public ICodeGenerator
{
public:
    TextCodeGenerator(std::ostream& os, unsigned int wordSize);

    void generate(const SyntaxAnalyzer::InstructionCodeList& instCodeList) override;

private:
    std::ostream& os;
    unsigned int wordSize;
};

#endif // TEXT_CODE_GENERATOR_H_
