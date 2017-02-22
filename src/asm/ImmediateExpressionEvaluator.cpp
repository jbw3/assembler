#include "Error.h"
#include "ImmediateExpressionEvaluator.h"
#include "Logger.h"

using namespace std;

ImmediateExpressionEvaluator::ImmediateExpressionEvaluator(const SymbolMap& symbols) :
    symbols(symbols)
{
}

int64_t ImmediateExpressionEvaluator::eval(const vector<Token>& tokens)
{
    int64_t value = 0;

    if (tokens.size() > 1)
    {
        throwError("Invalid syntax.", tokens[1]);
    }

    const Token& token = tokens[0];

    const string& tokenStr = token.getValue();

    if (isdigit(tokenStr[0]))
    {
        value = evalImmediateNum(token);
    }
    else
    {
        value = evalImmediateLabel(token);
    }

    return value;
}

int64_t ImmediateExpressionEvaluator::evalImmediateNum(const Token& token)
{
    bool error = false;
    int64_t value = 0;
    size_t pos = 0;

    const string& tokenStr = token.getValue();

    // determine base
    int base = 10;
    if (tokenStr.size() >= 2 && tokenStr[0] == '0')
    {
        switch (tokenStr[1])
        {
        case 'b':
        case 'B':
            base = 2;
            break;

        case 'o':
        case 'O':
            base = 8;
            break;

        case 'x':
        case 'X':
            base = 16;
            break;

        default:
            base = 10;
            break;
        }
    }

    // strip prefix if not a decimal number
    string noPrefixToken = (base == 10) ? tokenStr : tokenStr.substr(2);

    // try to convert the string to an integer
    try
    {
        value = stoll(noPrefixToken, &pos, base);
    }
    catch (invalid_argument)
    {
        error = true;
    }
    catch (out_of_range)
    {
        error = true;
    }

    // make sure the entire string was used
    if (pos != noPrefixToken.size())
    {
        error = true;
    }

    if (error)
    {
        throwError(tokenStr + " is not a valid integer.", token);
    }

    return value;
}

int64_t ImmediateExpressionEvaluator::evalImmediateLabel(const Token& token)
{
    int64_t value = 0;
    string label = token.getValue();

    auto iter = symbols.find(label);
    if (iter == symbols.end())
    {
        throwError("\"" + label + "\" is not a valid symbol.", token);
    }
    else
    {
        value = iter->second;
    }

    return value;
}

void ImmediateExpressionEvaluator::throwError(const std::string& message, const Token& token)
{
    Logger::getInstance().logError(message, token.getLine(), token.getColumn());
    throw Error();
}
