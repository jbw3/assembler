#ifndef ERROR_H_
#define ERROR_H_

#include <string>

class Error
{
public:
    Error(const std::string& message, long long line = 0) :
        message(message),
        line(line)
    {}

    std::string getMessage() const
    {
        return message;
    }

    long long getLine() const
    {
        return line;
    }

private:
    std::string message;
    long long line;
};

#endif // ERROR_H_
