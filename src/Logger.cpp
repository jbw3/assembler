#include <iostream>

#include "Logger.h"

using namespace std;

Logger& Logger::getInstance()
{
    static Logger instance;
    return instance;
}

void Logger::logWarning(const string& message)
{
    logMessage("WARNING", message);
}

void Logger::logError(const string& message)
{
    logMessage("ERROR", message);
}

void Logger::logMessage(const std::string& prefix, const std::string& message)
{
    cerr << prefix << ":\n" << message << "\n";
}
