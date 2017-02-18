#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "InstructionSet.h"
#include "InstructionSetRegister.h"
#include "SyntaxInfo.h"
#include "TextMateSyntaxWriter.h"
#include "utils.h"

using namespace std;

SyntaxInfo createSyntaxInfo(const InstructionSet* iSet);

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

    // get instruction set
    string iSetName = argv[1];
    const InstructionSet* iSet = InstructionSetRegister::getInstance().getInstructionSet(iSetName);
    if (iSet == nullptr)
    {
        cout << "\"" << iSetName << "\" is not a known instruction set.\n";
        return 1;
    }

    // get output stream
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

    SyntaxInfo info = createSyntaxInfo(iSet);

    SyntaxFileWriter* writer = new TextMateSyntaxWriter;
    writer->write(*os, info);
    delete writer;

    if (argc == 3)
    {
        file.close();
    }

    return 0;
}

SyntaxInfo createSyntaxInfo(const InstructionSet* iSet)
{
    const string& name = iSet->getName();

    string instRegex = "\\b(?i)(?:";
    const map<string, Instruction>& instructions = iSet->getInstructions();
    auto iter = instructions.cbegin();
    if (iter != instructions.cend())
    {
        instRegex += toUpper(iter->first);
        ++iter;
        for (; iter != instructions.cend(); ++iter)
        {
            instRegex += "|" + toUpper(iter->first);
        }
    }
    instRegex += ")\\b";

    SyntaxInfo info;

    info.name = name;
    info.fileTypes = {"asm", "inc", "s"};

    info.rules = {
        {"#.*$", "comment.line"},
        {R"(\b(?:0[Bb][01]+|0[Oo][0-7]+|[0-9]+|0[Xx][0-9A-Fa-f]+)\b)", "constant.numeric.integer"},
        {instRegex, "keyword.control"}
    };

    for (MatchRule& rule : info.rules)
    {
        rule.scope += "." + name;
    }

    return info;
}
