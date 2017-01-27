#include <iostream>

#include "Assembler.h"
#include "InstructionSet8Bit.h"
#include "TextCodeGenerator.h"

int main(int argc, const char* argv[])
{
    TextCodeGenerator codeGenerator(std::cout);

    Assembler assembler(ISET_8_BIT, &codeGenerator, std::cin);
    assembler.assemble();

    return 0;
}
