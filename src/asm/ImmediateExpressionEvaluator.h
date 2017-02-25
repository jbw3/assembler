#ifndef IMMEDIATE_EXPRESSION_EVALUATOR_H_
#define IMMEDIATE_EXPRESSION_EVALUATOR_H_

#include <list>
#include <unordered_set>

#include "SymbolMap.h"
#include "Token.h"

class ImmediateExpressionEvaluator
{
public:
    ImmediateExpressionEvaluator(const SymbolMap& symbols);

    std::int64_t eval(const TokenVec& tokens);

    std::int64_t eval(TokenVec::const_iterator begin, TokenVec::const_iterator end);

private:
    static const std::unordered_set<Token> UNARY_OPERATORS;
    static const std::unordered_set<Token> BINARY_OPERATORS;

    const SymbolMap& symbols;

    std::int64_t evalTerms(std::list<std::int64_t>& terms, std::list<Token>& binOperators);

    void evalTermsPrecedence(std::list<std::int64_t>& terms, std::list<Token>& binOperators, const TokenVec& operators);

    std::int64_t evalUnary(TokenVec::const_iterator first, TokenVec::const_iterator last);

    std::int64_t evalBinary(const Token& op, std::int64_t term1, std::int64_t term2);

    std::int64_t evalImmediate(const Token& token);

    std::int64_t evalNum(const Token& token);

    std::int64_t evalConstant(const Token& token);

    void throwError(const std::string& message, const Token& token);
};

#endif // IMMEDIATE_EXPRESSION_EVALUATOR_H_
