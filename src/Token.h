#ifndef TOKEN_H_
#define TOKEN_H_

#include <string>

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

/// End of line
const Token END_OF_LINE("\n", 0, 0);

/// Argument separator
const Token ARGUMENT_SEPARATOR(",", 0, 0);

#endif // TOKEN_H_
