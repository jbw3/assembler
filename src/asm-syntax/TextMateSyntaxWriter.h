#ifndef TEXT_MATE_SYNTAX_WRITER_H_
#define TEXT_MATE_SYNTAX_WRITER_H_

#include <vector>

#include "SyntaxFileWriter.h"

class TextMateSyntaxWriter : public SyntaxFileWriter
{
public:
    TextMateSyntaxWriter() = default;

    virtual ~TextMateSyntaxWriter() = default;

    void write(std::ostream& os, const SyntaxInfo& info) override;

private:
    static const char HEADER[];

    static const char FOOTER[];

    void writeFileTypes(std::ostream& os, const std::vector<std::string>& fileTypes);
};

#endif // TEXT_MATE_SYNTAX_WRITER_H_
