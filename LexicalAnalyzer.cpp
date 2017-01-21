#include "LexicalAnalyzer.h"

const std::string LexicalAnalyzer::WHITESPACE_CHARS = " \t";

LexicalAnalyzer::LexicalAnalyzer()
{
}

void LexicalAnalyzer::process(std::istream& is, std::vector<std::string>& tokens)
{
    tokens.clear();
    tokens.reserve(128);

    std::string line;
    while (!is.eof())
    {
        getline(is, line, '\n');
        parseLine(line, tokens);
    }
}

void LexicalAnalyzer::parseLine(const std::string& line, std::vector<std::string>& tokens)
{
    bool foundToken = false;

    std::size_t tokenStart = line.find_first_not_of(WHITESPACE_CHARS);
    while (tokenStart != std::string::npos)
    {
        foundToken = true;

        // find end of token
        std::size_t tokenEnd = line.find_first_of(WHITESPACE_CHARS, tokenStart);
        std::size_t tokenLen;
        if (tokenEnd == std::string::npos)
        {
            tokenLen = line.size() - tokenStart;
        }
        else
        {
            tokenLen = tokenEnd - tokenStart;
        }

        // add token to list
        tokens.push_back(line.substr(tokenStart, tokenLen));

        // find next token
        tokenStart = line.find_first_not_of(WHITESPACE_CHARS, tokenEnd);
    }

    // if at least one token was found on the line, add
    // an EOL token
    if (foundToken)
    {
        tokens.push_back("\n");
    }
}
