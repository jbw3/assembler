#ifndef TEXT_MATE_SYNTAX_WRITER_H_
#define TEXT_MATE_SYNTAX_WRITER_H_

#include <vector>

#include "SyntaxFileWriter.h"
#include "SyntaxInfo.h"

class TextMateSyntaxWriter : public SyntaxFileWriter
{
public:
    TextMateSyntaxWriter() = default;

    virtual ~TextMateSyntaxWriter() = default;

    void write(std::ostream& os, const SyntaxInfo& info) override;

private:
    static const char HEADER[];

    static const char FOOTER[];

    void writeName(std::ostream& os, const std::string& name);

    void writeFileTypes(std::ostream& os, const std::vector<std::string>& fileTypes);

    void writePatterns(std::ostream& os, const std::vector<MatchRule>& rules);
};

#endif // TEXT_MATE_SYNTAX_WRITER_H_
