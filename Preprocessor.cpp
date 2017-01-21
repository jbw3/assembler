#include <limits>
#include <string>

#include "Preprocessor.h"

Preprocessor::Preprocessor()
{
}

void Preprocessor::process(std::istream& is, std::ostream& os)
{
    std::string line;
    while (!is.eof())
    {
        getline(is, line, '\n');

        // remove comments
        std::size_t commentPos = line.find(COMMENT_CHAR);
        if (commentPos != std::string::npos)
        {
            line = line.substr(0, commentPos);
        }

        os << line << '\n';
    }
}
