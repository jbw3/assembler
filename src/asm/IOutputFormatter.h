#ifndef I_OUTPUT_FORMATTER_H_
#define I_OUTPUT_FORMATTER_H_

#include <ostream>

#include "CodeGenerator.h"

class IOutputFormatter
{
public:
    struct Config
    {
        std::ostream& os;
        unsigned int wordSize;
        const CodeGenerator::InstructionCodeList& instCodeList;
        bool isLittleEndian;
    };

    virtual ~IOutputFormatter() = default;

    virtual bool isBinaryOutput() const = 0;

    virtual void generate(Config& config) = 0;
};

#endif // I_OUTPUT_FORMATTER_H_
