#include <iostream>

#include "Logger.h"

using namespace std;

Logger& Logger::getInstance()
{
    static Logger instance;
    return instance;
}

Logger::Logger() :
    colorOutput(true)
{}

void Logger::setColorOutput(bool useColor)
{
    colorOutput = useColor;
}

void Logger::logWarning(const string& message, unsigned long line, unsigned long column)
{
    logMessage("WARNING", eYellow, message, line, column);
}

void Logger::logError(const string& message, unsigned long line, unsigned long column)
{
    logMessage("ERROR", eRed, message, line, column);
}

void Logger::logMessage(const std::string& prefix, EColor color, const std::string& message, unsigned long line, unsigned long column)
{
    if (colorOutput)
    {
        cerr << "\x1B[3" << static_cast<char>(color) << "m"
             << prefix << ":"
             << "\x1B[m";
    }
    else
    {
        cerr << prefix << ":";
    }

    // print line number
    if (line > 0)
    {
        cerr << " line: " << line;

        // print column number
        if (column > 0)
        {
            cerr << ", col: " << column;
        }
    }

    // print message
    cerr << "\n" << message << "\n";
}
