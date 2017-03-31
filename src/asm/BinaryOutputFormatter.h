#ifndef BINARY_OUTPUT_FORMATTER_H_
#define BINARY_OUTPUT_FORMATTER_H_

#include "IOutputFormatter.h"

class BinaryOutputFormatter : public IOutputFormatter
{
public:
    bool isBinaryOutput() const override;

    void generate(IOutputFormatter::Config& config) override;
};

#endif // BINARY_OUTPUT_FORMATTER_H_
