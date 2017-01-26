#include <fstream>
#include <sstream>

#include "Assembler.h"

#include "InstructionSet8Bit.h"

Assembler::Assembler(const std::string& filename) :
    inFilename(filename),
    syntaxAnalyzer(ISET_8_BIT)
{
}

void Assembler::assemble()
{
    bool ok = true;
    std::fstream inFile;

    /////////////////////////////////
    // Preprocess
    /////////////////////////////////

    inFile.open(inFilename, std::ios_base::in);
    std::stringstream preProcStream;

    preprocessor.process(inFile, preProcStream);

    inFile.close();

    std::cout << "Preprocess:\n"
              << preProcStream.str() << "\n\n";

    /////////////////////////////////
    // Lexical Analyzer
    /////////////////////////////////

    std::vector<std::string> tokens;

    lexicalAnalyzer.process(preProcStream, tokens);

    std::cout << "Tokens:\n";
    for (std::string token : tokens)
    {
        std::cout << token << "|";
    }
    std::cout << "\n\n";

    /////////////////////////////////
    // Syntax Analyzer
    /////////////////////////////////

    SyntaxAnalyzer::InstructionCodeList instCodeList;

    ok = syntaxAnalyzer.process(tokens, instCodeList);

    if (ok)
    {
        for (auto code : instCodeList)
        {
            std::cout << std::hex << code[0] << std::dec << "\n";
        }
    }
}
