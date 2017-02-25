#include <algorithm>

#include "Error.h"
#include "ImmediateExpressionEvaluator.h"
#include "Logger.h"

using namespace std;

const unordered_set<Token> ImmediateExpressionEvaluator::UNARY_OPERATORS =
{
    ADDITION_OPERATOR,
    SUBTRACTION_OPERATOR,
    COMPLEMENT_OPERATOR
};

const unordered_set<Token> ImmediateExpressionEvaluator::BINARY_OPERATORS =
{
    ADDITION_OPERATOR,
    SUBTRACTION_OPERATOR,
    MULTIPLICATION_OPERATOR,
    DIVISION_OPERATOR,
    MODULO_OPERATOR,
    AND_OPERATOR,
    OR_OPERATOR,
    XOR_OPERATOR,
    SHIFT_LEFT_OPERATOR,
    SHIFT_RIGHT_OPERATOR
};

ImmediateExpressionEvaluator::ImmediateExpressionEvaluator(const SymbolMap& symbols) :
    symbols(symbols)
{
}

int64_t ImmediateExpressionEvaluator::eval(const TokenVec& tokens)
{
    return eval(tokens.cbegin(), tokens.cend());
}

int64_t ImmediateExpressionEvaluator::eval(TokenVec::const_iterator begin, TokenVec::const_iterator end)
{
    list<int64_t> terms;
    list<Token> binOperators;

    bool expectBinOp = false;
    TokenVec termTokens;
    termTokens.reserve(8);
    for (auto tokenIter = begin; tokenIter != end; ++tokenIter)
    {
        if (expectBinOp)
        {
            if (BINARY_OPERATORS.find(*tokenIter) == BINARY_OPERATORS.cend())
            {
                throwError("Expected binary operator.", *tokenIter);
            }

            binOperators.push_back(*tokenIter);
            expectBinOp = false;
        }
        else
        {
            termTokens.push_back(*tokenIter);

            // if this is not an operator, it must be an immediate value
            if (UNARY_OPERATORS.find(*tokenIter) == UNARY_OPERATORS.cend())
            {
                int64_t term = evalUnary(termTokens.cbegin(), termTokens.cend() - 1);
                terms.push_back(term);

                termTokens.clear();
                expectBinOp = true;
            }
        }
    }

    // there should be one less operator than terms
    if (binOperators.size() != terms.size() - 1)
    {
        throwError("Expected term after operator.", binOperators.back());
    }

    int64_t value = evalTerms(terms, binOperators);

    return value;
}

int64_t ImmediateExpressionEvaluator::evalTerms(list<int64_t>& terms, list<Token>& binOperators)
{
    evalTermsPrecedence(terms, binOperators, {MULTIPLICATION_OPERATOR, DIVISION_OPERATOR, MODULO_OPERATOR});
    evalTermsPrecedence(terms, binOperators, {ADDITION_OPERATOR, SUBTRACTION_OPERATOR});
    evalTermsPrecedence(terms, binOperators, {SHIFT_LEFT_OPERATOR, SHIFT_RIGHT_OPERATOR});
    evalTermsPrecedence(terms, binOperators, {AND_OPERATOR});
    evalTermsPrecedence(terms, binOperators, {XOR_OPERATOR});
    evalTermsPrecedence(terms, binOperators, {OR_OPERATOR});

    return terms.front();
}

void ImmediateExpressionEvaluator::evalTermsPrecedence(list<int64_t>& terms, list<Token>& binOperators, const TokenVec& operators)
{
    auto term1Iter = terms.begin();
    for (auto opIter = binOperators.begin(); opIter != binOperators.end(); )
    {
        if (find(operators.begin(), operators.end(), *opIter) != operators.end())
        {
            auto term2Iter = term1Iter;
            ++term2Iter;

            *term1Iter = evalBinary(*opIter, *term1Iter, *term2Iter);

            terms.erase(term2Iter);
            opIter = binOperators.erase(opIter);
        }
        else
        {
            ++term1Iter;
            ++opIter;
        }
    }
}

int64_t ImmediateExpressionEvaluator::evalUnary(TokenVec::const_iterator first, TokenVec::const_iterator last)
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
    else if (*first == COMPLEMENT_OPERATOR)
    {
        // operator is complement
        value = ~evalUnary(first + 1, last);
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

int64_t ImmediateExpressionEvaluator::evalBinary(const Token& op, int64_t term1, int64_t term2)
{
    int64_t value = 0;

    if (op == ADDITION_OPERATOR)
    {
        value = term1 + term2;
    }
    else if (op == SUBTRACTION_OPERATOR)
    {
        value = term1 - term2;
    }
    else if (op == MULTIPLICATION_OPERATOR)
    {
        value = term1 * term2;
    }
    else if (op == DIVISION_OPERATOR)
    {
        value = term1 / term2;
    }
    else if (op == MODULO_OPERATOR)
    {
        value = term1 % term2;
    }
    else if (op == AND_OPERATOR)
    {
        value = term1 & term2;
    }
    else if (op == OR_OPERATOR)
    {
        value = term1 | term2;
    }
    else if (op == XOR_OPERATOR)
    {
        value = term1 ^ term2;
    }
    else if (op == SHIFT_LEFT_OPERATOR)
    {
        value = term1 << term2;
    }
    else if (op == SHIFT_RIGHT_OPERATOR)
    {
        value = term1 >> term2;
    }
    else
    {
        // if we get here, there was an error
        throwError("Invalid syntax: \"" + op.getValue() + "\".", op);
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
