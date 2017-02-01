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
    cerr << "WARNING:\n" << message << "\n";
}

void Logger::logError(const string& message)
{
    cerr << "ERROR:\n" << message << "\n";
}
