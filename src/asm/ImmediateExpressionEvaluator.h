#ifndef IMMEDIATE_EXPRESSION_EVALUATOR_H_
#define IMMEDIATE_EXPRESSION_EVALUATOR_H_

#include <unordered_set>

#include "SymbolMap.h"
#include "Token.h"

class ImmediateExpressionEvaluator
{
public:
    ImmediateExpressionEvaluator(const SymbolMap& symbols);

    std::int64_t eval(const TokenVec& tokens);

private:
    static const std::unordered_set<Token> UNARY_OPERATORS;
    static const std::unordered_set<Token> BINARY_OPERATORS;

    const SymbolMap& symbols;

    std::int64_t evalUnary(TokenVec::const_iterator first, TokenVec::const_iterator last);

    std::int64_t evalImmediate(const Token& token);

    std::int64_t evalNum(const Token& token);

    std::int64_t evalConstant(const Token& token);

    void throwError(const std::string& message, const Token& token);
};

#endif // IMMEDIATE_EXPRESSION_EVALUATOR_H_
