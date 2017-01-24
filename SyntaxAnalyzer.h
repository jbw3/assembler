#ifndef SYNTAX_ANALYZER_H_
#define SYNTAX_ANALYZER_H_

#include <string>
#include <vector>

class SyntaxAnalyzer
{
public:
    SyntaxAnalyzer() = default;

    void process(const std::vector<std::string>& tokens);
};

#endif // SYNTAX_ANALYZER_H_
