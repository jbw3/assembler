#ifndef TOKEN_H_
#define TOKEN_H_

#include <string>

class Token
{
public:
    Token();

    Token(const std::string& value);

    const std::string& getValue() const;

    bool operator ==(const Token& other) const;

    bool operator !=(const Token& other) const;

private:
    std::string value;
};

/// End of line
const Token END_OF_LINE("\n");

/// Argument separator
const Token ARGUMENT_SEPARATOR(",");

#endif // TOKEN_H_
