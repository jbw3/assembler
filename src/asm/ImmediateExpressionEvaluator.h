#ifndef IMMEDIATE_EXPRESSION_EVALUATOR_H_
#define IMMEDIATE_EXPRESSION_EVALUATOR_H_

#include <vector>

#include "SymbolMap.h"
#include "Token.h"

class ImmediateExpressionEvaluator
{
public:
    ImmediateExpressionEvaluator(const SymbolMap& symbols);

    std::int64_t eval(const TokenVector& tokens);

private:
    const SymbolMap& symbols;

    std::int64_t evalUnary(TokenVector::const_iterator first, TokenVector::const_iterator last);

    std::int64_t evalImmediate(const Token& token);

    std::int64_t evalNum(const Token& token);

    std::int64_t evalConstant(const Token& token);

    void throwError(const std::string& message, const Token& token);
};

#endif // IMMEDIATE_EXPRESSION_EVALUATOR_H_
