#ifndef TOKEN_H_
#define TOKEN_H_

#include <string>
#include <vector>

class Token
{
public:
    Token();

    Token(const std::string& value, unsigned long line, unsigned long column);

    const std::string& getValue() const;

    unsigned long getLine() const;

    unsigned long getColumn() const;

    bool operator ==(const Token& other) const;

    bool operator !=(const Token& other) const;

private:
    std::string value;
    unsigned long line;
    unsigned long column;
};

namespace std
{
template<>
struct hash<Token>
{
    std::size_t operator ()(const Token& token) const
    {
        return std::hash<std::string>{}(token.getValue());
    }
};
}

typedef std::vector<Token> TokenVec;
typedef std::vector<TokenVec> TokenVecVec;

/// End of line
const Token END_OF_LINE("\n", 0, 0);

/// Argument separator
const Token ARGUMENT_SEPARATOR(",", 0, 0);

/// Label separator
const Token LABEL_SEPARATOR(":", 0, 0);

/// Assignment operator
const Token ASSIGNMENT_OPERATOR("=", 0, 0);

/// Addition operator
const Token ADDITION_OPERATOR("+", 0, 0);

/// Subtraction operator
const Token SUBTRACTION_OPERATOR("-", 0, 0);

/// Multiplication operator
const Token MULTIPLICATION_OPERATOR("*", 0, 0);

/// Division operator
const Token DIVISION_OPERATOR("/", 0, 0);

/// Modulo operator
const Token MODULO_OPERATOR("%", 0, 0);

#endif // TOKEN_H_
