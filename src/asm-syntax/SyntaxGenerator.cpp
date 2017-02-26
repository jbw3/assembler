#include <map>

#include "InstructionSet.h"
#include "SyntaxGenerator.h"
#include "TextMateSyntaxWriter.h"
#include "Token.h"
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

    // generate instruction regex
    string instRegex = "\\b(?i)(?:";
    const map<string, Instruction>& instructions = iSet->getInstructions();
    auto instIter = instructions.cbegin();
    if (instIter != instructions.cend())
    {
        instRegex += toUpper(instIter->first);
        ++instIter;
        for (; instIter != instructions.cend(); ++instIter)
        {
            instRegex += "|" + toUpper(instIter->first);
        }
    }
    instRegex += ")\\b";

    // generate register regex
    string regRegex = "\\b(?i)(?:";
    const map<string, Register>& registers = iSet->getRegisters();
    auto regIter = registers.cbegin();
    if (regIter != registers.cend())
    {
        regRegex += toUpper(regIter->first);
        ++regIter;
        for (; regIter != registers.cend(); ++regIter)
        {
            regRegex += "|" + toUpper(regIter->first);
        }
    }
    regRegex += ")\\b";

    SyntaxInfo info;

    info.name = name;
    info.fileTypes = {"asm", "inc", "s"};

    info.rules = {
        {"#.*$", "comment.line"},
        {R"(\b(?:0[Bb][01]+|0[Oo][0-7]+|[0-9]+|0[Xx][0-9A-Fa-f]+)\b)", "constant.numeric.integer"},
        {"\\b" + CURRENT_ADDRESS.getValue() + "\\b", "variable.language"},
        {instRegex, "keyword.control"},
        {regRegex, "variable.language"}
    };

    for (MatchRule& rule : info.rules)
    {
        rule.scope += "." + name;
    }

    return info;
}
