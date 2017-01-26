#ifndef TEXT_CODE_GENERATOR_H_
#define TEXT_CODE_GENERATOR_H_

#include <string>

#include "ICodeGenerator.h"

class TextCodeGenerator : public ICodeGenerator
{
public:
    TextCodeGenerator(std::ostream& os);

    void generate(const SyntaxAnalyzer::InstructionCodeList& instCodeList) override;

private:
    std::ostream& os;
};

#endif // TEXT_CODE_GENERATOR_H_
