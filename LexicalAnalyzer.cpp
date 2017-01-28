#include <cctype>

#include "Error.h"
#include "LexicalAnalyzer.h"
#include "tokens.h"

using namespace std;

LexicalAnalyzer::LexicalAnalyzer() :
    token(""),
    isValid(false)
{
}

void LexicalAnalyzer::process(istream& is, vector<string>& tokens)
{
    tokens.clear();
    tokens.reserve(128);

    char buff[1];
    is.read(buff, 1);
    while (!is.eof())
    {
        parseChar(buff[0], tokens);

        is.read(buff, 1);
    }

    // check for leftover token
    if (!token.empty())
    {
        if (isValid)
        {
            tokens.push_back(token);
        }
        else
        {
            throw Error("Invalid token \"" + token + "\".");
        }
    }
}

void LexicalAnalyzer::parseChar(char ch, vector<string>& tokens)
{
    if (isblank(ch))
    {
        if (!token.empty())
        {
            if (isValid)
            {
                tokens.push_back(token);
                token = "";
                isValid = false;
            }
            else
            {
                throw Error("Invalid token \"" + token + "\".");
            }
        }
    }
    else // ch is not blank
    {
        if (isValid)
        {
            if (isValidToken(token + ch))
            {
                token += ch;
            }
            else
            {
                tokens.push_back(token);
                token = ch;
                isValid = isValidToken(token);
            }
        }
        else
        {
            token += ch;
            isValid = isValidToken(token);
        }
    }
}

bool LexicalAnalyzer::isValidToken(const string& token)
{
    // argument separator
    if (token == ARGUMENT_SEPARATOR)
    {
        return true;
    }

    // end of line
    if (token == END_OF_LINE)
    {
        return true;
    }

    // identifier (instruction, register, etc.)
    if (isalpha(token[0]))
    {
        size_t idx = 1;
        for (; idx < token.size(); ++idx)
        {
            if (!isalnum(token[idx]))
            {
                break;
            }
        }

        // if we made it through all the chars, this is an identifier
        if (idx == token.size())
        {
            return true;
        }
    }

    // decimal integer
    size_t idx = 0;
    for (; idx < token.size(); ++idx)
    {
        if (!isdigit(token[idx]))
        {
            break;
        }
    }

    // if we made it through all the chars, this is an integer
    if (idx == token.size())
    {
        return true;
    }

    return false;
}
