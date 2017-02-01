#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>

class Logger
{
public:
    static Logger& getInstance();

    void logWarning(const std::string& message);

    void logError(const std::string& message);

private:
    enum EColor : char
    {
        eBlack   = '0',
        eRed     = '1',
        eGreen   = '2',
        eYellow  = '3',
        eBlue    = '4',
        eMagenta = '5',
        eCyan    = '6',
        eWhite   = '7',
    };

    bool useColor;

    Logger();

    Logger(const Logger&) = default;

    Logger(Logger&&) = default;

    ~Logger() = default;

    Logger& operator =(const Logger&) = default;

    Logger& operator =(Logger&&) = default;

    void logMessage(const std::string& prefix, EColor color, const std::string& message);
};

#endif // LOGGER_H_
