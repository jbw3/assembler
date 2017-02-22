#ifndef IMMEDIATE_EXPRESSION_EVALUATOR_H_
#define IMMEDIATE_EXPRESSION_EVALUATOR_H_

#include <vector>

#include "SymbolMap.h"
#include "Token.h"

class ImmediateExpressionEvaluator
{
public:
    ImmediateExpressionEvaluator(const SymbolMap& symbols);

    std::int64_t eval(const std::vector<Token>& tokens);

private:
    const SymbolMap& symbols;

    std::int64_t evalImmediateNum(const Token& token);

    std::int64_t evalImmediateLabel(const Token& token);

    void throwError(const std::string& message, const Token& token);
};

#endif // IMMEDIATE_EXPRESSION_EVALUATOR_H_
