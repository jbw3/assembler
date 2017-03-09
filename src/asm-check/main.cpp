#include <cstring>
#include <iostream>

using namespace std;

constexpr char HELP_MESSAGE[] = R"(asm-check <instruction set> [-h]

Check a syntax definition.

Example:
  asm-check W8
)";

int main(int argc, const char* argv[])
{
    if (argc < 2)
    {
        cerr << "Error: No instruction set was given.\n";
        return 1;
    }
    else if (argc > 2)
    {
        cerr << "Error: Too many arguments were given.\n";
        return 1;
    }
    else if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
    {
        cout << HELP_MESSAGE;
        return 0;
    }

    cout << "TODO: Check stuff\n";

    return 0;
}
