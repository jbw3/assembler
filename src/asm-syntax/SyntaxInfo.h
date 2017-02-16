#ifndef SYNTAX_INFO_H_
#define SYNTAX_INFO_H_

#include <string>
#include <vector>

struct MatchRule
{
    std::string match;
    std::string scope;
};

struct SyntaxInfo
{
    std::string name;
    std::vector<std::string> fileTypes;
    std::vector<MatchRule> rules;
};

#endif // SYNTAX_INFO_H_
