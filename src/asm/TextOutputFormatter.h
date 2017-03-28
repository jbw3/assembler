#ifndef TEXT_OUTPUT_FORMATTER_H_
#define TEXT_OUTPUT_FORMATTER_H_

#include "IOutputFormatter.h"

class TextOutputFormatter : public IOutputFormatter
{
public:
    bool isBinaryOutput() const override;

    void generate(IOutputFormatter::Config& config) override;
};

#endif // TEXT_OUTPUT_FORMATTER_H_
