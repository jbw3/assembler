#include <fstream>
#include <sstream>

#include "Assembler.h"

Assembler::Assembler(const std::string& filename) :
    inFilename(filename)
{
}

void Assembler::assemble()
{
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
}
