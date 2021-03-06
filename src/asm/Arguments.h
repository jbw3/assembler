#ifndef ARGUMENTS_H_
#define ARGUMENTS_H_

#include <string>

class Arguments
{
public:
    enum struct EOutputFormat
    {
        eBinary,
        eText
    };

    Arguments();

    bool isDone() const;

    bool isError() const;

    void parse(int argc, const char* argv[]);

    std::string instructionSetName;
    std::string inFilename;
    std::string outFilename;
    EOutputFormat outputFormat;
    bool colorOutput;
    bool outputSymbols;

private:
    static const char* HELP_MESSAGE;

    bool done;
    bool error;
    bool seenFormatFlag;

    void parseNextArgs(int& idx, int argc, const char* argv[]);
};

#endif // ARGUMENTS_H_
