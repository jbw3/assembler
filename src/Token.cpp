#include "Token.h"

using namespace std;

Token::Token() :
    value("")
{}

Token::Token(const string& value) :
    value(value)
{}

const string& Token::getValue() const
{
    return value;
}

bool Token::operator ==(const Token& other) const
{
    return value == other.value;
}

bool Token::operator !=(const Token& other) const
{
    return value != other.value;
}
