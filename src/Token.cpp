#include "Token.h"

using namespace std;

Token::Token() :
    value(""),
    line(0),
    column(0)
{}

Token::Token(const string& value, unsigned long line, unsigned long column) :
    value(value),
    line(line),
    column(column)
{}

const string& Token::getValue() const
{
    return value;
}

unsigned long Token::getLine() const
{
    return line;
}

unsigned long Token::getColumn() const
{
    return column;
}

bool Token::operator ==(const Token& other) const
{
    return value == other.value;
}

bool Token::operator !=(const Token& other) const
{
    return value != other.value;
}
