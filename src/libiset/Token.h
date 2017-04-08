#ifndef TOKEN_H_
#define TOKEN_H_

#include <string>
#include <unordered_set>
#include <vector>

class Token
{
public:
    Token();

    Token(const std::string& value, unsigned long line = 0, unsigned long column = 0);

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
const Token END_OF_LINE("\n");

/// Argument separator
const Token ARGUMENT_SEPARATOR(",");

/// Label separator
const Token LABEL_SEPARATOR(":");

/// Assignment operator
const Token ASSIGNMENT_OPERATOR("=");

/// Addition operator
const Token ADDITION_OPERATOR("+");

/// Subtraction operator
const Token SUBTRACTION_OPERATOR("-");

/// Multiplication operator
const Token MULTIPLICATION_OPERATOR("*");

/// Division operator
const Token DIVISION_OPERATOR("/");

/// Modulo operator
const Token MODULO_OPERATOR("%");

/// Complement operator
const Token COMPLEMENT_OPERATOR("~");

/// AND operator
const Token AND_OPERATOR("&");

/// OR operator
const Token OR_OPERATOR("|");

/// XOR operator
const Token XOR_OPERATOR("^");

/// Bitshift left operator
const Token SHIFT_LEFT_OPERATOR("<<");

/// Bitshift right operator
const Token SHIFT_RIGHT_OPERATOR(">>");

/// Open parenthesis
const Token OPEN_PARENTHESIS("(");

/// Close parenthesis
const Token CLOSE_PARENTHESIS(")");

/// Current address
const Token CURRENT_ADDRESS("here");

/// Start address
const Token START_ADDRESS("start_address");

const std::unordered_set<Token> CONSTANT_OPERATORS = {
    ADDITION_OPERATOR,
    SUBTRACTION_OPERATOR,
    MULTIPLICATION_OPERATOR,
    DIVISION_OPERATOR,
    MODULO_OPERATOR,
    COMPLEMENT_OPERATOR,
    AND_OPERATOR,
    OR_OPERATOR,
    XOR_OPERATOR,
    SHIFT_LEFT_OPERATOR,
    SHIFT_RIGHT_OPERATOR,
    OPEN_PARENTHESIS,
    CLOSE_PARENTHESIS
};

const std::unordered_set<Token> RESERVED_IDENTIFIERS = {
    CURRENT_ADDRESS,
    START_ADDRESS
};

#endif // TOKEN_H_
