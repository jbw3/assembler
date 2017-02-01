#include <iostream>

#include "Logger.h"

using namespace std;

Logger& Logger::getInstance()
{
    static Logger instance;
    return instance;
}

Logger::Logger() :
    useColor(true)
{}

void Logger::logWarning(const string& message)
{
    logMessage("WARNING", eYellow, message);
}

void Logger::logError(const string& message)
{
    logMessage("ERROR", eRed, message);
}

void Logger::logMessage(const std::string& prefix, EColor color, const std::string& message)
{
    if (useColor)
    {
        cerr << "\x1B[3" << static_cast<char>(color) << "m"
             << prefix << ":"
             << "\x1B[m";
    }
    else
    {
        cerr << prefix << ":";
    }

    cerr << "\n" << message << "\n";
}
