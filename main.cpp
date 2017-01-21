#include <iostream>

#include "Assembler.h"

int main(int argc, const char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Error\n";
        return 0;
    }

    Assembler assembler(argv[1]);
    assembler.assemble();

    return 0;
}
