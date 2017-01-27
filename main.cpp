#include "Arguments.h"
#include "Assembler.h"

int main(int argc, const char* argv[])
{
    Arguments args;
    bool ok = args.parseArgs(argc, argv);

    if (ok)
    {
        Assembler assembler(args);
        assembler.assemble();
    }

    return ok ? 0 : 1;
}
