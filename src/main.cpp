#include "Arguments.h"
#include "Assembler.h"

int main(int argc, const char* argv[])
{
    Arguments args;
    args.parse(argc, argv);

    bool assembled = true;
    if (!args.isDone() && !args.isError())
    {
        Assembler assembler(args);
        assembled = assembler.assemble();
    }

    return (!args.isError() && assembled) ? 0 : 1;
}
