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
    Logger() = default;

    Logger(const Logger&) = default;

    Logger(Logger&&) = default;

    ~Logger() = default;

    Logger& operator =(const Logger&) = default;

    Logger& operator =(Logger&&) = default;

    void logMessage(const std::string& prefix, const std::string& message);
};

#endif // LOGGER_H_
