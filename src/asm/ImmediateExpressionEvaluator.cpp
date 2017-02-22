#include "Error.h"
#include "ImmediateExpressionEvaluator.h"
#include "Logger.h"

using namespace std;

ImmediateExpressionEvaluator::ImmediateExpressionEvaluator(const SymbolMap& symbols) :
    symbols(symbols)
{
}

int64_t ImmediateExpressionEvaluator::eval(const TokenVector& tokens)
{
    int64_t value = evalUnary(tokens.cbegin(), tokens.cend() - 1);

    return value;
}

int64_t ImmediateExpressionEvaluator::evalUnary(TokenVector::const_iterator first, TokenVector::const_iterator last)
{
    int64_t value = 0;

    if (first == last)
    {
        value = evalImmediate(*first);
    }
    else if (*first == SUBTRACTION_OPERATOR)
    {
        // operator is negative sign
        value = -evalUnary(first + 1, last);
    }
    else if (*first == ADDITION_OPERATOR)
    {
        // operator is positive sign; do nothing
        value = evalUnary(first + 1, last);
    }
    else
    {
        // if we get here, there was an error
        throwError("Invalid syntax: \"" + first->getValue() + "\".", *first);
    }

    return value;
}

int64_t ImmediateExpressionEvaluator::evalImmediate(const Token& token)
{
    int64_t value = 0;
    const string& tokenStr = token.getValue();

    if (isdigit(tokenStr[0]))
    {
        value = evalNum(token);
    }
    else
    {
        value = evalConstant(token);
    }

    return value;
}

int64_t ImmediateExpressionEvaluator::evalNum(const Token& token)
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

int64_t ImmediateExpressionEvaluator::evalConstant(const Token& token)
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
