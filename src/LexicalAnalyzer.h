#ifndef LEXICAL_ANALYZER_H_
#define LEXICAL_ANALYZER_H_

#include <istream>
#include <string>
#include <vector>

#include "Token.h"

class LexicalAnalyzer
{
public:
    LexicalAnalyzer();

    void process(std::istream& is, std::vector<Token>& tokens);

private:
    std::string tokenStr;
    bool isValid;
    unsigned long line;
    unsigned long column;

    void addToken(std::vector<Token>& tokens);

    void parseChar(char ch, std::vector<Token>& tokens);

    bool isValidToken(const std::string& str);
};

#endif // LEXICAL_ANALYZER_H_
