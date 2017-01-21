#include <fstream>

#include "Assembler.h"

Assembler::Assembler(const std::string& filename) :
    inFilename(filename)
{
}

void Assembler::assemble()
{
    std::fstream inFile;
    std::fstream outFile;

    inFile.open(inFilename, std::ios_base::in);
    outFile.open("temp.s", std::ios_base::out);

    // preprocess
    preprocessor.process(inFile, outFile);

    inFile.close();
    outFile.close();
}
