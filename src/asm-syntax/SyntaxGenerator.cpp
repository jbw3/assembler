#include <map>

#include "InstructionSet.h"
#include "SyntaxGenerator.h"
#include "TextMateSyntaxWriter.h"
#include "utils.h"

using namespace std;

void SyntaxGenerator::generate(std::ostream& os, const InstructionSet* iSet)
{
    SyntaxInfo info = createSyntaxInfo(iSet);

    SyntaxFileWriter* writer = new TextMateSyntaxWriter;
    writer->write(os, info);
    delete writer;
}

SyntaxInfo SyntaxGenerator::createSyntaxInfo(const InstructionSet* iSet)
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
