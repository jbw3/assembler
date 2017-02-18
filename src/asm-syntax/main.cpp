#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "SyntaxInfo.h"
#include "TextMateSyntaxWriter.h"

using namespace std;

SyntaxInfo createSyntaxInfo();

int main(int argc, const char* argv[])
{
    if (argc < 2)
    {
        cout << "No instruction set was given.\n";
        return 1;
    }
    else if (argc > 3)
    {
        cout << "Too many arguments were given.\n";
        return 1;
    }

    string iSet = argv[1];
    ostream* os = nullptr;
    fstream file;
    if (argc == 3)
    {
        string filename = argv[2];
        file.open(filename, ios::out);
        os = &file;
    }
    else
    {
        os = &cout;
    }

    SyntaxInfo info = createSyntaxInfo();

    SyntaxFileWriter* writer = new TextMateSyntaxWriter;
    writer->write(*os, info);
    delete writer;

    if (argc == 3)
    {
        file.close();
    }

    return 0;
}

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
