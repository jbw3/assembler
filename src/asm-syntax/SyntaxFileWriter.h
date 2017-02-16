#ifndef SYNTAX_FILE_WRITER_H_
#define SYNTAX_FILE_WRITER_H_

#include <ostream>

struct SyntaxInfo;

class SyntaxFileWriter
{
public:
    virtual ~SyntaxFileWriter() = default;

    virtual void write(std::ostream&, const SyntaxInfo&) = 0;
};

#endif // SYNTAX_FILE_WRITER_H_
