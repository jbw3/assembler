#include <iostream>

#include "Assembler.h"
#include "InstructionSet8Bit.h"
#include "TextCodeGenerator.h"

int main(int argc, const char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Error\n";
        return 0;
    }

    TextCodeGenerator codeGenerator(std::cout);

    /// @todo pass istream instead of file name
    Assembler assembler(ISET_8_BIT, &codeGenerator, argv[1]);
    assembler.assemble();

    return 0;
}
