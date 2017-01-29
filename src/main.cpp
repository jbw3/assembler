#include "Arguments.h"
#include "Assembler.h"

int main(int argc, const char* argv[])
{
    Arguments args;
    args.parse(argc, argv);

    if (!args.isDone() && !args.isError())
    {
        Assembler assembler(args);
        assembler.assemble();
    }

    return args.isError() ? 1 : 0;
}
