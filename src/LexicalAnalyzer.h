#ifndef LEXICAL_ANALYZER_H_
#define LEXICAL_ANALYZER_H_

#include <istream>
#include <string>
#include <vector>

class LexicalAnalyzer
{
public:
    LexicalAnalyzer();

    void process(std::istream& is, std::vector<std::string>& tokens);

private:
    std::string token;
    bool isValid;

    void parseChar(char ch, std::vector<std::string>& tokens);

    bool isValidToken(const std::string& token);
};

#endif // LEXICAL_ANALYZER_H_
