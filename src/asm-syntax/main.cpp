#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "SyntaxInfo.h"
#include "TextMateSyntaxWriter.h"

using namespace std;

/// @todo move this somewhere else?
SyntaxInfo createSyntaxInfo()
{
    string name = "W8";

    SyntaxInfo info;

    info.name = name;
    info.fileTypes = {"asm", "inc", "s"};

    info.rules = {
        {"#.*$", "comment.line"},
        {R"(\b(?:0[Bb][01]+|0[Oo][0-7]+|[0-9]+|0[Xx][0-9A-Fa-f]+)\b)", "constant.numeric.integer"}
    };

    for (MatchRule& rule : info.rules)
    {
        rule.scope += "." + name;
    }

    return info;
}

/// @todo options:
/// -i instruction set
/// -f format
/// -o output file
int main()
{
    SyntaxInfo info = createSyntaxInfo();

    fstream file;
    file.open("out.tmLanguage", ios::out);

    SyntaxFileWriter* writer = new TextMateSyntaxWriter;
    writer->write(file, info);
    delete writer;

    file.close();

    return 0;
}
